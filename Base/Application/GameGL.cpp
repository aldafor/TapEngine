/**
 * Name        : GameGL
 * Version     : 
 * Vendor      : Daniel Conde Linares - Genera Interactive
 * Description : 
 */

#include "GameGL.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::System;
using namespace Osp::Graphics;
using namespace Osp::Graphics::Opengl;
using namespace Osp::Ui;

GameGL::GameGL() :
	eDisplay(EGL_DEFAULT_DISPLAY),
	eSurface(EGL_NO_SURFACE),
	eConfig(null),
	eContext(EGL_NO_CONTEXT),
	m_pForm(NULL) {
	// Constructor
}

GameGL::~GameGL() {
	// Destructor
}

Application* GameGL::CreateInstance(void) {
	// Create the instance through the constructor.
	return new GameGL();
}

bool GameGL::OnAppInitializing(AppRegistry& appRegistry) {
	result r = E_SUCCESS;

	m_pForm = new GameForm(this);
	if(m_pForm == NULL)
	{
		AppLog("Allocation of GameForm failed!");
	}

	r = m_pForm->Construct(Controls::FORM_STYLE_NORMAL);
	if(IsFailed(r))
	{
		AppLog("Costruction of GameForm with normal style failed!");
	}

	m_pForm->SetOrientation(ORIENTATION_LANDSCAPE);
	r = GetAppFrame()->GetFrame()->AddControl(*m_pForm);
	if(IsFailed(r))
	{
		AppLog("Adding GameForm to the Application Failed!");
	}

	/*IAppFrame* pAppFrame = GetAppFrame();
	r = pAppFrame->AddTouchEventListener(*this);
	if (IsFailed(r))
	{
		AppLog("AddTouchEventListener(*this) has failed.\n");
	}
	Touch toucheable;
	toucheable.SetMultipointEnabled(*(GetAppFrame()->GetFrame()), true);

	r = pAppFrame->AddKeyEventListener(*this);
	if (IsFailed(r))
	{
		AppLog("AddKeyEventListener(*this) has failed.\n");
	}*/

    if (!InitEGL())
    	AppLog("Fail on EGL initialization");

    if (!InitGL())
    	AppLog("Fail on OpenGL Initial Setup");

    ScreenManager* l_pScreenManager = ScreenManager::getInstance();
    l_pScreenManager->Initialize();

    // Initialize the timer - Main Loop
	m_pTimer = new Osp::Base::Runtime::Timer;
	r = m_pTimer->Construct(*this);

	if (IsFailed(r)) {
		AppLog("Timer Failed");
	}

	return true;
}

bool GameGL::OnAppTerminating(AppRegistry& appRegistry, bool forcedTermination) {
	// TODO:
	// Deallocate resources allocated by this application for termination.
	// The application's permanent data and context can be saved via appRegistry.
	return true;
}

void GameGL::OnForeground(void) {
	// TODO:
	// Start or resume drawing when the application is moved to the foreground.

	if (m_pTimer) {
		m_pTimer->Start(1000 / m_iFPS);
	}
}

void GameGL::OnBackground(void) {
	// TODO:
	// Stop drawing when the application is moved to the background.
	if (m_pTimer) {
		m_pTimer->Cancel();
	}
}

void GameGL::OnLowMemory(void) {
	// TODO:
	// Free unused resources or close the application.
}

void GameGL::OnBatteryLevelChanged(BatteryLevel batteryLevel) {
	// TODO:
	// Handle any changes in battery level here.
	// Stop using multimedia features(camera, mp3 etc.) if the battery level is CRITICAL.
}

void GameGL::OnScreenOn(void) {
	// TODO:
	// Get the released resources or resume the operations that were paused or stopped in OnScreenOff().
}

void GameGL::OnScreenOff(void) {
	// TODO:
	//  Unless there is a strong reason to do otherwise, release resources (such as 3D, media, and sensors) to allow the device to enter the sleep mode to save the battery.
	// Invoking a lengthy asynchronous method within this listener method can be risky, because it is not guaranteed to invoke a callback before the device enters the sleep mode.
	// Similarly, do not perform lengthy operations in this listener method. Any operation must be a quick one.
}

void GameGL::OnTimerExpired(Osp::Base::Runtime::Timer& timer) {
	SystemTime::GetTicks(m_llTicks);

	if ( !Update(m_llTicks) )
	{
		AppLog("Fail on Update");
	}
	if ( !Draw(m_llTicks) )
	{
		AppLog("Fail on Draw");
	}

	m_pTimer->Start(1000 / m_iFPS);
}

void GameGL::OnTouchReleased(const Osp::Ui::Control& source,
			const Osp::Graphics::Point& currentPosition,
			const TouchEventInfo & touchInfo) {
	//AppLog("TouchPressed");
	//DisplayMultipointTouchInfo();
	ScreenManager::getInstance()->ProcessTouchs(touchInfo);
}

void GameGL::DisplayMultipointTouchInfo()
{
	Touch tempTouch;
	Collection::IList* l_pList = NULL;
	l_pList = tempTouch.GetTouchInfoListN();
	if(l_pList)
	{
		for(int i=0; i<l_pList->GetCount(); i++)
		{
			TouchInfo* l_pTouchInfo = static_cast<TouchInfo*> (l_pList->GetAt(i));
			AppLog("TouchInfo: [%d] %d,%d - %d", l_pTouchInfo->id, l_pTouchInfo->position.x, l_pTouchInfo->position.y, l_pTouchInfo->status);
		}
		l_pList->RemoveAll(true);
	}
}

