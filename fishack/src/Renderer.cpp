#ifndef FishackBegin
#define FishackBegin namespace Fishack {
#define FishackEnd }
#endif

#include <SDL.h>

#include "Renderer.h"

FishackBegin

void Renderer::rotate(float radians)
{
	this->Rotation += radians;
}
void Renderer::translate(Vector pos)
{
	this->Translation += pos.rotateNew(this->Rotation);
}
void Renderer::translate(float x, float y)
{
	return this->translate({ x, y });
}

Vector Renderer::GetTransformed(Vector pos)
{
	return pos.rotateNew(-this->Rotation) - this->Translation;
}
Vector Renderer::GetTransformed(float x, float y)
{
	return this->GetTransformed({ x, y });
}

void Renderer::reset()
{
	this->Translation.set(0.0f, 0.0f);
	this->Rotation = 0.0f;
}

void Renderer::line(Vector p1, Vector p2)
{
	Vector point1 = this->GetTransformed(p1);
	Vector point2 = this->GetTransformed(p2);

	SDL_RenderDrawLine(this->renderer, int(point1.x), int(point1.y), int(point2.x), int(point2.y));
}

void Renderer::fill(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
}

FishackEnd
