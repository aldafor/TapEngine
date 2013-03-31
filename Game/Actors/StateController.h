/*
 * StateController.h
 *
 *  Created on: 06/10/2010
 *      Author: Daniel Conde Linares
 */

#ifndef ANIMCONTROLLER_H_
#define ANIMCONTROLLER_H_

#include "Sprite.h"
#include "SpriteAnim.h"

namespace BadaGameLib {


/**
* @class StateController
*
* @brief Class for control the different states of an Actor.
*
* The controller is located on the Actor, and keep control of the different states that hold this
* Actor at every moment. NOTE: AT THE MOMENT IT HOLD SPRITES, NOT STATES!!!!
*
* @see Actor
*/
class StateController
{

public:
	StateController(bool blending = true);
	virtual ~StateController();

	void playAnim();
	void pauseAnim();
	void forceNext();

	void addNextAnim(Sprite* anim);
	void setDefaultState(Sprite* anim);
	void removeNextAnim();

	void updateAnim();

	Sprite* getCurrentAnimation();

private:
	bool animIsOver();

	Sprite* m_currentAnim;
	Sprite* m_nextAnim;
	Sprite* m_defaultAnim;

	bool m_animBlending;
};

}

#endif /* ANIMCONTROLLER_H_ */
