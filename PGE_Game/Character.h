#pragma once
#include "VisibleObject.h"
class Character :
	public VisibleObject
{
public:
	Character(unsigned int UID_, std::string name_) :
		VisibleObject(UID_, name_), _is_controlled(false), _health(100.0f), _is_ghost(false) {};
	Character(unsigned int UID_, std::string name_, std::string file_path_) :
		VisibleObject(UID_, name_, file_path_), _is_controlled(false), _health(100.0f), _is_ghost(false) {};
	~Character() {};

protected:
	bool _is_controlled;
	float _health;
	bool _is_ghost; // Not implemented yet, allows to ignore collisions

};

class Player :
	public Character
{
public:
	Player(unsigned int UID_, std::string name_) : Character(UID_, name_)
	{
		_is_controlled = true;
	};
	Player(unsigned int UID_, std::string name_, std::string file_path_) :
		Character(UID_, name_, file_path_)
	{
		_is_controlled = true;
	};
	~Player() {};
};

