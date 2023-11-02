#pragma once

#include <SDL.h>

#include <map> //Shapes
#include <vector> //Points
#include <functional> // Use Lambda functions for "ForeachPoint()"

#include "Renderer.h"

#define RUN_IF_NOT_EMPTY(vect) if(vect.size() < 1) return

FishackBegin

class ShapeGroup
{
public:
	ShapeGroup();
	ShapeGroup(std::vector<point> points);

	void Addpoint(point pt);
	void Addpoints(std::vector<point> pts);
	void AddShape(Shape pt);
	void AddShapes(std::vector<Shape> pts);
	void AddShape(std::vector<point> pts);

	void foreachShape(std::function<void(Shape&)> func);
	void foreachShape(std::function<void(Shape&, int)> func);

	void show(Renderer& renderer, int WW, int WH);
	void Update(float deltaTime, float inflationfactor);

	void resolveCollision(int ID, const Shape& other, std::vector<point>& otherPoints);

	int getShapeID(int index);
	Shape ShapeAt(int index);

	void deleteShapeByID(int ID);
	void deleteShapeAt(int index);
	bool delete_if_no_shapes(std::vector<ShapeGroup>& shapeGroups);

	void call_on_make_Shape(std::function<void(Shape&)>);

public:
	std::vector<point> points;
	std::map<int, Shape> Shapes;

private:
	void movetotarget(float inflationfactor);
	void move(float delta);

private:
	int shapesCreated = 0;
	std::function<void(Shape&)> on_create;
};

FishackEnd
