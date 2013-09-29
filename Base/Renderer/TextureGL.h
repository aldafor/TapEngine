/*
 * TextureGL.h
 *
 *  Created on: 29/09/2010
 *      Author: Daniel Conde Linares
 */

#ifndef TEXTUREGL_H_
#define TEXTUREGL_H_

#include <FBase.h>
#include <FMedia.h>
#include <FGraphicsOpenGL.h>
#include <FIo.h>

namespace BadaGameLib {

/**
* @class Texture
*
* @brief Class for encapsulate the Texture functionality.
*
* A Texture is an image that will be created when you load it from a file or a Bada Bitmap.
* It's should be linked with an Sprite (or Sprite type) to be drawn on the screen.
*
* @see Sprite
*/
class TextureGL {
public:
	TextureGL(bool resizable = true);
	virtual ~TextureGL();

	void loadFromFile(const Osp::Base::String& fileName);
	void loadFromFolder(const Osp::Base::String& folderName, const Osp::Base::String& tokenAnim, const int& numFrames);
	void loadFromFolder(const Osp::Base::String& folderName, const Osp::Base::String& tokenAnim, const int& numFrames, const int& initLoopFrames, const int& endLoopFrames );
	void updateTextureSize(int& iWidth, int& iHeight);

	Osp::Graphics::Opengl::GLuint* getTextureID();

	void updateTexture();
	bool isAnimated();
	bool isAnimEnded();
	void resetTexture();

private:
	Osp::Base::Collection::ArrayListT<Osp::Graphics::Opengl::GLuint*> m_arrayID;
	int m_iCurrentFrame, m_iStartLoopFrame, m_iEndLoopFrame, m_iTotalFrames;
	int m_iWidth, m_iHeight;
	static const Osp::Graphics::Opengl::GLfloat m_fDefaultCooords[];

	bool m_bResizable;
};

}

#endif /* TEXTURE_H_ */
