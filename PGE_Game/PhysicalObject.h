#pragma once
#include "TrackedObject.h"

class PhysicalObject :
	public TrackedObject
{
public:
	PhysicalObject(unsigned int UID_, std::string name_) 
		: TrackedObject(UID_, name_), _position({ 0,0 }), _speed({0.0f, 0.0f}),
		_acceleration({0.0f, 0.0f}), _max_acceleration(1.0e5), _max_speed(1.0e3),
		_breaking_force(1.0e2), _mass(50.0f), _min_speed(10.0f) {};
	~PhysicalObject() {};
	
	// Accessors
	inline auto getPosition(void)		const { return _position; }
	inline auto getSpeed(void)			const { return _speed; }
	inline auto getMaxSpeed(void)			const { return _max_speed; }
	inline auto getAcceleration(void)	const { return _acceleration; }
	inline auto getMaxAcceleration(void)	const { return _max_acceleration; }
	inline auto getSize(void)			const { return _size; }

	// These methods allow to by-pass the physical behavior of the object
	// Set the position
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

	// Computes speed & position
	void compute(float fElapsedTime, olc::vf2d input_force_);

protected:
	// These are defined in the current region frame of reference
	olc::vf2d _position;
	olc::vf2d _speed;
	olc::vf2d _acceleration;

	// Useful parameters for dynamic behavior.
	double _max_acceleration;
	double _breaking_force;
	double _max_speed; // Note: on normal objects, a natural maximum speed is reached
	double _min_speed;

	double		_mass;
	olc::vf2d	_size; // Width, Height of the object
};

