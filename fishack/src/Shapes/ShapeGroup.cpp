#ifndef FishackBegin
	#define FishackBegin namespace Fishack {
	#define FishackEnd }
#endif

#include "Math/Points.h"
#include "Shape.h"

#include "Renderer.h"

#include "ShapeGroup.h"

FishackBegin

ShapeGroup::ShapeGroup()
{
}
ShapeGroup::ShapeGroup(std::vector<point> points)
{
	this->AddShape(points);
}

/**
 * Calls a Function on Each Shape with params:
 *
 * @param Shape - current shape.
 */
void ShapeGroup::foreachShape(std::function<void(Shape&)> func)
{
	for (auto[id, shape] : this->Shapes)
	{
		func(shape);
	}
}

/**
 * Calls a Function on Each Shape with params:
 *
 * @param Shape - current shape.
 * @param index - shape index, starting from 0.
 */
void ShapeGroup::foreachShape(std::function<void(Shape&, int)> func)
{
	int ShapeCount = 0;
	for (auto[id, shape] : this->Shapes)
	{
		func(shape, ShapeCount);
		ShapeCount++;
	}
}

/**
 * Locally adds Point to Object's List.
 *
 * @param Point - point to add.
 */
void ShapeGroup::Addpoint(point pt)
{
	this->points.push_back(pt);
}

/**
 * Locally adds Points to Object's List.
 *
 * @param Points - Vector of Points to add.
 */
void ShapeGroup::Addpoints(std::vector<point> pts)
{
	this->points.insert(this->points.end(), pts.begin(), pts.end());
}

/**
 * Locally adds Shape to Object's List.
 *
 * @param Shape - shape to add.
 */
void ShapeGroup::AddShape(Shape shp)
{
	shp.setId(this->shapesCreated);
	this->Shapes[shp.getId()] = shp;
	if (this->on_create) this->on_create(shp);
	this->shapesCreated++;
}

/**
 * Creates a Shape from a Group of Points.
 *
 * @param Points - Points to make a shape from.
 */
void ShapeGroup::AddShape(std::vector<point> pts)
{
	size_t StartIndex;
	if (this->points.empty()) StartIndex = 0;
	else StartIndex = this->points.size();
	size_t Size = pts.size();
	this->Addpoints(pts);
	this->AddShape(Shape(StartIndex, Size));
}

/**
 * Locally adds Shapes to Object's List.
 *
 * @param Shapes - Vector of Shape to add.
 */
void ShapeGroup::AddShapes(std::vector<Shape> shps)
{
	for (auto shp : shps)
	{
		this->AddShape(shp);
	}
}

/**
 * Invokes The 'MoveToTarget' function of Each Shape
 */
void ShapeGroup::movetotarget()
{
	RUN_IF_NOT_EMPTY(this->Shapes);
	RUN_IF_NOT_EMPTY(this->points);
	this->foreachShape([&](Shape& shape) {
		shape.movetotarget(this->points);
	});
}

/**
 * Moves each Point according to DeltaTime.
 *
 * @param DeltaTime - Time passed between Frames
 */
void ShapeGroup::move(float delta)
{
	RUN_IF_NOT_EMPTY(this->points);
	for (int i = 0; i < this->points.size(); i++)
	{
		this->points[i].Update(delta);
	}
}

/**
 * Shows each of the Shapes.
 *
 * @param Renderer - default SDL_Renderer
 */
void ShapeGroup::show(Renderer& renderer)
{
	RUN_IF_NOT_EMPTY(this->Shapes);
	RUN_IF_NOT_EMPTY(this->points);

	renderer.reset();
	renderer.translate(-320, -240);

	point average = this->ShapeAt(0).averagepoint(this->points);
	renderer.translate(average.getVector());
	this->foreachShape([&](Shape& shape)
	{
		renderer.fill(255, 0, 0, 255);
		shape.foreachPoint(this->points, [&](point& curPoint, point& nextPoint) {
			renderer.line(curPoint.getVector(), nextPoint.getVector());
		});
	});
}

/**
 * Updates the position of Each Shape and Point.
 *
 * @param DeltaTime - Time passed between Frames
 */
void ShapeGroup::Update(float deltaTime)
{
	RUN_IF_NOT_EMPTY(this->Shapes);
	RUN_IF_NOT_EMPTY(this->points);
	this->movetotarget();
	this->move(deltaTime);
}

/**
 * Returns the ID of a Shape from its Index in List.
 *
 * @param index - Shape Index in List
 */
int ShapeGroup::getShapeID(int index)
{
	RUN_IF_NOT_EMPTY(this->Shapes) -1;
	if (index >= this->Shapes.size() || index < 0) return -1;

	int shapeCount = 0;
	for (auto&[id, shape] : this->Shapes)
	{
		if (shapeCount == index) return id;

		shapeCount++;
	}
	return -1;
}

/**
 * Returns the Shape at its Index in List.
 *
 * @param index - Shape's Index in List
 */
Shape ShapeGroup::ShapeAt(int index)
{
	RUN_IF_NOT_EMPTY(this->Shapes) {};
	int idx = this->getShapeID(index);
	if(idx != -1) return this->Shapes.at( idx );
	return {};
}

void ShapeGroup::resolveCollision(int shapeID, const Shape& other, std::vector<point>& otherPoints)
{
	other.foreachPoint(otherPoints, [&](point& pt) {
		this->Shapes.at(shapeID).resolveCollisions(pt, this->points);
	});/*
	this->Shapes.at(shapeID).foreachPoint(otherPoints, [&](point& pt) {
		if (!other.isPointInShape(pt, otherPoints)) return;
		other.resolveCollisions(pt, otherPoints);
	});*/
}

/**
 * Deletes a Shape by its ID. 
 *
 * @param ID - Shape ID
 */
void ShapeGroup::deleteShapeByID(int ID)
{
	RUN_IF_NOT_EMPTY(this->Shapes);
	RUN_IF_NOT_EMPTY(this->points);
	if (this->Shapes.size() > 0)
	{
		if (this->Shapes.find(ID) != this->Shapes.end())
		{
			this->Shapes.at(ID).deleteShapePoints(this->points);
			this->Shapes.at(ID).deleteShape(this->Shapes);
		}
	}
	if (this->Shapes.empty())
	{
		this->points.clear();
		this->shapesCreated = 0;
	}
}

/**
 * Deletes a Shape from its Index in List.
 *
 * @param Index - Index in Shape list. 
 */
void ShapeGroup::deleteShapeAt(int index)
{
	return this->deleteShapeByID(this->getShapeID(index));
}

/**
 * Deletes this Shape Group if no Shapes exist. 
 *
 * @param ShapeGroups - List of ShapeGroups to delete this from. 
 */
bool ShapeGroup::delete_if_no_shapes(std::vector<ShapeGroup>& shapeGroups)
{
	RUN_IF_NOT_EMPTY(shapeGroups) false;
	if (this->Shapes.size() > 0) return false;

	for (int i = 0; i < shapeGroups.size(); i++)
	{
		if (&shapeGroups[i] == this)
		{
			shapeGroups.erase(shapeGroups.begin() + i);
			return true;
		}
	}
	return false;
}

/**
 * Calls A function for every Shape Creation. 
 *
 * @param Function - Run on each Shape. 
 */
void ShapeGroup::call_on_make_Shape(std::function<void(Shape&)> func)
{
	this->on_create = func;
}

FishackEnd
