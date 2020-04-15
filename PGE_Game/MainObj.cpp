#define OLC_PGE_APPLICATION
#include "Region.h"
#include "Scene.h"
#include "Camera.h"
#include "Character.h"

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

		// Create 3 more layers layers
		for (int i = 0; i < 3; ++i) {
			CreateLayer();
			EnableLayer(i+1, true); // Layer 0 already exists
		}

		// Initialize the scene
		_scene = new Scene(_region, _player, 4);

		// Initialize the camera
		_camera = new Camera(ScreenWidth(), ScreenHeight());
		
		// Set _region to layer 1
		_scene->moveToLayer(1, _region);

		// Set the player to layer 0
		_scene->moveToLayer(0, _player);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		// Take-in user inputs

		// Compute stuff

		// Render everything
		renderAll();

		return true;
	}

	void renderAll(void) {
		unsigned int layer_index = 0;
		for (auto layer : *_scene->getLayers()) {
			Game::setupLayer(layer_index);
			for (auto elem : layer) {
				if (elem->getDecal() != nullptr) {
					DrawDecal(elem->getPosition(), elem->getDecal(), elem->getScale(), elem->getTint());
				}
				else if (elem->getSprite() != nullptr) {
					DrawSprite(elem->getPosition(), elem->getSprite(), elem->getScale().x);
				}
			}
			layer_index++;
		}
	}

	void setupLayer(unsigned int layer_index_) {
		switch (layer_index_) {
		case 0:
			SetDrawTarget(nullptr);
			Clear(olc::BLANK);
			SetPixelMode(olc::Pixel::NORMAL);
			break;

		case 1:
			SetDrawTarget(1);
			Clear(olc::BLANK);
			SetPixelMode(olc::Pixel::NORMAL);
			break;

		default:
			std::cout << "WARNING: an undefined layer has been reached" << std::endl;
		}

	}

private:
	Scene*	_scene;
	Region* _region;
	Camera* _camera;
	Player* _player;
};

int main() {
	Game demo;
	if (demo.Construct(1280, 720, 1, 1))
		demo.Start();
	return 0;
}