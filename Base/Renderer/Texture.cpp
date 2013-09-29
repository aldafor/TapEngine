/*
 * Texture.cpp
 *
 *  Created on: 29/09/2010
 *      Author: Daniel Conde Linares
 */

#include "Texture.h"

namespace BadaGameLib {

Texture::Texture() {
	// TODO Auto-generated constructor stub
	m_render = Texture::FULLSIZE;
}

Texture::~Texture() {
	// TODO Auto-generated destructor stub
	delete m_Bitmap;
	AppLog("Destruyendo Texture");
}

bool Texture::loadFromFile(const String& srcImagePath)
{
	result r;
	m_ImagePath = srcImagePath;
	Image* m_BitmapDecoder = new Image();
	r = m_BitmapDecoder->Construct();
	if(IsFailed(r))
	{
		AppLog("BadaGameLib: Failed to construct decoder on Texture Class");
		return false;
	}

	if(srcImagePath.EndsWith(L"jpg"))
	{
		m_Bitmap = m_BitmapDecoder->DecodeN(srcImagePath, BITMAP_PIXEL_FORMAT_RGB565);
	}
	else if(srcImagePath.EndsWith(L"bmp"))
	{
		m_Bitmap = m_BitmapDecoder->DecodeN(srcImagePath, BITMAP_PIXEL_FORMAT_RGB565);
	}
	else if(srcImagePath.EndsWith(L"png"))
	{
		m_Bitmap = m_BitmapDecoder->DecodeN(srcImagePath, BITMAP_PIXEL_FORMAT_ARGB8888);
	}
	else if (srcImagePath.EndsWith(L"gif"))
	{
		m_Bitmap = m_BitmapDecoder->DecodeN(srcImagePath, BITMAP_PIXEL_FORMAT_RGB565);
	}
	//m_Bitmap->Scale(Graphics::Dimension(getWidth(), getHeight()));

	if (!m_Bitmap)
	{
		AppLog("BadaGameLib: Failed to decode the image on Texture Class");
		return false;
	}

	delete m_BitmapDecoder;

	return true;
}

bool Texture::loadFromBitmap(Graphics::Bitmap* srcBitmap)
{
	if (!srcBitmap)
	{
		AppLog("BadaGameLib: Failed to load From Bitmap on Texture Class");
		return false;
	}
	else
	{
		m_Bitmap = srcBitmap;
		return true;
	}
}

String Texture::getPath()
{
	return m_ImagePath;
}

int Texture::getWidth()
{
	return m_Bitmap->GetWidth();
}

int Texture::getHeight()
{
	return m_Bitmap->GetHeight();
}

void Texture::scale(const Graphics::Dimension& newDimension)
{
	result r;
	AppLog("BadaGameLib: Escalando Textura (%i,%i) -> (%i, %i)", getWidth(), getHeight(), newDimension.width, newDimension.height);
	r = m_Bitmap->Scale(newDimension);
	if(IsFailed(r))
	{
		AppLog("BadaGameLib: Failed to Scale on Texture Class");
	}
}

const Graphics::Bitmap* Texture::getAsBitmap()
{
	return m_Bitmap;
}

Texture::eRenderState Texture::getRenderState()
{
	return m_render;
}

void Texture::setRenderState( Texture::eRenderState state)
{
	m_render = state;
}

}
