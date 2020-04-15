#include "TrackedObject.h"

void TrackedObject::addChild(std::shared_ptr<TrackedObject> child_)
{
	_children.push_back(child_);
}

std::shared_ptr<TrackedObject> TrackedObject::getChild(std::string name_)
{
	for (auto child : _children) {
		if (child->getName() == name_) return child;
	}
}

std::shared_ptr<TrackedObject> TrackedObject::getChild(unsigned int UID_)
{
	for (auto child : _children) {
		if (child->getUID() == UID_) return child;
	}
}
