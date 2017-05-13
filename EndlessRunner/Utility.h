#ifndef UTILITY_H
#define UTILITY_H

#include <SFML\Graphics.hpp>

using namespace sf;

namespace ut
{

	const float TO_DEGREE = 180 / 3.14159265358979f;
	const float TO_RADIAN = 3.14159265358979f / 180;

	float mag(const Vector2f& v)
	{
		return sqrt(pow(v.x, 2) + pow(v.y, 2));
	}

	float magP2(const Vector2f& v)
	{
		return pow(v.x, 2) + pow(v.y, 2);
	}

	float angle(Vector2f& v)
	{
		float angle = atan2(v.y, v.x) * TO_DEGREE;
		return angle >= 0 ? angle : angle + 360;
	}

	void limit(Vector2f& v, const float max)
	{
		if (mag(v) > max)
		{
			v /= mag(v);
			v *= max;
		}
	}

	float angleBetween(const Vector2f& v1, const Vector2f& v2)
	{
		return acos((v1.x * v2.x + v1.y * v2.y) / (mag(v1) * mag(v2))) * TO_DEGREE;
	}

	void normalize(Vector2f& v)
	{
		v /= mag(v);
	}

	float dist(Vector2f v1, Vector2f v2)
	{
		return mag(v1 - v2);
	}

	float distP2(Vector2f v1, Vector2f v2)
	{
		return magP2(v1 - v2);
	}

	Vector2f toVector(float mag, float angle)
	{
		return Vector2f(mag * cos(angle * TO_RADIAN), mag * sin(angle * TO_RADIAN));
	}

	float map(float value, float startMin, float startMax, float endMin, float endMax)
	{
		startMax -= startMin;
		value -= startMin;
		endMax -= endMin;
		return endMax * value / startMax + endMin;
	}
}

#endif