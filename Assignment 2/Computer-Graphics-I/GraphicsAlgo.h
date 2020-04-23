#pragma once
#include <Windows.h>
#include "Geometry.h"

namespace GraphicsAlgo
{
	
	void ClipLineOnCircle(HDC hdc, const Geometry::Circle& window, const Geometry::Line& line, COLORREF inColor, COLORREF outColor);
	void ClipCircleOnCircle(HDC hdc, const Geometry::Circle& window, const Geometry::Circle& circle, COLORREF inColor, COLORREF outColor);

	void ClipBezierOnCircle(HDC hdc, const Geometry::Circle& window, const Geometry::Bezier& bezier, COLORREF inColor, COLORREF outColor);

	void DrawLine(HDC hdc,const Geometry::Line& line, COLORREF color);
	void DrawCircle(HDC hdc, const Geometry::Circle& circle, COLORREF color);

	void DrawArc(HDC hdc, double startAngle, double endAngle, const Geometry::Circle& circle, COLORREF color);
	
};

