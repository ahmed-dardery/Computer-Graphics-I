#pragma once
#include <Windows.h>
#include "Geometry.h"

namespace GraphicsAlgo
{
	
	void ClipLineOnCircle(HDC hdc, const Geometry::Circle& window, const Geometry::Line& line, COLORREF inColor, COLORREF outColor);
	void ClipCircleOnCircle(HDC hdc, const Geometry::Circle& window, const Geometry::Circle& circle, COLORREF inColor, COLORREF outColor);
	void ClipBezierOnCircle(HDC hdc, const Geometry::Circle& window, const Geometry::Bezier& bezier, COLORREF inColor, COLORREF outColor);

	void ClipLineOnCircles(HDC hdc, const Geometry::Circle* window, int cnt, const Geometry::Line& line, COLORREF inColor, COLORREF outColor);
	void ClipArcOnCircles(HDC hdc, const Geometry::Circle* window, int cnt, const Geometry::Circle& circle, double startAngle, double endAngle, COLORREF inColor, COLORREF outColor);
	void ClipCircleOnCircles(HDC hdc, const Geometry::Circle* window, int cnt, const Geometry::Circle& circle, COLORREF inColor, COLORREF outColor);
	void ClipBezierOnCircles(HDC hdc, const Geometry::Circle* window, int cnt, const Geometry::Bezier& bezier, COLORREF inColor, COLORREF outColor);

	void DrawLine(HDC hdc,const Geometry::Line& line, COLORREF color);
	void DrawCircle(HDC hdc, const Geometry::Circle& circle, COLORREF color);

	void DrawArc(HDC hdc, const Geometry::Circle& circle, double startAngle, double endAngle, COLORREF color);
	
};

