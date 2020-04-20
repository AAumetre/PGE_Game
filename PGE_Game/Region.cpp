#include "Region.h"

void Region::setCollisionZone(std::string file_path_)
{
	_collision_zone = new olc::Sprite(file_path_);
}
