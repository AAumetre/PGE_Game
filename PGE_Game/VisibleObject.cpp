#include "VisibleObject.h"

void VisibleObject::setVisual(std::string file_path_)
{
	if ((_spr == nullptr) && (_dec == nullptr)) {
		_spr = new olc::Sprite(file_path_);
		_dec = new olc::Decal(_spr);
		_size = {	static_cast<float>(_spr->width), 
					static_cast<float>(_spr->height) };
	}
	else if (_spr != nullptr) { // There is already a sprite assigned
		delete _spr;
		delete _dec;
		VisibleObject::setVisual(file_path_);
	}
}
