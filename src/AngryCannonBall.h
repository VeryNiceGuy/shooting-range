#pragma once

class AngryCannonBall
{
	public:

		AngryCannonBall();
		~AngryCannonBall();

		void draw();

		math::Vector3& getOrigin();
		math::Vector3& getDirection();
		float getRadius() const;

		void setVisible(bool visible);
		bool getVisible()const;

		void setVelocity(float velocity);
		float getVelocity()const;

	private:

		float _radius;
		float _velocity;
		bool _visible;

		Render::Texture *_ball;
		math::Vector3 _origin;
		math::Vector3 _direction;
};
