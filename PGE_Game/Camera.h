#pragma once
#include "PhysicalObject.h"

class Camera :
	public PhysicalObject
{
public:
	Camera() : PhysicalObject(0, "unnamed"), _width(320), _height(160) {}
	Camera(int width_, int height_) : PhysicalObject(0, "unnamed"), _width(width_), _height(height_) {}
	Camera(unsigned int UID_, std::string name_, int width_, int height_) : 
		PhysicalObject(UID_, name_), _width(width_), _height(height_) {}
	~Camera() {};

private:
	int _width, _height;
};

