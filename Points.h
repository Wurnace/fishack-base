#pragma once
#include <iostream>

#include "vector.h"

struct POINT : public SDL_Point
{
	float x = 0;
	float y = 0;
	POINT() {}
	POINT(float x, float y) :x(x), y(y) {}
	POINT(int x, int y) :x((float)x), y((float)y) {}
	POINT(float x, float y, Vector vel) :x(x), y(y), vel(vel) {}
	POINT(int x, int y, Vector vel) :x((float)x), y((float)y), vel(vel) {}

	void add(Vector other)
	{
		this->operator+=(other);
	}

	Vector getVector() {
		return { this->x, this->y };
	}

	POINT operator+(Vector other)
	{
		POINT result;
		result.x = this->x + other.x;
		result.y = this->y + other.x;
		return result;
	}

	void operator+=(Vector other)
	{
		this->x += other.x;
		this->y += other.x;
	}

	POINT operator-(Vector other)
	{
		POINT result;
		result.x = this->x - other.x;
		result.y = this->y - other.x;
		return result;
	}

	POINT operator-(POINT other)
	{
		POINT result;
		result.x = this->x - other.x;
		result.y = this->y - other.x;
		return result;
	}

	void operator-=(Vector other)
	{
		this->x -= other.x;
		this->y -= other.x;
	}

	void operator-=(POINT other)
	{
		this->x -= other.x;
		this->y -= other.x;
	}

	float distTo(POINT other)
	{
		return sqrt(
			(this->x - other.x) * (this->x - other.x) + 
			(this->y - other.y) * (this->y - other.y)
		);
	}

	Vector vel = { 0, 0 };
	Vector acc = { 0, 0 };
	void applyForce(Vector force)
	{
		this->acc += force;
	}

	void Update() {
		this->vel += this->acc;
		this->add(vel);

		this->vel *= 0.998;
		this->acc.set(0, 0);
	}

	void Update(float delta) {
		this->vel += this->acc * delta;
		this->add(vel * delta);

		this->vel *= 0.998;
		this->acc.set(0, 0);
	}
};
