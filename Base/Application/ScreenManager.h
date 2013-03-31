/*
 * ScreenManager.h
 *
 *  Created on: 28/10/2010
 *      Author: Daniel Conde Linares
 */

#ifndef SCREENMANAGER_H_
#define SCREENMANAGER_H_

#include <FBase.h>
#include <FUi.h>

class Screen;

using namespace Osp;

class ScreenManager {
public:
	static ScreenManager* getInstance();
	virtual ~ScreenManager();

	// Initialization functionality
	void Initialize();

	// Basic Process functionality
	void Update(long long gameTime);
	void ProcessTouchs (const Ui::TouchEventInfo & touchInfo);

	// Basic Drawn functionality
	void Draw(long long gameTime);

protected:
	ScreenManager();

public:
	enum GameScreens {
			LOADING = 0,
			MAINMENU,
			OPTIONS,
			PLAYING,
			MAX_SCREENS
		};

protected:
	Base::Collection::ArrayListT<Screen*> m_screens;
	Base::Collection::ArrayListT<Screen*> m_screensToUpdate;
	//Screen** m_screens;

public:
	GameScreens m_eCurrentScreen;

private:
	static ScreenManager* m_pInstance;
//	void LoadContent();
//	Screen* getScreens();
//
//	bool getInitialized();
//
//	Bitmap* getDrawBuffer();
//	Canvas* getDrawBuffer();
//	OPENGL getDrawBuffer();
//
//	Font* getFont();
//
//	void setTraceEnabled(bool enable);
//	bool getTraceEnabled();
};

#endif /* SCREENMANAGER_H_ */
