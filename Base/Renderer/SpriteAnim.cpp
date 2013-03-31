/*
 * SpriteAnim.cpp
 *
 *  Created on: 01/10/2010
 *      Author: Daniel Conde Linares
 */

#include "SpriteAnim.h"

namespace BadaGameLib {

SpriteAnim::SpriteAnim(int x, int y, Graphics::Dimension* dimension, bool loop) : Sprite(new Quaternion(x, y, 0.0f)) {
	// TODO Auto-generated constructor stub
	m_currentFrame = 0;
	m_totalFrames = 0;
	m_animationPlaying = false;

	m_loopInitFrame = 0;
	m_loopEndFrame = 0;

	m_animationLoop = loop;

	m_dimension = dimension;
	m_rateDimension = new Graphics::Dimension();

	m_animationReverse = false;
}

SpriteAnim::~SpriteAnim() {
	// TODO Auto-generated destructor stub
	delete m_rateDimension;

	//delete spriteSheet;
	frameImageNames.RemoveAll();
	AppLog("Destruyendo SpriteAnim");
}

/*result SpriteAnim::loadFromFolder(const String& srcFolderPath, const int& numFrames)
{
	Directory dir;
	DirEnumerator *pDirEnum = null;
	result r = E_SUCCESS;

	frameImageNames.Construct();

	// open directory
	r = dir.Construct(srcFolderPath);
	if(IsFailed(r))
	{
		AppLog("Badagamelib: Fail to open Directory on SpriteAnim");
		return r;
	}

	// read all directory entries
	pDirEnum = dir.ReadN();
	if(!pDirEnum){
		AppLog("Badagamelib: Fail to read Directory on SpriteAnim");
		r = GetLastResult();
		return r;
	}

	// loop through all directory entries
	while(pDirEnum->MoveNext() == E_SUCCESS)
	{
		DirEntry dirEntry = pDirEnum->GetCurrentDirEntry();
		String file = (dirEntry.GetName());
		Sprite2D* newImage = new Sprite2D();
		newImage->loadTexture(file, Graphics::Dimension(480, 800));
		frameImageNames.Add(newImage);
		//AppLog((const char *)message.GetPointer());
		//printf();
		// do something...
	}

	// delete enumerator
	if(pDirEnum)
		delete pDirEnum;

	// opened directory will be closed automatically when the destructor of the Directory class is invoked.
	return r;
}*/

result SpriteAnim::loadFromFolder(const String& srcAnimPath, const int& numFrames)
{
	result r = E_SUCCESS;

	m_totalFrames = numFrames;
	m_currentFrame = 1;

	frameImageNames.Construct();

	for (int var = 1; var <= numFrames; ++var) {
		String nameStr;
		nameStr.Append(srcAnimPath);
		if (var<10) {
			nameStr.Append(0);
		}
		nameStr.Append((int)var);
		nameStr.Append(" copy.png");
		Texture* newImage = new Texture();
		AppLog("Creando Imagen %i/%i: newImage->loadFromFile(%S);", var, m_totalFrames, nameStr.GetPointer());
		newImage->loadFromFile(nameStr);
		newImage->scale(*m_dimension);
		frameImageNames.Add(newImage);
	}

	// Create AnimationFrames

	// opened directory will be closed automatically when the destructor of the Directory class is invoked.
	return r;
}

result SpriteAnim::loadFromFolder(const String& srcAnimPath, const String& endToken, const int& numFrames, Texture::eRenderState state)
{
	result r = E_SUCCESS;

	m_totalFrames = numFrames;
	m_currentFrame = 1;

	frameImageNames.Construct();

	for (int var = 0; var < numFrames; ++var) {
		String nameStr;
		nameStr.Append(srcAnimPath);
		/*if (var<10) {
			nameStr.Append(0);
		}*/
		nameStr.Append((int)var);
		nameStr.Append(endToken);
		Texture* newImage = new Texture();
		AppLog("Creando Imagen %i/%i: newImage->loadFromFile(%S);", var, m_totalFrames, nameStr.GetPointer());
		newImage->loadFromFile(nameStr);
		newImage->setRenderState(state);
		switch(state)
		{
		case Texture::FULLSIZE:
		{
			newImage->scale(*m_dimension);
			break;
		}
		case Texture::FIT_WIDTH:
		{
			float rate = (float)m_dimension->width / (float)newImage->getWidth();
			//float rate = 0.84f;
			m_rateDimension->width = m_dimension->width;
			m_rateDimension->height = newImage->getHeight()*rate;
			AppLog("BadaGameLib: Scale rate %f: %i / %i", rate, m_dimension->width, newImage->getWidth());
			newImage->scale(*m_rateDimension);
			break;
		}
		case Texture::FIT_HEIGHT:
		{
			float rate = m_dimension->height / newImage->getHeight();
			m_rateDimension->height = m_dimension->height;
			m_rateDimension->width = newImage->getWidth()*rate;
			AppLog("BadaGameLib: Scale rate %f", rate);
			newImage->scale(*m_rateDimension);
			//float rate = temp->getHeight()/m_dimension->height;
			//r = drawBuffer->Merge(destPoint, *(temp->getAsBitmap()), Graphics::Rectangle(0, 0, temp->getWidth()*rate, m_dimension->height));
			break;
		}
		default:
		{
			//m_rateDimension->height = newImage->getHeight();
			//m_rateDimension->width = newImage->getWidth();
			//newImage->scale(*m_rateDimension);
			//r = drawBuffer->Merge(destPoint, *(temp->getAsBitmap()), Graphics::Rectangle(0, 0, temp->getWidth(), temp->getHeight()));
			break;
		}
		}
		frameImageNames.Add(newImage);
	}

	// Create AnimationFrames

	// opened directory will be closed automatically when the destructor of the Directory class is invoked.
	return r;
}

result SpriteAnim::loadFromSpriteSheet(const String& srcAnimPath, const Graphics::Dimension& spriteOnSheetSize, const int& numFrames)
{
	result r = E_SUCCESS;

	//Create the SpriteSheet
	Media::Image* bitmapDecoder = new Media::Image();
	if(IsFailed(bitmapDecoder->Construct()))
	{
		AppLog("BadaGameLib: Failed to construct a bitmap decoder for SpriteSheet");
	}

	spriteSheet = bitmapDecoder->DecodeN(srcAnimPath, Graphics::BITMAP_PIXEL_FORMAT_RGB565);
	if (!spriteSheet)
	{
		AppLog("BadaGameLib: Failed to decode the image on Texture Class");
		return false;
	}

	//spriteSheet->Scale(Graphics::Dimension((spriteSheet->GetWidth() / spriteOnSheetSize.width)*m_dimension->width, (spriteSheet->GetHeight() / spriteOnSheetSize.height)*m_dimension->height));
	//spriteSheet->Scale(Graphics::Dimension(6 * m_dimension->width, 6 * m_dimension->height));

	int x = 0;
	int y = 0;
	//int totalRow = spriteSheet->GetWidth() / m_dimension->width;
	int totalCol = 6;
	//int totalColum = spriteSheet->GetHeight() / spriteOnSheetSize.height;

	m_totalFrames = numFrames;
	m_currentFrame = 1;

	frameSpriteNames.Construct(numFrames);

	for (int var = 0; var < numFrames; ++var) {
		//Graphics::Bitmap* tempBitmap = new Graphics::Bitmap();
		SpriteAnim::Frame* newFrame = new SpriteAnim::Frame;
		x = (var % totalCol) * spriteOnSheetSize.width;
		y = (var / totalCol) * spriteOnSheetSize.height;
		newFrame->position.x = x;
		newFrame->position.y = y;
		newFrame->size.width = spriteOnSheetSize.width;
		newFrame->size.height = spriteOnSheetSize.height;
		//tempBitmap->Construct(*spriteSheet, Graphics::Rectangle(x, y, spriteOnSheetSize.width, spriteOnSheetSize.height));
		//Texture* newImage = new Texture();
		AppLog("Creando Imagen %i/%i: Sprite(%i,%i)-(%i,%i) en %S", var, m_totalFrames, newFrame->position.x, newFrame->position.y, newFrame->size.width, newFrame->size.height, srcAnimPath.GetPointer());
		//newImage->loadFromBitmap(tempBitmap);
		//newImage->scale(*m_dimension);
		//frameImageNames.Add(newImage);
		frameSpriteNames.Add(newFrame);
	}

	// Create AnimationFrames

	// opened directory will be closed automatically when the destructor of the Directory class is invoked.
	return r;
}

int SpriteAnim::getCenterX()
{
	return (getPosition())->m_fX + getWidth()/2;
}

int SpriteAnim::getCenterY()
{
	return (getPosition())->m_fY + getHeight()/2;
}

int SpriteAnim::getWidth()
{
	Texture* temp;
	frameImageNames.GetAt(m_currentFrame-1, temp);
	return temp->getWidth();
}

int SpriteAnim::getHeight()
{
	Texture* temp;
	frameImageNames.GetAt(m_currentFrame-1, temp);
	return temp->getHeight();
}

void SpriteAnim::scale(const Graphics::Dimension& newDimension)
{
	// TODO Add a scale rate to make the scalation on drawing mode not on creation mode. This will
	// allow to change the scale on runtime mode
	//m_Texture->scale(newDimension);
}

result SpriteAnim::draw(Graphics::Canvas* currentCanvas)
{
	result r = E_SUCCESS;
	//if(m_animationPlaying)
	{
		this->nextFrame();

		Texture* temp;
		if (m_currentFrame-1<frameImageNames.GetCount())
		{
			frameImageNames.GetAt(m_currentFrame-1, temp);
			r =	currentCanvas->DrawBitmap(Point((getPosition())->m_fX, (getPosition())->m_fY), *(temp->getAsBitmap()));
			AppLog("BadaGameLib: SpriteAnim->draw(%S) frame %i/%i", (temp->getPath()).GetPointer(), m_currentFrame, m_totalFrames);
		}
	}

	return r;
}
result SpriteAnim::draw(Graphics::Bitmap* drawBuffer)
{
	result r = E_SUCCESS;
	this->nextFrame();

	if(frameImageNames.GetCount()>0)
	{
	Texture* temp;
	if (m_currentFrame-1<frameImageNames.GetCount())
	{
		frameImageNames.GetAt(m_currentFrame-1, temp);
		switch(temp->getRenderState())
		{
		case Texture::FULLSIZE:
		{
			r = drawBuffer->Merge(Point((getPosition())->m_fX, (getPosition())->m_fY), *(temp->getAsBitmap()), Graphics::Rectangle(0, 0, m_dimension->width, m_dimension->height));
			break;
		}
		case Texture::FIT_WIDTH:
		{
			float rate = temp->getWidth()/m_dimension->width;
			r = drawBuffer->Merge(Point((getPosition())->m_fX, (getPosition())->m_fY), *(temp->getAsBitmap()), Graphics::Rectangle(0, 0, m_dimension->width, temp->getHeight()*rate));
			break;
		}
		case Texture::FIT_HEIGHT:
		{
			float rate = temp->getHeight()/m_dimension->height;
			r = drawBuffer->Merge(Point((getPosition())->m_fX, (getPosition())->m_fY), *(temp->getAsBitmap()), Graphics::Rectangle(0, 0, temp->getWidth()*rate, m_dimension->height));
			break;
		}
		default:
		{
			r = drawBuffer->Merge(Point((getPosition())->m_fX, (getPosition())->m_fY), *(temp->getAsBitmap()), Graphics::Rectangle(0, 0, temp->getWidth(), temp->getHeight()));
			//break;
		}
		}
		AppLog("BadaGameLib: SpriteAnim->draw(%S) frame %i/%i", (temp->getPath()).GetPointer(), m_currentFrame, m_totalFrames);
	}
	else
	{
		AppLog("BadaGameLib: SpriteAnim->draw() frame %i/%i NOT DRAWN!!!", m_currentFrame, m_totalFrames);
	}
	}


	if(frameSpriteNames.GetCount()>0)
	{
	SpriteAnim::Frame* temp;
	if (m_currentFrame-1<frameSpriteNames.GetCount())
	{
		frameSpriteNames.GetAt(m_currentFrame-1, temp);
		//drawBuffer->Merge(destPoint, *spriteSheet, Graphics::Rectangle(temp->position.x, temp->position.y, temp->size.width, temp->size.height));
		//Graphics::Rectangle(temp->position.x, temp->position.y, temp->size.width, temp->size.height)
		Graphics::Bitmap* l_pBitmap = new Graphics::Bitmap();
		l_pBitmap->Construct(*spriteSheet, Graphics::Rectangle(temp->position.x, temp->position.y, temp->size.width, temp->size.height));
		l_pBitmap->Scale(*m_dimension);
		r = drawBuffer->Merge(Point((getPosition())->m_fX, (getPosition())->m_fY), *l_pBitmap, Graphics::Rectangle(0, 0, m_dimension->width, m_dimension->height));
		AppLog("BadaGameLib: SpriteAnim->draw() frame %i/%i (SPRITESHEET)", m_currentFrame, m_totalFrames);
		delete l_pBitmap;
	}
	else
	{
		AppLog("BadaGameLib: SpriteAnim->draw() frame %i/%i NOT DRAWN!!! (SPRITESHEET)", m_currentFrame, m_totalFrames);
	}
	}

	return r;
}

void SpriteAnim::nextFrame()
{
	if (!m_animationReverse)
	{
		++m_currentFrame;
		if(isAnimationLooping())
		{
			if(m_currentFrame > m_loopEndFrame)
			{
				m_currentFrame = m_loopInitFrame;
				//m_animationPlaying = false;
			}
		}
		else
		{
			if(m_currentFrame > m_totalFrames)
			{
				m_currentFrame = 1;
				//m_animationPlaying = false;
			}
		}
	}
	else
	{
		--m_currentFrame;
		if(m_currentFrame<=0)
		{
			m_animationLoop = false;
			m_currentFrame = 1;
			stop();
		}
	}
}

void SpriteAnim::setAnimationReverse(bool reverse)
{
	m_animationReverse = reverse;
}

void SpriteAnim::play()
{
	m_animationPlaying = true;
	if (isAnimationOver())
	{
		if(isAnimationLooping())
		{
			m_currentFrame = m_loopInitFrame;
		}
		else
		{
			m_currentFrame = 1;
		}
	}
}

void SpriteAnim::stop()
{
	m_animationPlaying = false;
}

void SpriteAnim::togleLoop()
{
	m_animationLoop = !m_animationLoop;
}

bool SpriteAnim::isAnimationPlaying()
{
	return m_animationPlaying;
}

bool SpriteAnim::isAnimationOver()
{
	if((m_currentFrame == m_totalFrames) || (m_animationReverse && (m_currentFrame == 1)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SpriteAnim::setLoopFrames(int initLoop, int endLoop)
{
	m_loopInitFrame = initLoop;
	m_loopEndFrame = endLoop;
}

bool SpriteAnim::isAnimationLooping()
{
	return m_animationLoop;
}

}
