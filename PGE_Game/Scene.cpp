#include "Scene.h"
#include "Logger.h"

bool Scene::addCharacter(Character* char_)
{
	// Check if the given ID is unique
	if (_characters.count(char_->getUID()) > 0) {
		std::ostringstream msg;
		msg << "The provided character ID(" << char_->getUID() << " is already present";
		warn(this, msg.str());
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
			std::ostringstream msg;
			msg << "The character " << char_->getName() << " with UID " << char_->getUID() << " does not exist.";
			warn(this, msg.str());
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

void Scene::detectCollisionsWithRegion(float fElapsedTime) {
	for (auto& c : _characters) {
		if (!c.second->isGhost()) { // Checks if collisions are not to be ignored
			// Define the region to analyze
			olc::vf2d p = c.second->getPosition() - _region->getPosition();
			olc::vi2d start = p + c.second->getCollisionRect()[0];
			olc::vi2d finish = p + c.second->getCollisionRect()[1];
			int width = (finish - start).x;
			int height = (finish - start).y;

			// 0 surface detection
			if (width*height == 0) { 
				std::ostringstream msg;
				msg << "The character " << c.second->getName() << " with UID " <<
					c.second->getUID() << " has a 0 cross-section collision rectangle.";
				warn(this, msg.str());
				continue; // No break, we want to keep doing the work for the others
			}

			// Create the collision picture
			std::vector<std::vector<uint8_t>> coll_pic; // line/col
			coll_pic.resize(height);
			for (auto& line : coll_pic)
				line.resize(width);
			int pxl_count = 0;
			for (int j = start.y; j < finish.y; ++j) {
				for (int i = start.x; i < finish.x; ++i) {
					if (_region->getCollisionZone()->GetPixel(i, j).a == 0xff) {
						pxl_count++;
						coll_pic[(j - start.y)][(i - start.x)] = 1;
					}
					else {
						coll_pic[(j - start.y)][(i - start.x)] = 0;
					}
				}
			}

			// Limit of pixels from which we consider enforcing the collision
			const int trigger = 3;
			// Only do the heavy lifting if there are enough pixels	
			if (pxl_count > trigger) {
				// Step 1: Set the player back to an acceptable position
				c.second->setPosition(c.second->getPosition() - 2 * c.second->getSpeed()*fElapsedTime);

				// Step 2: Start computing the "normal" to the collision region
				olc::vf2d normal{ 0.0f, 0.0f };
				// Check for transitions line-by-line
				uint8_t previous = 0;
				for (int i = 0; i < height; ++i) {
					previous = coll_pic[i][0];
					for (int j = 1; j < width; ++j) { // Not interested in the first one
						if (coll_pic[i][j] - previous == 0) {}// Nothing happens
						else if (coll_pic[i][j] - previous == 1) { // W->B transition
							normal += {-1.0f, 0.0f};
							previous = 1;
						}
						else { // B->W transition
							normal += {1.0f, 0.0f};
							previous = 0;
						}
					}
				}
				// Check for transitions column-by-column
				for (int j = 0; j < width; ++j) {
					previous = coll_pic[0][j];
					for (int i = 1; i < height; ++i) { // Not interested in the first one
						if (coll_pic[i][j] - previous == 0) {}// Nothing happens
						else if (coll_pic[i][j] - previous == 1) { // W->B transition
							normal += {0.0f, -1.0f};
							previous = 1;
						}
						else { // B->W transition
							normal += {0.0f, 1.0f};
							previous = 0;
						}
					}
				}

				// Normalize the vector
				if (normal.mag() > 0.0f) {
					normal = normal / normal.mag();
					// Compute the speed vector to be added
					olc::vf2d added_speed;
					float dot_p = c.second->getSpeed().x*normal.x + c.second->getSpeed().y*normal.y;
					const float bounce_efficiency = 0.4f;
					// Bounce-off
					c.second->setSpeed(bounce_efficiency*(c.second->getSpeed() - 2 * dot_p*normal));
				}
				else {
					std::ostringstream msg;
					msg << "The normal vector for the character " << c.second->getName() << 
						" with UID " << c.second->getUID() << " has a 0 magnitude.";
					warn(this, msg.str());
				}
			}
		}
	}
}
