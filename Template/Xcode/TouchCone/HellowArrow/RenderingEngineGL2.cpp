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
    GLuint m_iVertexBufferID;
    GLuint m_iIndexBufferID;
    GLuint m_iBodyIndexCount;
    GLuint m_iDiskIndexCount;
    
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
    m_PivotPoint = ivec2(width / 2, height / 2);
    
    const float fConeRadius = 0.5f;
    const float fConeHeight = 1.866f;
    const int iConeSlices = 40;
    const float dtheta = TwoPi / iConeSlices;
    const int vertexCount = iConeSlices * 2 + 1;
    
    vector<Vertex> coneVertices(vertexCount);
    vector<Vertex>::iterator vertex = coneVertices.begin();
    
    // Cone's body
    for (float theta = 0; vertex != coneVertices.end() - 1; theta += dtheta)
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
    
    vector<GLubyte> coneIndices(m_iBodyIndexCount + m_iDiskIndexCount);
    vector<GLubyte>::iterator index = coneIndices.begin();
    
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
    
    // Create the VBO for the vertices
    glGenBuffers(1, &m_iVertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_iVertexBufferID);
    glBufferData(GL_ARRAY_BUFFER,
                 coneVertices.size() * sizeof(coneVertices[0]),
                 &coneVertices[0],
                 GL_STATIC_DRAW);
    
    // Create VBO for the indices
    glGenBuffers(1, &m_iIndexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iIndexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 coneIndices.size() * sizeof(coneIndices[0]),
                 &coneIndices[0],
                 GL_STATIC_DRAW);
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
    
    // Calculate transformatio matrices
    mat4 rotation = mat4::Rotate(m_fRotationAngle);
    mat4 scale = mat4::Scale(m_fScale);
    mat4 translation = mat4::Translate(0, 0, -7);
    
    // Set the model view matrix
    GLint modelviewUniform = glGetUniformLocation(m_iSimpleProgram, "ModelView");
    mat4 modelviewMatrix = scale * rotation * translation;
    
    GLsizei stride = sizeof(Vertex);
    const GLvoid* pColorOffset = (GLvoid*) sizeof(vec3);
    
    //glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(modelviewUniform, 1, 0, modelviewMatrix.Pointer());
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iIndexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_iVertexBufferID);
    glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, stride, pColorOffset);
    glEnableVertexAttribArray(positionSlot);
    
    const GLvoid* pBodyOffset = 0;
    const GLvoid* pDiskOffset = (GLvoid*) m_iBodyIndexCount;
    
    glEnableVertexAttribArray(colorSlot);
    glDrawElements(GL_TRIANGLES, m_iBodyIndexCount, GL_UNSIGNED_BYTE, pBodyOffset);
    glDisableVertexAttribArray(colorSlot);
    glVertexAttrib4f(colorSlot, 1, 1, 1, 1);
    glDrawElements(GL_TRIANGLES, m_iDiskIndexCount, GL_UNSIGNED_BYTE, pDiskOffset);
    
    glDisableVertexAttribArray(positionSlot);
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