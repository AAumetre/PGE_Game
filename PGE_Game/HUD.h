#pragma once
#include "VisibleObject.h"
#include "Character.h"

class HUD :
	public VisibleObject
{
public:
	// Constructors
	HUD(unsigned int UID_, std::string name_) : VisibleObject(UID_, name_) {};
	// One taking a file path to its (empty) representation
	HUD(unsigned int UID_, std::string name_, std::string file_path_) : VisibleObject(UID_, name_) {
		setVisual(file_path_);
	}
	// One taking a reference to a Player that it monitors
	HUD(unsigned int UID_, std::string name_, Player* player_) : VisibleObject(UID_, name_), _player(player_) {
		initHUDforPlayer();
	};

	inline void setPlayer(Player* player_) { _player = player_; initHUDforPlayer(); }
	void initHUDforPlayer();
	void update();

protected:
	Player* _player;
};

