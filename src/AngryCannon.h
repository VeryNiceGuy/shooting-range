#pragma once
#include "AngryCannonBall.h"

class Enemy;

class AngryCannon
{
	public:

		AngryCannon();
		~AngryCannon();

		void createEnemies(size_t enemyCount);

		void draw();
		void update();
		void updateDirection(float mouseX, float mouseY);
		void fire();

		math::Vector3& getOrigin();
		float getAngle() const;

		size_t getAliveEnemyCount() const;
		size_t getEnemyCount() const;

		AngryCannonBall& getBall();

	private:

		float getCurrentTime()const;

		Render::Texture* _angryCannon;
		Render::Texture* _angryCannonAxis;
		Render::Texture* _angryCannonSeat;
		Render::Texture* _angryCannonBottom;

		math::Vector3 _origin;
		float _angle;

		AngryCannonBall _ball;
		EffectsContainer _ballEffectsContainer;
		ParticleEffect* _ballParticleEffect;

		Enemy* _enemies;

		size_t _enemyCount;
		size_t _aliveEnemyCount;

		bool _bottomDown;
		float _bottomAnimationScalar;
		float _shotTime;
};
