#include "Geometry.h"
#include <math.h>
#define ROUND(x) int(x+0.5)

int Geometry::Circle::intersectLine(const Line& line, Point& p1, Point& p2) const {
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
int Geometry::Circle::intersectCircle(const Circle& circle, Point& p1, Point& p2) const
{
	Point p = circle.org - org;
	long long xs = p.x, ys = p.y;

	long long R2 = 1ll * radius * radius;
	long long r2 = 1ll * circle.radius * circle.radius;

	long long q = xs * xs + ys * ys;
	long long c = (R2 - r2 + q);
	long long discr = 4 * q * R2 - c * c;

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
bool Geometry::Circle::isOutside(const Point& p) const {
	Point d = p - org;
	return (d.x * d.x + d.y * d.y > radius * radius);
}

void Geometry::Circle::setRadiusFromPoint(const Point& p) {
	Point d = p - org;
	radius = hypot(d.x, d.y);
}