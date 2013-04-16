//
//  RenderingEngineGL1.cpp
//  HellowArrow
//
//  Created by Daniel on 07/04/2013.
//  Copyright (c) 2013 Daniel Conde Linares. All rights reserved.
//

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include "IRenderingEngine.hpp"

static const float fRevolutionsPerSecond = 1.0f;

class RenderingEngineGL1 : public IRenderingEngine
{
public:
    RenderingEngineGL1();
    void Initialize(int width, int height);
    void Render(void) const;
    void UpdateAnimation(float timeStep);
    void OnRotate(DeviceOrientation orientation);
private:
    float RotationDirection() const;
private:
    GLuint m_iFrameBuffer;
    GLuint m_iRenderBuffer;
    
    float m_fDesiredAngle;
    float m_fCurrentAngle;
};

IRenderingEngine* CreateRenderer1()
{
    return new RenderingEngineGL1();
}

struct Vertex {
    float Position[2];
    float Color[4];
};

const Vertex Vertices[] = {
    {{-0.5, -0.866}, {1, 1, 0.5f, 1}},
    {{0.5, -0.866},  {1, 1, 0.5f, 1}},
    {{0, 1},         {1, 1, 0.5f, 1}},
    {{-0.5, -0.866}, {0.5f, 0.5f, 0.5f, 1}},
    {{0.5, -0.866},  {0.5f, 0.5f, 0.5f, 1}},
    {{0, -0.4f},     {0.5f, 0.5f, 0.5f, 1}},
};

RenderingEngineGL1::RenderingEngineGL1()
{
    glGenRenderbuffersOES(1, &m_iRenderBuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_iRenderBuffer);
}

void RenderingEngineGL1::Initialize(int width, int height)
{
    glGenFramebuffersOES(1, &m_iFrameBuffer);
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_iFrameBuffer);
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES,
                                 GL_COLOR_ATTACHMENT0_OES,
                                 GL_RENDERBUFFER_OES,
                                 m_iRenderBuffer);
    
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    
    const float maxX = 2.0f;
    const float maxY = 3.0f;
    
    glOrthof(-maxX, +maxX, -maxY, +maxY, -1, +1);
    
    glMatrixMode(GL_MODELVIEW);
    
    OnRotate(Portrait);
    m_fCurrentAngle = m_fDesiredAngle;
}

void RenderingEngineGL1::Render() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glPushMatrix();
    glRotatef(m_fCurrentAngle, 0, 0, 1);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &Vertices[0].Position[0]);
    glColorPointer(4, GL_FLOAT, sizeof(Vertex), &Vertices[0].Color[0]);
    
    GLsizei vertexCount = sizeof(Vertices) / sizeof(Vertex);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    glPopMatrix();
}

void RenderingEngineGL1::OnRotate(DeviceOrientation orientation)
{
    switch (orientation) {
        case LandscapeLeft:
            m_fDesiredAngle = 270;
            break;
        case PortraitUpsideDown:
            m_fDesiredAngle = 180;
            break;
        case LandscapeRight:
            m_fDesiredAngle = 90;
            break;
        default:
            m_fDesiredAngle = 0;
            break;
    }
}

float RenderingEngineGL1::RotationDirection() const
{
    float delta = m_fDesiredAngle - m_fCurrentAngle;
    if (delta == 0) {
        return 0;
    }
    
    bool counterClockwise = ((delta > 0 && delta <= 180) || (delta < -180));
    return counterClockwise ? +1 : -1;
}

void RenderingEngineGL1::UpdateAnimation(float timeStep)
{
    float direction = RotationDirection();
    if(direction == 0)
        return;
    
    float degrees = timeStep * 360 * fRevolutionsPerSecond;
    m_fCurrentAngle += degrees * direction;
    
    if(m_fCurrentAngle >= 360)
        m_fCurrentAngle -= 360;
    else if(m_fCurrentAngle < 0)
        m_fCurrentAngle += 360;
    
    if(RotationDirection() != direction)
        m_fCurrentAngle = m_fDesiredAngle;
}