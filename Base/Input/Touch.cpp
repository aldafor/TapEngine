/*
 * Touch.cpp
 *
 *  Created on: 06/10/2010
 *  Author: Daniel Conde Linares
 */

#include "Touch.h"

namespace BadaGameLib {

Touch::Touch(int data, int x, int y, int width, int height) {
	m_data   = data;
	m_x      = x;
	m_y      = y;
	m_width  = width;
	m_height = height;
}

Touch::~Touch() {
	// TODO Auto-generated destructor stub
}

bool Touch::isTouched(int x, int y)
{
	if ( x>= m_x && y>= m_y && x<=m_x+m_width && y<=m_y+m_height )
		return true;
	else
		return false;
}

bool Touch::isTouched(int x, int y, int width, int height)
{
	if ( x>= m_x && y>= m_y && width<=m_width && height<=m_height )
		return true;
	else
		return false;
}

int Touch::getData()
{
	return m_data;
}

}
