/*
 * StateController.cpp
 *
 *  Created on: 06/10/2010
 *      Author: Daniel Conde Linares
 */

#include "StateController.h"

namespace BadaGameLib {

StateController::StateController(bool blending) {
	// TODO Auto-generated constructor stub
	m_currentAnim = null;
	m_nextAnim = null;
	m_defaultAnim = null;

	m_animBlending = blending;
}

StateController::~StateController() {
	// TODO Auto-generated destructor stub
}

void StateController::playAnim()
{
	SpriteAnim *anim = dynamic_cast<SpriteAnim*> (m_currentAnim);
	if(anim)
		anim->play();
}

void StateController::pauseAnim()
{
	SpriteAnim *anim = dynamic_cast<SpriteAnim*> (m_currentAnim);
	if(anim)
		anim->stop();
}

void StateController::forceNext()
{
	// TODO Refactoring
	SpriteAnim *anim = dynamic_cast<SpriteAnim*> (m_currentAnim);
	if(anim)
	{
		anim->stop();
		anim->togleLoop();
	}

	m_currentAnim = m_nextAnim;
	m_nextAnim = m_defaultAnim;
}

void StateController::updateAnim()
{
	SpriteAnim *anim = dynamic_cast<SpriteAnim*> (m_currentAnim);
	if(anim)
	{
		if(animIsOver() && !anim->isAnimationLooping())
		{
			m_currentAnim = m_nextAnim;
			m_nextAnim = m_defaultAnim;
		}
	}

	Sprite2D *state = dynamic_cast<Sprite2D*> (m_currentAnim);
	if(state)
	{
		if(m_nextAnim != null)
		{
			m_currentAnim = m_nextAnim;
			m_nextAnim = m_defaultAnim;
		}
	}
}

void StateController::addNextAnim(Sprite* anim)
{
	// TODO: Revise Default Animations statements
	//if(m_animBlending)
	{
		if (m_currentAnim == null)
		{
			m_currentAnim = anim;
			//m_nextAnim = anim;
			//m_defaultAnim = anim;
		}
		else
		{
			if (m_nextAnim == null || m_nextAnim == m_defaultAnim)
				m_nextAnim = anim;
		}
	}
	/*else
	{
		m_currentAnim = anim;
		m_nextAnim = m_defaultAnim;
	}*/
}
void StateController::setDefaultState(Sprite* anim)
{
	m_defaultAnim = anim;
}

void StateController::removeNextAnim()
{
	m_nextAnim = m_defaultAnim;
}

Sprite* StateController::getCurrentAnimation()
{
//	SpriteAnim *anim = dynamic_cast<SpriteAnim*> (m_currentAnim);
//	if(anim)
//		return anim;
//	else
//		return NULL;
	return m_currentAnim;
}

bool StateController::animIsOver()
{
	SpriteAnim *anim = dynamic_cast<SpriteAnim*> (m_currentAnim);
	if(anim)
		return anim->isAnimationOver();
	else
		return true;
}

}
