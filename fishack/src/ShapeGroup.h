#pragma once

#include <SDL.h>
#include <vector>

#include "Shape.h"
#include "Points.h"

#define RUN_IF_NOT_EMPTY(vect) if(vect.size() < 1) return;

FishackBegin

class ShapeGroup
{
public:
	std::vector<point> points;
	std::vector<shape> shapes;

	void Addpoint(point pt);
	void Addpoints(std::vector<point> pts);
	void Addshape(shape pt);
	void Addshapes(std::vector<shape> pts);

	void movetotarget();
	void move(float delta);

	void show(SDL_Renderer& renderer);
	void Update(float deltaTime);

	void deleteShape(int idx);
};

void ShapeGroup::Addpoint(point pt)
{
	this->points.push_back(pt);
}

void ShapeGroup::Addpoints(std::vector<point> pts)
{
	this->points.insert(this->points.end(), pts.begin(), pts.end());
}

void ShapeGroup::Addshape(shape shp)
{
	this->shapes.push_back(shp);
}

void ShapeGroup::Addshapes(std::vector<shape> shps)
{
	this->shapes.insert(this->shapes.end(), shps.begin(), shps.end());
}

void ShapeGroup::movetotarget()
{
	RUN_IF_NOT_EMPTY(this->shapes);
	RUN_IF_NOT_EMPTY(this->points);
	for (int shapi = 0; shapi < this->shapes.size(); shapi++)
	{
		this->shapes[shapi].movetotarget(this->points);
	}
}

void ShapeGroup::move(float delta)
{
	RUN_IF_NOT_EMPTY(this->points);

	for (int i = 0; i < this->points.size(); i++)
	{
		this->points[i].Update(delta);
	}
}

void ShapeGroup::show(SDL_Renderer& renderer)
{
	RUN_IF_NOT_EMPTY(this->shapes);
	RUN_IF_NOT_EMPTY(this->points);

	#define drawLine(ren, a, b, off, w, h) SDL_RenderDrawLine(ren,  \
		int(a.x - off.x + w), int(a.y - off.y + h), int(b.x - off.x + w), int(b.y - off.y + h));
	
	point average = (this->shapes[0]).averagepoint(this->points) * 2;
	for (int shapi = 0; shapi < this->shapes.size(); shapi++)
	{
		SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
		this->shapes[shapi].foreachPoint(this->points, [&](point& curPoint, point nextPoint) {
			drawLine(&renderer, curPoint, nextPoint, average, 320, 240);
		});
	}
}

void ShapeGroup::Update(float deltaTime)
{
	RUN_IF_NOT_EMPTY(this->shapes);
	RUN_IF_NOT_EMPTY(this->points);
	this->movetotarget();
	this->move(deltaTime);
}

void ShapeGroup::deleteShape(int idx)
{
	RUN_IF_NOT_EMPTY(this->shapes);
	RUN_IF_NOT_EMPTY(this->points);
	if (idx < this->shapes.size())
	{
		this->shapes[idx].deleteShapePoints(this->points);
		this->shapes[idx].deleteShape(this->shapes, idx);
	}
}

FishackEnd
