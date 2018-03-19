#pragma once
#include "Vector2f.h"

// ��������� RectF ������������ �������������, ������� �������� ����������� ���� ���������.
struct Rect2F
{
	Rect2F() = default;

	Rect2F(float x, float y, float width, float height)
		: origin(x, y)
		, size(width, height)
	{
	}

	// ������� ������ �������� ����.
	Vector2f origin;
	// ������� �������������� (������, ������).
	Vector2f size;

	float leftEdge() const
	{
		return origin.x;
	}

	float rightEdge() const
	{
		return origin.x + size.x;
	}

	float topEdge() const
	{
		return origin.y;
	}

	float bottomEdge() const
	{
		return origin.y + size.y;
	}
};
