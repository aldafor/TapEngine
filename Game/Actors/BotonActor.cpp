/*
 * BotonActor.cpp
 *
 *  Created on: 23/11/2010
 *      Author: Daniel Conde Linares
 */

#include "BotonActor.h"

using namespace Osp::Base;

BotonActor::BotonActor(SpriteGL* sprite) : ActorGL(sprite) {
	// TODO Auto-generated constructor stub
	m_states = new Base::Collection::ArrayListT<TextureGL*>();
	m_states->Construct(BotonActor::MAX_STATES);
	m_pTouches = new Base::Collection::ArrayListT<BadaGameLib::Touch*>();
	m_pTouches->Construct();
	m_bToggle = false;
}

BotonActor::~BotonActor() {
	// TODO Auto-generated destructor stub
}

void BotonActor::init(const String& resourceName) {

	String temp = resourceName;
	AppLog("resourceName BUTON: %S", temp.GetPointer());
	TextureGL* l_pTextureInit = new TextureGL();
	l_pTextureInit->loadFromFile(temp);
	m_states->Add(l_pTextureInit);

	m_iCurrentState = 0;
	m_pSprite->setTexture(l_pTextureInit);

	temp.Insert(L"_on", temp.GetLength()-4);
	AppLog("resourceName BUTON ON: %S", temp.GetPointer());

	l_pTextureInit = new TextureGL();
	l_pTextureInit->loadFromFile(temp);
	m_states->Add(l_pTextureInit);

	// Creacion de los Toucheables!!!
	m_pTouches->Add(m_pDefaultTouch);
}

// Drawing Methods
void BotonActor::update(long long gameTime) {
	ActorGL::update(gameTime);
	//m_pSprite->update(gameTime);
	TextureGL* l_pCurrentTex;
		m_states->GetAt(m_iCurrentState, l_pCurrentTex);
		if(l_pCurrentTex)
		{
			if(l_pCurrentTex->isAnimated())
			{
				if(l_pCurrentTex->isAnimEnded())
				{
					setState(m_iNextState);
					m_iNextState = m_iDefaultState;
				}
			}
			else
			{
				setState(m_iNextState);
				m_iNextState = m_iDefaultState;
			}
		}
}

void BotonActor::processTouch(const TouchEventInfo & touchInfo)
{
	ActorGL::processTouch(touchInfo);
	if(m_pTouches)
	{
		BadaGameLib::Touch* tempTouch;
		for (int i = 0; i < m_pTouches->GetCount(); ++i)
		{
			m_pTouches->GetAt(i, tempTouch);
			if(tempTouch->isTouched(touchInfo.GetStartPosition().x, touchInfo.GetStartPosition().y))
			{
				AppLog("Touched!!");
				if(tempTouch->getData() == getState())
				{
					//setState(DEFAULT);
					enqueueState(DEFAULT);
				}
				else
				{
					enqueueState(tempTouch->getData());
				}
			}
		}
	}
}

void BotonActor::draw(long long gameTime) {
	ActorGL::draw(gameTime);
}

void BotonActor::setToggleable(bool isToggle)
{
	m_bToggle = isToggle;
}
