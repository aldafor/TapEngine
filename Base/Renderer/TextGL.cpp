/*
 * TextGL.cpp
 *
 *  Created on: 15/11/2010
 *      Author: Daniel Conde Linares
 */

#include "TextGL.h"

using namespace Osp::Graphics::Opengl;
using namespace Osp::Base;
using namespace Osp::Io;
using namespace BadaGameLib;

TextGL::TextGL(const String& texturePath, Quaternion* position, float width, float height) :
		SpriteGL(texturePath, position, width, height) {
	m_pText = NULL;
	m_pFontMap = new Collection::ArrayListT<TextNode*>;
	m_pFontMap->Construct();

	//TextureGL* l_pTextureInit = new TextureGL(false);
	//l_pTextureInit->loadFromFile(texturePath);
	//this->setTexture(l_pTextureInit);

	String file(texturePath);
	//file.Remove(file.GetLength()-3, 3);
	//file.Append(".js");
	file.Replace(L".png", L".js");
	AppLog("Initializing %S Font loading...", file.GetPointer());
	init(file);
	m_DisplayListID = 0;
}

TextGL::~TextGL() {
	if (m_pText != NULL)
		delete m_pText;
}

void TextGL::init(const String& filePath) {
	File file;
	String buffer;
	String delim(L"[',]");

	result r = E_SUCCESS;
	r = file.Construct(filePath, L"r");
	while(!IsFailed(r))
	{
		//  [' ', 0, 0, 4, 16, 0],
		//  ['!', 4, 0, 3, 16, 0],
		//  [],
		String temp;
		r = file.Read(buffer);
		buffer.Trim();
		Utility::StringTokenizer strTok(buffer, delim);
		if (strTok.GetTokenCount()>=5){
			TextNode* node = new TextNode;
			strTok.GetNextToken(temp);
//			if(temp[0]=="'")
//				temp.Replace("'", "");
//			else
//				temp.Replace("\"", "");
			node->character = *(temp.GetPointer());
			//AppLog("Token character: %c", node->character);
			strTok.GetNextToken(temp);
			temp.Trim();
			Integer::Parse(temp, node->x);
			//AppLog("Token x: %i", node->x);
			strTok.GetNextToken(temp);
			temp.Trim();
			Integer::Parse(temp, node->y);
			//AppLog("Token y: %i", node->y);
			strTok.GetNextToken(temp);
			temp.Trim();
			Integer::Parse(temp, node->width);
			//AppLog("Token width: %i", node->width);
			strTok.GetNextToken(temp);
			temp.Trim();
			Integer::Parse(temp, node->height);
			//AppLog("Token height: %i", node->height);
			m_pFontMap->Add(node);
		}
		//AppLog("------------------------------");
	}

}

void TextGL::initBFG(const String& filePath) {
	File file;
	String buffer;
	String delim(L"/t");

	result r = E_SUCCESS;
	r = file.Construct(filePath, L"r");
	while(!IsFailed(r))
	{
		//  [' ', 0, 0, 4, 16, 0],
		//  ['!', 4, 0, 3, 16, 0],
		//  [],
		String temp;
		r = file.Read(buffer);
		buffer.Trim();
		Utility::StringTokenizer strTok(buffer, delim);
		if (strTok.GetTokenCount()>=5){
			TextNode* node = new TextNode;
			strTok.GetNextToken(temp);
//			if(temp[0]=="'")
//				temp.Replace("'", "");
//			else
//				temp.Replace("\"", "");
			node->character = *(temp.GetPointer());
			//AppLog("Token character: %c", node->character);
			strTok.GetNextToken(temp);
			temp.Trim();
			Integer::Parse(temp, node->x);
			//AppLog("Token x: %i", node->x);
			strTok.GetNextToken(temp);
			temp.Trim();
			Integer::Parse(temp, node->y);
			//AppLog("Token y: %i", node->y);
			strTok.GetNextToken(temp);
			temp.Trim();
			Integer::Parse(temp, node->width);
			//AppLog("Token width: %i", node->width);
			strTok.GetNextToken(temp);
			temp.Trim();
			Integer::Parse(temp, node->height);
			//AppLog("Token height: %i", node->height);
			m_pFontMap->Add(node);
		}
		//AppLog("------------------------------");
	}

}

void TextGL::setText(const String& text) {
	if (m_pText != NULL)
			delete m_pText;
	m_pText = new TextNode[text.GetLength()];
	m_iTextSize = text.GetLength();
	for(int i = 0; i<text.GetLength(); ++i)
	{
		for(int j = 0; j<m_pFontMap->GetCount(); ++j)
		{
			TextNode* node;
			m_pFontMap->GetAt(j, node);
			if(node->character == (char) text[i])
			{
				AppLog("Caracter %c identificado - x:%i y:%i width:%i height:%i", node->character, node->x, node->y, node->width, node->height);
				m_pText[i].character = node->character;
				m_pText[i].x = node->x;
				m_pText[i].y = node->y;
				m_pText[i].width = node->width;
				m_pText[i].height = node->height;
				break;
			}
		}
	}
}

void TextGL::draw(long long gameTime) {
	glPushMatrix();
	//glTranslatef(m_fTranslateX, m_fTranslateY, m_fTranslateZ);
	// TODO Test efficiency between set SpriteCoords based on Position, or make translation
	float l_fCharWidthOffset = 0.0f;
	for ( int i = 0; i<m_iTextSize; ++i)
	{
		GLfloat l_spriteCoords[] = {
			m_pPosition->m_fX + l_fCharWidthOffset, m_pPosition->m_fY, m_pPosition->m_fZ,
			m_pPosition->m_fX + l_fCharWidthOffset + (float) m_pText[i].width / 256.0f * 0.800f, m_pPosition->m_fY, m_pPosition->m_fZ,
			m_pPosition->m_fX + l_fCharWidthOffset, m_pPosition->m_fY + (float) m_pText[i].height / 128.0f * 0.480f, m_pPosition->m_fZ,
			m_pPosition->m_fX + l_fCharWidthOffset + (float) m_pText[i].width / 256.0f * 0.800f, m_pPosition->m_fY + (float) m_pText[i].height / 128.0f * 0.480f, m_pPosition->m_fZ
		};

		//Aquí viene lo bueno
		GLfloat l_textureCoord[] = {
			(float) m_pText[i].x / 256.0f, (float) m_pText[i].y / 128.0f + (float) m_pText[i].height / 128.0f,
			(float) m_pText[i].x / 256.0f + (float) m_pText[i].width / 256.0f, (float) m_pText[i].y / 128.0f + (float) m_pText[i].height / 128.0f,
			(float) m_pText[i].x / 256.0f, (float) m_pText[i].y / 128.0f,
			(float) m_pText[i].x / 256.0f + (float) m_pText[i].width / 256.0f, (float) m_pText[i].y / 128.0f
		};

		if(m_fSpinAngle != 0)
			glRotatef(m_fSpinAngle, 0.0f, 0.0f, 1.0f);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, *(m_pTexture->getTextureID()));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, l_spriteCoords);
		glTexCoordPointer(2, GL_FLOAT, 0, l_textureCoord);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		l_fCharWidthOffset += (float) m_pText[i].width / 256.0f * 0.800f;
	}
	glPopMatrix();
}
