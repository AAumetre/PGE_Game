#include "HUD.h"

void HUD::initHUDforPlayer()
{
	// Display the player's postion in the world
	std::ostringstream content_pp;
	content_pp << "WORLD     Position X: " << _player->getPosition().x << " Y: " << _player->getPosition().y;
	Text text_pp(1, {20,20}, content_pp.str(), true);
	addText(text_pp);
	// Display the player's speed in the world
	std::ostringstream content_ps;
	content_ps << "WORLD     Speed    X: " << _player->getSpeed().x << " Y: " << _player->getSpeed().y;
	Text text_ps(2, {20,40}, content_ps.str(), true);
	addText(text_ps);
}

void HUD::update()
{
	// Display the player's postion in the world
	std::ostringstream content_pp;
	content_pp << "WORLD     Position X: " << _player->getPosition().x << " Y: " << _player->getPosition().y;
	_text.at(1)._content = content_pp.str();
	// Display the player's speed in the world
	std::ostringstream content_ps;
	content_ps << "WORLD     Speed    X: " << _player->getSpeed().x << " Y: " << _player->getSpeed().y;
	_text.at(2)._content = content_ps.str();
}
