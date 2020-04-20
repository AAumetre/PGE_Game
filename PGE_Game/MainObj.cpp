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
		// Assign it its collision zone
		_player->setCollisionRect(	{	0.25f*static_cast<float>(_player->getSprite()->width),
										0.8f*static_cast<float>(_player->getSprite()->height) },
									{	0.75f*static_cast<float>(_player->getSprite()->width),
										1.0f*static_cast<float>(_player->getSprite()->height) } );

		// ...and its arch-enemy
		_ennemy = new Character(4, "Titi", "../Sprites/Lui1.png");
		_ennemy->setPosition({ 20, 20 });
		_ennemy->setTint(olc::RED);

		// Create the HUD
		_hud = new HUD(2, "Main HUD", _player);

		// Create 3 more layers layers
		for (int i = 0; i < 3; ++i) {
			CreateLayer();
			EnableLayer(i+1, true); // Layer 0 already exists
		}
		// Initialize the scene with these layers
		_scene = new Scene(3, "Main scene", _region, _player, 4);

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
		_ennemy->compute(fElapsedTime, 1.0e3*(_player->getPosition() - _ennemy->getPosition()));
		detectCollisionsWithRegion(fElapsedTime);

		// Compute stuff
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
				// Compute the on-screen (camera) position
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

	void detectCollisionsWithRegion(float fElapsedTime) {
		// Limit of pixels from which we consider enforcing the collision
		const int trigger = 20;
		// Define the region to analyze
		olc::vf2d p = _player->getPosition() - _region->getPosition();
		olc::vi2d start = p + _player->getCollisionRect()[0];
		olc::vi2d finish = p + _player->getCollisionRect()[1];
		int width = (finish - start).x;
		int height = (finish - start).y;

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

		// Only do the heavy lifting if there are enough pixels	
		if (pxl_count > trigger) { 
			// Step 1: Set the player back to an acceptable position
			_player->setPosition(_player->getPosition() - _player->getSpeed()*fElapsedTime);

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
				normal = normal/normal.mag();

				// Compute the speed vector to be added
				olc::vf2d added_speed;
				float dot_p = _player->getSpeed().x*normal.x + _player->getSpeed().y*normal.y;
				const float bounce_efficiency = 0.4f;
				// Bounce-off
				_player->setSpeed(bounce_efficiency*(_player->getSpeed() - 2*dot_p*normal));
			}
		}
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