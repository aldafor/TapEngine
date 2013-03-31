/*
 * ResourceManager.cpp
 *
 *  Created on: 23/11/2010
 *      Author: Administrador
 */
/*
#include "ResourceManager.h"

using namespace Osp;
using namespace Osp::Base;
using namespace Osp::Graphics::Opengl;

ResourceManager* ResourceManager::m_pInstance = NULL;

ResourceManager* ResourceManager::getInstance() {
	if(m_pInstance == NULL) {
		m_pInstance = new ResourceManager();
	}

	return m_pInstance;
}

ResourceManager::ResourceManager() {
	// TODO Auto-generated constructor stub
	m_TexturesMap.Construct();
}

ResourceManager::~ResourceManager() {
	// TODO Auto-generated destructor stub
}

GLuint* ResourceManager::getTexture(const String& resourceName)
{
	GLuint* pTextureID = NULL;
	Collection::IMapEnumeratorT<int, GLuint*>* pMapEnum = m_TexturesMap.GetMapEnumeratorN();

	while (pMapEnum->MoveNext() == E_SUCCESS)
	{
		int tempKey;
		pMapEnum->GetKey(tempKey);
		if(tempKey == resourceName.GetHashCode())
		{
			pMapEnum->GetValue(pTextureID);
			break;
		}
	}

	delete pMapEnum;

	if (!pTextureID)
	{
		Osp::Graphics::Bitmap* l_pBitmap;
		result r;
		Osp::Media::Image* m_BitmapDecoder = new Osp::Media::Image();
		r = m_BitmapDecoder->Construct();
		if (IsFailed(r)) {
			AppLog("BadaGameLib: Failed to construct decoder on Texture Class");
			return false;
		}

		l_pBitmap = m_BitmapDecoder->DecodeN(resourceName,
				Osp::Graphics::BITMAP_PIXEL_FORMAT_R8G8B8A8);
		if (!l_pBitmap) {
			AppLog("BadaGameLib: Failed to decode the image on Texture Class");
			return false;
		}

		delete m_BitmapDecoder;

		int iWidth = l_pBitmap->GetWidth();
		int iHeight = l_pBitmap->GetHeight();
		updateTextureSize(iWidth, iHeight);
		l_pBitmap->Scale(Osp::Graphics::Dimension(iWidth, iHeight));
		AppLog("Tamaño final de la textura: %i x %i", iWidth, iHeight);
		Osp::Graphics::BufferInfo buffer;

		l_pBitmap->Lock(buffer);
		glGenTextures(1, pTextureID);
		glBindTexture(GL_TEXTURE_2D, *pTextureID);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, l_pBitmap->GetWidth(),
				l_pBitmap->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
				buffer.pPixels);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		l_pBitmap->Unlock();
		delete l_pBitmap;

		AppLog("Textura %i Creada", *pTextureID);
	}

	return pTextureID;
}
bool ResourceManager::releaseTexture(const String& resourceName)
{
	GLuint* pTextureID = NULL;
	Collection::IMapEnumeratorT<int, GLuint*>* pMapEnum =
			m_TexturesMap.GetMapEnumeratorN();

	while (pMapEnum->MoveNext() == E_SUCCESS) {
		int tempKey;
		pMapEnum->GetKey(tempKey);
		if(tempKey == resourceName.GetHashCode()) {
			pMapEnum->GetValue(pTextureID);
			glDeleteTextures(1, pTextureID);
			m_TexturesMap.Remove(tempKey);
			break;
		}
	}

	return true;
}

void updateTextureSize(int& iWidth, int& iHeight)
{
    int i;
	bool bSizeToFit = false;

	if ((iWidth != 1) && (iWidth & (iWidth - 1))) {
		i = 1;
		while ((bSizeToFit ? 2 * i : i) < iWidth)
			i *= 2;
		iWidth = i;
	}

	if ((iHeight != 1) && (iHeight & (iHeight - 1))) {
		i = 1;
		while ((bSizeToFit ? 2 * i : i) < iHeight)
			i *= 2;
		iHeight = i;
	}
}
*/
