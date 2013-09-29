/*
 * ScreenManager.cpp
 *
 *  Created on: 28/10/2010
 *      Author: Daniel Conde Linares
 */

#include "ScreenManager.h"
#include "Screen.h"
#include "PlayingScreen.h"
#include "TreckyScreen.h"
#include "MainMenuScreen.h"

ScreenManager* ScreenManager::m_pInstance = NULL;

ScreenManager* ScreenManager::getInstance() {
	if(m_pInstance == NULL) {
		m_pInstance = new ScreenManager();
	}

	return m_pInstance;
}

ScreenManager::ScreenManager() {
	// TODO Auto-generated constructor stub
	m_screens.Construct(ScreenManager::MAX_SCREENS);
	m_screensToUpdate.Construct();
	// m_screens = new Screen*[ScreenManager::MAX_SCREENS];
}

ScreenManager::~ScreenManager() {
	// TODO Auto-generated destructor stub
}

void ScreenManager::Initialize()
{
	TreckyScreen* l_pTreckyScreen = new TreckyScreen(this);
	l_pTreckyScreen->initialize();
	m_screens.Add(l_pTreckyScreen); // Screen 0
	// m_screens[ScreenManager::LOADING] = l_pTreckyScreen;

	//PlayingScreen* l_pPlayingScreen = new PlayingScreen(this);
	//l_pPlayingScreen->initialize();
	//m_screens.Add(l_pPlayingScreen); // Screen 1
	// m_screens[ScreenManager::MAINMENU] = l_pPlayingScreen;

	MainMenuScreen* l_pMainMenuScreen = new MainMenuScreen(this);
	l_pMainMenuScreen->initialize();
	m_screens.Add(l_pMainMenuScreen); // Screen 2
	//m_screens[ScreenManager::OPTIONS] = l_pMainMenuScreen;

	m_eCurrentScreen = ScreenManager::MAINMENU;
}

void ScreenManager::Update(long long gameTime) {
	// Process Game Logic
	Screen* pCurrentScreen;
	m_screens.GetAt(m_eCurrentScreen, pCurrentScreen);

	if(pCurrentScreen)
		pCurrentScreen->update(gameTime, true, true);

	//AppLog("Screen Updated!");

	/*
	// Make a copy of the master screen list, to avoid confusion if
	// the process of updating one screen adds or removes others.
	m_screensToUpdate.RemoveAll();

	for(int i = 0; i<m_screens.GetCount(); ++i)
	{
		Screen* pCurrentScreen;
		m_screens.GetAt(i, pCurrentScreen);
		if(pCurrentScreen)
			m_screensToUpdate.Add(pCurrentScreen);
	}

	bool otherScreenHasFocus = false; //!Game.IsActive;
	bool coveredByOtherScreen = false;

	// Loop as long as there are screens waiting to be updated.
	while (m_screensToUpdate.GetCount() > 0) {
		// Pop the topmost screen off the waiting list.
		Screen* screen;
		m_screensToUpdate.GetAt(m_screensToUpdate.GetCount() - 1, screen);

		m_screensToUpdate.RemoveAt(m_screensToUpdate.GetCount() - 1);

		// Update the screen.
		screen->update(gameTime, otherScreenHasFocus, coveredByOtherScreen);

		if (screen->getState() == Screen::TRANSITION_ON
				|| screen->getState() == Screen::ACTIVE) {
			// If this is the first active screen we came across,
			// give it a chance to handle input.
			if (!otherScreenHasFocus) {
				//screen.HandleInput(EngineManager.Input);

				otherScreenHasFocus = true;
			}

			// If this is an active non-popup, inform any subsequent
			// screens that they are covered by it.
			if (!screen->getIsPopup())
				coveredByOtherScreen = true;
		}
	}

	// Print debug trace?
	//if (_traceEnabled)
	//TraceScreens();
	*/
}

void ScreenManager::ProcessTouchs(const TouchEventInfo & touchInfo) {
	// Process Screen Touches
	Screen* pCurrentScreen;
	m_screens.GetAt(m_eCurrentScreen, pCurrentScreen);

	if(pCurrentScreen)
		pCurrentScreen->processTouchs(touchInfo);

//	AppLog("TouchPressed %d,%d - %d,%d",
//					touchInfo.GetStartPosition().x,
//					touchInfo.GetStartPosition().y,
//					touchInfo.GetCurrentPosition().x,
//					touchInfo.GetCurrentPosition().y);
}

void ScreenManager::Draw(long long gameTime) {
	// Process Game Rendering
	Screen* pCurrentScreen;
	m_screens.GetAt(m_eCurrentScreen, pCurrentScreen);

	if (pCurrentScreen)
		pCurrentScreen->draw(gameTime);
}


/*void ScreenManager::LoadContent()
{
	// TODO Create the default Draw Buffer

	// TODO Create the default Font

	// TODO Create the default Texture


}*/

