/*
 * GameForm.h
 *
 *  Created on: 06/10/2010
 *      Author: Daniel Conde Linares
 */

#ifndef GAMEFORM_H_
#define GAMEFORM_H_

#include <FBase.h>
#include <FUi.h>
#include <FGraphics.h>

class GameGL;

using namespace Osp::Graphics;
using namespace Osp::Base;
using namespace Osp::Ui;

class GameForm :
	public Controls::Form
 {
public:
	GameForm(GameGL* pApp);
	virtual ~GameForm(void) {};

	float getOrientationRotation();

public:
	virtual result OnDraw(void);

private:
	GameGL* __pApp;
};

#endif /* GAMEFORM_H_ */
