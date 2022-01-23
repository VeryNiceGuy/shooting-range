#include "stdafx.h"
#include "AngryCannonBall.h"

AngryCannonBall::AngryCannonBall(): _radius(32),
									_visible(false),
									_velocity(50.0f)
{
	_ball = Core::resourceManager.getTexture("AngryCannonBall");
}

AngryCannonBall::~AngryCannonBall(){}

void AngryCannonBall::draw()
{
	if(!_visible)
		return;

	Render::device.PushMatrix();

		_ball->Bind();
		Render::device.MatrixTranslate(_origin);
		Render::device.MatrixTranslate(math::Vector3(-32.0f, -32.0f, 0.0f));
		Render::DrawRect(FRect(_ball->getBitmapRect()), FRect(0,1,0,1));

	Render::device.PopMatrix();
}

math::Vector3& AngryCannonBall::getOrigin()
{
	return _origin;
};

math::Vector3& AngryCannonBall::getDirection()
{
	return _direction;
};

float AngryCannonBall::getRadius()const
{
	return _radius;
};

void AngryCannonBall::setVisible(bool visible)
{
	_visible = visible;
}

bool AngryCannonBall::getVisible()const
{
	return _visible;
}

void AngryCannonBall::setVelocity(float velocity)
{
	_velocity = velocity;
}

float AngryCannonBall::getVelocity()const
{
	return _velocity;
}