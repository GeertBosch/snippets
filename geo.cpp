#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

struct S2Point {
	S2Point(double lat, double lng) {
		double phi = lat * (M_PI / 180);
		double theta = lng * (M_PI / 180);
		cout << "phi = " << phi << ", theta = " << theta << "\n";
		double sinphi = sin(phi);
		double cosphi = cos(phi);
		double sintheta = sin(theta);
		double costheta = cos(theta);
		cout << "sin(phi) = " << sinphi << ", cos(phi) = " << cosphi << "\n";
		cout << "sin(theta) = " << sintheta << ", cos(theta) = " << costheta << "\n";
		p[0] = costheta * cosphi;
		p[1] = sintheta * cosphi;
		p[2] = sinphi;
	}
	string toString() const {
		stringstream ss;
		ss << setprecision(16);
		ss << "<" << p[0] << ", " << p[1] << ", " << p[2] << ">";
		return ss.str();
	}
	int largestAbsComponent() const {
		double pa[3] = { fabs (p[0]), fabs (p[1]), fabs (p[2]) };
		return pa[0] > pa[1]
			? (pa[0] > pa[2] ? 0 : 2)
			: (pa[1] > pa[2] ? 1 : 2);
	}
        int toFaceUV(double* pu, double* pv) const {
		int face = largestAbsComponent();
		if (p[face] < 0) face += 3;
		switch (face) {
			case 0:  *pu =  p[1] / p[0]; *pv =  p[2] / p[0]; break;
			case 1:  *pu = -p[0] / p[1]; *pv =  p[2] / p[1]; break;
			case 2:  *pu = -p[0] / p[2]; *pv = -p[1] / p[2]; break;
			case 3:  *pu =  p[2] / p[0]; *pv =  p[1] / p[0]; break;
			case 4:  *pu =  p[2] / p[1]; *pv = -p[0] / p[1]; break;
			default: *pu = -p[1] / p[2]; *pv = -p[0] / p[2]; break;
		}
		return face;
	}

	double p[3];
};

const int kMaxLevel = 30;
const int kMaxSize = 1 << kMaxLevel;

double UVtoST(double u) { return u >= 0 ? 0.5 * sqrt(1 + 3*u) : 1 - 0.5 * sqrt(1 - 3*u); }
int STtoIJ (double s) { return max(0, min(kMaxSize - 1, (int)lrint(kMaxSize * s - 0.5))); }

struct S2Cell {
	S2Cell (S2Point const& p) {
		double u, v;
		face = p.toFaceUV(&u, &v);
		i = STtoIJ(UVtoST(u));
		j = STtoIJ(UVtoST(v));
	}

	string toString() const {
		stringstream ss;
		ss << setprecision(16);
		ss << "<" << face << ", " << hex  << i << ", " << j << dec << ">";
		return ss.str();
	}
	int face, i, j;
};

int main(int argc, char **argv) {
	cout << setprecision(16);

	auto point = S2Point(48, -18);
	if (argc >= 3) {
		double lat = atof (argv[1]);
		double lng = atof (argv[2]);
		cout << "lat = " << lat << ", lng = " << lng << "\n";
		point = S2Point (lat, lng);
	}

	auto cell = S2Cell(point);
        cout << "point = " << point.toString() << "\n";
	cout << "cell = " << cell.toString() << "\n";
	return 0;
}
