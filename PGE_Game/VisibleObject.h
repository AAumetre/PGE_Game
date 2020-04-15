#pragma once
#include "PhysicalObject.h"
#include "../../olcPixelGameEngine/olcPixelGameEngine.h"

class VisibleObject :
	public PhysicalObject
{
public:
	// Constructor & Destructor
	VisibleObject(unsigned int UID_, std::string name_) :
		PhysicalObject(UID_, name_),
		_scale({ 1.0f,1.0f }), _tint(olc::WHITE) {};
	VisibleObject(unsigned int UID_, std::string name_, std::string file_path_) :
		PhysicalObject(UID_, name_),
		_scale({ 1.0f,1.0f }), _tint(olc::WHITE)
	{
		setVisual(file_path_);
	};
	~VisibleObject() {};

	// Accessors
	inline const auto getSprite(void)	{ return _spr; }
	inline const auto getDecal(void)	{ return _dec; }
	inline const auto getScale(void)	{ return _scale; }
	inline const auto getTint(void)		{ return _tint; }

	void setVisual(std::string file_path_);

protected:
	olc::Sprite*	_spr;
	olc::Decal*		_dec;
	olc::vf2d		_scale; // In case where a sprite is drawn, only scale.x is used
	olc::Pixel		_tint;
};

