#include "AnimatedObject.h"
#include "Logger.h"

void AnimatedObject::setDefaultVisual(std::string path_, PoSize po_size_)
{
	// Extract
	if ((_spr == nullptr) && (_dec == nullptr)) {
		_spr = new olc::Sprite(/**/);
		_dec = new olc::Decal(_spr);
	}
	else if (_spr != nullptr) { // There is already a sprite assigned
		log(this, "Sprite already assigned, deleting the former one.");
		delete _spr;
		delete _dec;
		AnimatedObject::setDefaultVisual(path_);
	}
}

void AnimatedObject::update(float fElapsedTime)
{
	// Update the time
	_time += fElapsedTime;
	// Un-trigger animation
}

bool AnimatedObject::triggerAnimation(size_t ID_)
{
	// Check if the given ID exists
	if (_animations.count(ID_) == 0) {
		std::ostringstream msg;
		msg << "The provided animation ID(" << ID_ << ") does not exist";
		warn(this, msg.str());
		return false;
	}
	// Proceed
	// Initialize the time
	_time = 0.0f;
	// Set the current animation
	_current_animation = &_animations[ID_];
	return true;
}

auto AnimatedObject::getDecal()
{
	// If there is no animation, just return the default decal
	if (_current_animation == nullptr) {
		return _dec;
	}
	// Set the right decal, as a function of local time
}
