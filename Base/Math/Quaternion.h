/*
 * Quaternion.h
 *
 *  Created on: 15/11/2010
 *      Author: Daniel Conde Linares
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include <FBase.h>

namespace BadaGameLib {

class Quaternion {
public:
	Quaternion();
	Quaternion(float x, float y, float z);
	Quaternion(const Quaternion& a);
	virtual ~Quaternion();

	Quaternion operator+(Quaternion& a);
	void operator+=(Quaternion& a);
	Quaternion operator-(Quaternion& a);
	void operator-=(Quaternion& a);
	float product(Quaternion& a);
	void operator=(Quaternion& a);
	bool operator==(Quaternion& a);
	Quaternion operator*(Quaternion& a);
	Quaternion operator*(float a);
	float length();
	void normalize();

public:
	float m_fX, m_fY, m_fZ, m_fW;
};

}

#endif /* QUATERNION_H_ */
