//
//  IRenderingEngine.h
//  HellowArrow
//
//  Created by Daniel on 31/03/2013.
//  Copyright (c) 2013 Daniel Conde Linares. All rights reserved.
//

#ifndef IRenderingEngine_hh
#define IRenderingEngine_hh

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

class IRenderingEngine{
public:
    virtual void Initialize(int width, int height) = 0;
    virtual void Render(void) const = 0;
    virtual void UpdateAnimation(float timeStep) = 0;
    virtual void OnRotate(DeviceOrientation orientation) = 0;
    virtual ~IRenderingEngine() {}
};

#endif