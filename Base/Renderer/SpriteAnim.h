/*
 * SpriteAnim.h
 *
 *  Created on: 01/10/2010
 *      Author: Daniel Conde Linares
 */

#ifndef SPRITEANIM_H_
#define SPRITEANIM_H_

#include <FBase.h>
#include <FGraphics.h>
#include <FIo.h>
#include <FUi.h>
#include "Sprite.h"
#include "Sprite2D.h"

using namespace Osp::Base;
using namespace Osp::Graphics;
using namespace Osp;
using namespace Osp::Io;

namespace BadaGameLib {


/**
* @class SpriteAnim
*
* @brief Class for encapsulate the Animated Sprites functionality.
*
* A SpriteAnim is a kind of Sprite that hold a determined number of Textures inside.
* Everytime it draw the determined Texture depending of how it is configured.
*
* @see Sprite
* @see Texture
*/
class SpriteAnim: public Sprite {
public:
	SpriteAnim(int x, int y, Graphics::Dimension* dimension, bool loop = false);
	virtual ~SpriteAnim();

	result loadFromFolder(const String& srcAnimPath, const int& numFrames);
	result loadFromFolder(const String& srcAnimPath, const String& endToken, const int& numFrames, Texture::eRenderState state);
	result loadFromSpriteSheet(const String& srcAnimPath, const Graphics::Dimension& spriteOnSheetSize, const int& numFrames);

	//int getCurrentFrame();

public:
	virtual int getCenterX();
	virtual int getCenterY();

	virtual int getWidth();
	virtual int getHeight();

	virtual void scale(const Graphics::Dimension& newDimension);

	virtual result draw(Graphics::Canvas* currentCanvas);
	virtual result draw(Graphics::Bitmap* drawBuffer);
	// For Compatibility with new GL Framework
	virtual void draw( const long long gameTime ) {};

public:
	void nextFrame();
	void play();
	void stop();
	void togleLoop();
	bool isAnimationPlaying();
	bool isAnimationOver();

	void setLoopFrames(int initLoop, int endLoop);
	bool isAnimationLooping();

	void setAnimationReverse(bool reverse);

private:
	int m_currentFrame;
	int m_totalFrames;

	int m_loopInitFrame;
	int m_loopEndFrame;

	bool m_animationPlaying;
	Collection::ArrayListT<Texture*> frameImageNames;

	bool m_animationLoop;
	bool m_animationReverse;

private:
	struct Frame
	{
		Graphics::Point position;
		Graphics::Dimension size;
	};

	Collection::ArrayListT<Frame*> frameSpriteNames;
	Graphics::Bitmap* spriteSheet;

	Dimension* m_dimension;
	Dimension* m_rateDimension;
};

}

#endif /* SPRITEANIM_H_ */
