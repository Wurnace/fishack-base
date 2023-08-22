#pragma once

#include "Math/Vector2.h"

FishackBegin

class Renderer
{
public:
	Renderer(SDL_Renderer* renderer) :renderer(renderer) {}
	Renderer(Renderer* r) {
		r = this;
	}
	void rotate(float radians);
	void translate(Vector t);
	void translate(float x, float y);

	Vector GetTransformed(Vector pos);
	Vector GetTransformed(float x, float y);

	void reset();

	void line(Vector p1, Vector p2);
	void fill(int r, int g, int b, int a);

private:
	SDL_Renderer* renderer;

	float Rotation = 0.0f;
	Vector Translation;
};

FishackEnd
