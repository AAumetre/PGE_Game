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

class Scene :
	public TrackedObject
{
public:
	// Constructor & Destructor
	Scene(unsigned int UID_, std::string name_)	: TrackedObject(UID_, name_) {};
	~Scene() {};
	Scene(unsigned int UID_, std::string name_, const Region* region_,
		Player* player_, unsigned int n_layers_) :
		TrackedObject(UID_, name_), _region(region_)
	{
		_layers.resize(n_layers_);
		addCharacter(player_);
	}

	inline Layers* getLayers() { return &_layers; }
	bool addCharacter(Character* char_);
	bool removeCharacter(const Character* char_);
	void moveToLayer(unsigned int index_, VisibleObject* object_);
	void limitCharactersToRegion(void);

private:
	const Region*				_region;
	std::map< int, Character* >	_characters;
	// const HUD*		_hud;

	Layers _layers;
};

