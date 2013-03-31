/*
 * Game.h
 *
 *  Created on: 06/10/2010
 *      Author: Daniel Conde Linares
 */

#ifndef GAME_H_
#define GAME_H_

#include <FBase.h>
#include <FGraphics.h>
#include <FUi.h>
#include "Scene.h"

using namespace Osp;

namespace BadaGameLib {

/**
 * @class Game
 *
 * @brief Class for encapsulate the main Game functionality.
 *
 * A Game is a set of Scenes that displays different Actors that behave different states depending
 * of how are configured and the user interaction.
 * [SINGLETON] Just One game can be used at the same time on an Application.
 *
 * TODO Add FPS control of timer.
 * TODO Add a track about the form that contains it or the canvas where it must be drawn
 */
class Game {

public:
	Game();
	virtual ~Game();

//	// Game Controller Functionality
	void process(void);
	void render(Graphics::Canvas* currentCanvas);
//
//	// Scene Manager Functionality
	void addScene();
	void setScene();
	Scene* getCurrentScene();
	void removeScene();
//
//	// Touch Controller Functionality

private:
	Base::Collection::ArrayListT<Scene*>* m_scenes;

};

}

#endif /* GAME_H_ */
