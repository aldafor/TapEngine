/*
 * Scene.cpp
 *
 *  Created on: 06/10/2010
 *      Author: Daniel Conde Linares
 */

#include "Scene.h"

namespace BadaGameLib {

Scene::Scene()
{
	// TODO Auto-generated constructor stub
	m_actors = new Base::Collection::ArrayListT<Actor*>();
	m_actors->Construct();
}

Scene::~Scene() {
	// TODO Auto-generated destructor stub
	Actor* tempActor;
	for (int i = 0; i < m_actors->GetCount(); ++i)
	{
		m_actors->GetAt(i, tempActor);
		delete tempActor;
	}

	m_actors->RemoveAll();

	delete m_actors;

	AppLog("Destruyendo Scene");
}

void Scene::process(int delayedTime)
{
	Actor* tempActor;
	for (int i = 0; i < m_actors->GetCount(); ++i)
	{
		m_actors->GetAt(i, tempActor);
		tempActor->process(delayedTime);
	}
}

void Scene::processTouch(const Graphics::Point& touchPoint)
{
	Actor* tempActor;
	for (int i = 0; i < m_actors->GetCount(); ++i)
	{
		m_actors->GetAt(i, tempActor);
		tempActor->processTouchs(touchPoint);
	}
}

void Scene::render(Graphics::Canvas* currentCanvas)
{
	Graphics::Bitmap* buffer = new Graphics::Bitmap();
	Graphics::Dimension dim((currentCanvas->GetBounds()).width, (currentCanvas->GetBounds()).height);
	buffer->Construct(dim, Graphics::BITMAP_PIXEL_FORMAT_RGB565);
	Actor* tempActor;
	for (int i = 0; i < m_actors->GetCount(); ++i)
	{
		m_actors->GetAt(i, tempActor);
		tempActor->render(buffer);
	}
	buffer->Scale(Graphics::Dimension(480,800));
	currentCanvas->DrawBitmap(Graphics::Point(0, 0), *buffer);
	delete buffer;
}

void Scene::render(Graphics::Bitmap* drawBuffer)
{
	Actor* tempActor;
	for (int i = 0; i < m_actors->GetCount(); ++i)
	{
		m_actors->GetAt(i, tempActor);
		tempActor->render(drawBuffer);
		AppLog("Pintando actor %i", i);
	}
}

Actor* Scene::getActor(int actorID)
{
	Actor* tempActor = NULL;
	if (actorID < m_actors->GetCount())
	{
		m_actors->GetAt(actorID, tempActor);
	}
	return tempActor;
}

int Scene::getNActors()
{
	return m_actors->GetCount();
}

void Scene::addActor(Actor* newActor)
{
	m_actors->Add(newActor);
}

void Scene::removeActor(Actor* newActor)
{
	m_actors->Remove(newActor);
}

}
