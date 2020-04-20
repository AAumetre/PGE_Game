#include "PhysicalObject.h"

void PhysicalObject::compute(float fElapsedTime, olc::vf2d input_force_)
{
	// We only want to set the speed to 0 if the speed was previously above a given threshold
	bool cappable = (_speed.mag() > _min_speed);

	// Compute the total acceleration & integrate
	olc::vf2d total_acceleration = (input_force_ - _breaking_force * _speed) / _mass;
	_speed += fElapsedTime * total_acceleration;
	_position += fElapsedTime * _speed;

	// Lower-bound
	if (cappable && (_speed.mag() < _min_speed)) _speed = { 0.0f, 0.0f };

	// Higher-bound
	if (_speed.mag() > _max_speed) {
		_speed = _speed / _speed.mag() * _max_speed;
	}
}

void PhysicalObject::setCollisionRect(olc::vf2d x1_, olc::vf2d x2_)
{
	_collision_rect[0] = x1_;
	_collision_rect[1] = x2_;
}
