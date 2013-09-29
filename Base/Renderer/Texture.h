/*
 * Texture.h
 *
 *  Created on: 29/09/2010
 *      Author: Daniel Conde Linares
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <FBase.h>
#include <FGraphics.h>
#include <FMedia.h>
#include <FIo.h>

using namespace Osp::Base;
using namespace Osp;
using namespace Osp::Io;
using namespace Osp::Media;
using namespace Osp::Graphics;

namespace BadaGameLib {

/**
* @class Texture
*
* @brief Class for encapsulate the Texture functionality.
*
* A Texture is an image that will be created when you load it from a file or a Bada Bitmap.
* It's should be linked with an Sprite (or Sprite type) to be drawed on the screen.
*
* @see Sprite
*/
class Texture {
public:
	Texture();
	virtual ~Texture();

	bool loadFromFile(const String& srcImagePath);
	bool loadFromBitmap(Bitmap* srcBitmap);
	String getPath();
	int getWidth();
	int getHeight();
	void scale(const Dimension& newDimension);
	const Bitmap* getAsBitmap();

public:
	enum eRenderState {
		DEFAULT = 0,
		PROPORTIONAL_ANDROID,
		FIT_WIDTH,
		FIT_HEIGHT,
		FULLSIZE,
		FULLSIZE_LANDSCAPE
	};

public:
	eRenderState getRenderState();
	void setRenderState( eRenderState state);

private:
	Bitmap* m_Bitmap;

	String m_ImagePath;

	eRenderState m_render;
};



}

#endif /* TEXTURE_H_ */
