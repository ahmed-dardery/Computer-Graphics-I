#include "GraphicsAlgo.h"
#include <math.h>
#include <algorithm>

#include "Geometry.h"

#include "Win32.h"
#define SetPixel Win32::SetPixelFast

using namespace Geometry;

const double PI = acos(-1.0);

#define ROUND(x) int(x+0.5)

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
		double a1 = atan2(1.0 * - p1.y + circle.org.y, 1.0 * p1.x - circle.org.x);
		double a2 = atan2(1.0 * - p2.y + circle.org.y, 1.0 * p2.x - circle.org.x);
		if (a1 < 0) a1 += 2*PI;
		if (a2 < 0) a2 += 2*PI;

		if ((p1 - window.org) * (p2 - p1) < 0) std::swap(a1, a2);

		DrawArc(hdc, a1, a2, circle, outColor);
		DrawArc(hdc, a2, a1, circle, inColor);
	}
	
}

void GraphicsAlgo::ClipBezierOnCircle(HDC hdc, const Circle& window, const Bezier& bezier, COLORREF inColor, COLORREF outColor)
{
	int a3 = -bezier.p1.x + 3 * bezier.p2.x - 3 * bezier.p3.x + bezier.p4.x;
	int a2 = 3 * bezier.p1.x - 6 * bezier.p2.x + 3 * bezier.p3.x;
	int a1 = -3 * bezier.p1.x + 3 * bezier.p2.x;
	int a0 = bezier.p1.x;

	int b3 = -bezier.p1.y + 3 * bezier.p2.y - 3 * bezier.p3.y + bezier.p4.y;
	int b2 = 3 * bezier.p1.y - 6 * bezier.p2.y + 3 * bezier.p3.y;
	int b1 = -3 * bezier.p1.y + 3 * bezier.p2.y;
	int b0 = bezier.p1.y;

	double dt = 0.0001;
	for (double t = 0; t <= 1; t += dt)
	{
		int x = ROUND(a0 + a1 * t + a2 * t * t + a3 * t * t * t);
		int y = ROUND(b0 + b1 * t + b2 * t * t + b3 * t * t * t);
		
		if (window.isOutside({ x,y }))
			SetPixel(hdc, x,y, outColor);
		else
			SetPixel(hdc, x, y, inColor);
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

void GraphicsAlgo::DrawArc(HDC hdc, double startAngle, double endAngle, const Geometry::Circle& circle, COLORREF color)
{
	double dtheta = 1.0 / circle.radius;
	if (startAngle > endAngle) endAngle += 2 * PI;
	for (double theta = startAngle; theta <= endAngle; theta += dtheta) {
		int x = ROUND(circle.radius * cos(theta));
		int y = ROUND(circle.radius * sin(theta));
		SetPixel(hdc, circle.org.x+x, circle.org.y - y, color);
	}
}
