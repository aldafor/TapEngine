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

struct Animation {
    Quaternion Start;
    Quaternion End;
    Quaternion Current;
    float Elapsed;
    float Duration;
};

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
    vector<Vertex> m_Cone;
    vector<Vertex> m_Disk;
    
    Animation m_Animation;
    
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

RenderingEngineGL1::RenderingEngineGL1()
{
    // Create & Bind the color buffer so that the caller can allocate its space
    glGenRenderbuffersOES(1, &m_iColorRenderBuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_iColorRenderBuffer);
}

void RenderingEngineGL1::Initialize(int width, int height)
{
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
    
    mat4 rotation(m_Animation.Current.ToMatrix());
    glMultMatrixf(rotation.Pointer());
    
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

void RenderingEngineGL1::OnRotate(DeviceOrientation orientation)
{
    vec3 direction;
    
    switch (orientation) {
        case Unknown:
        case Portrait:
            direction = vec3(0,1,0);
            break;
        case PortraitUpsideDown:
            direction = vec3(0,-1,0);
            break;
        case FaceDown:
            direction = vec3(0,0,-1);
            break;
        case FaceUp:
            direction = vec3(0,0,1);
            break;
        case LandscapeLeft:
            direction = vec3(1,0,0);
            break;
        case LandscapeRight:
            direction = vec3(-1,0,0);
            break;
    }
    
    m_Animation.Elapsed = 0;
    m_Animation.Current = m_Animation.End;
    m_Animation.Start = m_Animation.Current;
    m_Animation.End = Quaternion::CreateFromVectors(vec3(0,1,0), direction);
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
    if(m_Animation.Current == m_Animation.End)
        return;
    
    m_Animation.Elapsed += timeStep;
    
    if (m_Animation.Elapsed >= m_Animation.Duration) {
        m_Animation.Current = m_Animation.End;
    } else {
        float mu = m_Animation.Elapsed / m_Animation.Duration;
        m_Animation.Current = m_Animation.Start.Slerp(mu, m_Animation.End);
    }
}