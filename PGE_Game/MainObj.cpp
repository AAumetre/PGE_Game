#define OLC_PGE_APPLICATION
#include "Region.h"
#include "Scene.h"
#include "Camera.h"
#include "Character.h"
#include "HUD.h"
#include "Logger.h"

class Game : public olc::PixelGameEngine
{
public:
	Game() { sAppName = "An OOP game"; }

	// Pixel Game Engine methods
	bool OnUserCreate() override {
		// Create a region
		// _region = new Region(static_cast<int>(0), "First region", "../Sprites/Town.jpg");
		// _region->setCollisionZone("../Sprites/Town_Collisions.png");
		_region = new Region(static_cast<int>(0), "First region", "../Sprites/Map.png");
		_region->setCollisionZone("../Sprites/Map_Collisions.png");

		// Create the main character
		_player = new Player(1, "Toto", "../Sprites/Lui1.png");
		_player->setPosition({ 20, 20 });
		_player->setCollisionRect(0.25f, 0.75f, 0.8f, 1.0f);

		// ...and its arch-enemy
		_ennemy = new Character(4, "Titi", "../Sprites/Lui1.png");
		_ennemy->setPosition({ 20, 20 });
		_ennemy->setTint(olc::RED);
		_ennemy->setCollisionRect(0.25f, 0.75f, 0.8f, 1.0f);

		// Create the HUD
		_hud = new HUD(2, "Main HUD", _player);

		// Create 3 more layers layers
		for (int i = 0; i < 3; ++i) {
			CreateLayer();
			EnableLayer(i+1, true); // Layer 0 already exists
		}
		// Initialize the scene with these layers
		_scene = new Scene(3, "Main scene", _region, _player, 4);
		_scene->addCharacter(_ennemy);

		// Initialize the camera
		_camera = new Camera(ScreenWidth(), ScreenHeight());
		_camera->assignFocusPoint(_player);
		
		// Set _region to layer 2
		_scene->moveToLayer(2, _region);

		// Set the HUD to layer 1
		_scene->moveToLayer(1, _hud);
		_scene->moveToLayer(1, _ennemy);

		// Set the player to layer 0
		_scene->moveToLayer(0, _player);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		// Take-in user inputs
		Game::handleInputs(fElapsedTime);

		// Update other character's positions
		_ennemy->compute(fElapsedTime, 1.0e3*(_player->getPosition() - _ennemy->getPosition()));

		// Compute stuff
		_scene->detectCollisionsWithRegion(fElapsedTime);
		_camera->focus(fElapsedTime);
		Game::limitCameraToRegion();
		_scene->limitCharactersToRegion();
		_hud->update();

		// Render everything
		Game::renderAll();

		return true;
	}

	void renderAll(void) {
		unsigned int layer_index = 0;
		for (auto layer : *_scene->getLayers()) {
			Game::setupLayer(layer_index);
			for (auto elem : layer) {
				// Compute the on-screen position
				olc::vi2d elem_position = elem->getPosition() - _camera->getPosition();
				if (elem->getDecal() != nullptr) { // Prefer decals to sprites
					DrawDecal(elem_position, elem->getDecal(), elem->getScale(), elem->getTint());
				}
				else if (elem->getSprite() != nullptr) {
					DrawSprite(elem_position, elem->getSprite(), elem->getScale().x);
				}
				if (elem->hasText()) {
					for (const auto& msg : elem->getText()) {
						DrawString(msg.second._position, msg.second._content, msg.second._color, msg.second._scale);
					}
				}
			}
			layer_index++;
		}
		// Display zones for debug
		SetDrawTarget(nullptr);
		olc::vf2d zone_position = _player->getPosition() + _player->getCollisionRect()[0] - _camera->getPosition();
		DrawRect(zone_position, _player->getCollisionRect()[1] - _player->getCollisionRect()[0], olc::BLUE);
	}

	void setupLayer(unsigned int layer_index_) {
		switch (layer_index_) {
		case 0:
			SetPixelMode(olc::Pixel::NORMAL);
			SetDrawTarget(nullptr);
			Clear(olc::BLANK);
			SetPixelMode(olc::Pixel::NORMAL);
			break;

		case 1:
			SetPixelMode(olc::Pixel::NORMAL);
			SetDrawTarget(1);
			Clear(olc::BLANK);
			SetPixelMode(olc::Pixel::NORMAL);
			break;

		case 2:
			SetPixelMode(olc::Pixel::NORMAL);
			SetDrawTarget(2);
			Clear(olc::VERY_DARK_BLUE);
			SetPixelMode(olc::Pixel::NORMAL);
			break;

		case 3:
			SetPixelMode(olc::Pixel::NORMAL);
			SetDrawTarget(3);
			Clear(olc::VERY_DARK_BLUE);
			SetPixelMode(olc::Pixel::NORMAL);
			break;

		default:
			std::cout << "WARNING: an undefined layer has been reached" << std::endl;
		}
	}

	void handleInputs(float fElapsedTime) {
		olc::vf2d input_a = { 0.0f, 0.0f };
		// Up/Down/Left/Right directions
		if (GetKey(olc::Key::Q).bHeld) {
			input_a.x -= _player->getMaxAcceleration();
		}
		if (GetKey(olc::Key::D).bHeld) {
			input_a.x += _player->getMaxAcceleration();
		}
		if (GetKey(olc::Key::Z).bHeld) {
			input_a.y -= _player->getMaxAcceleration();
		}
		if (GetKey(olc::Key::S).bHeld) {
			input_a.y += _player->getMaxAcceleration();
		}

		// Space bar to jump
		if (GetKey(olc::Key::SPACE).bReleased) {
			input_a.y -= 5 * _player->getMaxAcceleration();
		}

		// Mouse
		if (GetMouse(0).bPressed) {
			_ennemy->setGhost(!_ennemy->isGhost());
		}

		// Updating the player's physical quantities
		_player->compute(fElapsedTime, input_a);
	}

	void limitCameraToRegion() {
		if (_camera->getPosition().x + ScreenWidth() > _region->getSprite()->width)
			_camera->setPositionX(_region->getSprite()->width - ScreenWidth());
		if (_camera->getPosition().x < 0) _camera->setPositionX( 0.0f );
		if (_camera->getPosition().y + ScreenHeight() > _region->getSprite()->height) 
			_camera->setPositionY(_region->getSprite()->height - ScreenHeight());
		if (_camera->getPosition().y < 0) _camera->setPositionY(0.0f);
	}

private:
	Scene*		_scene;
	Region*		_region;
	HUD*		_hud;
	Camera*		_camera;
	Player*		_player;
	Character*	_ennemy;
};

int main() {
	Game demo;
	if (demo.Construct(1280, 720, 1, 1))
		demo.Start();
	return 0;
}