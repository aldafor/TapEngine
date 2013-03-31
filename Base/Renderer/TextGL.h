/*
 * TextGL.h
 *
 *  Created on: 15/11/2010
 *      Author: Daniel Conde Linares
 */

#ifndef TEXTGL_H_
#define TEXTGL_H_

#include <FBase.h>
#include <FIo.h>
#include <FGraphics.h>
#include <FGraphicsOpenGL.h>
#include "SpriteGL.h"
#include "Quaternion.h"

class TextGL : public SpriteGL {
public:
	TextGL(const Osp::Base::String& texturePath, BadaGameLib::Quaternion* position, float width, float height);
	virtual ~TextGL();

	void setText(const Osp::Base::String& text);

	// virtual void update(long long gameTime);
	virtual void draw(long long gameTime);

private:
	void init(const Osp::Base::String& filePath);
	void initBFG(const Osp::Base::String& filePath);

private:
	Osp::Graphics::Opengl::GLuint m_DisplayListID;

	struct TextNode {
		mchar character;
		int x;
		int y;
		int width;
		int height;
	};
	TextNode* m_pText;
	int m_iTextSize;
	Osp::Base::Collection::ArrayListT<TextNode*>* m_pFontMap;
};

#endif /* TEXTGL_H_ */
