#include "stdafx.h"
#include "AngryCannon.h"
#include "Enemy.h"
#include <cmath>
#include <ctime>

AngryCannon::AngryCannon(): _origin(512.0f, 128.0f, 0.0f),
							_angle(0),
							_enemies(0),
							_enemyCount(0),
							_aliveEnemyCount(0),
							_bottomDown(false),
							_bottomAnimationScalar(0.0f),
							_shotTime(0.0f),
							_ballParticleEffect(0)
{
	_angryCannon = Core::resourceManager.getTexture("AngryCannon");
	_angryCannonAxis = Core::resourceManager.getTexture("AngryCannonAxis");
	_angryCannonSeat = Core::resourceManager.getTexture("AngryCannonSeat");
	_angryCannonBottom = Core::resourceManager.getTexture("AngryCannonBottom");
}

AngryCannon::~AngryCannon()
{
	if(_enemies)
		delete[]_enemies;
}

void AngryCannon::createEnemies(size_t enemyCount)
{
	if(!enemyCount)
		return;

	if(_enemies)
		delete[]_enemies;

	_enemies = new Enemy[enemyCount];
	_aliveEnemyCount = _enemyCount = enemyCount;

	Render::Texture* textures[3] = {Core::resourceManager.getTexture("BlueUFO"),
									Core::resourceManager.getTexture("YellowUFO"),
									Core::resourceManager.getTexture("PinkUFO")};

	for(size_t i = 0; i < enemyCount; ++i)
		_enemies[i].setTexture(textures[rand() % 3]);

	_ball.setVisible(false);
	_ballEffectsContainer.KillAllEffects();
}

void AngryCannon::draw()
{
	Render::device.PushMatrix();

		Render::device.MatrixTranslate(_origin);

		Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);
		Render::device.MatrixTranslate(math::Vector3(-64, -84, 0));

		_angryCannon->Bind();
		Render::DrawRect(FRect(_angryCannon->getBitmapRect()),
						FRect(0,1,0,1));

	Render::device.PopMatrix();

	Render::device.PushMatrix();

		Render::device.MatrixTranslate(_origin);
		Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

		if(_bottomDown)
			Render::device.MatrixTranslate(math::Vector3(-16, -85, 0).Lerp(math::Vector3(-16, -95, 0),
																			_bottomAnimationScalar));
		else
			Render::device.MatrixTranslate(math::Vector3(-16, -85, 0));

		_angryCannonBottom->Bind();
		Render::DrawRect(FRect(_angryCannonBottom->getBitmapRect()),
						FRect(0,1,0,1));

	Render::device.PopMatrix();

	Render::device.PushMatrix();

		Render::device.MatrixTranslate(_origin);
		Render::device.MatrixTranslate(math::Vector3(-64, -110, 0));

		_angryCannonSeat->Bind();
		Render::DrawRect(FRect(_angryCannonSeat->getBitmapRect()),
						FRect(0,1,0,1));

	Render::device.PopMatrix();

	Render::device.PushMatrix();

		Render::device.MatrixTranslate(_origin);
		Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle * -1.0f);
		Render::device.MatrixTranslate(math::Vector3(-32, -32, 0));

		_angryCannonAxis->Bind();
		Render::DrawRect(FRect(_angryCannonAxis->getBitmapRect()),
						FRect(0,1,0,1));

	Render::device.PopMatrix();

	if(_enemyCount)
	{
		for(size_t i = 0; i < _enemyCount; ++i)
			_enemies[i].draw();
	}

	_ball.draw();
	_ballEffectsContainer.Draw();
}

void AngryCannon::update()
{
	const float currentTime = getCurrentTime();
	const float elapsedTimeSinceShot = currentTime - _shotTime;

	if(_bottomDown)
	{
		const float animationTime = 100.0f;
		
		elapsedTimeSinceShot >= animationTime ?
		_bottomDown = false :
		_bottomAnimationScalar = elapsedTimeSinceShot / animationTime;
	}

	if(_ball.getVisible())
	{
		if(_enemyCount)
		{
			for(size_t i = 0; i < _enemyCount; ++i)
			{
				if(_enemies[i].getVisible())
				{
					if((_enemies[i].getOrigin() - _ball.getOrigin()).Length() -
						(_ball.getRadius() + _enemies[i].getRadius()) <= 0)
					{
						_ball.setVisible(false);
						_ballParticleEffect->Finish();
						_enemies[i].blowUp();
						--_aliveEnemyCount;
					}
				}
			}
		}

		if(_ball.getOrigin().x - _ball.getRadius() > 1024.0f ||
		_ball.getOrigin().x + _ball.getRadius() < 0.0f ||
		_ball.getOrigin().y - _ball.getRadius() > 768.0f ||
		_ball.getOrigin().y + _ball.getRadius() < 0.0f)
		{
			_ball.setVisible(false);
			_ballParticleEffect->Finish();
		}
		else
		{
			_ball.getOrigin() += _ball.getDirection() *
			(_ball.getVelocity() / 1000.0f) * 
			elapsedTimeSinceShot;

			_ballParticleEffect->posX = _ball.getOrigin().x;
			_ballParticleEffect->posY = _ball.getOrigin().y;
		}
	}

	for(size_t i = 0; i < _enemyCount; ++i)
		if(_enemies[i].getVisible())
			_enemies[i].update();
}

void AngryCannon::updateDirection(float mouseX, float mouseY)
{
	math::Vector3 distance = (math::Vector3(mouseX, mouseY, 0.0f) - _origin);
	distance.Normalize();

	_angle = (180.0f / math::PI) * acosf(distance.DotProduct(math::Vector3::UnitY));

	if(mouseX > _origin.x)
		_angle = -_angle;

	if(_angle > 120.0f)
		_angle = 120.0f;

	if(_angle < -120.0f)
		_angle = -120.0f;
}

void AngryCannon::fire()
{
	if(_ball.getVisible())
		return;

	_shotTime = getCurrentTime();
	_bottomDown = true;

	_ball.setVisible(true);
	_ball.getDirection() = math::Vector3::UnitY.TransformNormal(math::Matrix4::RotationZ((math::PI / 180.0f) * _angle));
	_ball.getOrigin() = _origin + (_ball.getDirection() * 170.0f);

	_ballParticleEffect = _ballEffectsContainer.AddEffect("Flame");
	_ballParticleEffect->posX = _ball.getOrigin().x;
	_ballParticleEffect->posY = _ball.getOrigin().y;
	_ballParticleEffect->Reset();

	MM::manager.FadeSample(MM::manager.PlaySample("Shot"), 1);
}

math::Vector3& AngryCannon::getOrigin()
{
	return _origin;
}

float AngryCannon::getAngle() const
{
	return _angle;
}

size_t AngryCannon::getAliveEnemyCount() const
{
	return _aliveEnemyCount;
}

size_t AngryCannon::getEnemyCount() const
{
	return _enemyCount;
}

AngryCannonBall& AngryCannon::getBall()
{
	return _ball;
}

float AngryCannon::getCurrentTime() const
{
	return static_cast<float>(clock()) /
			(static_cast<float>(CLOCKS_PER_SEC) / 1000.0f);
}