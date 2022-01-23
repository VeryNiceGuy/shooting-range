#include "stdafx.h"
#include "Enemy.h"
#include <cstdlib>
#include <ctime>

Enemy::Enemy(): _radius(32.0f),
				_enemy(0),
				_velocity(200.0f),
				_origin(static_cast<float>(utils::random(74.0f, 950.0f)),
						static_cast<float>(utils::random(480.0f, 672.0f)),
						0.0f),
				_visible(true),
				_lastTime(0.0f)
{
	_direction = math::Vector3::UnitY.TransformNormal(math::Matrix4::RotationZ((math::PI / 180.0f) * 
													static_cast<float>(rand() % 360)));
}

Enemy::~Enemy(){}

void Enemy::draw()
{
	if(!_enemy)
		return;

	if(_visible)
	{
		Render::device.PushMatrix();

			_enemy->Bind();
			Render::device.MatrixTranslate(_origin);
			Render::device.MatrixTranslate(math::Vector3(-64.0f, -32.0f, 0.0f));
			Render::DrawRect(FRect(_enemy->getBitmapRect()), FRect(0,1,0,1));

		Render::device.PopMatrix();
	}

	_effectContainer.Draw();
}

void Enemy::update()
{
	if(!_visible)
		return;

	const float currentTime = getCurrentTime();

	if(_lastTime == 0.0f || currentTime - _lastTime > 500.0f)
		_lastTime = currentTime;

	if(_origin.x + (_radius * 2.0f) >= 1024.0f)
	{
		_direction = math::Vector3::UnitY.TransformNormal(math::Matrix4::RotationZ((math::PI / 180.0f) * 
														static_cast<float>(rand() % 180)));
		_velocity = utils::random(200.0f, 300.0f);
	}

	if(_origin.x - (_radius * 2.0f) <= 0.0f)
	{
		_direction = math::Vector3::UnitY.TransformNormal(math::Matrix4::RotationZ((math::PI / 180.0f) * 
														-static_cast<float>(rand() % 180)));
		_velocity = utils::random(200.0f, 300.0f);
	}

	if(_origin.y + _radius >= 768.0f)
	{
		_direction = math::Vector3::UnitX.TransformNormal(math::Matrix4::RotationZ((math::PI / 180.0f) * 
														-static_cast<float>(rand() % 180)));
		_velocity = utils::random(200.0f, 300.0f);
	}

	if(_origin.y - _radius <= 384.0f)
	{
		_direction = math::Vector3::UnitX.TransformNormal(math::Matrix4::RotationZ((math::PI / 180.0f) * 
														static_cast<float>(rand() % 180)));
		_velocity = utils::random(200.0f, 300.0f);
	}

	_origin += _direction * (_velocity / 1000.0f) * (currentTime - _lastTime);
	_lastTime = currentTime;
}

void Enemy::blowUp()
{
	setVisible(false);

	ParticleEffect *effect = _effectContainer.AddEffect("Bang");
	effect->posX = _origin.x;
	effect->posY = _origin.y;
	effect->Reset();

	MM::manager.FadeSample(MM::manager.PlaySample("Explosion"),1);
}

math::Vector3& Enemy::getOrigin()
{
	return _origin;
}

void Enemy::setTexture(Render::Texture* texture)
{
	_enemy = texture;
}

void Enemy::setVisible(bool visible)
{
	_visible = visible;
}

bool Enemy::getVisible()const
{
	return _visible;
}

float Enemy::getRadius() const
{
	return _radius;
}

float Enemy::getCurrentTime() const
{
	return static_cast<float>(clock()) /
			(static_cast<float>(CLOCKS_PER_SEC) / 1000.0f);
}