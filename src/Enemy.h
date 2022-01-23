#pragma once

class Enemy
{
	public:

		Enemy();
		~Enemy();

		void draw();
		void update();
		void blowUp();

		math::Vector3& getOrigin();
		void setTexture(Render::Texture* texture);

		void setVisible(bool visible);
		bool getVisible()const;

		float getRadius() const;

	private:

		float getCurrentTime() const;

		float _radius;
		float _velocity;
		float _lastTime;

		Render::Texture *_enemy;
		math::Vector3 _origin;
		math::Vector3 _direction;
		bool _visible;

		EffectsContainer _effectContainer;
};