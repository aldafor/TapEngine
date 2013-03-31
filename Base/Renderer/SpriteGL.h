/*
 * SpriteGL.h
 *
 *  Created on: 11/11/2010
 *      Author: Administrador
 */

#ifndef SPRITEGL_H_
#define SPRITEGL_H_

#include <FBase.h>
#include <FMedia.h>
#include <FGraphics.h>
#include <FGraphicsOpenGL.h>
#include <FSystem.h>
#include "Sprite.h"
#include "TextureGL.h"
#include "Quaternion.h"

class SpriteGL : public BadaGameLib::Sprite {
public:
	SpriteGL(const Osp::Base::String& texturePath, BadaGameLib::Quaternion* position, float width, float height);
	SpriteGL(BadaGameLib::Quaternion* position, float width, float height);
	virtual ~SpriteGL();

	virtual void update(long long gameTime);
	virtual void draw(long long gameTime);

	void setTexture(BadaGameLib::TextureGL* textureID);

private:
	Osp::Graphics::Opengl::GLuint* loadTexture(const Osp::Base::String& sPath);
	void updateTextureSize(int& iWidth, int& iHeight);

protected:
	BadaGameLib::TextureGL* m_pTexture;

	long long m_lAccumTime;

	bool m_bTransparent;
};

#endif /* SPRITEGL_H_ */
