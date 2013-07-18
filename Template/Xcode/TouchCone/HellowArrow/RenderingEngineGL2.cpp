//
//  RenderingEngineGL1.cpp
//  HellowArrow
//
//  Created by Daniel on 07/04/2013.
//  Copyright (c) 2013 Daniel Conde Linares. All rights reserved.
//

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "IRenderingEngine.hpp"
#include "Quaternion.hpp"
#include <vector>
#include <iostream>

#define STRINGIFY(A) #A
#include "Simple.vert"
#include "Simple.frag"

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

class RenderingEngineGL2 : public IRenderingEngine
{
public:
    RenderingEngineGL2();
    void Initialize(int width, int height);
    void Render(void) const;
    void UpdateAnimation(float timeStep);
    void OnRotate(DeviceOrientation orientation);
    void OnFingerUp(ivec2 location);
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
private:
    GLuint BuildShader(const char* source, GLenum shaderType) const;
    GLuint BuildProgram(const char* vShader, const char* fShader) const;
private:
    vector<Vertex> m_Cone;
    vector<Vertex> m_Disk;
    
    Animation m_Animation;
    
    GLuint m_iFrameBuffer;
    GLuint m_iColorRenderBuffer;
    GLuint m_iDepthRenderBuffer;
    
    GLuint m_iSimpleProgram;
    
    float m_fRotationAngle;
    float m_fScale;
    
    ivec2 m_PivotPoint;
};

IRenderingEngine* CreateRenderer2()
{
    return new RenderingEngineGL2();
}

RenderingEngineGL2::RenderingEngineGL2() : m_fRotationAngle(0), m_fScale(1)
{
    // Create & Bind the color buffer so that the caller can allocate its space
    glGenRenderbuffers(1, &m_iColorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_iColorRenderBuffer);
}

void RenderingEngineGL2::Initialize(int width, int height)
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
    glGenRenderbuffers(1, &m_iDepthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_iDepthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER,
                             GL_DEPTH_COMPONENT16,
                             width,
                             height);
    
    // Create the frame buffer object. Attach the depth and color buffers
    glGenFramebuffers(1, &m_iFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_iFrameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                 GL_COLOR_ATTACHMENT0,
                                 GL_RENDERBUFFER,
                                 m_iColorRenderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                 GL_DEPTH_ATTACHMENT,
                                 GL_RENDERBUFFER,
                                 m_iDepthRenderBuffer);
    
    // Bind the color buffer for rendering
    glBindRenderbuffer(GL_RENDERBUFFER, m_iColorRenderBuffer);
    
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    
    m_iSimpleProgram = BuildProgram(SimpleVertexShader, SimpleFragmentShader);
    
    glUseProgram(m_iSimpleProgram);
    
    GLint projectionUniform = glGetUniformLocation(m_iSimpleProgram, "Projection");
    
    mat4 projectionMatrix = mat4::Frustum(-1.6f, 1.6f, -2.4f, 2.4f, 5, 10);
    glUniformMatrix4fv(projectionUniform, 1, 0, projectionMatrix.Pointer());
}

GLuint RenderingEngineGL2::BuildProgram(const char *vShader, const char *fShader) const
{
    GLuint vertexShader = BuildShader(vShader, GL_VERTEX_SHADER);
    GLuint fragmentShader = BuildShader(fShader, GL_FRAGMENT_SHADER);
    
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        std::cout << messages;
        exit(1);
    }
    
    return programHandle;
}

GLuint RenderingEngineGL2::BuildShader(const char *source, GLenum shaderType) const
{
    GLuint shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &source, 0);
    glCompileShader(shaderHandle);
    
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        std::cout << messages;
        exit(1);
    }
    
    return shaderHandle;
}

void RenderingEngineGL2::Render() const
{
    GLuint positionSlot = glGetAttribLocation(m_iSimpleProgram, "Position");
    GLuint colorSlot = glGetAttribLocation(m_iSimpleProgram, "SourceColor");
    
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnableVertexAttribArray(positionSlot);
    glEnableVertexAttribArray(colorSlot);
    
    mat4 rotation = mat4::Rotate(m_fRotationAngle);
    mat4 scale = mat4::Scale(m_fScale);
    mat4 translation = mat4::Translate(0, 0, -7);
    
    // Set the model view matrix
    GLint modelviewUniform = glGetUniformLocation(m_iSimpleProgram, "ModelView");
    
    mat4 modelviewMatrix = scale * rotation * translation;
    glUniformMatrix4fv(modelviewUniform, 1, 0, modelviewMatrix.Pointer());
    
    // Draw the cone
    {
        GLsizei stride = sizeof(Vertex);
        const GLvoid* pCoords = &m_Cone[0].Position.x;
        const GLvoid* pColors = &m_Cone[0].Color.x;
        
        glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, stride, pCoords);
        glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, stride, pColors);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, m_Cone.size());
    }
    
    // Draw the disk that caps off the base of the cone
    {
        GLsizei stride = sizeof(Vertex);
        const GLvoid* pCoords = &m_Disk[0].Position.x;
        const GLvoid* pColors = &m_Disk[0].Color.x;
        
        glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, stride, pCoords);
        glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, stride, pColors);
        glDrawArrays(GL_TRIANGLE_FAN, 0, m_Disk.size());
    }
    
    glDisableVertexAttribArray(positionSlot);
    glDisableVertexAttribArray(colorSlot);
}

void RenderingEngineGL2::OnFingerUp(ivec2 location)
{
    m_fScale = 1.0f;
}

void RenderingEngineGL2::OnFingerDown(ivec2 location)
{
    m_fScale = 1.5f;
    OnFingerMove(location, location);
}

void RenderingEngineGL2::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    vec2 direction = vec2(newLocation - m_PivotPoint).Normalized();
    
    // Flip the y-axis because the pixel coords increase towards the bottom
    direction.y = -direction.y;
    
    m_fRotationAngle = std::acos(direction.y)*180.0f/3.14159f;
    if(direction.x>0)
        m_fRotationAngle = -m_fRotationAngle;
}

void RenderingEngineGL2::OnRotate(DeviceOrientation orientation)
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

void RenderingEngineGL2::UpdateAnimation(float timeStep)
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