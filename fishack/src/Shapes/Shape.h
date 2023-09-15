#pragma once

#include <vector> // Lists
#include <map> // Dictionaries/Objects/etc.

#include <functional> // Use Lambda functions for "ForeachPoint()"
#include <algorithm> // Used for deleting Point's Index. 

#include "Maths/Points.h"

FishackBegin

inline float getarea(std::vector<point>& points)
{
	float area = 0.0;
	int j = int(points.size()) - 1;
	for (int i = 0; i < points.size(); i++)
	{
		area += (points[j].x + points[i].x) * (points[j].y - points[i].y);
		j = i;
	};
	return float(abs(area) / 2.0);
}

class Shape
{
public:
	Shape();
	Shape(int start, int size);
	Shape(size_t start, size_t size);

	void addIndex(int idx);
	void addIndices(int start, int size);
	int findPointIndex(const point& pt, const std::vector<point>& points) const;
	void removeIndex(int idx);

	void addShapeToPoints(std::vector<point>& points);

	int getId() const { return this->id; }
	void setId(int ID) { this->id = ID; }

	std::vector<unsigned int> getIndices() const { return indices; }

	void foreachPoint(std::vector<point>& points, std::function<void(point&)> func) const;
	void foreachPoint(std::vector<point>& points, std::function<void(point&, int)> func) const;
	void foreachPoint(std::vector<point>& points, std::function<void(point&, point&)> func) const;

	point averagepoint(std::vector<point>& points) const;
	float averageangle(std::vector<point>& points, point& averagep) const;
	void assignOffset(std::vector<point>& points);
	void jiggle(std::vector<point>& points, float force);
	void movetotarget(std::vector<point>& points);

	void fixCollision(point& pt, std::vector<point>& allpoints);
	void resolveCollisions(const point& pt, const std::vector<point>& allpoints);

	void deleteShape(std::map<int, Shape>& Shapes);
	void deleteShapePoints(std::vector<point>& points);
	void deletePoints(std::vector<point>& points, std::map<int, Shape>& Shapes);

private:
	int id = -1;
	std::vector<unsigned int> indices;
};

FishackEnd
