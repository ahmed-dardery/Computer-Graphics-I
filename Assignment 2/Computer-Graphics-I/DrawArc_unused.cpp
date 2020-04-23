#include <Windows.h>
#include <math.h>
#include <algorithm>
#include "Geometry.h"

using namespace Geometry;
const double PI = acos(-1.0);

void (*pixelLocs[])(int, int, int&, int&) = {
	[](int x, int y, int& xr, int& yr) { xr = x, yr = -y; },
	[](int x, int y, int& xr, int& yr) { xr = y, yr = -x; },
	[](int x, int y, int& xr, int& yr) { xr = -y, yr = -x; },
	[](int x, int y, int& xr, int& yr) { xr = -x, yr = -y; },
	[](int x, int y, int& xr, int& yr) { xr = -x, yr = y; },
	[](int x, int y, int& xr, int& yr) { xr = -y, yr = x; },
	[](int x, int y, int& xr, int& yr) { xr = y, yr = x; },
	[](int x, int y, int& xr, int& yr) { xr = x, yr = y; }
};

void SetMultiPixelArc(HDC hdc, int xc, int yc, COLORREF color, int x, int y, decltype(pixelLocs) arr, int cnt) {
	for (int i = 0; i < cnt; i++) {
		int xx, yy;
		arr[i](x, y, xx, yy);
		SetPixel(hdc, xc + xx, yc + yy, color);
	}
}
double arcStops[] = { 0,PI / 4,PI / 2,3 * PI / 4,PI,5 * PI / 4,3 * PI / 2,7 * PI / 4, 2 * PI };


void DrawArcHelper(HDC hdc, int sector, double startAngle, double endAngle, const Circle& circle, COLORREF color) {
	if (sector < 0 || sector >= 8) return;
	int xx, yy;

	int x = circle.radius;
	int y = 0;


	pixelLocs[sector](x, y, xx, yy);

	double angle = atan2(-yy, xx);
	if (angle < 0) angle += 2 * PI;
	if (angle >= startAngle && angle < endAngle)
		SetPixel(hdc, circle.org.x + xx, circle.org.y + yy, color);
	int d = 5 - 4 * circle.radius;
	while (x > y)
	{
		if (d > 0) {
			d += -8 * x + 8;
			x--;
		}
		d += 8 * y + 12;
		y++;

		pixelLocs[sector](x, y, xx, yy);
		double angle = atan2(-yy, xx);
		if (angle < 0) angle += 2 * PI;
		if (angle >= startAngle && angle < endAngle)
			SetPixel(hdc, circle.org.x + xx, circle.org.y + yy, color);
	}

}

void DrawArc(HDC hdc, double startAngle, double endAngle, const Circle& circle, COLORREF color) {
	decltype(pixelLocs) mine; int cnt = 0;

	if (startAngle < endAngle) {
		int s = std::lower_bound(arcStops, arcStops + 8, startAngle) - arcStops;
		int e = std::upper_bound(arcStops, arcStops + 8, endAngle) - arcStops - 1;

		if (e >= s) {

			DrawArcHelper(hdc, s - 1, startAngle, arcStops[s], circle, color);
			DrawArcHelper(hdc, e, arcStops[e], endAngle, circle, color);

			for (int i = s; i < e; i++)
				mine[cnt++] = pixelLocs[i];
		}
		else {
			DrawArcHelper(hdc, s - 1, startAngle, endAngle, circle, color);
		}
	}
	else {
		int s = std::upper_bound(arcStops, arcStops + 8, endAngle) - arcStops - 1;
		int e = std::lower_bound(arcStops, arcStops + 8, startAngle) - arcStops;
		if (e <= s) {
			for (int i = 0; i < 8; i++)
				mine[cnt++] = pixelLocs[i];
		}
		else {

			DrawArcHelper(hdc, s, arcStops[s], endAngle, circle, color);
			DrawArcHelper(hdc, e - 1, startAngle, arcStops[e], circle, color);

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
