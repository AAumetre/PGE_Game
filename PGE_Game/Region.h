#pragma once
#include "VisibleObject.h"

class Region : 
	public VisibleObject
{
public:
	// Constructors
	Region(unsigned int UID_, std::string name_) : VisibleObject(UID_, name_) {};
	Region(unsigned int UID_, std::string name_, std::string file_path_) : VisibleObject(UID_, name_) {
		setVisual(file_path_);
	};

private:
	// Sub-regions
	std::vector< std::shared_ptr<Region> > _sub_regions; // superseeded by _children ?

	// Elements contained by this region
	std::unique_ptr< RegionMap > _map;	// Not implemented
	std::vector< Music* > _musics;		// Not implemented
	std::vector< Sound* > _sounds;		// Not implemented

};

