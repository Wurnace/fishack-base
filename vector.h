#pragma once

#include "SDL.h"
#include <iostream>

struct Vector
{
public:
	Vector() {}
	Vector(float x, float y) : x(x), y(y) {}
	float x;
	float y;
	float mag()
	{
		return sqrt((this->x * this->x) + (this->y * this->y));
	}

	void set(float x, float y)
	{
		this->x = x; this->y = y;
	}

	void setMag(float mag)
	{
		this->x *= mag / this->mag();
		this->y *= mag / this->mag();
	}

	Vector setNewMag(float mag)
	{
		if (this->mag() != 0)
		{
			return Vector(this->x, this->y) / this->mag() * mag;
		}
		else
		{
			return { 0, 0 };
		}
	}

	Vector normalize()
	{
		return this->copy().setNewMag(1);
	}

	Vector copy()
	{
		return Vector(this->x, this->y);
	}

	float dot(Vector other)
	{
		return (this->x * other.x) + (this->y * other.y);
	}

	Vector operator+(Vector other)
	{
		Vector result;
		result.x = this->x + other.x;
		result.y = this->y + other.y;
		return result;
	}
	void operator+=(Vector other)
	{
		this->x += other.x;
		this->y += other.y;
	}
	Vector operator-(Vector other)
	{
		Vector result;
		result.x = this->x - other.x;
		result.y = this->y - other.y;
		return result;
	}
	void operator-=(Vector other)
	{
		this->x -= other.x;
		this->y -= other.y;
	}
	Vector operator*(Vector other)
	{
		Vector result;
		result.x = this->x * other.x;
		result.y = this->y * other.y;
		return result;
	}
	Vector operator*(float number)
	{
		Vector result;
		result.x = this->x * number;
		result.y = this->y * number;
		return result;
	}
	void operator*=(float number)
	{
		this->x *= number;
		this->y *= number;
	}
	void operator*=(Vector other)
	{
		this->x *= other.x;
		this->y *= other.y;
	}
	Vector operator/(Vector other)
	{
		Vector result;
		result.x = this->x / other.x;
		result.y = this->y / other.y;
		return result;
	}
	void operator/=(Vector other)
	{
		this->x /= other.x;
		this->y /= other.y;
	}
	Vector operator/(float number)
	{
		Vector result;
		result.x = this->x / number;
		result.y = this->y / number;
		return result;
	}
	void operator/=(float number)
	{
		this->x /= number;
		this->y /= number;
	}
	void rotate(float radians, Vector centre)
	{
		this->x = ((this->x - centre.x)*cos(radians)) - ((this->y - centre.y)*sin(radians)) + centre.x;
		this->y = ((this->x - centre.x)*sin(radians)) + ((this->y - centre.y)*cos(radians)) + centre.y;
	}
	Vector rotateNew(float radians, Vector centre)
	{
		Vector copy = this->copy();
		copy.rotate(radians, centre);
		return copy;
	}
	float heading()
	{
		return atan2(this->y, this->x);
	}
};
