/*
 * Screen.cpp
 *
 *  Created on: 28/10/2010
 *      Author: Administrador
 */

#include "Screen.h"
#include "ScreenManager.h"

Screen::Screen(ScreenManager* manager) {
	// TODO Auto-generated constructor stub
	m_llTransitionOnTime = 0;
	m_llTransitionOffTime = 0;
	m_iTransitionPosition = 1.0f;
	m_eScreenState = Screen::TRANSITION_ON;
	m_bIsPopup = false;
	m_bIsExiting = false;

	m_pScreenManager = manager;
}

Screen::~Screen() {
	// TODO Auto-generated destructor stub
}

void Screen::update(long long gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	m_bOtherScreenHasFocus = otherScreenHasFocus;

	if (m_bIsExiting)
	{
		// If the screen is going away to die, it should transition off.
		m_eScreenState = Screen::TRANSITION_OFF;

		if (!updateTransition(gameTime, m_llTransitionOffTime, 1))
		{
			// When the transition finishes, remove the screen.
			//ScreenManager.RemoveScreen(this);

			m_bIsExiting = false;
		}
	}
	else if (coveredByOtherScreen)
	{
		// If the screen is covered by another, it should transition off.
		if (updateTransition(gameTime, m_llTransitionOffTime, 1))
		{
			// Still busy transitioning.
			m_eScreenState = Screen::TRANSITION_OFF;
		}
		else
		{
			// Transition finished!
			m_eScreenState = Screen::HIDDEN;
		}
	}
	else
	{
		// Otherwise the screen should transition on and become active.
		if (updateTransition(gameTime, m_llTransitionOffTime, -1))
		{
			// Still busy transitioning.
			m_eScreenState = Screen::TRANSITION_ON;
		}
		else
		{
			// Transition finished!
			m_eScreenState = Screen::ACTIVE;
		}
	}
}

bool Screen::updateTransition(long long gameTime, long long time, int direction)
{
	// How much should we move by?
	float transitionDelta;

	if (time == 0)
		transitionDelta = 1;
	else
		transitionDelta = (float)(gameTime / time);

	// Update the transition position.
	m_iTransitionPosition += transitionDelta * direction;

	// Did we reach the end of the transition?
	if ((m_iTransitionPosition <= 0) || (m_iTransitionPosition >= 1))
	{
		//m_iTransitionPosition = BadaGameLib::Clamp(m_iTransitionPosition, 0, 1);
		return false;
	}

	// Otherwise we are still busy transitioning.
	return true;

}

void Screen::exitScreen()
{
	if (m_llTransitionOffTime == 0)
	{
		// If the screen has a zero transition time, remove it immediately.
		//ScreenManager.RemoveScreen(this);
	}
	else
	{
		// Otherwise flag that it should transition off and then exit.
		m_bIsExiting = true;
	}
}

bool Screen::isActive() {
	return (m_eScreenState == Screen::TRANSITION_ON || m_eScreenState == Screen::ACTIVE);
}

bool Screen::isExiting() {
	return m_bIsExiting;
}

int Screen::getState() {
	return m_eScreenState;
}
void Screen::setState(int state) {
	m_eScreenState = state;
}

int Screen::getTransitionAlpha() {
	return (255 - m_iTransitionPosition * 255);
}

float Screen::getTransitionPosition() {
	return m_iTransitionPosition;
}
void Screen::setTransitionPosition(float position) {
	m_iTransitionPosition = position;
}

long long Screen::getTransitionOnTime() {
	return m_llTransitionOnTime;
}
void Screen::setTransitionOnTime(long long time) {
	m_llTransitionOnTime = time;
}

long long Screen::getTransitionOffTime() {
	return m_llTransitionOffTime;
}
void Screen::setTransitionOffTime(long long time) {
	m_llTransitionOffTime = time;
}

bool Screen::getIsPopup() {
	return m_bIsPopup;
}
void Screen::setIsPopup(bool popup) {
	m_bIsPopup = popup;
}
