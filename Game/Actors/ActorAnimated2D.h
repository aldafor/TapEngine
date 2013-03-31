/*
 * ActorAnimated2D.h
 *
 *  Created on: 01/10/2010
 *      Author: Daniel Conde Linares
 */

#ifndef ACTORANIMATED2D_H_
#define ACTORANIMATED2D_H_

#include <FBase.h>
#include <FGraphics.h>
#include "Sprite2D.h"

using namespace Osp::Base;
using namespace Osp::Graphics;
using namespace Osp;

namespace BadaGameLib {

class ActorAnimated2D {
public:
	ActorAnimated2D(int x, int y, Dimension* dim, const String& path);
	virtual ~ActorAnimated2D();

	void process(long long elapsedTime);

	result draw(Canvas* currentCanvas);

private:
	Sprite2D* m_sAppareance;

	long long m_llElapsedTime;
};

}

#endif /* ACTORANIMATED2D_H_ */
