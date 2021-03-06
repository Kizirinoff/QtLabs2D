﻿#pragma once

#include "RectF.h"
#include <QtGui/QPainter>
#include <libplatform/IGraphicsScene.h>

// Класс PoolTableScene реализует сцену бильярдного стола, с шариками и стенками.
class PoolTableScene : public platform::IGraphicsScene
{
public:
	PoolTableScene(const Rect2F &bounds);

	void update(float deltaSeconds) final;
	void redraw(QPainter &painter) final;

private:
	Vector2f m_ballPosition;
	const Vector2f m_ballSize;
	const Rect2F m_bounds;
	float m_time = 0;
};
