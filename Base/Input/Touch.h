/*
 * Touch.h
 *
 *  Created on: 06/10/2010
 *      Author: Daniel Conde Linares
 */

#ifndef TOUCH_H_
#define TOUCH_H_

namespace BadaGameLib {

/**
* @class Touch
*
* @brief Class for encapsulate the Touch functionality.
*
* A Touch is linked with a enum data that hold a behaviour (State) on an Actor. Linked Actor must manage
* all its linked Touchs and check them for keep their data and process the behaviour (State).
*
* @see Actor
*/
class Touch
{

public:
	explicit Touch(int data, int x, int y, int widht, int height);
	virtual ~Touch();

	// Control
	bool isTouched(int x, int y);
	bool isTouched(int x, int y, int widht, int height);

	// Touch Info
	int getData();

private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;

	int m_data;
};

}

#endif /* TOUCH_H_ */
