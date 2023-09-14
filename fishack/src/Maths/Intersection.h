#pragma once

#include "Points.h"
#include "Line.h"
#include <vector>

FishackBegin
namespace Intersection
{
	bool onLine(line, point);
	int direction(point, point, point);
	bool isIntersect(line, line);
	bool checkInside(std::vector<point>&, point);
}
FishackEnd