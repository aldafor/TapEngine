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
    vector<Vertex> m_Cone;
    vector<Vertex> m_Disk;
    
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
    m_PivotPoint = ivec2(width/2, height/2);
    
    const float fConeRadius = 0.5f;
    const float fConeHeight = 1.866f;
    const int iConeSlices = 40;
    
    {
        // Generate vertices for the disk
        m_Disk.resize(iConeSlices + 2);
        
        // Initialize de center vertex of the triangle fan
        vector<Vertex>::iterator currentVertex = m_Disk.begin();
        currentVertex->Color = vec4(0.75f, 0.75f, 0.75f, 1);
        currentVertex->Position.x = 0;
        currentVertex->Position.y = 1 - fConeHeight;
        currentVertex->Position.z = 0;
        currentVertex++;
        
        //Initialize de rim vertices of the triangle fan
        const float dtheta = TwoPi / iConeSlices;
        for (float theta = 0; currentVertex != m_Disk.end(); theta += dtheta)
        {
            currentVertex->Color = vec4(0.75f, 0.75f, 0.75f, 1);
            currentVertex->Position.x = fConeRadius * cos(theta);
            currentVertex->Position.y = 1 - fConeHeight;
            currentVertex->Position.z = fConeRadius * sin(theta);
            currentVertex++;
        }
    }
    
    {
        // Generate vertices for the body of the cone
        m_Cone.resize((iConeSlices + 1) * 2);
        
        // Initialice vertices of the triangle strip
        vector<Vertex>::iterator currentVertex = m_Cone.begin();
        const float dtheta = TwoPi / iConeSlices;
        for (float theta = 0; currentVertex != m_Cone.end(); theta += dtheta)
        {
            //Grayscale gradient
            float brightness = abs(sin(theta));
            vec4 color(brightness, brightness, brightness, 1);
            
            //Apex vertex
            currentVertex->Position = vec3(0,1,0);
            currentVertex->Color = color;
            currentVertex++;
            
            //Rim vertex
            currentVertex->Position.x = fConeRadius * cos(theta);
            currentVertex->Position.y = 1 - fConeHeight;
            currentVertex->Position.z = fConeRadius * sin(theta);
            currentVertex->Color = color;
            currentVertex++;
        }
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
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glRotatef(m_fRotationAngle, 0, 0, 1);
    glScalef(m_fScale, m_fScale, m_fScale);
    
    // Draw the cone
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &m_Cone[0].Position.x);
    glColorPointer(4, GL_FLOAT, sizeof(Vertex), &m_Cone[0].Color.x);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_Cone.size());
    
    // Draw the disk that caps off the base of the cone
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &m_Disk[0].Position.x);
    glColorPointer(4, GL_FLOAT, sizeof(Vertex), &m_Disk[0].Color.x);
    glDrawArrays(GL_TRIANGLE_FAN, 0, m_Disk.size());
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
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