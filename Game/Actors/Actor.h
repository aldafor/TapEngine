/*
 * Actor.h
 *
 *  Created on: 06/10/2010
 *      Author: Daniel Conde Linares
 */

#ifndef ACTOR_H_
#define ACTOR_H_

#include <FBase.h>
#include <FGraphics.h>
#include <FMedia.h>
#include "Touch.h"
#include "Sprite.h"
#include "StateController.h"
#include "SoundManager.h"

using namespace Osp;

namespace BadaGameLib {


/**
 * @class Actor
 *
 * @brief Class for encapsulate the Actor functionality.
 *
 * An Actor is an element that can hold different states and respond to interactions on a Scene.
 *
 * TODO AT THE MOMENT HOLD SPRITES INSTEAD OF STATES
 * TODO MUST be initialized before be processed
 */
class Actor
{
public:
	Actor();
	Actor(bool active);
	virtual ~Actor();

	// Control code
	void setActive(bool active);
	bool isActive();
	void setState(int stateData);
	void setDefaultState(int stateData);
	int getState();

	// Drawable Object
	void setPosition(int x, int y);
	virtual void process(int delayedTime);
	virtual void render(Graphics::Bitmap* drawBuffer);

	// Animation Control
	void setBlending(bool blending);
	bool isBlendeable();

	// Touches control
	virtual void processTouchs(const Graphics::Point& touchPoint);
protected:
	virtual void init(bool active);

protected:
	Base::Collection::ArrayListT<Touch*>* m_touches;
	Base::Collection::ArrayListT<Sprite*>* m_states;
	StateController* m_controller;

	SoundManager* m_soundManager;
	//bool m_stateBlending;

	bool m_active;

	int m_currentState;

	int m_accumulatedTime;

	int m_x, m_y;

};

}

#endif /* ACTOR_H_ */
