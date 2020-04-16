#define OLC_PGE_APPLICATION
#include "Region.h"
#include "Scene.h"
#include "Camera.h"
#include "Character.h"
#include "HUD.h"

class Game : public olc::PixelGameEngine
{
public:
	Game() { sAppName = "An OOP game"; }


	// Pixel Game Engine methods
	bool OnUserCreate() override {
		// Create a region
		_region = new Region(static_cast<int>(0), "First region", "../Sprites/Town.jpg");

		// Create the main character
		_player = new Player(1, "Toto", "../Sprites/Lui1.png");
		_player->setPosition({ 200,200 });

		// Create the HUD
		_hud = new HUD(2, "Main HUD", _player);

		// Create 3 more layers layers
		for (int i = 0; i < 3; ++i) {
			CreateLayer();
			EnableLayer(i+1, true); // Layer 0 already exists
		}
		// Initialize the scene with these layers
		_scene = new Scene(_region, _player, 4);

		// Initialize the camera
		_camera = new Camera(ScreenWidth(), ScreenHeight());
		_camera->assignFocusPoint(_player);
		
		// Set _region to layer 2
		_scene->moveToLayer(2, _region);

		// Set the HUD to layer 1
		_scene->moveToLayer(1, _hud);

		// Set the player to layer 0
		_scene->moveToLayer(0, _player);

		return true;
	}

	// double time_out = 0.0f;

	bool OnUserUpdate(float fElapsedTime) override {
		// Take-in user inputs
		handleInputs(fElapsedTime);

		// Compute stuff
		_camera->focus(fElapsedTime);
		limitCameraToRegion();
		_hud->update();

		// Render everything
		renderAll();

		/*time_out += fElapsedTime;
		if (time_out > 0.5) {
			std::cout << "=========================================================================" << std::endl;
			std::cout << "Elapsed time: " << 1e3*fElapsedTime << " ms" << std::endl;
			std::cout << "Position: " << _player->getPosition().x << " " << _player->getPosition().y << std::endl;
			std::cout << "Speed: " << _player->getSpeed().x << " " << _player->getSpeed().y << std::endl;
			time_out = 0.0;
		}*/

		return true;
	}

	void renderAll(void) {
		// Should depend on the camera
		// The positions should be computed via the camera
		unsigned int layer_index = 0;
		for (auto layer : *_scene->getLayers()) {
			Game::setupLayer(layer_index);
			for (auto elem : layer) {
				// Compute the on-screen (camera) position
				olc::vi2d elem_position = elem->getPosition() - _camera->getPosition();
				if (elem->getDecal() != nullptr) {
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
			Clear(olc::WHITE);
			SetPixelMode(olc::Pixel::NORMAL);
			break;

		case 3:
			SetPixelMode(olc::Pixel::NORMAL);
			SetDrawTarget(3);
			Clear(olc::WHITE);
			SetPixelMode(olc::Pixel::NORMAL);
			break;

		default:
			std::cout << "WARNING: an undefined layer has been reached" << std::endl;
		}
	}

	void handleInputs(float fElapsedTime) {
		// Accelerate
		if (GetKey(olc::Key::Q).bHeld) {
			_player->incrSpeed({ _player->getAcceleration().x*fElapsedTime, 0.0f });
			if (_player->getSpeed().x > _player->getMaxSpeed().x) _player->setMaxSpeedX();
			_player->incrPosition({ -_player->getSpeed().x*fElapsedTime, 0});
		}
		if (GetKey(olc::Key::D).bHeld) {
			_player->incrSpeed({ _player->getAcceleration().x*fElapsedTime, 0.0f });
			if (_player->getSpeed().x > _player->getMaxSpeed().x) _player->setMaxSpeedX();
			_player->incrPosition({ _player->getSpeed().x*fElapsedTime, 0});
		}
		if (GetKey(olc::Key::Z).bHeld) {
			_player->incrSpeed({ 0.0f, _player->getAcceleration().y*fElapsedTime });
			if (_player->getSpeed().y > _player->getMaxSpeed().y) _player->setMaxSpeedY();
			_player->incrPosition({ 0, -_player->getSpeed().y*fElapsedTime } );
		}
		if (GetKey(olc::Key::S).bHeld) {
			_player->incrSpeed({ 0.0f, _player->getAcceleration().y*fElapsedTime });
			if (_player->getSpeed().y > _player->getMaxSpeed().y) _player->setMaxSpeedY();
			_player->incrPosition({ 0, _player->getSpeed().y*fElapsedTime } );
		}
		// Decelerate twice as much
		if (!GetKey(olc::Key::Q).bHeld && !GetKey(olc::Key::D).bHeld) {
			_player->incrSpeed({-2.0f*_player->getAcceleration().x*fElapsedTime, 0.0f});
			if (_player->getSpeed().x < _player->getMinSpeed().x) _player->setMinSpeedX();
		}
		if (!GetKey(olc::Key::Z).bHeld && !GetKey(olc::Key::S).bHeld) {
			_player->incrSpeed({ 0.0f, -2.0f*_player->getAcceleration().x*fElapsedTime });
			if (_player->getSpeed().y < _player->getMinSpeed().y) _player->setMinSpeedY();
		}
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
	Scene*	_scene;
	Region* _region;
	HUD*	_hud;
	Camera* _camera;
	Player* _player;
};

int main() {
	Game demo;
	if (demo.Construct(1280, 720, 1, 1))
		demo.Start();
	return 0;
}