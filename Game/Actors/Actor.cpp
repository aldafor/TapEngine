/*
 * Actor.cpp
 *
 *  Created on: 06/10/2010
 *      Author: Daniel Conde Linares
 */

#include "Actor.h"

namespace BadaGameLib {

Actor::Actor()
{
	// TODO Auto-generated constructor stub
	init(true);
}

Actor::Actor(bool active)
{
	// TODO Auto-generated constructor stub
	init(active);
}

void Actor::init(bool active)
{
	// TODO Auto-generated constructor stub
	m_touches = new Base::Collection::ArrayListT<Touch*>();
	m_states = new Base::Collection::ArrayListT<Sprite*>();
	m_soundManager = new SoundManager();
	m_controller = new StateController();

	m_active = active;

	m_currentState = 0;

	m_accumulatedTime = 0;

	m_x = 0;
	m_y = 0;
}

Actor::~Actor() {
	// TODO Auto-generated destructor stub
	delete m_controller;

	Touch* tempTouch;
	for (int i = 0; i < m_touches->GetCount(); ++i)
	{
		m_touches->GetAt(i, tempTouch);
		delete tempTouch;
	}

	m_touches->RemoveAll();

	delete m_touches;

	Sprite* tempState;
	for (int i = 0; i < m_states->GetCount(); ++i)
	{
		m_states->GetAt(i, tempState);
		delete tempState;
	}

	m_states->RemoveAll();

	delete m_states;

	delete m_soundManager;

	AppLog("Destruyendo Actor");
}

void Actor::process(int delayedTime)
{
	// TODO Implement animation processing
	if(m_active)
	{
		m_accumulatedTime += delayedTime;
		m_controller->updateAnim();
	}
}

void Actor::processTouchs(const Graphics::Point& touchPoint)
{
	if(m_active){
		Touch* tempTouch;
		for (int i = 0; i < m_touches->GetCount(); ++i)
		{
			m_touches->GetAt(i, tempTouch);
			if(tempTouch->isTouched(touchPoint.x, touchPoint.y))
			{
				this->setState(tempTouch->getData());
				m_soundManager->Stop(tempTouch->getData());
				m_soundManager->Play(tempTouch->getData());

				m_accumulatedTime = 0;
			}
		}
	}
}

void Actor::setPosition(int x, int y)
{
	m_x=x;
	m_y=y;
}

void Actor::render(Graphics::Bitmap* drawBuffer)
{
	if(m_active)
	{
		// TODO Implement the drawing method
		Sprite2D* tempSprite2D = dynamic_cast<Sprite2D*> (m_controller->getCurrentAnimation());
		if(tempSprite2D)
		{
			tempSprite2D->draw(drawBuffer);
			AppLog("BadaGameLib: Actor->Render2D");
		}

		SpriteAnim* tempSpriteAnim = dynamic_cast<SpriteAnim*> (m_controller->getCurrentAnimation());
		if(tempSpriteAnim)
		{
			tempSpriteAnim->draw(drawBuffer);
			AppLog("BadaGameLib: Actor->RenderAnim");
		}
	}
}

void Actor::setActive(bool active)
{
	m_active = active;
}

bool Actor::isActive()
{
	return m_active;
}

void Actor::setState(int stateData)
{
	m_currentState = stateData;
	Sprite* tempState;
	m_states->GetAt(stateData, tempState);
	m_controller->addNextAnim(tempState);
}
void Actor::setDefaultState(int stateData)
{
	Sprite* tempState;
	m_states->GetAt(stateData, tempState);
	m_controller->setDefaultState(tempState);
}

int Actor::getState()
{
	return m_currentState;
}

void Actor::setBlending(bool blending)
{
	// TODO: Solve where to hold Anim Blending Info
}

bool Actor::isBlendeable()
{
	// TODO: Solve where to hold Anim Blending Info
	return true;
}

}
