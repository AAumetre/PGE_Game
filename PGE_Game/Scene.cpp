#include "Scene.h"

void Scene::moveToLayer(unsigned int index_, VisibleObject * object_)
{
	// TODO: Remove from other layers

	// Move to new layer
	_layers.at(index_).push_back(object_);
}

