/*
 * Scene.h
 *
 *  Created on: 06/10/2010
 *      Author: Daniel Conde Linares
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <FBase.h>
#include <FGraphics.h>
#include "Actor.h"

using namespace Osp;

namespace BadaGameLib {

/**
 * @class Scene
 *
 * @brief Class for encapsulate the Scenes functionality.
 *
 * Represent an scenery where actors that are on it are Acting on an determined Scene.
 * Every Actor that we want that appear on the Scene MUST be initialized before, otherwise Scene
 * could not be process or drawn properly due this fact.
 *
 * TODO Inheritance of SceneCanvas or SceneGL
 */
class Scene
{

public:
	Scene();
	virtual ~Scene();

	// Drawable Object
	void process(int delayedTime);
	void processTouch(const Graphics::Point& touchPoint);
	void render(Graphics::Canvas* currentCanvas);
	void render(Graphics::Bitmap* drawBuffer);

	// Actors Manager Code
	void addActor(Actor* newActor);
	Actor* getActor(int actorID);
	int getNActors();
	void removeActor(Actor* newActor);

protected:
	Base::Collection::ArrayListT<Actor*>* m_actors;

};

}

#endif /* SCENE_H_ */
