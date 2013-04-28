//
//  GLView.h
//  HellowArrow
//
//  Created by Daniel on 31/03/2013.
//  Copyright (c) 2013 Daniel Conde Linares. All rights reserved.
//

#import "IRenderingEngine.hpp"
#import <OpenGLES/EAGL.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

@interface GLView : UIView
{
@private
    EAGLContext* m_pGLContext;
    IRenderingEngine* m_pRenderingEngine;
    float m_fTimeStamp;
}

- (void) drawView: (CADisplayLink*) displayLink;
- (void) didRotate: (NSNotification*) notification;

@end