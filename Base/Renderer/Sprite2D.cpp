/*
 * Sprite.cpp
 *
 *  Created on: 29/09/2010
 *      Author: Daniel Conde Linares
 */

#include "Sprite2D.h"

namespace BadaGameLib {

Sprite2D::Sprite2D(int x, int y, Dimension* dimension) : Sprite(new Quaternion(x, y, 0.0f)) {
	// TODO Auto-generated constructor stub
	m_Texture = new Texture();

	m_dimension = dimension;
	m_rateDimension = new Dimension();
}

Sprite2D::Sprite2D(int x, int y, Dimension* dimension, const String& srcImagePath, Texture::eRenderState state) : Sprite(new Quaternion(x, y, 0.0f))
{
	m_Texture = new Texture();

	m_dimension = dimension;
	m_rateDimension = new Dimension();

	loadTexture(srcImagePath, state);
}

Sprite2D::~Sprite2D() {
	// TODO Auto-generated destructor stub
	delete m_Texture;
	delete m_rateDimension;
	AppLog("Destruyendo Sprite2D");
}

void Sprite2D::loadTexture(const String& srcImagePath)
{
	this->loadTexture(srcImagePath, Texture::DEFAULT);
}

void Sprite2D::loadTexture(const String& srcImagePath, const Dimension& newDimension)
{
	this->loadTexture(srcImagePath, Texture::DEFAULT);
	m_Texture->scale(newDimension);
}

void Sprite2D::loadTexture(const String& srcImagePath, Texture::eRenderState state)
{
	AppLog("Creando Imagen: newImage->loadFromFile(%S)", srcImagePath.GetPointer());
	m_Texture->loadFromFile(srcImagePath);
	m_Texture->setRenderState(state);
	switch(state)
	{
	case Texture::PROPORTIONAL_ANDROID:
	{
		float rateW = (float)m_dimension->width / (float)320.0f;
		float rateH = (float)m_dimension->height / (float)480.0f;
		m_rateDimension->width = m_Texture->getWidth()*rateW;
		m_rateDimension->height = m_Texture->getHeight()*rateH;
		AppLog("BadaGameLib: Scale rate %f,%f: %i/%i", rateW, rateH, m_Texture->getWidth(), m_Texture->getHeight());
		m_Texture->scale(*m_rateDimension);
		break;
	}
	case Texture::FIT_WIDTH:
	{
		float rate = (float)m_dimension->width / (float)m_Texture->getWidth();
		m_rateDimension->width = m_dimension->width;
		m_rateDimension->height = m_Texture->getHeight()*rate;
		AppLog("BadaGameLib: Scale rate %f: %i / %i", rate, m_dimension->width, m_Texture->getWidth());
		m_Texture->scale(*m_rateDimension);
		break;
	}
	case Texture::FIT_HEIGHT:
	{
		float rate = m_dimension->height / m_Texture->getHeight();
		m_rateDimension->height = m_dimension->height;
		m_rateDimension->width = m_Texture->getWidth()*rate;
		AppLog("BadaGameLib: Scale rate %f", rate);
		m_Texture->scale(*m_rateDimension);
		//float rate = temp->getHeight()/m_dimension->height;
		//r = drawBuffer->Merge(destPoint, *(temp->getAsBitmap()), Graphics::Rectangle(0, 0, temp->getWidth()*rate, m_dimension->height));
		break;
	}
	case Texture::FULLSIZE:
	{
		m_Texture->scale(*m_dimension);
		break;
	}
	case Texture::FULLSIZE_LANDSCAPE:
	{
		m_rateDimension->height = m_dimension->width;
		m_rateDimension->width = m_dimension->height;
		m_Texture->scale(*m_rateDimension);
		break;
	}
	default:
	{
		//m_rateDimension->height = newImage->getHeight();
		//m_rateDimension->width = newImage->getWidth();
		//newImage->scale(*m_rateDimension);
		//r = drawBuffer->Merge(destPoint, *(temp->getAsBitmap()), Graphics::Rectangle(0, 0, temp->getWidth(), temp->getHeight()));
		break;
	}
	}
}

int Sprite2D::getCenterX()
{
	return (getPosition())->m_fX + getWidth()/2;
}

int Sprite2D::getCenterY()
{
	return (getPosition())->m_fY + getHeight()/2;
}

int Sprite2D::getWidth()
{
	return m_Texture->getWidth();
}

int Sprite2D::getHeight()
{
	return m_Texture->getHeight();
}

const Point Sprite2D::getCenter()
{
	return Point(this->getWidth()/2, this->getHeight()/2);
}

void Sprite2D::scale(const Dimension& newDimension)
{
	//int xDif = getCenterX();
	//int yDif = getCenterY();
	m_Texture->scale(newDimension);
	//m_iX -= xDif-getCenterX();
	//m_iY -= yDif-getCenterY();
}

result Sprite2D::draw(Canvas* currentCanvas)
{
	result r = E_SUCCESS;
	r =	currentCanvas->DrawBitmap(Point((getPosition())->m_fX, (getPosition())->m_fY), *(m_Texture->getAsBitmap()));
	AppLog("BadaGameLib: Sprite2D->draw(%S)", (m_Texture->getPath()).GetPointer());

	return r;
}

result Sprite2D::draw(Graphics::Canvas* currentCanvas, const Graphics::Point& pivot, int degree)
{
	result r = E_SUCCESS;
	r = currentCanvas->DrawBitmap(Point((getPosition())->m_fX, (getPosition())->m_fY), *(m_Texture->getAsBitmap()), pivot, degree);

	return r;
}

result Sprite2D::draw(Graphics::Bitmap* drawBuffer)
{
	result r = E_SUCCESS;

	r = drawBuffer->Merge(Point((getPosition())->m_fX, (getPosition())->m_fY), *(m_Texture->getAsBitmap()), Graphics::Rectangle(0, 0, m_Texture->getWidth(), m_Texture->getHeight()));

	AppLog("BadaGameLib: Sprite2D->draw(%S)", (m_Texture->getPath()).GetPointer());

	return r;
}

}
