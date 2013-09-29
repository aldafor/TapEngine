/*
 * GameForm.cpp
 *
 *  Created on: 06/10/2010
 *      Author: Daniel Conde Linares
 */

#include "GameForm.h"
#include "GameGL.h"

GameForm::GameForm(GameGL* pApp)
{
	__pApp = pApp;
}

float GameForm::getOrientationRotation() {

	float angle = 0.0f;

	switch(this->GetOrientation())
	{
	case Ui::ORIENTATION_PORTRAIT:
		AppLog("Orientacion Portrait");
		angle = 0.0f;
		break;
	case Ui::ORIENTATION_PORTRAIT_REVERSE:
		AppLog("Orientacion Portrait Reverse");
		angle = 180.0f;
		break;
	case Ui::ORIENTATION_LANDSCAPE:
		AppLog("Orientacion Landscape");
		angle = -90.0f;
		break;
	case Ui::ORIENTATION_LANDSCAPE_REVERSE:
		AppLog("Orientacion Landscape Reverse");
		angle = 90.0f;
		break;
	default:
		AppLog("Orientacion Default");
		break;
	}

	return angle;
}

result GameForm::OnDraw(void) {
	if (__pApp) {
		__pApp->Draw(0);
	}

	return E_SUCCESS;
}