void GameGL::OnKeyPressed(const Osp::Ui::Control& source, Osp::Ui::KeyCode keyCode)
{
	switch((int)keyCode)
	{
	case Ui::KEY_SIDE_UP:
		{
			BadaGameLib::SoundManager::getInstance()->IncreaseVolume();
			break;
		}
	case Ui::KEY_SIDE_DOWN:
		{
			BadaGameLib::SoundManager::getInstance()->DecreaseVolume();
			break;
		}
	}
}

bool GameGL::InitEGL() {
	// Bind EGL with OpenGLES API
    eglBindAPI(EGL_OPENGL_ES_API);

    // Set the default number of EGL configurations that allow our Device
    EGLint numConfigs = 1;

    // Preferred Display configuration
    EGLint eConfigList[] = {
    	EGL_RED_SIZE, 			8,	// Bits for Red Color
		EGL_GREEN_SIZE, 		8,	// Bits for Green Color
        EGL_BLUE_SIZE, 			8,	// Bits for Blue Color
        EGL_ALPHA_SIZE,         8,
		//EGL_DEPTH_SIZE,			8,  // Bits for Depth buffer
        EGL_RENDERABLE_TYPE,		// Which client API are supported
        EGL_OPENGL_ES_BIT,			// EGLConfing will support OpenGL ES
        EGL_SURFACE_TYPE,			// Which types of EGL surfaces are supported
        EGL_WINDOW_BIT,				// EGLConfig will support Windows
        EGL_NONE 					// Config list must terminate with it
    };

    // Get the default display monitor
    eDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);
    if (EGL_NO_DISPLAY == eDisplay) {
		AppLog("[GlesCube11] eglGetDisplay() is failed. [0x%x]\n", eglGetError());
		return false;
	}

	// Initialize EGL
	if (EGL_FALSE == eglInitialize(eDisplay, null, null) || EGL_SUCCESS != eglGetError()) {
		AppLog("[GlesCube11] eglInitialize() is failed. [0x%x]\n", eglGetError());
		return false;
	}

	// Make EGL to choose the best Display configuration according with the Preferred
	if (EGL_FALSE == eglChooseConfig(eDisplay, eConfigList, &eConfig, 1, &numConfigs) || EGL_SUCCESS != eglGetError()) {
		AppLog("[GlesCube11] eglChooseConfig() is failed. [0x%x]\n", eglGetError());
		return false;
	}
	if (!numConfigs) {
		AppLog("[GlesCube11] eglChooseConfig() has been failed. because of matching config doesn't exist \n");
		return false;
	}

	// Create buffer for Draw
	eSurface = eglCreateWindowSurface(eDisplay, eConfig, (EGLNativeWindowType) m_pForm, NULL);
	//eSurface = eglCreateWindowSurface(eDisplay, eConfig, (EGLNativeWindowType) GetAppFrame()->GetFrame(), NULL);
	if (EGL_NO_SURFACE == eSurface || EGL_SUCCESS != eglGetError()) {
		AppLog("[GlesCube11] eglCreateWindowSurface() has been failed. EGL_NO_SURFACE [0x%x]\n", eglGetError());
		return false;
	}

	// Create an internal state of OpenGL ES
	eContext = eglCreateContext(eDisplay, eConfig, EGL_NO_CONTEXT, NULL);
	if (EGL_NO_CONTEXT == eContext || EGL_SUCCESS != eglGetError()) {
		AppLog("[GlesCube11] eglCreateContext() has been failed. [0x%x]\n", eglGetError());
		return false;
	}

	// Tell EGL to use the current OpenGL Context
	if (false == eglMakeCurrent(eDisplay, eSurface, eSurface, eContext) || EGL_SUCCESS != eglGetError()) {
		AppLog("[GlesCube11] eglMakeCurrent() has been failed. [0x%x]\n", eglGetError());
		return false;
	}

	return true;
}

bool GameGL::InitGL() {

	// Get the GameForm dimensions CHANGING the width & height because the LANDSCAPE ORIENTATION
	m_pForm->GetBounds(m_iX, m_iY, m_iHeight, m_iWidth);
	//m_pForm->GetBounds(m_iX, m_iY, m_iWidth, m_iHeight);
	//GetAppFrame()->GetFrame()->GetBounds(m_iX, m_iY, m_iWidth, m_iHeight);

	// Map Cartesian coordinates to physical screen pixels
	glViewport(0, 0, m_iWidth, m_iHeight);
	// (-1, 1) (0, 1) (1, 1)
	// (-1, 0) (0, 0) (1, 0)
	// (-1,-1) (0,-1) (1,-1)

	// Set Shade Model to smooth, for color interpolation
	glShadeModel(GL_SMOOTH);
	// Activate the back face culling (Don't render back geometries faces)
	glEnable(GL_CULL_FACE);

	// Setup and adjust the Matrix projection to orthogonal mode
	glMatrixMode(GL_PROJECTION);
	glOrthof(0, 1, 0, 1, -12, +12);
	glMatrixMode(GL_MODELVIEW);

	// Set RGBA default background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return true;
}

bool GameGL::Update(long long  gameTime) {
	// Call Screen Manager to update current active screens
	ScreenManager::getInstance()->Update(gameTime);

	return true;
}

bool GameGL::Draw(long long gameTime) {

	// Clear screen with the default background color
    glClear(GL_COLOR_BUFFER_BIT );

    // Setup default transformations matrix
    glMatrixMode( GL_MODELVIEW);
    glLoadIdentity();

    //glRotatef(m_pForm->getOrientationRotation(), 0.0f, 1.0f, 0.0f);

    // Call Screen Manager to draw current visible screens
    ScreenManager::getInstance()->Draw(gameTime);

    // Flip backBuffer with screenBuffer to see geometry on the screen
    eglSwapBuffers(eDisplay, eSurface);

    return true;
}
