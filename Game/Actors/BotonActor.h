/*
 * BotonActor.h
 *
 *  Created on: 29/11/2010
 *      Author: Daniel Conde Linares
 */

#ifndef BOTONACTOR_H_
#define BOTONACTOR_H_

#include <FBase.h>
#include <FGraphicsOpenGL.h>
#include <FUi.h>
#include "ActorGL.h"
#include "Touch.h"
#include "SpriteGL.h"
#include "TextureGL.h"
#include "Quaternion.h"

using namespace BadaGameLib;

class BotonActor : public ActorGL {
public:
	BotonActor(SpriteGL* sprite);
	virtual ~BotonActor();

	// Initialization Methods
	void init(const Osp::Base::String& resourceName);

	// Drawing Methods
	void update(long long gameTime);
	void processTouch(const Osp::Ui::TouchEventInfo & touchInfo);
	void draw(long long gameTime);

	void setToggleable(bool isToggle);
	//virtual void activate() = 0;
	//virtual void deactivate() = 0;

	//void addGroupedButton();
	//void removeGroupedButton();

public:
	enum eStates {
		DEFAULT = 0,
		ACTIVE,
		MAX_STATES
	};

private:
	bool m_bToggle;
};

#endif /* BOTONACTOR_H_ */
