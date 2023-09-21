#pragma once

#include "SDL.h"
#include <iostream>

FishackBegin

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
		if (this->mag() == 0) return;
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

	void limit(float maxMag)
	{
		if (this->mag() > maxMag)
		{
			this->setMag(maxMag);
		}
	}

	Vector normalize()
	{
		return this->setNewMag(1);
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

	void rotate(double Angle, Vector Pivot = { 0, 0 })
	{
		return this->rotate(float(Angle), Pivot);
	}

	void rotate(float Angle, Vector Pivot = { 0, 0 })
	{
		if (int(Angle * 180 / M_PI) % 360 == 0)
			return;

		float s = sin(Angle);
		float c = cos(Angle);

		float x = this->x - Pivot.x;
		float y = this->y - Pivot.y;

		this->x = (x * c) - (y * s) + Pivot.x;
		this->y = (x * s) + (y * c) + Pivot.y;

	}

	Vector rotateNew(double Angle, Vector Pivot = { 0, 0 })
	{
		return this->rotateNew(float(Angle), Pivot);
	}

	Vector rotateNew(float radians, Vector Pivot = { 0, 0 })
	{
		Vector myCopy = this->copy();
		myCopy.rotate(radians, Pivot);
		return myCopy;
	}

	float heading()
	{
		if (this->y >= 0) return acos(this->x / this->mag());
		return -acos(this->x / this->mag());
	}

	bool operator==(Vector other) { return (other.x == this->x) && (other.y == this->y); }
	bool operator!=(Vector other) { return !this->operator==(other); }
};

FishackEnd
