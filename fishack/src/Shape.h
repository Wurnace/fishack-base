#pragma once

#include <SDL.h>

#include "Points.h"

float getarea(point* points)
{
	int xpoints = sizeof(points);
	float area = 0.0;
	int j = xpoints - 1;
	for (int i = 0; i < xpoints; i++)
	{
		area += (points[j].x + points[i].x) * (points[j].y - points[i].y);
		j = i;
	};
	return float(abs(area) / 2.0);
}

point averagepoint(point* points, int start, int size)
{
	point average = { 0, 0 };
	for (int i = start; i < start + size; i++)
	{
		average += points[i].getVector();
	};
	average /= size;
	return average;
}

float averageangle(point* points, int start, int size, point averagep)
{
	float average = 0;

	#ifndef averagep
		averagep = averagepoint(points, start, size);
	#endif

	for (int i = start; i < start + size; i++)
	{
		average += averagep.angleBetween(points[i], averagep + points[i].targetOff);
	};
	return average / size;
}

struct shape
{
	int start;
	int size;
	// size is the amount of points it is

	void assignOffset(point* points);
	void jiggle(point* points);
	void movetotarget(point* points);
};

void shape::assignOffset(point* points) {
	point average = averagepoint(points, this->start, this->size);
	for (int i = this->start; i < this->start + this->size; i++)
	{
		points[i].setOffset(points[i] - average);
	};
}

void shape::jiggle(point* points)
{
	for (int i = this->start; i < this->start + this->size; i++)
	{
		Vector unit_random = { float(rand() % 3 - 1), float(rand() % 3 - 1) };
		points[i].vel += unit_random * 1/2;
	};
}

void shape::movetotarget(point* points)
{
	point average = averagepoint(points, this->start, this->size);
	float angleaverage = averageangle(points, this->start, this->size, average);
	for (int i = this->start; i < this->start + this->size; i++)
	{
		Vector OffsetCopy = (points[i].targetOff).rotateNew(angleaverage, { 0, 0 });
		//if (OffsetCopy.mag() - (points[i].targetOff).mag() <= 0.001) continue;
		points[i].moveToTarget(average.getVector() + OffsetCopy);
	}
}
