#pragma once
#include <Windows.h>
namespace GraphicsAlgo
{
	struct Point {
		int x, y;
		Point operator-(const Point& rhs) const {
			return { x - rhs.x, y - rhs.y };
		}
		long long operator*(const Point& rhs) const {
			return 1ll* x * rhs.x + 1ll* y * rhs.y;
		}
	};
	struct Line {
		Point st, en;
		void GetParam(int& a, int& b, int& c) const{
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
	};

	void ClipLineOnCircle(HDC hdc, const Circle& window, const Line& line, COLORREF inColor, COLORREF outColor);
	void ClipCircleOnCircle(HDC hdc, const Circle& window, const Circle& circle, COLORREF inColor, COLORREF outColor);

	void DrawLine(HDC hdc,const Line& line, COLORREF color);
	void DrawCircle(HDC hdc, const Circle& circle, COLORREF color);

	void DrawArc(HDC hdc, double startAngle, double endAngle, const Circle& circle, COLORREF color);
};

