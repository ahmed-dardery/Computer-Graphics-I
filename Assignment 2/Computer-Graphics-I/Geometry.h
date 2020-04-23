#pragma once
namespace Geometry
{
	struct Point {
		int x, y;
		Point operator-(const Point& rhs) const {
			return { x - rhs.x, y - rhs.y };
		}
		long long operator*(const Point& rhs) const {
			return 1ll * x * rhs.x + 1ll * y * rhs.y;
		}
	};
	struct Line {
		Point st, en;
		void GetParam(int& a, int& b, int& c) const {
			a = en.y - st.y;
			b = st.x - en.x;
			c = en.x * st.y - en.y * st.x;
		}
		bool OnSegment(const Point& p) const {
			Point ab = en - st;
			Point ba = st - en;
			Point bc = en - p;
			Point ac = st - p;
			return ab * bc >= 0 && ba * ac >= 0;
		}
	};
	struct Circle {
		Point org;
		int radius;
		int intersectLine(const Line& line, Point& p1, Point& p2) const;
		int intersectCircle(const Circle& circle, Point& p1, Point& p2) const;
		bool isOutside(const Point& p) const;
		void setRadiusFromPoint(const Point& p);
	};
	struct Bezier {
		Point p1, p2, p3, p4;
		Point& operator[](int i) {
			return *(&p1 + i);
		}
	};
};

