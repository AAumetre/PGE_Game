#pragma once
#include "TrackedObject.h"

class PhysicalObject :
	public TrackedObject
{
public:
	PhysicalObject(unsigned int UID_, std::string name_) 
		: TrackedObject(UID_, name_), _position({0,0}) {};
	~PhysicalObject() {};
	
	// Accessors
	inline const auto getPosition(void)		{ return _position; }
	inline const auto getSpeed(void)		{ return _speed; }
	inline const auto getAcceleration(void) { return _acceleration; }

protected:
	// These are defined in the current region frame of reference
	olc::vi2d _position;
	olc::vf2d _speed;
	olc::vf2d _acceleration;
};

