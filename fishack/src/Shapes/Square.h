#pragma once

FishackBegin

// `Shapes/Square.h` now Contains Construction for ANY Regular Polygon. 

std::vector<point> Polygon(float x, float y, float radius, int sides = 4, float dir = 0.0f)
{
	std::vector<point> pts;
	float deltaAngle = float(2 * M_PI) / sides;
	for (float side = 0.0f; side < sides; side++)
	{
		Vector v(1, 0);
		v.setMag(radius);
		v.rotate((dir + deltaAngle/2) + side * deltaAngle);
		pts.push_back({ v.x + x, v.y + y });
	}
	return pts;
}

std::vector<point> Square(Vector pos, float w, float dir = 0.0f)
{
	return Polygon(pos.x, pos.y, float(w / 2), 4, dir);
}

std::vector<point> Square(Vector pos, int w, float dir = 0.0f)
{
	return Polygon(pos.x, pos.y, float(w / 2), 4, dir);
}

std::vector<point> Square(int x, int y, int w, float dir = 0.0f)
{
	return Polygon((float)x, (float)y, float(w / 2), 4, dir);
}

FishackEnd