#pragma once
#include "VisibleObject.h"

struct PoSize {
	olc::vi2d pos;
	olc::vi2d size;
};

template <class T>
class TimedObject {
public:
	TimedObject() = delete;
	TimedObject(float time_, T object_) {
		object_.first = time_;
		object_.second = object_;
	}
	~TimedObject() {};
private:
	std::pair<float, T> object_;
};


struct AnimationSheet {
	std::vector< TimedObject<PoSize> > decal;
	olc::Decal* sheet;
};

class AnimatedObject :
	public VisibleObject
{
public:
	// AnimatedObject(unsigned int UID_, std::string name_)
	// 	: VisibleObject(UID_, name_) {}
	AnimatedObject(unsigned int UID_, std::string name_, std::string file_path_)
		: VisibleObject(UID_, name_, file_path_) {}
	AnimatedObject(unsigned int UID_, std::string name_, std::string file_path_, PoSize po_size_)
		: VisibleObject(UID_, name_) {
		setDefaultVisual(file_path_, po_size_);
	}

	~AnimatedObject() {};

	void setDefaultVisual(std::string path_, PoSize po_size_);

	void update(float fElapsedTime);

	bool triggerAnimation(size_t ID_);

	auto getDecal(void) override;

protected:
	std::map<size_t, AnimationSheet> _animations;
	AnimationSheet* _current_animation;
	float _time;
};

