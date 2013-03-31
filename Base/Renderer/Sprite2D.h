/*
 * Sprite.h
 *
 *  Created on: 29/09/2010
 *      Author: Daniel Conde Linares
 */

#ifndef SPRITE2D_H_
#define SPRITE2D_H_

#include "Texture.h"
#include "Sprite.h"
#include "Quaternion.h"

using namespace Osp::Graphics;

namespace BadaGameLib {

/**
* @class Sprite2D
*
* @brief Class for encapsulate the static Sprites functionality.
*
* A Sprite2D is a kind of Sprite that hold a Texture inside.
*
* @see Sprite
* @see Texture
*/
class Sprite2D : public Sprite {
public:
	Sprite2D(int x, int y, Dimension* dimension);
	Sprite2D(int x, int y, Dimension* dimension, const String& srcImagePath, Texture::eRenderState state = Texture::DEFAULT);
	virtual ~Sprite2D();

	void loadTexture(const String& srcImagePath);
	void loadTexture(const String& srcImagePath, const Dimension& newDimension);
	void loadTexture(const String& srcImagePath, Texture::eRenderState state);

	virtual int getCenterX();
	virtual int getCenterY();

	virtual int getWidth();
	virtual int getHeight();

	const Point getCenter();

	void scale(const Dimension& newDimension);

	virtual void draw(const long long gameTime) {};
	virtual result draw(Canvas* currentCanvas);
	virtual result draw(Bitmap* drawBuffer);
	virtual result draw(Canvas* currentCanvas, const Point& pivot, int degree);

private:
	Dimension* m_dimension;
	Dimension* m_rateDimension;
	Texture* m_Texture;
};

}

#endif /* SPRITE2D_H_ */
