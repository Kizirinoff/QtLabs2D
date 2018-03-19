#pragma once
#include "Vector2f.h"

// ��������� RectF ������������ �������������, ������� �������� ����������� ���� ���������.
struct Rect2F
{
	Rect2F() = default;
	Rect2F(float x, float y, float width, float height);
	Rect2F(Vector2f topLeft, Vector2f sz);

	// ������� ������ �������� ����.
	Vector2f origin;
	// ������� �������������� (������, ������).
	Vector2f size;

	float leftEdge() const;
	float rightEdge() const;
	float topEdge() const;
	float bottomEdge() const;
};
