#pragma once

#include <iostream>

#include "Vector2.h"

#define Round(x, presc) round(x * presc) / presc
// Round used in point::angleBetween();

struct point : public SDL_Point
{
	std::vector<int> ShapeIDs;

	float x = 0;
	float y = 0;

	Vector targetOff;

	point() {}
	point(float x, float y) :x(x), y(y) {}
	point(int x, int y) :x((float)x), y((float)y) {}
	point(float x, float y, Vector vel) :x(x), y(y), vel(vel) {}
	point(int x, int y, Vector vel) :x((float)x), y((float)y), vel(vel) {}

	void setOffset(int targetOffx, int targetOffy) {
		this->targetOff = { float(targetOffx), float(targetOffy) };
	}
	void setOffset(float targetOffx, float targetOffy) {
		this->targetOff = { targetOffx, targetOffy };
	}
	void setOffset(point other)
	{
		this->targetOff = { other.x, other.y };
	}

	void set(float x, float y) {
		this->x = x;
		this->y = y;
	}
	void set(Vector other) {
		this->set(other.x, other.y);
	}

	Vector getVector() {
		return { this->x, this->y };
	}

	void add(Vector other) {
		this->operator+=(other);
	}
	point operator+(Vector other)
	{
		point result;
		result.x = this->x + other.x;
		result.y = this->y + other.y;
		return result;
	}
	void operator+=(Vector other)
	{
		this->x += other.x;
		this->y += other.y;
	}

	point operator-(point other)
	{
		point result;
		result.x = this->x - other.x;
		result.y = this->y - other.y;
		return result;
	}
	void operator-=(Vector other)
	{
		this->x -= other.x;
		this->y -= other.y;
	}

	point operator*(int sf)
	{
		point result;
		result.x *= sf;
		result.y *= sf;
		return result;
	}
	void operator*=(int sf)
	{
		this->x *= sf;
		this->y *= sf;
	}

	point operator/(int sf)
	{
		point result;
		result.x /= sf;
		result.y /= sf;
		return result;
	}
	void operator/=(int sf)
	{
		this->x /= sf; // Scale Factor
		this->y /= sf;
	}

	bool operator==(point& other)
	{
		if (other.x != this->x)			return false;
		if (other.y != this->y)			return false;
		if (other.vel != this->vel)		return false;
		if (other.force != this->force) return false;

		return true;
	}

	float distTo(Vector other)
	{
		return sqrt(
			(this->x - other.x) * (this->x - other.x) +
			(this->y - other.y) * (this->y - other.y)
		);
	}

	Vector vel = { 0, 0 };
	Vector force = { 0, 0 };
	void applyForce(Vector force)
	{
		this->force += force;
	}

	float angleBetween(point pt1, point pt2);

	void Update() {
		this->Update(1);
	}

	void Update(float delta) {
		this->vel += this->force * delta;
		this->add(vel * delta);

		this->vel *= 0.999f;
		this->force *= 0.25f;
	}

	void moveToTarget(Vector Target);
};

float point::angleBetween(point pt1, point pt2)
{
	float a = (pt1.getVector() - this->getVector()).heading();
	float b = (pt2.getVector() - this->getVector()).heading();

	return a - b;
}

void point::moveToTarget(Vector Target) {
	float dist = this->distTo(Target);
	if (dist < 25)
	{
		dist = 25;
	}
	this->applyForce((Target - this->getVector()) / dist);
}
