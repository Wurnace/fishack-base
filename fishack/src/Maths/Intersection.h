#pragma once

#include "Points.h"
#include "Line.h"
#include <vector>

FishackBegin
namespace Intersection
{
	bool onLine(const line&, const point&);
	int direction(const point&, const point&, const point&);
	bool isIntersect(const line&, const line&);
	bool checkInside(const std::vector<point>&, const point&);
	bool checkInside(const std::vector<point>& allpoints, const std::vector<unsigned int>& indices, const point& p)
}
FishackEnd
