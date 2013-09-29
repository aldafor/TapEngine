#ifndef _GAMEGL_H_
#define _GAMEGL_H_

#include <FApp.h>
#include <FBase.h>
#include <FGraphics.h>
#include <FSystem.h>
#include <FGraphicsOpengl.h>
#include <FUi.h>
#include "GameForm.h"
#include "ScreenManager.h"
#include "SoundManager.h"

/**
 * [GameGL] application must inherit from Application class
 * which provides basic features necessary to define an application.
 */
class GameGL :
        public Osp::App::Application,
        public Osp::Base::Runtime::ITimerEventListener,
        public Osp::System::IScreenEventListener,
        public Osp::Ui::IKeyEventListener,
        public Osp::Ui::ITouchEventListener
{
public:
    // [GameGL] application must have a factory method that creates an instance of itself.
	static Osp::App::Application* CreateInstance(void);

public:
	GameGL();
	~GameGL();

public:

	// Called when the application is initializing.
	bool OnAppInitializing(Osp::App::AppRegistry& appRegistry);

	// Called when the application is terminating.
	bool OnAppTerminating(Osp::App::AppRegistry& appRegistry,
			bool forcedTermination = false);

	// Called when the application's frame moves to the top of the screen.
	void OnForeground(void);

	// Called when this application's frame is moved from top of the screen to the background.
	void OnBackground(void);

	// Called when the system memory is not sufficient to run the application any further.
	void OnLowMemory(void);

	// Called when the battery level changes.
	void OnBatteryLevelChanged(Osp::System::BatteryLevel batteryLevel);

	// Called when the screen turns on.
	void OnScreenOn(void);

	// Called when the screen turns off.
	void OnScreenOff(void);

public:

	// Called after Timer ends - Used as Main Loop
	void OnTimerExpired(Osp::Base::Runtime::Timer& timer);

public:

    /**
	 * Notifies when an entity is touch pressed.
	 *
	 * @param[in] 	source			The source of the event
	 * @param[in] 	currentPosition	The current position
	 * @param[in] 	touchInfo		The touch event information
	 */
	void OnTouchPressed(const Osp::Ui::Control& source,
			const Osp::Graphics::Point& currentPosition,
			const TouchEventInfo & touchInfo) {};

	/**
	 * Notifies when an entity is touched and held for a long time.
	 *
	 * @param[in] 	source			The source of the event
	 * @param[in] 	currentPosition	The current position
	 * @param[in] 	touchInfo		The touch event information
	 */
	void OnTouchLongPressed(const Osp::Ui::Control& source,
			const Osp::Graphics::Point& currentPosition,
			const TouchEventInfo & touchInfo) {};

	/**
	 * Notifies when an entity is touch released.
	 *
	 * @param[in] 	source			The source of the event
	 * @param[in] 	currentPosition	The current position
	 * @param[in] 	touchInfo		The touch event information
	 */
	void OnTouchReleased(const Osp::Ui::Control& source,
			const Osp::Graphics::Point& currentPosition,
			const TouchEventInfo & touchInfo);

	/**
	 * Notifies when an entity is touch moved.
	 *
	 * @param[in] 	source			The source of the event
	 * @param[in] 	currentPosition	The current position
	 * @param[in] 	touchInfo		The touch event information
	 */
	void OnTouchMoved(const Osp::Ui::Control& source,
			const Osp::Graphics::Point& currentPosition,
			const TouchEventInfo & touchInfo) {};

	/**
	 * Notifies when an entity is touched and double-pressed.
	 *
	 * @param[in] 	source			The source of the event
	 * @param[in] 	currentPosition	The current position
	 * @param[in] 	touchInfo		The touch event information
	 */
	void OnTouchDoublePressed(const Osp::Ui::Control& source,
			const Osp::Graphics::Point& currentPosition,
			const TouchEventInfo & touchInfo) {};

	/**
	 * Notifies when an entity becomes touch focus-in.
	 *
	 * @param[in] 	source			The source of the event
	 * @param[in] 	currentPosition	The current position
	 * @param[in] 	touchInfo		The touch event information
	 */
	void OnTouchFocusIn(const Osp::Ui::Control& source,
			const Osp::Graphics::Point& currentPosition,
			const TouchEventInfo & touchInfo) {};

	/**
	 * Notifies when an entity becomes touch focus-out.
	 *
	 * @param[in] 	source			The source of the event
	 * @param[in] 	currentPosition	The current position
	 * @param[in] 	touchInfo		The touch event information
	 */
	void OnTouchFocusOut(const Osp::Ui::Control& source,
			const Osp::Graphics::Point& currentPosition,
			const TouchEventInfo & touchInfo) {};

	void DisplayMultipointTouchInfo();

public:

	void OnKeyPressed(const Osp::Ui::Control& source, Osp::Ui::KeyCode keyCode);
	void OnKeyReleased(const Osp::Ui::Control& source, Osp::Ui::KeyCode keyCode){};
	void OnKeyLongPressed(const Osp::Ui::Control& source, Osp::Ui::KeyCode keyCode){};

private:
	bool InitEGL(); //initialze EGL
	bool InitGL(); //Initialize OpenGL

public:
	bool Update(long long gameTime); //fill backround with green color
	bool Draw(long long gameTime); //fill backround with green color

private:
	Osp::Graphics::Opengl::EGLDisplay eDisplay; //needed in EGL initialization -part 2
	Osp::Graphics::Opengl::EGLSurface eSurface; //needed in EGL initialization -part 2
	Osp::Graphics::Opengl::EGLConfig eConfig; //needed in EGL initialization -part 2
	Osp::Graphics::Opengl::EGLContext eContext; //needed in EGL initialization -part 2

	int m_iX, m_iY, m_iWidth, m_iHeight;
	//Osp::Ui::Controls::Form* m_pForm;
	GameForm* m_pForm;

private:
	//const static int m_iTimerSpan = 10;
	const static int m_iFPS = 12;
	long long m_llTicks;

	Osp::Base::Runtime::Timer* m_pTimer;

	float m_fDesiredAngle; // For rotate the screen on Device orientation changes
};

#endif
