#pragma once

#include <vector> //Lists
#include <functional> // Use Lambda functions for "ForeachPoint()"
#include <algorithm> // Used for deleting Point's Index. 

#include "Points.h"

float getarea(std::vector<point>& points)
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

struct shape
{
	shape() { this->randomizeId(); };
	shape(int start, int size) {
		this->addIndices(start, size);
		this->randomizeId();
	}

	void addIndices(int start, int size) {
		for (int i = start; i < start + size; i++) {
			this->indices.push_back(i);
		}
	}

	void addShapeToPoints(std::vector<point>& points);

	int getId() { return this->id;  }

private:
	void randomizeId()
	{
		this->id = rand() % 10000;
	}
	int id = 0;

	std::vector<unsigned int> indices;

public:
	void foreachPoint(std::vector<point>& points, std::function<void(point&)> func);
	void foreachPoint(std::vector<point>& points, std::function<void(point&, int)> func);
	void foreachPoint(std::vector<point>& points, std::function<void(point&, point)> func);

	point averagepoint(std::vector<point>& points);
	float averageangle(std::vector<point>& points, point averagep);
	void assignOffset(std::vector<point>& points);
	void jiggle(std::vector<point>& points);
	void movetotarget(std::vector<point>& points);

	void removeIndex(int idx)
	{
		indices.erase(std::remove(indices.begin(), indices.end(), idx), indices.end());
		if (indices.size() < 1) this->deleteShape();
	}

	void deleteShape() { delete this;  }
	void deleteShapePoints(std::vector<point>& points) 
		// Only deletes points - not shapes they connect to too.
	{
		this->foreachPoint(points, [](point curPoint) {
			delete &curPoint;
		});
		this->deleteShape();
	}

	void deletePoints(std::vector<point>& points, std::vector<shape>& shapes)
		// Deletes points and shapes they connect to.
	{
		this->foreachPoint(points, [&](point& curPoint, int idx) {
			for (int shape_id : curPoint.ShapeIDs)
			{
				for (shape n : shapes) if (n.getId() == shape_id)
				{
					//n.removeIndex(idx);
				}
			}
			points[idx] = {};
			// delete curPoint;
		});
		// this->deleteShape();

		/*
		int idx = 0;
		for (shape n : shapes) if (n.getId() == this->id) {
			idx = 0; 
			break;
		}
		*/
		//shapes.erase(std::remove(shapes.begin(), shapes.end(), idx), shapes.end());
	}
};

void shape::addShapeToPoints(std::vector<point>& points)
{
	this->foreachPoint(points, [&](point curPoint) {
		for (int &shapeID : curPoint.ShapeIDs)
		{
			if (shapeID == this->id) return;
		}
		curPoint.ShapeIDs.push_back(this->id);
	});
}

void shape::foreachPoint(std::vector<point>& points, std::function<void(point&)> func)
{
	for (int i = 0; i < this->indices.size(); i++) {
		func(points[ this->indices[i] ]);
	}
}

void shape::foreachPoint(std::vector<point>& points, std::function<void(point&, int)> func)
{
	for (int i = 0; i < this->indices.size(); i++) {
		func(points[this->indices[i]], i);
	}
}

void shape::foreachPoint(std::vector<point>& points, std::function<void(point&, point)> func)
{
	for (int i = 0; i < this->indices.size(); i++) {
		int j = (i - 1); if (j < 0) j = int(this->indices.size() - 1);
		func(points[ this->indices[i] ], points[ this->indices[j] ]);
	}
}

point shape::averagepoint(std::vector<point>& points)
{
	point average = { 0, 0 };
	this->foreachPoint(points, [&average] (point& curPoint) {
		average += curPoint.getVector();
	});
	average /= int(points.size());
	return average;
}

float shape::averageangle(std::vector<point>& points, point averagep)
{
	float average = 0;

#ifndef averagep
	averagep = this->averagepoint(points);
#endif

	this->foreachPoint(points, [&average, &averagep](point& curPoint) {
		average += averagep.angleBetween(curPoint, averagep + curPoint.targetOff);
	});
	return average / points.size();
}

void shape::assignOffset(std::vector<point>& points) {
	point average = this->averagepoint(points);
	this->foreachPoint(points, [&average](point& curPoint) {
		curPoint.setOffset(curPoint - average);
	});
}

void shape::jiggle(std::vector<point>& points)
{
	this->foreachPoint(points, [](point& curPoint) {
		Vector unit_random = { float(rand() % 3 - 1), float(rand() % 3 - 1) };
		curPoint.vel += unit_random * 4;
	});
}

void shape::movetotarget(std::vector<point>& points)
{
	point average = averagepoint(points);
	float angleaverage = averageangle(points, average);
	this->foreachPoint(points, [&average, &angleaverage](point& curPoint) {
		Vector OffsetCopy = (curPoint.targetOff).rotateNew(angleaverage, { 0, 0 });
		curPoint.moveToTarget(average.getVector() + OffsetCopy);
	});
}