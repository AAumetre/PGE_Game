#define OLC_PGE_APPLICATION
#include "../../olcPixelGameEngine/olcPixelGameEngine.h"
constexpr auto M_PI = 3.14159265358979323846;

class Essai :
	public olc::PixelGameEngine {

public:
	Essai() {
		sAppName = "Essai";
	}
	// Sprites & Decals
	olc::Sprite* sprPerso	= nullptr;
	olc::Sprite* sprBar		= nullptr;
	olc::Sprite* sprTown	= nullptr;
	olc::Decal* decPerso	= nullptr;
	olc::Decal* decBar		= nullptr;
	olc::Decal* decTown		= nullptr;
	// Postions & speeds
	olc::vi2d back_pos{ 0,0 };
	olc::vi2d world_perso_pos;
	olc::vf2d perso_pos;
	olc::vf2d screen_pos;
	olc::vf2d perso_def_speed	{ 100, 100 };
	olc::vf2d perso_max_speed	{ 300, 300 };
	olc::vf2d perso_speed		{ 100, 100 };
	olc::vf2d perso_acc			{ 180, 180 };
	// No-scroll rectangle
	olc::vi2d no_scroll_rect_size;
	olc::vi2d no_scroll_rect_pos;
	float		no_scroll_ratio = 1.0f / 3.0f;
	// Layers's #
	int nLayerBack = 0;
	int nLayerHUD = 0;

	bool OnUserCreate() override {
		// Sprites assignment
		sprPerso	= new olc::Sprite("../Sprites/Lui1.png");
		sprTown		= new olc::Sprite("../Sprites/Town.jpg");
		sprBar		= new olc::Sprite("../Sprites/Bar.png");
		// Decals assignment
		decPerso	= new olc::Decal(sprPerso);
		decBar		= new olc::Decal(sprBar);
		decTown		= new olc::Decal(sprTown);
		no_scroll_rect_size.x = ScreenWidth() * no_scroll_ratio;
		no_scroll_rect_size.y = ScreenHeight() * no_scroll_ratio;
		no_scroll_rect_pos.x = no_scroll_rect_size.x;
		no_scroll_rect_pos.y = no_scroll_rect_size.y;

		// Layer creation
		nLayerHUD = CreateLayer();
		EnableLayer(nLayerHUD, true);
		SetDrawTarget(nullptr);

		nLayerBack = CreateLayer();
		EnableLayer(nLayerBack, true);
		SetDrawTarget(nullptr);

		// Perso position
		world_perso_pos.x = sprTown->width / 2;
		world_perso_pos.y = sprTown->height / 2;
		screen_pos = { 0.0f, 0.0f };

		return true;
	}

	

	bool OnUserUpdate(float fElapsedTime) override {
		// Background
		SetDrawTarget(nLayerBack);
		Clear(olc::BLANK);
		SetPixelMode(olc::Pixel::ALPHA);
		DrawDecal(back_pos, decTown);
		SetPixelMode(olc::Pixel::NORMAL);
		EnableLayer(nLayerBack, true);
		SetDrawTarget(nullptr);


		// What to do with this?
		Clear(olc::BLANK);

		// Handle inputs
		olc::vf2d mouse = { static_cast<float>(GetMouseX()),
							static_cast<float>(GetMouseY()) };
		// Accelerate
		if (GetKey(olc::Key::Z).bHeld) {
			perso_speed.y += perso_acc.y*fElapsedTime;
			if (perso_speed.y > perso_max_speed.y) perso_speed.y = perso_max_speed.y;
			world_perso_pos.y -= perso_speed.y*fElapsedTime;
		}
		if (GetKey(olc::Key::Q).bHeld){
			perso_speed.x += perso_acc.x*fElapsedTime;
			if (perso_speed.x > perso_max_speed.x) perso_speed.x = perso_max_speed.x;
			world_perso_pos.x -= perso_speed.x*fElapsedTime;
		}
		if (GetKey(olc::Key::S).bHeld) {
			perso_speed.y += perso_acc.y*fElapsedTime;
			if (perso_speed.y > perso_max_speed.y) perso_speed.y = perso_max_speed.y;
			world_perso_pos.y += perso_speed.y*fElapsedTime;
		}
		if (GetKey(olc::Key::D).bHeld) {
			perso_speed.x += perso_acc.x*fElapsedTime;
			if (perso_speed.x > perso_max_speed.x) perso_speed.x = perso_max_speed.x;
			world_perso_pos.x += perso_speed.x*fElapsedTime;
		}
		// Decelerate twice as much
		if (!GetKey(olc::Key::Q).bHeld && !GetKey(olc::Key::D).bHeld) {
			perso_speed.x -= 2.0f*perso_acc.x*fElapsedTime;
			if (perso_speed.x < perso_def_speed.x) perso_speed.x = perso_def_speed.x;
		}
		if (!GetKey(olc::Key::Z).bHeld && !GetKey(olc::Key::S).bHeld) {
			perso_speed.y -= 2.0f*perso_acc.y*fElapsedTime;
			if (perso_speed.y < perso_def_speed.y) perso_speed.y = perso_def_speed.y;
		}

		// Limit screen to world
		if (screen_pos.x + ScreenWidth() > sprTown->width)
			screen_pos.x = sprTown->width - ScreenWidth();
		if (screen_pos.x < 0) screen_pos.x = 0;
		if (screen_pos.y + ScreenHeight() > sprTown->height)
			screen_pos.y = sprTown->height - ScreenHeight();
		if (screen_pos.y < 0) screen_pos.y = 0;

		// Update background drawing position
		back_pos.x = -screen_pos.x;
		back_pos.y = -screen_pos.y;

		// Limit world position
		if (world_perso_pos.x < 0) world_perso_pos.x = 0;
		if (world_perso_pos.x + static_cast<float>(sprPerso->width) > sprTown->width)
			world_perso_pos.x = sprTown->width - static_cast<float>(sprPerso->width);
		if (world_perso_pos.y < 0) world_perso_pos.y = 0;
		if (world_perso_pos.y + static_cast<float>(sprPerso->height) > sprTown->height)
			world_perso_pos.y = sprTown->height - static_cast<float>(sprPerso->height);

		// Compute screen position wrt world position
		perso_pos.x = world_perso_pos.x - screen_pos.x;
		perso_pos.y = world_perso_pos.y - screen_pos.y;

		// Scrolling
		if ((perso_pos.x + sprPerso->width) > (1 - no_scroll_ratio)*ScreenWidth()) {
			int delta = static_cast<int>(
				perso_pos.x + sprPerso->width - (1 - no_scroll_ratio)*ScreenWidth());
			screen_pos.x += delta;
			// Check whether the right boundary of the world has been reached
			if (screen_pos.x + ScreenWidth() > sprTown->width){
				screen_pos.x = sprTown->width - ScreenWidth();
			} else {
				perso_pos.x -= delta;
			}
		}
		if (perso_pos.x < no_scroll_ratio*ScreenWidth()) {
			int delta = static_cast<int>(no_scroll_ratio*ScreenWidth() - perso_pos.x);
			screen_pos.x -= delta;
			// Check whether the left boundary of the world has been reached
			if (screen_pos.x <= 0) {
				screen_pos.x = 0;
			} else {
				perso_pos.x += delta;
			}
		}
		if ((perso_pos.y + sprPerso->height) > (1 - no_scroll_ratio)*ScreenHeight()) {
			int delta = static_cast<int>(
				perso_pos.y + sprPerso->height - (1 - no_scroll_ratio)*ScreenHeight());
			screen_pos.y += delta;
			// Check whether the right boundary of the world has been reached
			if (screen_pos.y + ScreenHeight() > sprTown->height){
				screen_pos.y = sprTown->height - ScreenHeight();
			} else {
				perso_pos.y -= delta;
			}
		}
		if (perso_pos.y < no_scroll_ratio*ScreenHeight()) {
			int delta = static_cast<int>(no_scroll_ratio*ScreenHeight() - perso_pos.y);
			screen_pos.y -= delta;
			// Check whether the left boundary of the world has been reached
			if (screen_pos.y <= 0) {
				screen_pos.y = 0;
			} else {
				perso_pos.y += delta;
			}
		}

		// Drawing the scene
		// Layer 0
		DrawDecal(perso_pos, decPerso);
		olc::vf2d bar_perso = { perso_pos.x + 50, perso_pos.y + 50 };

		// HUD layer
		SetDrawTarget(nLayerHUD);
		Clear(olc::BLANK);
		SetPixelMode(olc::Pixel::NORMAL);
		// Display the player's postion in the world
		std::ostringstream world_player_position;
		world_player_position << "WORLD     Position X: " << world_perso_pos.x << " Y: " << world_perso_pos.y;
		DrawString({ 20,20 }, world_player_position.str());
		// Display the player's postion
		std::ostringstream player_position;
		player_position << "CAMERA    Position X: " << perso_pos.x << " Y: " << perso_pos.y;
		DrawString({ 20,40 }, player_position.str());
		// Display the player's speed
		std::ostringstream player_speed;
		player_speed << "Speed    X: " << perso_speed.x << " Y: " << perso_speed.y;
		DrawString({ 20,60 }, player_speed.str());
		// Display the time
		std::ostringstream current_time;
		current_time << "Time per frame: " << fElapsedTime*1e3 << " ms";
		DrawString({ 20,80 }, current_time.str());
		float scale_bar = 4e-2;
		olc::vf2d bar_pos;
		// Display the speed bar
		float max_speed_norm = std::sqrt(	perso_max_speed.x*perso_max_speed.x +
											perso_max_speed.y*perso_max_speed.y);
		float perso_speed_norm = std::sqrt(	perso_speed.x*perso_speed.x +
											perso_speed.y*perso_speed.y);
		int nBars = 20 * perso_speed_norm / max_speed_norm;
		for (int i = 0; i < nBars; ++i) {
			// Height and width mixed because of rotation
			bar_pos.x = (2+i) * sprBar->height*scale_bar; 
			bar_pos.y = ScreenHeight() - 1.25f * sprBar->width*scale_bar;
			DrawRotatedDecal(bar_pos, decBar, static_cast<float>(M_PI / 2.0f), 
				{ 0.0f,0.0f }, {scale_bar, scale_bar});
		}
		// Display the no-scroll zone
		DrawRect(no_scroll_rect_pos, no_scroll_rect_size);
		SetPixelMode(olc::Pixel::NORMAL);
		EnableLayer(nLayerHUD, true);
		SetDrawTarget(nullptr);

		return true;
	}
};


int main() {
	Essai demo;
	if (demo.Construct(1280, 720, 1, 1))
		demo.Start();

	return 0;
}
