/*
 * Quaternion.cpp
 *
 *  Created on: 15/11/2010
 *      Author: Daniel Conde Linares
 */

#include "Quaternion.h"

namespace BadaGameLib {

Quaternion::Quaternion() {
	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_fW = 1.0f; // Normalize
}

Quaternion::Quaternion(float x, float y, float z) {
	m_fX = x;
	m_fY = y;
	m_fZ = z;
	m_fW = 1.0f; // Normalize
}

Quaternion::Quaternion(const Quaternion& a) {
	m_fX = a.m_fX;
	m_fY = a.m_fY;
	m_fZ = a.m_fZ;
	m_fW = a.m_fW;
}

Quaternion::~Quaternion() {
	// TODO Auto-generated destructor stub
}

Quaternion Quaternion::operator+(Quaternion& a) {
	return Quaternion(
			m_fX + a.m_fX,
			m_fY + a.m_fY,
			m_fZ + a.m_fZ
			);
}

void Quaternion::operator+=(Quaternion& a) {
	m_fX += a.m_fX;
	m_fY += a.m_fY;
	m_fZ += a.m_fZ;
}

Quaternion Quaternion::operator-(Quaternion& a) {
	return Quaternion(
			m_fX - a.m_fX,
			m_fY - a.m_fY,
			m_fZ - a.m_fZ
			);
}

void Quaternion::operator-=(Quaternion& a) {
	m_fX -= a.m_fX;
	m_fY -= a.m_fY;
	m_fZ -= a.m_fZ;
}

float Quaternion::product(Quaternion& a) {
	return ( m_fX*a.m_fX + m_fY*a.m_fY + m_fZ*a.m_fZ );
}

void Quaternion::operator=(Quaternion& a) {
	m_fX = a.m_fX;
	m_fY = a.m_fY;
	m_fZ = a.m_fZ;
	m_fW = a.m_fW;
}

bool Quaternion::operator==(Quaternion& a) {
	return ( m_fX==a.m_fX && m_fY==a.m_fY && m_fZ==a.m_fZ );
}

Quaternion Quaternion::operator*(Quaternion& a) {
	return Quaternion(
			m_fY*a.m_fZ - m_fZ*a.m_fY,
			m_fZ*a.m_fX - m_fX*a.m_fZ,
			m_fX*a.m_fY - m_fY*a.m_fX
			);
}

Quaternion Quaternion::operator*(float a) {
	return Quaternion(
			m_fX * a,
			m_fY * a,
			m_fZ * a
			);
}

float Quaternion::length() {
	return Osp::Base::Utility::Math::Sqrt(m_fX*m_fX + m_fY*m_fY + m_fZ*m_fZ);
}

void Quaternion::normalize()
{
	float len = length();
	m_fX = m_fX / len;
	m_fY = m_fY / len;
	m_fZ = m_fZ / len;
	m_fW = 1.0f;
}

}
