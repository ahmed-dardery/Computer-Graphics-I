#include "GraphicsAlgo.h"
#include <math.h>
#include <algorithm>

#define ROUND(x) int(x+0.5)
const double PI = acos(-1.0);


int GraphicsAlgo::Circle::intersectLine(const Line& line, Point& p1, Point& p2) const {
	int aa, bb, cc;
	long long a, b, c;
	Line{ line.st - org, line.en - org }.GetParam(aa, bb, cc);
	a = aa, b = bb, c = cc;
	long long q = a * a + b * b;

	int points = 0;
	long long discr = 1ll * radius * radius * q - c * c;

	if (discr < 0) return 0;
	else points = 1 + (discr != 0);

	double sq = sqrt(discr);

	double x1 = (-a * c + b * sq) / q;
	double y1 = (-b * c - a * sq) / q;

	double y2 = (-b * c + a * sq) / q;
	double x2 = (-a * c - b * sq) / q;

	p1 = { ROUND(x1 + org.x), ROUND(y1 + org.y) };
	p2 = { ROUND(x2 + org.x), ROUND(y2 + org.y) };

	return points;
}
int GraphicsAlgo::Circle::intersectCircle(const Circle& circle, Point& p1, Point& p2) const
{
	Point p = circle.org - org;
	long long xs = p.x, ys = p.y;

	long long R2 = 1ll * radius * radius;
	long long r2 = 1ll * circle.radius * circle.radius;

	long long q = xs * xs + ys * ys;
	long long c = (R2 - r2 + q);
	long long discr = 4* q * R2 - c * c;

	int points = 0;
	if (discr < 0) return 0;
	else points = 1 + (discr != 0);

	double sq = sqrt(discr);
	q *= 2;
	double x1 = (xs * c + ys * sq) / q;
	double y1 = (ys * c - xs * sq) / q;
	double x2 = (xs * c - ys * sq) / q;
	double y2 = (ys * c + xs * sq) / q;

	p1 = { ROUND(x1 + org.x), ROUND(y1 + org.y) };
	p2 = { ROUND(x2 + org.x), ROUND(y2 + org.y) };
	return points;
}
bool GraphicsAlgo::Circle::isOutside(const Point& p) const {
	Point d = p - org;
	return (d.x * d.x + d.y * d.y > radius * radius);
}

void GraphicsAlgo::ClipLineOnCircle(HDC hdc, const Circle& window, const Line& line, COLORREF inColor, COLORREF outColor)
{
	bool stOutside = window.isOutside(line.st);
	bool enOutside = window.isOutside(line.en);
	if (!stOutside && !enOutside) {
		DrawLine(hdc, line, inColor);
		return;
	}
	Point p1, p2;
	int intersectCnt = window.intersectLine(line, p1, p2);
	if (intersectCnt < 2) {
		DrawLine(hdc, line, outColor);
		return;
	}
	bool p1On = line.OnSegment(p1);
	bool p2On = line.OnSegment(p2);

	if (p1On && p2On) {
		Point d1 = line.st - p1, d2 = line.st - p2;
		if (d1 * d1 > d2 * d2) std::swap(p1, p2);

		DrawLine(hdc, { line.st, p1 }, outColor);
		DrawLine(hdc, { p1, p2 }, inColor);
		DrawLine(hdc, { p2, line.en }, outColor);
	}
	else if (p1On || p2On) {
		if (p2On) p1 = p2;
		if (stOutside) {
			DrawLine(hdc, { line.st, p1 }, outColor);
			DrawLine(hdc, { p1, line.en }, inColor);
		}
		else {
			DrawLine(hdc, { line.st, p1 }, inColor);
			DrawLine(hdc, { p1, line.en }, outColor);
		}
	}
	else {
		
		DrawLine(hdc, { line.st, line.en }, stOutside ? outColor : inColor);
	}
}

void GraphicsAlgo::ClipCircleOnCircle(HDC hdc, const Circle& window, const Circle& circle, COLORREF inColor, COLORREF outColor)
{
	Point p1, p2;
	if (window.intersectCircle(circle, p1, p2) < 2) {
		if (window.isOutside(circle.org) || circle.radius>window.radius)
			DrawCircle(hdc, circle, outColor);
		else
			DrawCircle(hdc, circle, inColor);
	}
	else {
		//DEBUG
		DrawCircle(hdc, { p1,5 }, inColor);
		DrawCircle(hdc, { p2,5 }, inColor);
		//DEBUG

		double a1 = atan2(1.0 * p1.y - circle.org.y, 1.0 * p1.x - circle.org.x);
		double a2 = atan2(1.0 * p2.y - circle.org.y, 1.0 * p2.x - circle.org.x);
		if (a1 < 0) a1 += 2*PI;
		if (a2 < 0) a2 += 2*PI;

		if ((p1 - window.org) * (p2 - p1) < 0) std::swap(a1, a2);

		DrawArc(hdc, a1, a2,circle, inColor);
		DrawArc(hdc, a2, a1, circle, outColor);
	}
	
}

