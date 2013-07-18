//
//  IRenderingEngine.h
//  HellowArrow
//
//  Created by Daniel on 31/03/2013.
//  Copyright (c) 2013 Daniel Conde Linares. All rights reserved.
//

#ifndef IRenderingEngine_hh
#define IRenderingEngine_hh

#include "Vector.hpp"

enum DeviceOrientation
{
    Unknown,
    Portrait,
    PortraitUpsideDown,
    LandscapeLeft,
    LandscapeRight,
    FaceUp,
    FaceDown
};

class IRenderingEngine* CreateRenderer1();
class IRenderingEngine* CreateRenderer2();

class IRenderingEngine{
public:
    virtual void Initialize(int width, int height) = 0;
    virtual void Render(void) const = 0;
    virtual void UpdateAnimation(float timeStep) = 0;
    virtual void OnRotate(DeviceOrientation orientation) = 0;
    virtual void OnFingerUp(ivec2 location) = 0;
    virtual void OnFingerDown(ivec2 location) = 0;
    virtual void OnFingerMove(ivec2 oldLocation, ivec2 newLocation) = 0;
    virtual ~IRenderingEngine() {}
};

#endif