/*
 * Sprite.h
 *
 *  Created on: 01/10/2010
 *      Author: Daniel Conde Linares
 */

#ifndef SPRITE_H_
#define SPRITE_H_

#include <FGraphics.h>
#include "Quaternion.h"

using namespace Osp;

namespace BadaGameLib {

/**
* @class Sprite
*
* @brief Class for encapsulate the Sprite functionality.
*
* A Sprite the simplest element that is used to draw an image on an Scene.
*
* @see Sprite
* @see Texture
*/
class Sprite {
public:
	Sprite(Quaternion* position);
	virtual ~Sprite();

	// Spatial Methods
	Quaternion* getPosition();
	void setPosition(Quaternion* position);
	float getSpin();
	void setSpin(const float& spin);

	// Dimension Methods
	float getWidth() { return m_fWidth; };
	float getHeight() { return m_fHeight; };

	// Update Method
	void update( const long long &gameTime ) {}; // Possibly can be not overloaded REFACTORING

	// Drawing Method
	virtual void draw( const long long &gameTime ) = 0;

protected:
	Quaternion* m_pPosition;
	float m_fSpinAngle;

	float m_fWidth;
	float m_fHeight;
};

}

#endif /* SPRITE_H_ */