void GraphicsAlgo::DrawLine(HDC hdc, const Line& line, COLORREF color)
{
	int xe = line.en.x, xs = line.st.x, ye = line.en.y, ys = line.st.y;
	int dx = xe - xs;
	int dy = ye - ys;
	if (abs(dx) >= abs(dy)) {
		if (xe < xs) std::swap(xs, xe), std::swap(ys, ye);
		SetPixel(hdc, xs, ys, color);

		int x = xs;
		int y = ys;
		int diff = ye > ys ? 1 : -1;
		int d = 2 * dy - diff * dx;

		int dir = -dx * diff;
		while (x < xe) {
			if (d * dir < 0)
				y += diff, d -= 2 * dx * diff;
			++x;
			d += 2 * dy;
			SetPixel(hdc, x, y, color);
		}
	}
	else {
		if (ye < ys) std::swap(xs, xe), std::swap(ys, ye);
		SetPixel(hdc, xs, ys, color);

		int x = xs;
		int y = ys;
		int diff = xe > xs ? 1 : -1;
		int d = dy * diff - 2 * dx;

		int dir = dy * diff;

		while (y < ye) {
			if (d * dir < 0)
				x += diff, d += 2 * dy * diff;
			++y;
			d -= 2 * dx;
			SetPixel(hdc, x, y, color);
		}
	}
}




void SetMultiPixel(HDC hdc, int xc, int yc, COLORREF color, int x, int y) {
	SetPixel(hdc, xc + x, yc + y, color); //1
	SetPixel(hdc, xc + y, yc + x, color); //2
	SetPixel(hdc, xc - y, yc + x, color); //3
	SetPixel(hdc, xc - x, yc + y, color); //4
	SetPixel(hdc, xc - x, yc - y, color); //5
	SetPixel(hdc, xc - y, yc - x, color); //6
	SetPixel(hdc, xc + y, yc - x, color); //7
	SetPixel(hdc, xc + x, yc - y, color); //8
}

void (*pixelLocs[])(int, int, int&, int&) = {
	[](int x, int y, int& xr, int& yr) { xr = x, yr = y; },
	[](int x, int y, int& xr, int& yr) { xr = y, yr = x; },
	[](int x, int y, int& xr, int& yr) { xr = -y, yr = x; },
	[](int x, int y, int& xr, int& yr) { xr = -x, yr = y; },
	[](int x, int y, int& xr, int& yr) { xr = -x, yr = -y; },
	[](int x, int y, int& xr, int& yr) { xr = -y, yr = -x; },
	[](int x, int y, int& xr, int& yr) { xr = y, yr = -x; },
	[](int x, int y, int& xr, int& yr) { xr = x, yr = -y; }
};

void SetMultiPixelArc(HDC hdc, int xc, int yc, COLORREF color, int x, int y, decltype(pixelLocs) arr, int cnt) {
	for (int i = 0; i < cnt; i++){
		int xx, yy;
		arr[i](x, y, xx, yy);
		SetPixel(hdc, xc + xx, yc + yy, color);
	}
}
double arcStops[] = { 0,PI / 4,PI / 2,3 * PI / 4,PI,5 * PI / 4,3 * PI / 2,7 * PI / 4 };

void GraphicsAlgo::DrawArc(HDC hdc, double startAngle, double endAngle, const Circle& circle, COLORREF color) {
	decltype(pixelLocs) mine; int cnt = 0;

	if (startAngle < endAngle) {
		int s = std::lower_bound(arcStops, arcStops + 8, startAngle) - arcStops;
		int e = std::upper_bound(arcStops, arcStops + 8, endAngle) - arcStops - 1;

		if (e > s)
			for (int i = s; i < e; i++)
				mine[cnt++] = pixelLocs[i];
	}
	else {
		int s = std::upper_bound(arcStops, arcStops + 8, endAngle) - arcStops - 1;
		int e = std::lower_bound(arcStops, arcStops + 8, startAngle) - arcStops;
		if (e <= s) {
			for (int i = 0; i < 8; i++)
				mine[cnt++] = pixelLocs[i];
		}
		else {
			for (int i = 0; i < s; i++)
				mine[cnt++] = pixelLocs[i];
			for (int i = e; i < 8; i++)
				mine[cnt++] = pixelLocs[i];
		}
	}

	int x = circle.radius;
	int y = 0;
	SetMultiPixelArc(hdc, circle.org.x, circle.org.y, color, x, y, mine, cnt);
	int d = 5 - 4 * circle.radius;
	while (x > y)
	{
		if (d > 0) {
			d += -8 * x + 8;
			x--;
		}
		d += 8 * y + 12;
		y++;

		SetMultiPixelArc(hdc, circle.org.x, circle.org.y, color, x, y, mine, cnt);
	}
}

void GraphicsAlgo::DrawCircle(HDC hdc, const Circle& circle, COLORREF color) {


	int x = circle.radius;
	int y = 0;
	SetMultiPixel(hdc, circle.org.x, circle.org.y, color, x, y);
	int d = 5 - 4 * circle.radius;
	while (x > y)
	{
		if (d > 0) {
			d += -8 * x + 8;
			x--;
		}
		d += 8 * y + 12;
		y++;

		SetMultiPixel(hdc, circle.org.x, circle.org.y, color, x, y);
	}
}