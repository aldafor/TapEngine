/*
 * State.cpp
 *
 *  Created on: 15/11/2010
 *      Author: Daniel Conde Linares
 */

#include "ActorGL.h"

using namespace BadaGameLib;
using namespace Osp::Ui;

ActorGL::ActorGL(SpriteGL* sprite) {
	// TODO Auto-generated constructor stub
	m_pSprite = sprite;
	m_bLooping = false;
	m_iTicks = 0;
	m_Position = sprite->getPosition();
	m_Velocity = NULL;
	m_pTargetPosition = NULL;
	m_pTouches = NULL;
	m_pDefaultTouch = new BadaGameLib::Touch(
			1,
			(int)(m_Position->m_fX * 480.0f),
			800 - ((int)(m_Position->m_fY * 800.0f) + (int)(getHeight() * 800.0f)),
			(int)(getWidth() * 480.0f),
			(int)(getHeight() * 800.0f));

	m_iDefaultState = 0;
	m_iCurrentState = m_iDefaultState;
	m_iNextState = m_iDefaultState;
}

ActorGL::~ActorGL() {
	// TODO Auto-generated destructor stub
	delete m_Position;
	delete m_Velocity;
	delete m_pTargetPosition;
	delete m_pSprite;
}

// Drawing Methods
void ActorGL::update(long long gameTime) {
	//if (m_iTicks == 0 || gameTime <= m_iTicks)
	if(m_Position != NULL && m_Velocity != NULL) {
		if (m_pTargetPosition != NULL) {
			if(m_Position->m_fX > m_pTargetPosition->m_fX)
				m_Position->m_fX -= m_Velocity->m_fX;
			if(m_Position->m_fX < m_pTargetPosition->m_fX)
				m_Position->m_fX += m_Velocity->m_fX;
			if(m_Position->m_fY > m_pTargetPosition->m_fY)
				m_Position->m_fY -= m_Velocity->m_fY;
			if(m_Position->m_fY < m_pTargetPosition->m_fY)
				m_Position->m_fY += m_Velocity->m_fY;
		}
		else {
			*m_Position += *m_Velocity;
		}
		//AppLog("ActorGL: Me he movido a (%f,%f,%f,%f)", m_Position->m_fX, m_Position->m_fY, m_Position->m_fZ, m_Position->m_fW);
	}

	m_pSprite->update(gameTime);
}

void ActorGL::processTouch(const TouchEventInfo & touchInfo)
{
	AppLog("--- PROCESANDO ACTOR TOUCH ---");
	AppLog("TouchEventInfo: StartPosition(%i,%i)", touchInfo.GetStartPosition().x, touchInfo.GetStartPosition().y);
	AppLog("ActorPosition: Position(%i,%i)", (int)(m_Position->m_fX * 480.0f), 800 - (int)(m_Position->m_fY * 800.0f));
	AppLog("ActorDimension: Dimension(%i,%i)", (int)(getWidth() * 480.0f), (int)(getHeight() * 800.0f));
	AppLog("ActorTouchRectangle: UpLeft(%i,%i) - DownRight(%i,%i)",
			(int)(m_Position->m_fX * 480.0f),
			800 - ((int)(m_Position->m_fY * 800.0f) + (int)(getHeight() * 800.0f)),
			(int)(m_Position->m_fX * 480.0f) + (int)(getWidth() * 480.0f),
			800 - (int)(m_Position->m_fY * 800.0f));
	AppLog("------------------------------");

	/*if(m_pTouches)
	{
		BadaGameLib::Touch* tempTouch;
		for (int i = 0; i < m_pTouches->GetCount(); ++i)
		{
			m_pTouches->GetAt(i, tempTouch);
			if(tempTouch->isTouched(touchInfo.GetStartPosition().x, touchInfo.GetStartPosition().y))
			{
				AppLog("Touched!!");
				// BASIC Check for toggle
				if(tempTouch->getData() == getState())
				{

					setState(0);
				}
				else
				{
					setState(tempTouch->getData());
				}
			}
		}
	}*/
}

void ActorGL::draw(long long gameTime) {
	Osp::Graphics::Opengl::glPushMatrix();
	//AppLog("Sprite position: %f, %f, %f", m_pSprite->getPosition()->m_fX, m_pSprite->getPosition()->m_fY, m_pSprite->getPosition()->m_fZ);

	//TODO PORQUE FALLA!!!????
	//Osp::Graphics::Opengl::glTranslatef(m_Position->m_fX, m_Position->m_fY, m_Position->m_fZ);

	//AppLog("Sprite translated: %f, %f, %f", m_Position->m_fX, m_Position->m_fY, m_Position->m_fZ);
	m_pSprite->draw(gameTime);
	Osp::Graphics::Opengl::glPopMatrix();
}

// Spatial Methods
Quaternion* ActorGL::getPosition() {
	return m_Position;
}
void ActorGL::setPosition(Quaternion* position) {
	if(m_Position != NULL)
		delete m_Position;
	m_Position=position;
}

Quaternion* ActorGL::getTargetPosition() {
	return m_pTargetPosition;
}
void ActorGL::setTargetPosition(Quaternion* targetPosition) {
	if(m_pTargetPosition != NULL)
		delete m_pTargetPosition;
	m_pTargetPosition = targetPosition;
}

Quaternion* ActorGL::getVelocity() {
	return m_Velocity;
}
void ActorGL::setVelocity(Quaternion* velocity) {
	if(m_Velocity != NULL)
			delete m_Velocity;
	m_Velocity=velocity;
}
void ActorGL::setVelocity(Quaternion* velocity, int milliseconds) {
	setVelocity(velocity);
	m_iTicks = milliseconds;
}

float ActorGL::getSpin() {
	return m_fSpinAngle;
}
void ActorGL::setSpin(const float& spin) {
	m_fSpinAngle = spin;
}

// Alive Methods
bool ActorGL::isLooping() {
	return m_bLooping;
}
void ActorGL::setLooping(const bool& looping) {
	m_bLooping = looping;
}

void ActorGL::setState(int state) {
	TextureGL* l_pTextureInit = NULL;
	m_states->GetAt(state, l_pTextureInit);
	if(l_pTextureInit)
	{
		l_pTextureInit->resetTexture();
		m_pSprite->setTexture(l_pTextureInit);
		m_iCurrentState = state;
	}
	SoundManager::getInstance()->Play(state);
}

void ActorGL::setDefaultState(int state) {

	m_iDefaultState = state;
	AppLog("Setting DefaultState to %i", state);
}

void ActorGL::enqueueState(int state) {

	m_iNextState = state;
	AppLog("Setting NextState to %i", state);
}
