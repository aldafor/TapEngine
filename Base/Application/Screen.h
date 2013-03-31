/*
 * Screen.h
 *
 *  Created on: 28/10/2010
 *      Author: Daniel Conde Linares
 */

#ifndef SCREEN_H_
#define SCREEN_H_

//#include <FUi.h>
//#include "MathHelper.h"
#include <Touch.h>

class ScreenManager;

class Screen {
public:
	Screen(ScreenManager* manager);
	virtual ~Screen();

//	void loadContent();
//	void unloadContent();

	//Another option is to make update ABSTRACT
	virtual void update(long long gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen);
	virtual bool updateTransition(long long gameTime, long long time, int direction);
	virtual void processTouchs(const Touch& touchInfo) = 0;
//	void updateTransition();
//
//	void proccessInput();

	//Another option is to make draw ABSTRACT
	virtual void draw(long long gameTime) = 0;

	void exitScreen();

	bool isActive();
	bool isExiting();

	int getState();
	void setState(int state);

	int getTransitionAlpha();

	float getTransitionPosition();
	void setTransitionPosition(float position);

	long long getTransitionOnTime();
	void setTransitionOnTime(long long time);

	long long getTransitionOffTime();
	void setTransitionOffTime(long long time);

	bool getIsPopup();
	void setIsPopup(bool popup);

public:
	enum eScreenState {
		TRANSITION_ON,
		ACTIVE,
		TRANSITION_OFF,
		HIDDEN
	};

protected:
	long long m_llTransitionOnTime;
	long long m_llTransitionOffTime;
	float m_iTransitionPosition;
	int m_eScreenState;
	bool m_bIsExiting;
	bool m_bIsPopup;

private:
	bool m_bOtherScreenHasFocus;

protected:
	ScreenManager* m_pScreenManager;
};

#endif /* SCREEN_H_ */
