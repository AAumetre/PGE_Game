#include "Camera.h"
#include "Logger.h"

void Camera::focus(float fElapsedTime)
{
	if (!_focus_point) { // No focus point was set
		warn(this, "no focus point was set for this camera.");
		return;
	}

	// Compute focus point position on the screen
	olc::vf2d focus_p = _focus_point->getPosition() - _position;

	// Compute the displacement wrt to the center of the screen
	olc::vf2d center = { static_cast<float>(_width/2.0f), static_cast<float>(_height/2.0f) };
	olc::vf2d dis_center = focus_p - center;

	// Compute the dynamic behavior of the camera wrt to the focus_p
	// TODO: implement simple spring-like dynamic:	m*x" - k*x = 0
	float focus_smoothing = 1.0f;
	// olc::vf2d delta = focus_smoothing * dis_center*fElapsedTime;
	olc::vf2d delta = dis_center;
	
	_position += delta;


	/* First version with a rectangle, not working at the moment
	// Out-of-focus limit inside the screen
	//     A +------------+B
	//P+     |			  |
	//		 |            |
	//     D +------------+C
	olc::vf2d A{static_cast<float>(_no_scroll_ratio*_width),
		static_cast<float>(_no_scroll_ratio*_height)};
	olc::vf2d B{static_cast<float>((1-_no_scroll_ratio)*_width),
		static_cast<float>(_no_scroll_ratio*_height)};
	olc::vf2d C{static_cast<float>((1-_no_scroll_ratio)*_width),
		static_cast<float>((1-_no_scroll_ratio)*_height)};
	olc::vf2d D{static_cast<float>(_no_scroll_ratio*_width),
		static_cast<float>((1-_no_scroll_ratio)*_height)};
	olc::vf2d AP = focus_p - A;
	olc::vf2d BP = focus_p - B;
	olc::vf2d CP = focus_p - C;
	olc::vf2d DP = focus_p - D;
	olc::vf2d AB = B - A;
	olc::vf2d AD = D - A;
	float APdotAB = AP.x*AB.x + AP.y*AB.y;
	float APdotAD = AP.x*AD.x + AP.y*AD.y;
	bool focus_point_inside_zone =	(0.0f < APdotAB) && (APdotAB < AB.mag2()) &&
									(0.0f < APdotAD) && (APdotAD < AD.mag2());

	if (!focus_point_inside_zone) {
		if ((AP.mag2() < BP.mag2()) && (AP.mag2() < CP.mag2()) && (AP.mag2() < DP.mag2())) {
			// A is the closest point
			//std::cout << "AP= " << AP.x << " " << AP.y << std::endl;
			_position += AP;
		}
		if ((BP.mag2() < CP.mag2()) && (BP.mag2() < DP.mag2()) && (BP.mag2() < AP.mag2())) {
			// B is the closest point
			_position += BP;
		}
		if ((CP.mag2() < DP.mag2()) && (CP.mag2() < AP.mag2()) && (CP.mag2() < BP.mag2())) {
			// C is the closest point
			_position += CP;
		}
		if ((DP.mag2() < AP.mag2()) && (DP.mag2() < BP.mag2()) && (DP.mag2() < CP.mag2())) {
			// D is the closest point
			_position += DP;
		}
	}

	// Scrolling
	/*float delta = focus_p.x + _focus_point->getSize().x - (1 - _no_scroll_ratio)*_width;
	if ( delta > 0.0f) {
		focus_p.x += delta;
		// Check whether the right boundary of the world has been reached
		if (_position.x + _width > _focus_point->getSize().x) {
			float new_position =  static_cast<float>(_focus_point->getSize().x - _width);
			_position.x = new_position;
		}
		else {
			focus_p.x -= delta;
		}
	}*/
	/*if (perso_pos.x < no_scroll_ratio*ScreenWidth()) {
		int delta = static_cast<int>(no_scroll_ratio*ScreenWidth() - perso_pos.x);
		screen_pos.x -= delta;
		// Check whether the left boundary of the world has been reached
		if (screen_pos.x <= 0) {
			screen_pos.x = 0;
		}
		else {
			perso_pos.x += delta;
		}
	}
	if ((perso_pos.y + sprPerso->height) > (1 - no_scroll_ratio)*ScreenHeight()) {
		int delta = static_cast<int>(
			perso_pos.y + sprPerso->height - (1 - no_scroll_ratio)*ScreenHeight());
		screen_pos.y += delta;
		// Check whether the right boundary of the world has been reached
		if (screen_pos.y + ScreenHeight() > sprTown->height) {
			screen_pos.y = sprTown->height - ScreenHeight();
		}
		else {
			perso_pos.y -= delta;
		}
	}
	if (perso_pos.y < no_scroll_ratio*ScreenHeight()) {
		int delta = static_cast<int>(no_scroll_ratio*ScreenHeight() - perso_pos.y);
		screen_pos.y -= delta;
		// Check whether the left boundary of the world has been reached
		if (screen_pos.y <= 0) {
			screen_pos.y = 0;
		}
		else {
			perso_pos.y += delta;
		}
	}
	*/
}

void Camera::init()
{
	_no_scroll_ratio = 1.0f / 3.0f;
}