/*
private static List<GameScreen> _screens = new List<GameScreen>();
private static List<GameScreen> _screensToUpdate = new List<GameScreen>();

/// <summary>
/// Expose an array holding all the screens. We return a copy rather
/// than the real master list, because screens should only ever be added
/// or removed using the AddScreen and RemoveScreen methods.
/// </summary>
public static GameScreen[] GetScreens()
{
	return _screens.ToArray();
}

private static bool _initialized = false;
/// <summary>
/// Is the ScreenManagers Initialized, used for test cases and setup of Effects.
/// </summary>
public static bool Initialized
{
	get { return _initialized; }
}

private static SpriteBatch _spriteBatch;
/// <summary>
/// A default SpriteBatch shared by all the screens. This saves
/// each screen having to bother creating their own local instance.
/// </summary>
public static SpriteBatch SpriteBatch
{
	get { return _spriteBatch; }
}

private static SpriteFont _font;
/// <summary>
/// A default font shared by all the screens. This saves
/// each screen having to bother loading their own local copy.
/// </summary>
public static SpriteFont Font
{
	get { return _font; }
}

bool _traceEnabled = true;
/// <summary>
/// If true, the manager prints out a list of all the screens
/// each time it is updated. This can be useful for making sure
/// everything is being added and removed at the right times.
/// </summary>
public bool TraceEnabled
{
	get { return _traceEnabled; }
	set { _traceEnabled = value; }
}

/// <summary>
/// Constructs a new screen manager component.
/// </summary>
public ScreenManager(Game game)
            		: base(game)
            		  {
	Enabled = true;
            		  }

protected override void LoadContent()
{
	base.LoadContent();

	_spriteBatch = new SpriteBatch(EngineManager.Device);
	_font = EngineManager.ContentManager.Load<SpriteFont>("Content/Fonts/menufont");
	TextureManager.AddTexture(new RoeTexture("Content/Textures/blank"), "blank");

	foreach (GameScreen screen in _screens)
	{
		screen.LoadContent();
	}
}

protected override void UnloadContent()
{
	base.UnloadContent();

	foreach (GameScreen screen in _screens)
	{
		screen.UnloadContent();
	}
}

/// <summary>
/// Initializes each screen and the screen manager itself.
/// </summary>
public override void Initialize()
{
	base.Initialize();

	_initialized = true;
}

/// <summary>
/// Allows each screen to run logic.
/// </summary>
public override void Update(GameTime gameTime)
{
	// Read the keyboard and gamepad.
	EngineManager.Input.Update();

	// Make a copy of the master screen list, to avoid confusion if
	// the process of updating one screen adds or removes others.
	_screensToUpdate.Clear();

	foreach (GameScreen screen in _screens)
	_screensToUpdate.Add(screen);

	bool otherScreenHasFocus = !Game.IsActive;
	bool coveredByOtherScreen = false;

	// Loop as long as there are screens waiting to be updated.
	while (_screensToUpdate.Count > 0)
	{
		// Pop the topmost screen off the waiting list.
		GameScreen screen = _screensToUpdate[_screensToUpdate.Count - 1];

		_screensToUpdate.RemoveAt(_screensToUpdate.Count - 1);

		// Update the screen.
		screen.Update(gameTime, otherScreenHasFocus, coveredByOtherScreen);

		if (screen.ScreenState == ScreenState.TransitionOn ||
				screen.ScreenState == ScreenState.Active)
		{
			// If this is the first active screen we came across,
			// give it a chance to handle input.
			if (!otherScreenHasFocus)
			{
				screen.HandleInput(EngineManager.Input);

				otherScreenHasFocus = true;
			}

			// If this is an active non-popup, inform any subsequent
			// screens that they are covered by it.
			if (!screen.IsPopup)
				coveredByOtherScreen = true;
		}
	}

	// Print debug trace?
	if (_traceEnabled)
		TraceScreens();
}

/// <summary>
/// Prints a list of all the screens, for debugging.
/// </summary>
private void TraceScreens()
{
	List<string> screenNames = new List<string>();

	foreach (GameScreen screen in _screens)
	screenNames.Add(screen.GetType().Name);

	Trace.WriteLine(string.Join(", ", screenNames.ToArray()));
}

/// <summary>
/// Tells each screen to draw itself.
/// </summary>
public override void Draw(GameTime gameTime)
{
	foreach (GameScreen screen in _screens)
            		{
		if (screen.ScreenState == ScreenState.Hidden)
			continue;

		screen.Draw(gameTime);
            		}

	foreach (GameScreen screen in _screens)
	{
		if (screen.ScreenState == ScreenState.Hidden)
			continue;
		screen.PostUIDraw(gameTime);
	}
}

/// <summary>
/// Adds a new screen to the screen manager.
/// </summary>
public static void AddScreen(GameScreen screen)
{
	// If we have a graphics device, tell the screen to load content.
	_screens.Add(screen);
	if (_initialized)
	{
		screen.LoadContent();
	}
}

/// <summary>
/// Removes a screen from the screen manager. You should normally
/// use GameScreen.ExitScreen instead of calling this directly, so
/// the screen can gradually transition off rather than just being
/// instantly removed.
/// </summary>
public static void RemoveScreen(GameScreen screen)
{
	// If we have a graphics device, tell the screen to unload content.

	if (_initialized)
	{
		screen.UnloadContent();
	}

	_screens.Remove(screen);
	_screensToUpdate.Remove(screen);
}

/// <summary>
/// Helper draws a translucent black fullscreen sprite, used for fading
/// screens in and out, and for darkening the background behind popups.
/// </summary>
public static void FadeBackBufferToBlack(int alpha)
{
	Viewport viewport = EngineManager.Device.Viewport;

	_spriteBatch.Begin();

	_spriteBatch.Draw(TextureManager.GetTexture("blank").BaseTexture as Texture2D,
			new Rectangle(0, 0, viewport.Width, viewport.Height),
			new Color(0, 0, 0, (byte)alpha));

	_spriteBatch.End();
}
}
}
*/
