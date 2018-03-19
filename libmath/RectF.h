#pragma once
#include "Vector2f.h"

// ��������� RectF ������������ �������������, ������� �������� ����������� ���� ���������.
struct RectF
{
	RectF() = default;
	RectF(float x, float y, float width, float height);
	RectF(Vector2f topLeft, Vector2f sz);

	// ������� ������ �������� ����.
	Vector2f origin;
	// ������� �������������� (������, ������).
	Vector2f size;

	float leftEdge() const;
	float rightEdge() const;
	float topEdge() const;
	float bottomEdge() const;
};
