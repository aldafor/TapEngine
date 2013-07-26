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
#include "Quaternion.hpp"
#include <vector>

static const float fAnimationDuration = 0.25f;

using namespace std;

struct Vertex {
    vec3 Position;
    vec4 Color;
};

class RenderingEngineGL1 : public IRenderingEngine
{
public:
    RenderingEngineGL1();
    void Initialize(int width, int height);
    void Render(void) const;
    void UpdateAnimation(float timeStep) {}
    void OnRotate(DeviceOrientation orientation) {}
    void OnFingerUp(ivec2 location);
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
private:
    float RotationDirection() const;
private:
    vector<Vertex> m_ConeVertices;
    vector<GLubyte> m_ConeIndices;
    GLuint m_iBodyIndexCount;
    GLuint m_iDiskIndexCount;
    
    GLfloat m_fRotationAngle;
    GLfloat m_fScale;
    
    ivec2 m_PivotPoint;
    
    GLuint m_iFrameBuffer;
    GLuint m_iColorRenderBuffer;
    GLuint m_iDepthRenderBuffer;
    
    float m_fDesiredAngle;
    float m_fCurrentAngle;
};

IRenderingEngine* CreateRenderer1()
{
    return new RenderingEngineGL1();
}

RenderingEngineGL1::RenderingEngineGL1() : m_fRotationAngle(0), m_fScale(1)
{
    // Create & Bind the color buffer so that the caller can allocate its space
    glGenRenderbuffersOES(1, &m_iColorRenderBuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_iColorRenderBuffer);
}

void RenderingEngineGL1::Initialize(int width, int height)
{
    m_PivotPoint = ivec2(width / 2, height / 2);
    
    const float fConeRadius = 0.5f;
    const float fConeHeight = 1.866f;
    const int iConeSlices = 40;
    const float dtheta = TwoPi / iConeSlices;
    const int vertexCount = iConeSlices * 2 + 1;
    
    m_ConeVertices.resize(vertexCount);
    vector<Vertex>::iterator vertex = m_ConeVertices.begin();
    
    // Cone's body
    for (float theta = 0; vertex != m_ConeVertices.end() - 1; theta += dtheta)
    {
        //Grayscale gradient
        float brightness = abs(sin(theta));
        vec4 color(brightness, brightness, brightness, 1);
        
        //Apex vertex
        vertex->Position = vec3(0,1,0);
        vertex->Color = color;
        vertex++;
        
        //Rim vertex
        vertex->Position.x = fConeRadius * cos(theta);
        vertex->Position.y = 1 - fConeHeight;
        vertex->Position.z = fConeRadius * sin(theta);
        vertex->Color = color;
        vertex++;
    }
    
    // Disk center
    vertex->Position = vec3(0, 1 - fConeHeight, 0);
    vertex->Color = vec4(1, 1, 1, 1);
    
    // Index generation
    m_iBodyIndexCount = iConeSlices * 3;
    m_iDiskIndexCount = iConeSlices * 3;
    
    m_ConeIndices.resize(m_iBodyIndexCount + m_iDiskIndexCount);
    vector<GLubyte>::iterator index = m_ConeIndices.begin();
    
    // Body triangles
    for (int i = 0; i < iConeSlices; i += 2)
    {
        *index++ = i;
        *index++ = (i + 1) % (2 * iConeSlices);
        *index++ = (i + 3) % (2 * iConeSlices);
    }
    
    // Disk triangles
    const int iDiskCenterIndex = vertexCount - 1;
    for (int i = 1; i < iConeSlices * 2 + 1; i += 2)
    {
        *index++ = iDiskCenterIndex;
        *index++ = i;
        *index++ = (i + 2) % (2 * iConeSlices);
    }
    
    // Create the depth buffer
    glGenRenderbuffersOES(1, &m_iDepthRenderBuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_iDepthRenderBuffer);
    glRenderbufferStorageOES(GL_RENDERBUFFER_OES,
                             GL_DEPTH_COMPONENT16_OES,
                             width,
                             height);
    
    // Create the frame buffer object. Attach the depth and color buffers
    glGenFramebuffersOES(1, &m_iFrameBuffer);
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_iFrameBuffer);
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES,
                                 GL_COLOR_ATTACHMENT0_OES,
                                 GL_RENDERBUFFER_OES,
                                 m_iColorRenderBuffer);
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES,
                                 GL_DEPTH_ATTACHMENT_OES,
                                 GL_RENDERBUFFER_OES,
                                 m_iDepthRenderBuffer);
    
    // Bind the color buffer for rendering
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_iColorRenderBuffer);
    
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_PROJECTION);
    glFrustumf(-1.6f, 1.6f, -2.4f, 2.4f, 5, 10);
    
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0, 0, -7);
    
//    OnRotate(Portrait);
//    m_fCurrentAngle = m_fDesiredAngle;
}

void RenderingEngineGL1::Render() const
{
    GLsizei stride = sizeof(Vertex);
    const GLvoid* pCoords = &m_ConeVertices[0].Position.x;
    const GLvoid* pColors = &m_ConeVertices[0].Color.x;
    
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    
    glRotatef(m_fRotationAngle, 0, 0, 1);
    glScalef(m_fScale, m_fScale, m_fScale);
    
    glVertexPointer(3, GL_FLOAT, stride, pCoords);
    glColorPointer(4, GL_FLOAT, stride, pColors);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    const GLvoid* pBodyIndices = &m_ConeIndices[0];
    const GLvoid* pDiskIndices = &m_ConeIndices[m_iBodyIndexCount];
    
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawElements(GL_TRIANGLES, m_iBodyIndexCount, GL_UNSIGNED_BYTE, pBodyIndices);
    glDisableClientState(GL_COLOR_ARRAY);
    glColor4f(1, 1, 1, 1);
    glDrawElements(GL_TRIANGLES, m_iDiskIndexCount, GL_UNSIGNED_BYTE, pDiskIndices);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void RenderingEngineGL1::OnFingerUp(ivec2 location)
{
    m_fScale = 1.0f;
}

void RenderingEngineGL1::OnFingerDown(ivec2 location)
{
    m_fScale = 1.5f;
    OnFingerMove(location, location);
}

void RenderingEngineGL1::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    vec2 direction = vec2(newLocation - m_PivotPoint).Normalized();
    
    // Flip the y-axis because the pixel coords increase towards the bottom
    direction.y = -direction.y;
    
    m_fRotationAngle = std::acos(direction.y)*180.0f/3.14159f;
    if(direction.x>0)
        m_fRotationAngle = -m_fRotationAngle;
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