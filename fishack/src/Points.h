#pragma once
#include <iostream>

#include "vector.h"

#define Round(x, presc) round(x * presc) / presc
// Round used in point::angleBetween();

struct point : public SDL_Point
{
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
		this->x /= sf;
		this->y /= sf;
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

		this->vel *= float(0.99);
		this->force.set(0 , 0);
	}

	void moveToTarget(Vector Target);
};

float point::angleBetween(point pt1, point pt2)
{
	point a = pt1 - *this;
	point b = pt2 - *this;

	float dot = a.x * b.x + a.y * b.y;
	float magA = a.getVector().mag();
	float magB = b.getVector().mag();
	return acos(Round(dot / (magA * magB), 1000));
}

void point::moveToTarget(Vector Target) {
	float dist = this->distTo(Target);
	if (dist < 1)
	{
		dist = 1;
	}
	this->applyForce((Target - this->getVector()) / dist);
}