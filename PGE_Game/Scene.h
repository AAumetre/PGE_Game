#pragma once
#include "AllIncludes.h"
#include "VisibleObject.h"
#include "Region.h"
#include "Character.h"

// A Scene contains:	- a Region, 
//						- the Player,
//						- NPCs,
//						- visual effects,
//						- the HUD.
// They are all organized on predefined layers.

 // Layers as vectors of VisibleObject
using Layers = std::vector< std::vector<VisibleObject*> > ;

class Scene
{
public:
	// Constructor & Destructor
	~Scene() {};
	Scene(const Region* region_, const Player* player_, unsigned int n_layers_) :
		_region(region_), _player(player_) {
		_layers.resize(n_layers_);
	}

	inline Layers* getLayers() { return &_layers; }
	void moveToLayer(unsigned int index_, VisibleObject* object_);

private:
	const Region*	_region;
	const Player*	_player;
	// const HUD*		_hud;

	Layers _layers;
};

