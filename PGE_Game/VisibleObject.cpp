#include "VisibleObject.h"
#include "Logger.h"

bool VisibleObject::addText(Text text_)
{
	// Check if the given ID does not already exist
	if (_text.count(text_._ID) > 0) {
		std::string msg = "the provided text ID(" + text_._ID;
		msg += " is already present";
		warn(this, msg);
		return false;
	}
	// Proceed
	_text[text_._ID] = text_;
	// Check if this text is to be displayed
	if (text_._active) _has_active_text = true;
	return true;
}

bool VisibleObject::removeText(int ID_)
{
	// Check if the given ID exists
	if (_text.count(ID_) == 1) {
		// Found it, check if it was active
		if (_text[ID_]._active) updateTextStatus(); // Maybe it was the last bit of text to be displayed
		// Now remove it
		_text.erase(ID_);
		return true;
	}
	// At this point, the element has not been found
	std::string msg = "the provided text ID(" + ID_;
	msg += " was not found";
	warn(this, msg);
	return false;
}

void VisibleObject::updateTextStatus()
{
	for (const auto& msg : _text) {
		if (msg.second._active) {
			// If at least one element is to be drawn
			_has_active_text = true;
			return;
		}
	}
	// No text to be drawn has been found
	_has_active_text = false;
}

void VisibleObject::setVisual(std::string file_path_)
{
	if ((_spr == nullptr) && (_dec == nullptr)) {
		_spr = new olc::Sprite(file_path_);
		_dec = new olc::Decal(_spr);
	}
	else if (_spr != nullptr) { // There is already a sprite assigned
		log(this, "Sprite already assigned, deleting the former one.");
		delete _spr;
		delete _dec;
		VisibleObject::setVisual(file_path_);
	}
}

void VisibleObject::setCollisionRect(float rx1_, float rx2_, float ry1_, float ry2_)
{
	if (_spr == nullptr) {
		std::string msg = "Collision rectangle for object " + _name;
		msg += " and UID " + _UID;
		msg += " could not be set: no sprite attached.";
		warn(this, msg);
		return;
	}

	// Ratios rx1, rx2 must be such that: 0 <= rx1 < rx2 <= 1 
	bool ratios_x_valid = (0.0f <= rx1_) && (rx1_ < rx2_) && (rx2_ <= 1.0f);
	// Idem for rxy 1 and ry2
	bool ratios_y_valid = (0.0f <= ry1_) && (ry1_ < ry2_) && (ry2_ <= 1.0f);
	if (!ratios_x_valid || !ratios_y_valid) {
		std::string msg = "Invalid ratios for collision rectangle given to " + _name;
		msg += " with UID " + _UID;
		warn(this, msg);
		return;
	}

	PhysicalObject::setCollisionRect(
		{	rx1_*static_cast<float>(this->getSprite()->width),
			ry1_*static_cast<float>(this->getSprite()->height) }
		,
		{	rx2_*static_cast<float>(this->getSprite()->width),
			ry2_*static_cast<float>(this->getSprite()->height) });
}
