#ifndef FishackBegin
	#define FishackBegin namespace Fishack {
	#define FishackEnd }
#endif

#include "Maths/Points.h"

#include "Shape.h"

#include "Maths/Intersection.h"

FishackBegin

Shape::Shape()
{
}
Shape::Shape(int start, int size)
{
	this->addIndices(start, size);
}
Shape::Shape(size_t start, size_t size)
{
	this->addIndices((int)start, (int)size);
}

void Shape::assignOffset(std::vector<point>& points) {
	point average = this->averagepoint(points);
	this->foreachPoint(points, [&average](point& curPoint) {
		curPoint.setOffset(curPoint - average);
	});
}

void Shape::addShapeToPoints(std::vector<point>& points)
{
	this->foreachPoint(points, [&](point curPoint) {
		for (int &ShapeID : curPoint.ShapeIDs)
		{
			if (ShapeID == this->id) return;
		}
		curPoint.ShapeIDs.push_back(this->id);
	});
}

void Shape::addIndex(int idx)
{
	this->indices.push_back(idx);
}
void Shape::addIndices(int start, int size) {
	for (int i = start; i < start + size; i++) {
		this->indices.push_back(i);
	}
}
void Shape::removeIndex(int idx)
{
	indices.erase(std::remove(indices.begin(), indices.end(), idx), indices.end());
}
int Shape::findPointIndex(const point& pt, const std::vector<point>& points) const 
{
	for (int i = 0; i < this->indices.size(); i++)
	{
		if (points[i] == pt) return i;
	}
	return -1;
}

void Shape::foreachPoint(std::vector<point>& points, std::function<void(point&)> func) const 
{
	for (int i = 0; i < this->indices.size(); i++) {
		func(points[this->indices[i]]);
	}
}
void Shape::foreachPoint(std::vector<point>& points, std::function<void(point&, int)> func) const
{
	for (int i = 0; i < this->indices.size(); i++) {
		func(points[this->indices[i]], i);
	}
}
void Shape::foreachPoint(std::vector<point>& points, std::function<void(point&, point&)> func) const
{
	for (int i = 0; i < this->indices.size(); i++) {
		int j = (i - 1); if (j < 0) j = int(this->indices.size() - 1);
		func(points[this->indices[i]], points[this->indices[j]]);
	}
}

point Shape::averagepoint(std::vector<point>& points) const
{
	point average = { 0, 0 };
	this->foreachPoint(points, [&average](point& curPoint) {
		average += curPoint.getVector();
	});
	average /= (int)this->indices.size();
	return average;
}
float Shape::averageangle(std::vector<point>& points, point& averagep) const
{
	float average = 0;

#ifndef averagep
	averagep = this->averagepoint(points);
#endif

	this->foreachPoint(points, [&average, &averagep](point& curPoint) {
		average += averagep.angleBetween(averagep + curPoint.targetOff, curPoint);
	});
	std::cout << average / points.size() << std::endl;
	return average / points.size();
}

void Shape::jiggle(std::vector<point>& points, float force)
{
	this->foreachPoint(points, [force](point& curPoint) {
		Vector unit_random = { float(rand() % 3 - 1), float(rand() % 3 - 1) };
		curPoint.vel += unit_random * force;
	});
}

void Shape::movetotarget(std::vector<point>& points)
{
	point average = averagepoint(points);
	float angleaverage = averageangle(points, average);
	this->foreachPoint(points, [&average, &angleaverage](point& curPoint) {
		Vector OffsetCopy = (average.getVector() + curPoint.targetOff).rotateNew(angleaverage, average.getVector());
		curPoint.moveToTarget(OffsetCopy);
	});
}

namespace Projections
{
	bool Project(Vector a, Vector b, Vector pt, Vector& Answer)
	{
		Vector aCopy = (b - a).normalize();
		float sp = (pt - a).dot(aCopy);
		if (sp < 0 || sp >(a - pt).mag()) return false;
		Answer = (aCopy * sp) + a;
		return true;
	}
	struct Projection
	{
		Projection() {}

		Projection(Vector Proj, const point& pt, int edgePoint1, int edgePoint2)
		{
			this->Proj = Proj;
			this->edgeIdx1 = edgePoint1;
			this->edgeIdx2 = edgePoint2;
			this->dist = pt.distTo(Proj);
		}

		Vector Proj;
		int edgeIdx1 = -1;
		int edgeIdx2 = -1;

		float dist = INFINITY;
	};
	void Project(point& pt, int a, int b, std::vector<point>& allPoints, std::vector<Projection>& Result)
	{
		Vector v;
		if (Project(allPoints[a].getVector(), allPoints[a].getVector(), pt.getVector(), v))
		{
			Result.push_back({ v, pt, a, b });
		}
	}
}

void Shape::fixCollision(point& pt, std::vector<point>& allpoints)
{
	using namespace Projections;

	// Project each Edge. 
	std::vector<Projection> list;
	for(int i = 0; i < this->indices.size(); i++)
	{
		int a = this->indices[i];
		int b = this->indices[(i + 1) % this->indices.size()];
		list.push_back({ allpoints[a].getVector(), pt, a, b });
		Project(pt, a, b, allpoints, list);
	}

	// Find Closest Edge
	Projection ClosestEdge;
	for (Projection& p : list)
	{
		if(p.dist < ClosestEdge.dist) ClosestEdge = p;
	}

	// Add Force to Edge Points depending on Which one is Closer. 
	float ratio = pt.distTo(allpoints[ClosestEdge.edgeIdx1].getVector());
	ratio = ratio / ( ratio + pt.distTo(allpoints[ClosestEdge.edgeIdx2].getVector()) );
	
	// The Direction opposite to before. 
	Vector dir = ClosestEdge.Proj - pt.getVector();

	// Apply the Forces.  
	pt.moveToTarget(ClosestEdge.Proj);
	//allpoints[ClosestEdge.edgeIdx1].moveToTarget(allpoints[ClosestEdge.edgeIdx1].getVector() - dir * ratio);
	//allpoints[ClosestEdge.edgeIdx2].moveToTarget(allpoints[ClosestEdge.edgeIdx2].getVector() - dir * (1 - ratio));
}

void Shape::resolveCollisions(const point& pt, const std::vector<point>& allpoints)
{
	if(!Intersection::checkInside(allpoints, this->indices, pt))
		return;

	// Pysics Code goes Here
	// BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH
	// BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH
	// BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH
	// BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH
	// BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH
	// BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH BLAH
}

void Shape::deleteShape(std::map<int, Shape>& Shapes)
{
	if (Shapes.find(this->id) == Shapes.end()) return;

	if (Shapes.size() == 1) return Shapes.clear();
	Shapes.erase(Shapes.find(this->id));
}
void Shape::deleteShapePoints(std::vector<point>& points)
// Only deletes points - not Shapes they connect to too.
{
	for (int j = 0; j < this->indices.size(); j++)
	{
		int i = this->indices[j];
		points[i] = {};
	}
}
void Shape::deletePoints(std::vector<point>& points, std::map<int, Shape>& Shapes)
// Deletes points and Shapes they connect to.
{
	this->foreachPoint(points, [&](point& curPoint, int idx) {
		for (int Shape_id : curPoint.ShapeIDs)
		{
			if (Shapes.find(Shape_id) != Shapes.end())
			{
				Shape& other = Shapes.at(Shape_id);
				other.removeIndex(other.findPointIndex(curPoint, points));
			}
		}
		// delete curPoint;
		points[idx] = {};
	});
}

FishackEnd
