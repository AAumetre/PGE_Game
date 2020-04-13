#define OLC_PGE_APPLICATION
#include "../../olcPixelGameEngine/olcPixelGameEngine.h"

class Essai :
	public olc::PixelGameEngine {

public:
	Essai() {
		sAppName = "Essai";
	}

	olc::Sprite* sprPerso	= nullptr;
	olc::Decal* decPerso	= nullptr;
	olc::vf2d perso_pos{ ScreenWidth() / 2, ScreenHeight() / 2 };
	olc::vf2d perso_speed{ 100, 100 };

	bool OnUserCreate() override {
		sprPerso = new olc::Sprite("../Sprites/Lui1.png");
		decPerso = new olc::Decal(sprPerso);
		return true;
	}

	

	bool OnUserUpdate(float fElapsedTime) override {
		// Background
		Clear(olc::DARK_BLUE);

		// Handle inputs
		olc::vf2d mouse = { static_cast<float>(GetMouseX()),
							static_cast<float>(GetMouseY()) };
		if (GetKey(olc::Key::Z).bHeld) perso_pos.y -= perso_speed.y*fElapsedTime;
		if (GetKey(olc::Key::Q).bHeld) perso_pos.x -= perso_speed.x*fElapsedTime;
		if (GetKey(olc::Key::S).bHeld) perso_pos.y += perso_speed.y*fElapsedTime;
		if (GetKey(olc::Key::D).bHeld) perso_pos.x += perso_speed.x*fElapsedTime;

		// Limit position to screen
		if (perso_pos.x <= 0) perso_pos.x = 0;
		if (perso_pos.y <= 0) perso_pos.y = 0;
		if (perso_pos.x > ScreenWidth()) perso_pos.x = ScreenWidth();
		if (perso_pos.y > ScreenHeight()) perso_pos.y = ScreenHeight();



		// Drawing the scene
		DrawDecal(perso_pos, decPerso);

		return true;
	}
};


int main() {
	Essai demo;
	if (demo.Construct(640, 320, 1, 1))
		demo.Start();

	return 0;
}
