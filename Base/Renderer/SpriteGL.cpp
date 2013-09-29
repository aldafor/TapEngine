/*
 * SpriteGL.cpp
 *
 *  Created on: 11/11/2010
 *      Author: Daniel Conde Linares
 */

#include "SpriteGL.h"

using namespace Osp::Graphics::Opengl;
using namespace Osp::Base;
using namespace BadaGameLib;

SpriteGL::SpriteGL(const String& texturePath, Quaternion* position, float width, float height) : Sprite(position) {
	// TODO Auto-generated constructor stub
	//m_TextureID = loadTexture(texturePath);
	m_pTexture = new TextureGL();
	m_pTexture->loadFromFile(texturePath);
	m_fWidth = width;
	m_fHeight = height;
	m_fSpinAngle = 0;
	m_bTransparent = false;
	Osp::System::SystemTime::GetTicks(m_lAccumTime);
}

SpriteGL::SpriteGL(Quaternion* position, float width, float height) : Sprite(position) {
	// TODO Auto-generated constructor stub
	m_pTexture = NULL;
	m_fWidth = width;
	m_fHeight = height;
	m_fSpinAngle = 0;
	m_bTransparent = false;
	Osp::System::SystemTime::GetTicks(m_lAccumTime);
}

SpriteGL::~SpriteGL() {
	// TODO Auto-generated destructor stub
	delete m_pTexture;
}

void SpriteGL::update(long long gameTime) {
	// Update method
	//if(gameTime - m_lAccumTime >1000 / 12) { // For 12 Frames per Second
		m_pTexture->updateTexture();
		//m_lAccumTime = gameTime;
	//}
}

void SpriteGL::draw(long long gameTime)
{
	glPushMatrix();
	//glTranslatef(m_fTranslateX, m_fTranslateY, m_fTranslateZ);
	// TODO Test efficiency between set SpriteCoords based on Position, or make translation
	GLfloat l_spriteCoords[] = {
		m_pPosition->m_fX, m_pPosition->m_fY, m_pPosition->m_fZ,
		m_pPosition->m_fX + m_fWidth, m_pPosition->m_fY, m_pPosition->m_fZ,
		m_pPosition->m_fX, m_pPosition->m_fY + m_fHeight, m_pPosition->m_fZ,
		m_pPosition->m_fX + m_fWidth, m_pPosition->m_fY + m_fHeight, m_pPosition->m_fZ,
	};
	GLfloat l_textureCoord[] = { 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };

	if(m_fSpinAngle != 0)
		glRotatef(m_fSpinAngle, 0.0f, 0.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *(m_pTexture->getTextureID()));

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, l_spriteCoords);
	glTexCoordPointer(2, GL_FLOAT, 0, l_textureCoord);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glPopMatrix();
}

GLuint* SpriteGL::loadTexture(const String& sPath) {
	GLuint* lTexture = new GLuint;
	Osp::Graphics::Bitmap* l_pBitmap;

	result r;
	Osp::Media::Image* m_BitmapDecoder = new Osp::Media::Image();
	r = m_BitmapDecoder->Construct();
	if (IsFailed(r)) {
		AppLog("BadaGameLib: Failed to construct decoder on Texture Class");
		return false;
	}

	l_pBitmap = m_BitmapDecoder->DecodeN(sPath,Osp::Graphics::BITMAP_PIXEL_FORMAT_R8G8B8A8);
	if (sPath.EndsWith(L"jpg"))
	{
//		l_pBitmap = m_BitmapDecoder->DecodeN(sPath,Osp::Graphics::BITMAP_PIXEL_FORMAT_R8G8B8A8);
	}
	else if(sPath.EndsWith(L"bmp"))
	{
//		l_pBitmap = m_BitmapDecoder->DecodeN(sPath, Osp::Graphics::BITMAP_PIXEL_FORMAT_R8G8B8A8);
	}
	else if(sPath.EndsWith(L"png"))
	{
//		l_pBitmap = m_BitmapDecoder->DecodeN(sPath, Osp::Graphics::BITMAP_PIXEL_FORMAT_R8G8B8A8);
		m_bTransparent = true;
	}
	else if (sPath.EndsWith(L"gif"))
	{
//		l_pBitmap = m_BitmapDecoder->DecodeN(sPath, Osp::Graphics::BITMAP_PIXEL_FORMAT_R8G8B8A8);
	}

	if (!l_pBitmap)
	{
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
	glGenTextures(1, lTexture);
	glBindTexture(GL_TEXTURE_2D, *lTexture);
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

	AppLog("Textura %i Creada", *lTexture);

	return lTexture;
}

void SpriteGL::setTexture(TextureGL* textureID)
{
	/*if(m_pTexture)
	{
		delete m_pTexture;
	}*/
	// If we treat it like a instance of the "resource manager" OsitoActor, we can't delete the texture
	m_pTexture = textureID;
}

void SpriteGL::updateTextureSize(int& iWidth, int& iHeight)
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
