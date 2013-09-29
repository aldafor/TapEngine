#ifndef STDAFX_H
#define STDAFX_H

// This file is used to include all precompiled files into this project

// Reduce the size of the win32 header files
#define WIN32_LEAN_AND_MEAN

// Required Win32 libraries
#include <windows.h>
#include <ObjBase.h> // CoInitialize
//#include <WinBase.h> // CreateMutex

// Required C++ Standard libraries
#include <stdio.h>
#include <time.h>
#include <MMSystem.h>

// Required OpenGL Renderer libraries
#include <gl/GL.h>
#include <gl/GLU.h>

#endif //STDAFX_H