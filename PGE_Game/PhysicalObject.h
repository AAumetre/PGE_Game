#pragma once
#include "TrackedObject.h"

class PhysicalObject :
	public TrackedObject
{
public:
	PhysicalObject(unsigned int UID_, std::string name_) 
		: TrackedObject(UID_, name_), _position({ 0,0 }), _acceleration({180.0f, 180.0f}),
		_min_speed({ 100.0f, 100.0f }), _max_speed({ 300.0f, 300.0f }) {};
	~PhysicalObject() {};
	
	// Accessors
	inline auto getPosition(void)		const { return _position; }
	inline auto getSpeed(void)			const { return _speed; }
	inline auto getAcceleration(void)	const { return _acceleration; }
	inline auto getMinSpeed(void)		const { return _min_speed; }
	inline auto getMaxSpeed(void)		const { return _max_speed; }
	inline auto getSize(void)			const { return _size; }

	// Set the position
	// inline void setPosition(olc::vf2d& position_)	{ _position = position_; }
	inline void setPosition(olc::vf2d position_)	{ _position = position_; }

	// Set the position only on x/y
	inline void setPositionX(float x_)	{ _position.x = x_; }
	inline void setPositionY(float y_)	{ _position.y = y_; }

	// Used to increment the position
	inline void incrPosition(olc::vf2d& displacement_)	{ _position += displacement_; }
	inline void incrPosition(olc::vf2d displacement_)	{ _position += displacement_; }

	// Used to increment the speed
	inline void incrSpeed(olc::vf2d& added_speed_)	{ _speed += added_speed_; }
	inline void incrSpeed(olc::vf2d added_speed_)	{ _speed += added_speed_; }

	// Set the current speed to its minimum/maximum value on either X or Y
	inline void setMinSpeedX(void) { _speed.x = _min_speed.x; }
	inline void setMinSpeedY(void) { _speed.y = _min_speed.y; }
	inline void setMaxSpeedX(void) { _speed.x = _max_speed.x; }
	inline void setMaxSpeedY(void) { _speed.y = _max_speed.y; }

protected:
	// These are defined in the current region frame of reference
	olc::vf2d _position;
	olc::vf2d _speed;
	olc::vf2d _acceleration;

	olc::vf2d _min_speed;
	olc::vf2d _max_speed;

	double		_mass;
	olc::vf2d	_size; // Width, Height of the object
};

