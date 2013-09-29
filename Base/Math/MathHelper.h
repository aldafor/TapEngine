/*
 * MathHelper.h
 *
 *  Created on: 28/10/2010
 *  Author: Daniel Conde Linares
 *  Taked from Wikipedia Implementation
 */

namespace BadaGameLib {

static float Clamp(float X, float Min, float Max) {
	if (X > Max)
		X = Max;
	else if (X < Min)
		X = Min;

	return X;
};

};
