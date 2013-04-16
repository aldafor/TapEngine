//
//  RenderingEngineGL1.cpp
//  HellowArrow
//
//  Created by Daniel on 07/04/2013.
//  Copyright (c) 2013 Daniel Conde Linares. All rights reserved.
//

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <cmath>
#include <iostream>
#include "IRenderingEngine.hpp"

#define STRINGIFY(A) #A
#include "Simple.vert"
#include "Simple.frag"

static const float fRevolutionsPerSecond = 1.0f;

class RenderingEngineGL2 : public IRenderingEngine
{
public:
    RenderingEngineGL2();
    void Initialize(int width, int height);
    void Render(void) const;
    void UpdateAnimation(float timeStep);
    void OnRotate(DeviceOrientation orientation);
private:
    float RotationDirection() const;
    GLuint BuildShader(const char* source, GLenum shaderType) const;
    GLuint BuildProgram(const char* vShader, const char* fShader) const;
    void ApplyOrtho(float maxX, float maxY) const;
    void ApplyRotation(float degrees) const;
private:
    GLuint m_iFrameBuffer;
    GLuint m_iRenderBuffer;
    GLuint m_iSimpleProgram;
    
    float m_fDesiredAngle;
    float m_fCurrentAngle;
};

IRenderingEngine* CreateRenderer2()
{
    return new RenderingEngineGL2();
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

RenderingEngineGL2::RenderingEngineGL2()
{
    glGenRenderbuffers(1, &m_iRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_iRenderBuffer);
}

void RenderingEngineGL2::Initialize(int width, int height)
{
    // Create a Framebuffer object and attach the color buffer
    glGenFramebuffers(1, &m_iFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_iFrameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                 GL_COLOR_ATTACHMENT0,
                                 GL_RENDERBUFFER,
                                 m_iRenderBuffer);
    
    glViewport(0, 0, width, height);
    
    m_iSimpleProgram = BuildProgram(SimpleVertexShader, SimpleFragmentShader);
    
    glUseProgram(m_iSimpleProgram);
    
    // Initialice de projection matrix
    ApplyOrtho(2, 3);
    
    // Initialice rotation animation state
    OnRotate(Portrait);
    m_fCurrentAngle = m_fDesiredAngle;
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

void RenderingEngineGL2::ApplyOrtho(float maxX, float maxY) const
{
    float a = 1.0f / maxX;
    float b = 1.0f / maxY;
    float ortho[16] = {
        a, 0, 0, 0,
        0, b, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1
    };
    
    GLint projectionUniform = glGetUniformLocation(m_iSimpleProgram, "Projection");
    glUniformMatrix4fv(projectionUniform, 1, 0, &ortho[0]);
}

void RenderingEngineGL2::ApplyRotation(float degrees) const
{
    float radians = degrees * 3.14159f / 180.0f;
    float fSin = std::sin(radians);
    float fCos = std::cos(radians);
    float zRotation[16] = {
        fCos, fSin, 0, 0,
        -fSin, fCos, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    
    GLint modelViewUniform = glGetUniformLocation(m_iSimpleProgram, "ModelView");
    glUniformMatrix4fv(modelViewUniform, 1, 0, &zRotation[0]);
}

void RenderingEngineGL2::Render() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    ApplyRotation(m_fCurrentAngle);
    
    GLuint positionSlot = glGetAttribLocation(m_iSimpleProgram, "Position");
    GLuint colorSlot = glGetAttribLocation(m_iSimpleProgram, "SourceColor");
    
    glEnableVertexAttribArray(positionSlot);
    glEnableVertexAttribArray(colorSlot);
    
    GLsizei stride = sizeof(Vertex);
    const GLvoid* pCoords = &Vertices[0].Position[0];
    const GLvoid* pColors = &Vertices[0].Color[0];
    
    glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, stride, pCoords);
    glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, stride, pColors);
    
    GLsizei vertexCount = sizeof(Vertices) / sizeof(Vertex);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    
    glDisableVertexAttribArray(positionSlot);
    glDisableVertexAttribArray(colorSlot);
}

void RenderingEngineGL2::OnRotate(DeviceOrientation orientation)
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

float RenderingEngineGL2::RotationDirection() const
{
    float delta = m_fDesiredAngle - m_fCurrentAngle;
    if (delta == 0) {
        return 0;
    }
    
    bool counterClockwise = ((delta > 0 && delta <= 180) || (delta < -180));
    return counterClockwise ? +1 : -1;
}

void RenderingEngineGL2::UpdateAnimation(float timeStep)
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