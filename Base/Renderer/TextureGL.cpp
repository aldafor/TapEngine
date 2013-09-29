#include "TextureGL.h"

using namespace Osp::Base;
using namespace Osp;
using namespace Osp::Io;
using namespace Osp::Media;
using namespace Osp::Graphics;
using namespace Osp::Graphics::Opengl;

namespace BadaGameLib {

const GLfloat TextureGL::m_fDefaultCooords[] =
{
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f
};

TextureGL::TextureGL(bool resizable)
{
	m_arrayID.Construct();
	m_iCurrentFrame = 0;
	m_iStartLoopFrame = 0;
	m_iEndLoopFrame = 0;
	m_iTotalFrames = 0;
	m_iWidth = 0;
	m_iHeight = 0;
	m_bResizable = resizable;
}

TextureGL::~TextureGL()
{
	GLuint* l_pTextureID;
	for(int i = 0; i<m_arrayID.GetCount(); ++i)
	{
		m_arrayID.GetAt(i, l_pTextureID);
		glDeleteTextures(1, l_pTextureID);
		delete l_pTextureID;
	}
	m_arrayID.RemoveAll();
}

void TextureGL::loadFromFile(const String& fileName)
{
	GLuint* lTexture = new GLuint;
	Bitmap* l_pBitmap;

	result r = E_SUCCESS;
	Image* m_BitmapDecoder = new Image();
	r = m_BitmapDecoder->Construct();
	if (IsFailed(r)) {
		AppLog("BadaGameLib: Failed to construct decoder on Texture Class");
	}

	l_pBitmap = m_BitmapDecoder->DecodeN(fileName, BITMAP_PIXEL_FORMAT_R8G8B8A8);

	if (!l_pBitmap)
	{
		AppLog("BadaGameLib: Failed to decode the image on Texture Class");
		AppLog("BadaGameLib: %S failed", fileName.GetPointer());
	}

	delete m_BitmapDecoder;

	m_iWidth = l_pBitmap->GetWidth();
	m_iHeight = l_pBitmap->GetHeight();
	if(m_bResizable)
	{
		updateTextureSize(m_iWidth, m_iHeight);
		l_pBitmap->Scale(Dimension(m_iWidth, m_iHeight));
	}
	//AppLog("Tamaño final de la textura: %i x %i", m_iWidth, m_iHeight);
	BufferInfo buffer;

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

	//AppLog("Textura %i Creada", *lTexture);
	m_arrayID.Add(lTexture);

}

void TextureGL::loadFromFolder(const String& folderName, const String& tokenAnim, const int& numFrames )
{
	result r = E_SUCCESS;

	m_iTotalFrames = numFrames;

	for (int var = 0; var < m_iTotalFrames; ++var) {
		String nameStr;
		nameStr.Append(folderName);
		nameStr.Append(tokenAnim);
		nameStr.Append("_");
		if (var<1000) {
			nameStr.Append(0);
		}
		if (var<100) {
			nameStr.Append(0);
		}
		if (var<10) {
			nameStr.Append(0);
		}
		nameStr.Append((int)var);
		nameStr.Append(".jpg");
		//AppLog("Creando Imagen %i/%i: newImage->loadFromFile(%S);", var+1, m_iTotalFrames, nameStr.GetPointer());
		loadFromFile(nameStr);
	}
}

void TextureGL::loadFromFolder(const String& folderName, const String& tokenAnim, const int& numFrames, const int& initLoopFrames, const int& endLoopFrames )
{
	loadFromFolder( folderName, tokenAnim, numFrames );
	m_iStartLoopFrame = initLoopFrames;
	m_iEndLoopFrame = endLoopFrames;
}

void TextureGL::updateTextureSize(int& iWidth, int& iHeight)
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

GLuint* TextureGL::getTextureID()
{
	if(m_iCurrentFrame<m_arrayID.GetCount())
	{
	GLuint* l_pTextureID;
	result r = E_SUCCESS;
	r = m_arrayID.GetAt(m_iCurrentFrame, l_pTextureID);
	if(IsFailed(r))
	{
		AppLog("BadaGameLib: Failed to get the current Texture ID");
		return NULL;
	}

	return l_pTextureID;
	}
	else
	{
		return NULL;
	}
}

void TextureGL::updateTexture()
{
	// AppLog("Current: %i ArrayLast: %i StartLoop: %i EndLoop: %i", m_iCurrentFrame, m_arrayID.GetCount()-1, m_iStartLoopFrame, m_iEndLoopFrame);
	if(m_iCurrentFrame < m_arrayID.GetCount()-1)
	{
		++m_iCurrentFrame;
		if(m_iEndLoopFrame != 0)
		{
			if(m_iCurrentFrame > m_iEndLoopFrame)
				m_iCurrentFrame = m_iStartLoopFrame;
		}
	}
	else
	{
		if(m_iEndLoopFrame != 0)
		{
			if(m_iCurrentFrame >= m_iEndLoopFrame)
				m_iCurrentFrame = m_iStartLoopFrame;
		}
		//m_iCurrentFrame = 0;
	}
}

bool TextureGL::isAnimated()
{
	return m_iTotalFrames > 1;
}

bool TextureGL::isAnimEnded()
{
	return m_iCurrentFrame >= m_arrayID.GetCount()-1;
}

void TextureGL::resetTexture()
{
	m_iCurrentFrame = 0;
}

}
