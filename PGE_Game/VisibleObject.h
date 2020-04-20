#pragma once
#include "PhysicalObject.h"
#include "../../olcPixelGameEngine/olcPixelGameEngine.h"

struct Text {
	// Default constructor
	Text() :	 _ID(0), _position({ 0.0f, 0.0f }), _content("no content"),
				_color(olc::WHITE), _scale(1), _active(false) {}
	Text(int ID_, olc::vf2d position_, std::string content_, bool active_) :
		_ID(ID_), _position(position_), _content(content_),
		_color(olc::WHITE), _scale(1), _active(active_) {}
	int			_ID;
	olc::vf2d	_position;
	std::string _content;
	olc::Pixel	_color;
	int			_scale;
	bool		_active; // Indicates whether this text is to be drawn
};

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
	inline const auto getSprite(void) { return _spr; }
	inline const auto getSprite(void) const { return _spr; }
	inline const auto getDecal(void) { return _dec; }
	inline const auto getScale(void)	{ return _scale; }
	inline const auto getTint(void)		{ return _tint; }

	inline const auto	getText(void)	{ return _text; }
	inline bool			hasText(void)	{ return _has_active_text; }
	bool addText(Text text_);
	bool removeText(int ID_);
	void updateTextStatus();

	void setVisual(std::string file_path_);

protected:
	// Image storage
	olc::Sprite*	_spr;
	olc::Decal*		_dec;
	olc::vf2d		_scale; // In case where a sprite is drawn, only scale.x is used
	olc::Pixel		_tint;

	// Text storage
	std::map< int, Text >	_text;
	bool					_has_active_text; // Indicates whether the object has text to display
};

