/*
 * Actor.cpp
 *
 *  Created on: 01/10/2010
 *      Author: Daniel Conde Linares
 */

#include "ActorAnimated2D.h"

namespace BadaGameLib {

ActorAnimated2D::ActorAnimated2D(int x, int y, Dimension* dim, const String& path) {
	// TODO Auto-generated constructor stub
	 //m_statesMap = new Collection::ArrayListT<Collection::MapEntryT<String, Graphics2D::Sprite*> >();
	 //m_statesMap->Construct();
	 m_llElapsedTime = 0;

	 m_sAppareance = new Sprite2D(x, y, dim);
	 m_sAppareance->loadTexture(path, Texture::DEFAULT);
}

ActorAnimated2D::~ActorAnimated2D() {
	// TODO Auto-generated destructor stub
}

void ActorAnimated2D::process(long long elapsedTime)
{
	m_llElapsedTime = m_llElapsedTime + elapsedTime;
}

result ActorAnimated2D::draw(Graphics::Canvas* currentCanvas)
{
	// TODO: Implement drawing method
	result r = E_SUCCESS;
	//m_statesMap->GetValue(m_currentState);
	//Graphics2D::Sprite* temp = (Graphics2D::Sprite*) m_statesMap->GetValue(m_currentState);
	m_sAppareance->draw(currentCanvas);
	//temp->draw(destPoint, currentCanvas);
	return r;
}

}
