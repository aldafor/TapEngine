/*
 * ActorGL.h
 *
 *  Created on: 15/11/2010
 *      Author: Daniel Conde Linares
 */

#ifndef ACTORGL_H_
#define ACTORGL_H_

#include <FGraphicsOpenGL.h>
#include <FBase.h>
#include <FUi.h>
#include "SpriteGL.h"
#include "TextureGL.h"
#include "Touch.h"
#include "Quaternion.h"
#include "SoundManager.h"

template <class T>
struct State{
	int m_iCurrentFrame;
	int m_iTotalFrames;
	int m_iStartLoopFrame;
	int m_iEndLoopFrame;
	T* data;
};


class ActorGL {
public:
	ActorGL(SpriteGL* sprite);
	virtual ~ActorGL();

	// Drawing Methods
	void update(long long gameTime);
	void processTouch(const Osp::Ui::TouchEventInfo & touchInfo);
	void draw(long long gameTime);

	// Dimension Methods
	float getWidth() { return m_pSprite->getWidth(); };
	float getHeight() { return m_pSprite->getHeight(); };

	// Spatial Methods
	BadaGameLib::Quaternion* getPosition();
	void setPosition(BadaGameLib::Quaternion* position);
	BadaGameLib::Quaternion* getTargetPosition();
	void setTargetPosition(BadaGameLib::Quaternion* targetPosition);
	BadaGameLib::Quaternion* getVelocity();
	void setVelocity(BadaGameLib::Quaternion* velocity);
	void setVelocity(BadaGameLib::Quaternion* velocity, int milliseconds);
	float getSpin();
	void setSpin(const float& spin);

	// Alive Methods
	bool isLooping();
	void setLooping(const bool& looping);

	// State Managing methods
	virtual void setState(int state);
	virtual void setDefaultState(int state);
	virtual void enqueueState(int state);
	virtual int getState() { return m_iCurrentState; };

protected:
	SpriteGL* m_pSprite;
	Base::Collection::ArrayListT<BadaGameLib::Touch*>* m_pTouches;
	Base::Collection::ArrayListT<BadaGameLib::TextureGL*>* m_states;

	int m_iDefaultState;
	int m_iCurrentState;
	int m_iNextState;

	//For next Iteration
	//Base::Collection::ArrayListT<ActorGL*>* m_childActors;

	BadaGameLib::Quaternion* m_Position;
	BadaGameLib::Quaternion* m_Velocity;
	BadaGameLib::Quaternion* m_pTargetPosition;
	float m_fSpinAngle;
	bool m_bLooping;

	int m_iTicks;

	BadaGameLib::Touch* m_pDefaultTouch;
};

#endif /* ACTORGL_H_ */
