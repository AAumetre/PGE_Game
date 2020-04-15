#include "VisibleObject.h"

void VisibleObject::setVisual(std::string file_path_)
{
	if ((_spr == nullptr) && (_dec == nullptr)) {
		_spr = new olc::Sprite(file_path_);
		_dec = new olc::Decal(_spr);
	}
	else if (_spr != nullptr) {
		delete _spr;
		delete _dec;
		VisibleObject::setVisual(file_path_);
	}
}
