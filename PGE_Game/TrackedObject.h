#pragma once
#include "AllIncludes.h"

class TrackedObject
{
public:
	// Constructor & Destructor
	TrackedObject(unsigned int UID_, std::string name_) : _UID(UID_), _name(name_) {};
	~TrackedObject() {};

	// Accessors, inlined
	inline void			setUID(int UID_)			{ _UID = UID_; }
	inline void			setName(std::string name_)	{ _name = name_; }
	inline unsigned int	getUID(void)				{ return _UID; }
	inline std::string	getName(void)				{ return _name; }
	//inline unsigned int	getChildCount(void)			{ return _children.size; }
	// Accessors, not inlined
	void addChild(std::shared_ptr<TrackedObject> child_);
	std::shared_ptr<TrackedObject> getChild(std::string name_);
	std::shared_ptr<TrackedObject> getChild(unsigned int UID_);
	// TODO: _children needs to be iterated over
	// TODO: use map ffs !!!!

protected:
	// Properties
	unsigned int _UID;	// Unique Identifier
	std::string _name;	// Meaningful name, may be used for display
	std::vector< std::shared_ptr<TrackedObject> > _children; // Children storage
};

