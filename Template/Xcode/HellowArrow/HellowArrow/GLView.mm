//
//  GLView.m
//  HellowArrow
//
//  Created by Daniel on 31/03/2013.
//  Copyright (c) 2013 Daniel Conde Linares. All rights reserved.
//

#import <OpenGLES/EAGLDrawable.h>
#import "GLView.h"
#import <OpenGLES/ES2/gl.h>

@implementation GLView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*) super.layer;
        eaglLayer.opaque = YES;
        
        m_pGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        if (!m_pGLContext || ![EAGLContext setCurrentContext:m_pGLContext])
        {
            [self release];
            return nil;
        }
        
        m_pRenderingEngine = CreateRenderer1();
        
        [m_pGLContext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:eaglLayer];
        
        m_pRenderingEngine->Initialize(CGRectGetWidth(frame), CGRectGetHeight(frame));
        
        [self drawView:nil];
        m_fTimeStamp = CACurrentMediaTime();
        
        CADisplayLink* displayLink;
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didRotate:) name:UIDeviceOrientationDidChangeNotification object:nil];
    }
    return self;
}

- (void) didRotate:(NSNotification *)notification
{
    UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
    m_pRenderingEngine->OnRotate((DeviceOrientation)orientation);
    [self drawView:nil];
}

- (void) drawView:(CADisplayLink *)displayLink
{
    if(displayLink != nil)
    {
        float elapsedSeconds = displayLink.timestamp - m_fTimeStamp;
        m_fTimeStamp = displayLink.timestamp;
        m_pRenderingEngine->UpdateAnimation(elapsedSeconds);
    }
    
    m_pRenderingEngine->Render();
    
    [m_pGLContext presentRenderbuffer:GL_RENDERBUFFER_OES];
}

/*
 Redefine default layer class as Quartz OpenGL layer
 */
+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (void) dealloc
{
    if([EAGLContext currentContext] == m_pGLContext)
        [EAGLContext setCurrentContext:nil];
    
    [m_pGLContext release];
    [super dealloc];
}

@end
