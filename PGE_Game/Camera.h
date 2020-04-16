#pragma once
#include "PhysicalObject.h"

class Camera :
	public PhysicalObject
{
public:
	Camera() : PhysicalObject(0, "unnamed camera"), _width(320), _height(160) {}
	Camera(int width_, int height_) :
		PhysicalObject(0, "unnamed camera"), _width(width_), _height(height_)
	{
		init();
	}
	Camera(unsigned int UID_, std::string name_, int width_, int height_) : 
		PhysicalObject(UID_, name_), _width(width_), _height(height_)
	{
		init();
	}
	~Camera() {};

	inline void assignFocusPoint(PhysicalObject* focus_point_) { _focus_point = focus_point_; }
	void focus(float fElapsedTime);
	void init();

private:
	PhysicalObject* _focus_point; // Point followed by the camera
	int _width;
	int _height;
	double _no_scroll_ratio;
};

