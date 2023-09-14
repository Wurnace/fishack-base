
#ifndef FishackBegin
	#define FishackBegin namespace Fishack {
	#define FishackEnd }
#endif

#include "Intersection.h"

FishackBegin
namespace Intersection
{
	bool onLine(line l1, point p)
	{
		#define max(a, b) (a > b ? a : b)
		#define min(a, b) (a < b ? a : b)

		if (p.x <= max(l1.p1.x, l1.p2.x)
			&& p.x >= min(l1.p1.x, l1.p2.x)
			&& (p.y <= max(l1.p1.y, l1.p2.y)
				&& p.y >= min(l1.p1.y, l1.p2.y)))
			return true;

		return false;
	}
	int direction(point a, point b, point c)
	{
		float val = (b.y - a.y) * (c.x - b.x)
			- (b.x - a.x) * (c.y - b.y);

		if (val == 0.0f)

			// Collinear
			return 0;

		else if (val < 0.0f)

			// Anti-clockwise direction
			return 2;

		// Clockwise direction
		return 1;
	}
	bool isIntersect(line l1, line l2)
	{

		// Four directions for two lines and points of the other line
		int dir1 = direction(l1.p1, l1.p2, l2.p1);
		int dir2 = direction(l1.p1, l1.p2, l2.p2);
		int dir3 = direction(l2.p1, l2.p2, l1.p1);
		int dir4 = direction(l2.p1, l2.p2, l1.p2);

		// When intersecting
		if (dir1 != dir2 && dir3 != dir4)
			return true;

		return false;
	}
	bool checkInside(std::vector<point>& poly, point p)
	{
		#define n poly.size()

		// When polygon has less than 3 edge, it is not polygon
		if (n < 3)
			return false;

		// Create a point at infinity, y is same as point p
		line exline = { p, { 9999.0f, p.y } };
		int count = 0;
		int i = 0;
		do {

			// Forming a line from two consecutive points of
			// poly
			line side = { poly[i], poly[(i + 1) % n] };
			if (isIntersect(side, exline)) {

				// If side is intersects exline
				if (direction(side.p1, p, side.p2) == 0)
					return onLine(side, p);
				count++;
			}
			i = (i + 1) % n;
		} while (i != 0);

		// When count is odd
		return count & 1;
	}
}
FishackEnd