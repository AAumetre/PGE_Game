#include "Scene.h"
#include "Logger.h"

bool Scene::addCharacter(Character* char_)
{
	// Check if the given ID is unique
	if (_characters.count(char_->getUID()) > 0) {
		std::string msg = "the provided character ID(" + char_->getUID();
		msg += " is already present";
		warn(this, msg);
		return false;
	}
	// Proceed
	_characters[char_->getUID()] = char_;
}

bool Scene::removeCharacter(const Character * char_)
{
	size_t ret_erase = _characters.erase(char_->getUID());
	if (ret_erase == 1) return true; // Success
	else {
		if (ret_erase == 0) { // Not found
			std::string msg = "The character " + char_->getName();
			msg += " with UID ";
			msg += char_->getUID();
			msg += " does not exist.";
			warn(this, msg.c_str());
		}
		if (ret_erase > 1) {
			warn(this, "Wow, that should NOT have happened...");
		}
	}
}

void Scene::moveToLayer(unsigned int index_, VisibleObject* object_)
{
	// TODO: Remove from other layers

	// Move to new layer
	_layers.at(index_).push_back(object_);
}

void Scene::limitCharactersToRegion(void)
{
	for (auto& c : _characters) {
		olc::vf2d region_pos = c.second->getPosition();

		// Compare & cut-off
		if (region_pos.x < 0) region_pos.x = 0;
		if ((region_pos.x + c.second->getSprite()->width) > _region->getSprite()->width) {
			region_pos.x = _region->getSprite()->width - c.second->getSprite()->width;
		}
		if (region_pos.y < 0) region_pos.y = 0;
		if ((region_pos.y + c.second->getSprite()->height) > _region->getSprite()->height)
			region_pos.y = _region->getSprite()->height - c.second->getSprite()->height;

		// Set cut-off positions
		c.second->setPosition(region_pos);
	}
}

