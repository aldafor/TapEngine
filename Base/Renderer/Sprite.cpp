/*
 * Sprite.cpp
 *
 *  Created on: 01/10/2010
 *      Author: Daniel Conde Linares
 */

#include "Sprite.h"

namespace BadaGameLib {

Sprite::Sprite(Quaternion* position) {
	m_pPosition = position;
}

Sprite::~Sprite() {
	if(m_pPosition)
	{
		delete m_pPosition;
		m_pPosition = NULL;
	}
}

// Spatial Methods
Quaternion* Sprite::getPosition() {
	return m_pPosition;
}
void Sprite::setPosition(Quaternion* position) {
	delete m_pPosition;
	m_pPosition=position;
}

float Sprite::getSpin() {
	return m_fSpinAngle;
}
void Sprite::setSpin(const float& spin) {
	m_fSpinAngle = spin;
}

}
