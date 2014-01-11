#include "Tetrix3DEngine.h"

//The Tetrix3DEngine pointer
static Tetrix3DEngine *pTetrix3DEngine = NULL;


//-----------------------------------------------------------------------------
// Name : Tetrix3DEngine () (Constructor)
// Desc : Tetrix3DEngine Class Constructor
//-----------------------------------------------------------------------------
Tetrix3DEngine::Tetrix3DEngine()
{
	pTetrix3DEngine = this;

	/////////////////////////////////
	CurListIndx = 1;
	/////////////////////////////////
	glGenBuffersARB    = NULL;	// VBO Name Generation Procedure
	glBindBufferARB    = NULL;	// VBO Bind Procedure
	glBufferDataARB    = NULL;	// VBO Data Loading Procedure
	glDeleteBuffersARB = NULL;	// VBO Deletion Procedure
	/////////////////////////////////
	pSpaceMem      = NULL;
	pSpaceMemX     = NULL;
	pSpaceMemY     = NULL;
	pSpaceMemZ     = NULL;
	pSpaceMemColor = NULL;
	pTmpSpaceMem   = NULL;
	pLinesToRemove = NULL;
	pZ_Score       = NULL;
	//pLineDonePerc  = NULL;
	/////////////////////////////////
	Fader = new CTetrix3DFader;
	pCurrentShape = new CShapeStruct;
	pNextShape = new CShapeStruct;
	g_ScreenDat.Registered = false;
	g_FPS = 0.0f;
	g_InvertMouse = false;
	xRotation = 0;
	yRotation = 0;
	g_SoundEnabled = false;		
	g_ShowFPS = true;
	g_ShowBck = true;
	g_ShowGhost = true;//change this later to false...
	ZeroMemory(&g_MouseButtonState[0], 3);
	ZeroMemory(&g_Key[0], 256);
	g_HiScore = 0;
	g_RenderIntervalRatioMem = 0.0f;
	g_CurrentMessage = MsgNoMsg;
	g_MessageTimeLeft = 0.0f;
	g_VBO = false;
	g_MusicEnabled = true;
	/////////////////////////////////
	//Set the transparency color of our texture loader class to blue
	Textures.SetTransparentColor(0, 255, 255);
	//Select Start as the first option selected
	g_IntroSelection = INTRO_START;
	//Tell if the intro animation is finish
	IsIntroAnimationFinish = false;
	//Init the seed of Rand
	srand((unsigned)time(NULL));
	//...
	g_InRedraw = false;
	g_Lines = 0;
	g_SakeCount = 0;

	//
	Init_GameOver_Pattern();
	InitTimerSpeed();

	////////NEW//////////
	Is_AI_Playing = false;
	TimeLeftBeforeAIStart = DEF_TIME_BEFORE_AI_START;
	AI_MovesCounter = 0;

	//
	AI_RotationNeeded[0] = 0x000001F1;
	AI_RotationNeeded[1] = 0x00000001;
	AI_RotationNeeded[2] = 0x0F0FFFFF;
	AI_RotationNeeded[3] = 0x0F0FFFFF;
	AI_RotationNeeded[4] = 0x0F0FFFFF;
	AI_RotationNeeded[5] = 0x0F0FFFFF;
	AI_RotationNeeded[6] = 0x0F0FFFFF;

    hTerminateAIThreadEvent = CreateEvent(NULL, 0, 0, "TerminateAIThreadEvent");
}

//-----------------------------------------------------------------------------
// Name : ~Tetrix3DEngine () (Destructor)
// Desc : Tetrix3DEngine Class Destructor
//-----------------------------------------------------------------------------
Tetrix3DEngine::~Tetrix3DEngine()
{
	// This will wait and kill the AI thread, if running
	SetEvent(hTerminateAIThreadEvent);
	WaitForSingleObject(hMakeAIPlay, INFINITE);
	ResetEvent(hTerminateAIThreadEvent);
	//Close the handle
	CloseHandle(hTerminateAIThreadEvent);
	CloseHandle(hMakeAIPlay);

	//Free the loaded fonts
	FreeFont();

	SAFE_DELETE_OBJECT(Fader);
	SAFE_DELETE_OBJECT(pCurrentShape);
	SAFE_DELETE_OBJECT(pNextShape);

	// Delete VBOs(this code suck, must find other way later)
	if(g_VBO){
		UINT nBuffers[12] = {The3DTextModel.NormalVBO_ID, The3DTextModel.TexVertsVBO_ID, The3DTextModel.VertsVBO_ID, BlockModel[0].NormalVBO_ID, BlockModel[0].TexVertsVBO_ID, BlockModel[0].VertsVBO_ID, BlockModel[1].NormalVBO_ID, BlockModel[1].TexVertsVBO_ID, BlockModel[1].VertsVBO_ID, BlockModel[2].NormalVBO_ID, BlockModel[2].TexVertsVBO_ID, BlockModel[2].VertsVBO_ID};
		glDeleteBuffersARB(12, nBuffers);						// Free The Memory
	}
	
	//Do user cleanup
	GameCleanup();

	//Clean this object
	CleanUpOpenGL();

	//Unregister the window
	DestroyGameWindow();

	//Save current setting to the registry
	RegistryReaderWriter(WRITETOREG, 
		&g_ScreenDat.Width, &g_ScreenDat.Height, /*&Bpp,*/ &g_ScreenDat.FullScreen,
		&g_CubeDetail, &g_ShowTexture, &g_SoundEnabled, &g_MusicEnabled, 
		&X_RowLength, &Y_RowLength, &Z_RowLength, 
		&g_GameKey[0],
		&g_HiScore);
}




//-----------------------------------------------------------------------------
// Name : WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// Desc : A pointer to MsgProc()
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return pTetrix3DEngine->MsgProc(hWnd, uMsg, wParam, lParam);
}


//-----------------------------------------------------------------------------
// Name : MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// Desc : Process all the window message here.
//-----------------------------------------------------------------------------
LRESULT Tetrix3DEngine::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	//This will close the app.
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;	
	case WM_SYSCOMMAND:			// Intercept System Commands
		switch (wParam)			// Check System Calls
		{
		case SC_SCREENSAVE:		// Screensaver Trying To Start?
		case SC_MONITORPOWER:	// Monitor Trying To Enter Powersave?
			return 0;			// Prevent From Happening
		}
		break;				
    //Keyboard messages
	case WM_KEYDOWN:
		#ifdef IGNORE_KEY_REPETITION
		if(!g_Key[wParam]){
			g_Key[wParam] = true; 
			OnKeyDown(wParam);
		}
		#else
		g_Key[wParam] = true; 
		OnKeyDown(wParam);
		#endif
		break;

	case WM_KEYUP:
		g_Key[wParam] = false; 
		OnKeyUp(wParam);
		break;

	//Mouse messages
	case WM_MOUSEMOVE:
		OnMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_LBUTTONDOWN:
		g_MouseButtonState[LEFT_MOUSE_BUTTON] = true;
		OnMouseDown(LEFT_MOUSE_BUTTON);

		break;
	case WM_LBUTTONUP:
		g_MouseButtonState[LEFT_MOUSE_BUTTON] = false;
		OnMouseUp(LEFT_MOUSE_BUTTON);
		break;
	case WM_MBUTTONDOWN:
		g_MouseButtonState[MIDDLE_MOUSE_BUTTON] = true;
		OnMouseDown(MIDDLE_MOUSE_BUTTON);
		break;
	case WM_MBUTTONUP:
		g_MouseButtonState[MIDDLE_MOUSE_BUTTON] = false;
		OnMouseUp(MIDDLE_MOUSE_BUTTON);
		break;
	case WM_RBUTTONDOWN:
		g_MouseButtonState[RIGHT_MOUSE_BUTTON] = true;
		OnMouseDown(RIGHT_MOUSE_BUTTON);
		break;
	case WM_RBUTTONUP:
		g_MouseButtonState[RIGHT_MOUSE_BUTTON] = false;
		OnMouseUp(RIGHT_MOUSE_BUTTON);
		break;

	case WM_AI_COMMAND:
		ProcessAICmd(wParam, lParam);
		break;

	default: break;
	}
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//-----------------------------------------------------------------------------
// Name : CreateGameWindow()
// Desc : Create the game window we need
//-----------------------------------------------------------------------------
bool Tetrix3DEngine::CreateGameWindow()
{
	WNDCLASSEX wc;                            // Window class object.
	// This is the Window class.
	wc.cbSize        = sizeof(WNDCLASSEX);           // size of the WNDCLASSEX structure.
	wc.style         = CS_HREDRAW | CS_VREDRAW;       // style of the window.
	wc.lpfnWndProc   = WndProc;                 // Address to the windows procedure.
	wc.cbClsExtra    = 0;                        // Extra class information.
	wc.cbWndExtra    = 0;                        // Extra window information.
	wc.hInstance     = g_hInstance;                 // Handle of application Instance.
	wc.hIcon         = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_TETRIX3D_ICON));// Window Icon.
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW); // Load mouse cursor.
	wc.hbrBackground = /*NULL;*/(HBRUSH)(COLOR_WINDOW + 2);                  // Background color.
	wc.lpszMenuName  = NULL;                   // Menu.
	wc.lpszClassName = CLASS_NAME;            // Name of the window class.
	wc.hIconSm       = NULL;// Minimized window icon.
	
	// You must register you class with Windows.
	if(RegisterClassEx(&wc))
		g_ScreenDat.Registered = true;
	else
		return false;
	
    DWORD dwStyle;
	if(g_ScreenDat.FullScreen){ 						// Check if we wanted full screen mode													// Set the window properties for full screen mode
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		
		//Change the display mode if not the same has the current one
		DEVMODE DisplayMode;
		memset(&DisplayMode, 0, sizeof(DisplayMode));
		if(!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DisplayMode)){return false;}
		DisplayMode.dmPelsWidth	 = g_ScreenDat.Width;
		DisplayMode.dmPelsHeight = g_ScreenDat.Height;
		//DisplayMode.dmBitsPerPel = g_ScreenDat.BitsPerPixel;
		int ChangeDisplayResult = ChangeDisplaySettings(&DisplayMode, CDS_FULLSCREEN);	
		if(ChangeDisplayResult != DISP_CHANGE_SUCCESSFUL)
				MessageBox(NULL,"Error: Failed to change display mode.", APP_NAME, 0);
		
		
	} else 
		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	
	

	RECT rWindow;
	rWindow.left	= g_ScreenDat.Left;								// Set Left Value To 0
	rWindow.right	= g_ScreenDat.Width;							// Set Right Value To Requested Width
	rWindow.top	    = g_ScreenDat.Top;								// Set Top Value To 0
	rWindow.bottom	= g_ScreenDat.Height;							// Set Bottom Value To Requested Height
	
	//
	AdjustWindowRect(&rWindow, dwStyle, false);		// Adjust Window To True Requested Size
	
	
	// Create the window.
	g_hWnd = CreateWindowEx(NULL,                  // The extended window style.
		CLASS_NAME,         // Window class.
		CLASS_CAPTION,      // Window name.
		dwStyle,            // Window style.
		g_ScreenDat.Left, 
		g_ScreenDat.Top,    // X, Y coords.
		g_ScreenDat.Width, 
		g_ScreenDat.Height, // Window size.
		NULL,               // Handle to parent window.
		NULL,               // Menu.
		g_hInstance,        // Handle to app instance.
		NULL);              // Pointer to window.
	
	
	// If there was an error with creating the window, then close the program.
	if(!g_hWnd)
		return false;
	
	ShowWindow(g_hWnd, SW_SHOW); // Show the window.
	UpdateWindow(g_hWnd);        // Update its display.
	
	SetFocus(g_hWnd);			 // Sets Keyboard Focus To The Window	
	
   return true;
}


//-----------------------------------------------------------------------------
// Name : SetupPixelFormatDescriptor()
// Desc : Setup the PIXELFORMATDESCRIPTOR beast
//-----------------------------------------------------------------------------
bool Tetrix3DEngine::SetupPixelFormatDescriptor(HDC hdc) 
{ 
    PIXELFORMATDESCRIPTOR pfd; 
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
    pfd.nVersion = 1;									// Always set this to 1
														// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGB and Alpha pixel type
    pfd.cColorBits = 32;								// Here we use our #define for the color bits
    pfd.cDepthBits = 32;								// Depthbits is ignored for RGBA, but we do it anyway
    pfd.cAccumBits = 0;									// No special bitplanes needed
    pfd.cStencilBits = 0;								// We desire no stencil bits
 
	// This gets us a pixel format that best matches the one passed in from the device
    if((pixelformat = ChoosePixelFormat(hdc, &pfd)) == false){ 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return false; 
    } 
 
	// This sets the pixel format that we extracted from above
    if(SetPixelFormat(hdc, pixelformat, &pfd) == false){ 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return false; 
    } 
 
    return true;   // Return a success!
}


//-----------------------------------------------------------------------------
// Name : InitializeOpenGL()
// Desc : Initialise OpenGL stuff here.
//-----------------------------------------------------------------------------
bool Tetrix3DEngine::InitializeOpenGL() 
{  
    g_hDC = GetDC(g_hWnd);								// This sets our global HDC
														// We don't free this hdc until the end of our program
    if(!SetupPixelFormatDescriptor(g_hDC))						// This sets our pixel format/information
        PostQuitMessage(0);							    // If there's an error, quit

	g_hRC = wglCreateContext(g_hDC);					// This creates a rendering context from our hdc
	wglMakeCurrent(g_hDC, g_hRC);						// This makes the rendering context we just created the one we want to use
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background
	
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	//glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	//glEnable(GL_CULL_FACE);								// Enable back face culling
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	
	// Turn on a lighting and enable it, we will just use the default values in this case
	// We also want color, so we turn that on
	glEnable(GL_LIGHT0);								// Turn on a light with defaults set
	glEnable(GL_LIGHTING);								// Turn on lighting
	glEnable(GL_COLOR_MATERIAL);						// Allow color
	
	ResizeGLScreen(g_ScreenDat.Width, g_ScreenDat.Height);

	//Select the game hrc(the one we use the more)
	wglMakeCurrent(g_hDC, g_hRC);						// This makes the rendering context we just created the one we want to use
	return true;
}


//-----------------------------------------------------------------------------
// Name : ResizeGLScreen(int w, int h)
// Desc : resize the screen or viewport
//-----------------------------------------------------------------------------
void Tetrix3DEngine::ResizeGLScreen(int w, int h)
{
	if (h == 0){h = 1;}   // Prevent A Divide By Zero error
	if (w == 0){w = 1;}   // Prevent A Divide By Zero error
	
	glViewport(0, 0, w, h);// Make our viewport the whole window					
	
	glMatrixMode(GL_PROJECTION);// Select The Projection Matrix
	glLoadIdentity();			// Reset The Projection Matrix
	
	gluPerspective(45.0f, (float)w/(float)h, 0.1f, DEF_GL_DEPTH);
	
	glMatrixMode(GL_MODELVIEW);	// Select The Modelview Matrix
	glLoadIdentity();			// Reset The Modelview Matrix
}


//-----------------------------------------------------------------------------
// Name : EnterMessageLoop()
// Desc : The main game loop witch will process the
//        Windows messages or the game loop	and rendering task.
//-----------------------------------------------------------------------------
void Tetrix3DEngine::EnterMessageLoop()
{
	MSG msg;
	static bool  done = false;
	// Application loop.
	while(!done){
		if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			// If a quit message is received then stop rendering and quit the app.
			if(msg.message == WM_QUIT){
				FModObj.StopUpdateThread();
				done = true;
			}

			TranslateMessage(&msg);  // Translate any messages.
			DispatchMessage(&msg);   // Dispatch any messages.
		} else {
			UpdateScene();           // Control the game flow 
			RenderScene();           // Render a frame.
		}
	}
}


//-----------------------------------------------------------------------------
// Name : InitializeFontSystem()
// Desc : Initialise my game fonts, 
//        set the text, size, name, color and pos of each of 'em
//-----------------------------------------------------------------------------
void Tetrix3DEngine::InitializeFontSystem()
{
	//Calculate the ratio of thw current screen size with the 320x240 mode
	float FontRatio = (float)g_ScreenDat.Width / 320.0f;

	g_GLFonts.FirstFontList = CurListIndx;
	g_GLFonts.GLFontRatio = FontRatio;
	
	g_GLFonts.GLPressStartFont.BuildFont(g_hDC, XBANDFontName, (35*FontRatio), &CurListIndx, (14 * FontRatio), (107 * FontRatio), glYellow3b); 
	g_GLFonts.GLPauseFont.BuildFont(g_hDC, XBANDFontName, (50*FontRatio), &CurListIndx, (57 * FontRatio), (101 * FontRatio), glYellow3b);
	g_GLFonts.GLGameOverFont.BuildFont(g_hDC, XBANDFontName, (35*FontRatio), &CurListIndx, (38 * FontRatio), (107 * FontRatio), glRed3b);
	
	g_GLFonts.GLFPSFont.BuildFont(g_hDC, DigitFontName, (6*FontRatio), &CurListIndx, (15 * FontRatio), (220 * FontRatio), glGreen3b);
	g_GLFonts.GLMessageFont.BuildFont(g_hDC, DigitFontName, (6*FontRatio), &CurListIndx, (150 * FontRatio), (220 * FontRatio), glYellow3b);
	
	g_GLFonts.GLCurrentScore.BuildFont(g_hDC, DigitFontName, (11*FontRatio), &CurListIndx, (252 * FontRatio), (199 * FontRatio), glYellow3b);
	g_GLFonts.GLHiScore.BuildFont(g_hDC, DigitFontName, (11*FontRatio), &CurListIndx, (252 * FontRatio), (154 * FontRatio), glYellow3b);
	
	g_GLFonts.GLIntroFont.BuildFont(g_hDC, DigitFontName, (16*FontRatio), &CurListIndx, 0, 60, glYellow3b);
	g_GLFonts.GLOptionsFont.BuildFont(g_hDC, XBANDFontName, (18*FontRatio), &CurListIndx, 0, 0, glGreen3b);
	g_GLFonts.GLKeyConfigFont.BuildFont(g_hDC, DigitFontName, (10*FontRatio), &CurListIndx, 0, 0, glGreen3b);
	
	g_GLFonts.GLConfirmExitFont.BuildFont(g_hDC, XBANDFontName, (40*FontRatio), &CurListIndx, (14 * FontRatio), (107 * FontRatio), glYellow3b); 

	g_GLFonts.GLAI_MoveDone_Font.BuildFont (g_hDC, "Arial", (4*FontRatio), &CurListIndx, (247 * FontRatio), (51 * FontRatio), glYellow3b); 
	g_GLFonts.GLAI_Score_Font.BuildFont    (g_hDC, "Arial", (4*FontRatio), &CurListIndx, (247 * FontRatio), (46 * FontRatio), glYellow3b); 
	g_GLFonts.GLAI_RotNeeded_Font.BuildFont(g_hDC, "Arial", (4*FontRatio), &CurListIndx, (247 * FontRatio), (41 * FontRatio), glYellow3b); 
	g_GLFonts.GLAI_MovNeeded_Font.BuildFont(g_hDC, "Arial", (4*FontRatio), &CurListIndx, (247 * FontRatio), (36 * FontRatio), glYellow3b); 

	g_GLFonts.LastFontList = CurListIndx;
}


//-----------------------------------------------------------------------------
// Name : InitializeSoundSystem()
// Desc : Initialise my game sounds and tunes
//-----------------------------------------------------------------------------
bool Tetrix3DEngine::InitializeSoundSystem()
{
	//Init FMod
	if(!FModObj.InitFMod())
		return false;

	//Load midi files
	if(!FModObj.LoadTunes())
		return false;

	//Loas wav files
	if(!FModObj.LoadSounds())
		return false;

	//return success to caller if all's ok
	return true;
}


//-----------------------------------------------------------------------------
// Name : InitializeGameEngine(HINSTANCE *hInstance, CGameWindowDat *ScreenDat)
// Desc : Initialise the engine step by step
//-----------------------------------------------------------------------------
bool Tetrix3DEngine::InitializeGameEngine(HINSTANCE *hInstance, CGameWindowDat *ScreenDat)
{
	g_hInstance = *hInstance;
	//hold screen info for CreateGameWindow
	g_ScreenDat = *ScreenDat;

	//Hide the cursor if we are in fullscreen
	ShowCursor(!g_ScreenDat.FullScreen);

	//Create the game window
	if(!CreateGameWindow()){
		MessageBox(NULL,"Error: Failed to create game window.", APP_NAME, 0);
		return false;
	}


	//Init. OpenGL
	if(!InitializeOpenGL()){
		MessageBox(NULL,"Error: Failed to Initialise OpenGL.", APP_NAME, 0);
		return false;
	}
	
	// Load the font
	if(!LoadFont()){
		MessageBox(NULL,"Error: Failed to load font.", APP_NAME, 0);
		return false;
	}

	// Init the font
	InitializeFontSystem();

	// Load the sounds		
	if(!InitializeSoundSystem()){
		MessageBox(NULL,"Error: Failed to Initialise sound system.", APP_NAME, 0);
		return false;
	}

	// Load textures
	if(!InitializeTextureSystem()){
		MessageBox(NULL,"Error: Failed to Initialise texture system.", APP_NAME, 0);
		return false;
	}

	// Load models
	if(!InitializeModelsArrays()){
		MessageBox(NULL,"Error: Failed to load 3ds model.", APP_NAME, 0);
		return false;
	}

	ResizeGameParam(true);
	
	RESET_INTRO;
	g_GameState = GAME_INTRO;
	
	// Initialise the first tick of the timer(not very necessary, but...) 
	g_Timer.Tick();

	if(g_MusicEnabled)
		FModObj.StartUpdateThread();

	// Fade in when we start
	Fader->StartFader(g_ScreenDat.Width, g_ScreenDat.Height, 0, 0, 0, DEF_FADE_TIME_LENGTH, 0);

	return true;
}


//-----------------------------------------------------------------------------
// Name : ResizeGameParam(bool IncGridPtr)
// Desc : Intialise all needed variables there for first time.
//-----------------------------------------------------------------------------
void Tetrix3DEngine::ResizeGameParam(bool IncGridPtr)
{
	// Init the Space value(size of the game boundarie, stuff like that...)
	Init_GameSpaceStuff();
	// This will change the size of the dynamics arrays
	ReallocDynamicsArrays();
	// Init some arrays
	OthersSpaceMemInit();
	// Set the Initial Z Camera Position Correctly(depending on the size of the game)
	StartingZCamPos = (X_RowLength >= Y_RowLength) ? INIT_CAMERA_MACRO(X_RowLength) : INIT_CAMERA_MACRO(Y_RowLength);
	// 
	CCAMDAT_MACRO(g_Camera, 0.0f, 0.0f, StartingZCamPos, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	// Delete the old data
	if(!IncGridPtr){glDeleteLists(g_Grid.ListIndex, 1);}
	// Init the Grid
	g_Grid.Initialise(&CurListIndx, &X_RowLength, &Y_RowLength, &Z_RowLength, &PreCalc.Z_RowLengthDiv2, &PreCalc.MinXGridVal, &PreCalc.MaxXGridVal, &PreCalc.MinYGridVal, &PreCalc.MaxYGridVal, &PreCalc.MinZGridVal, &PreCalc.MaxZGridVal, IncGridPtr);	
	//
	//g_Shape.Initialise(Z_RowLength);
	g_Shape.LoadPcx(BLOCK_FNAME, Z_RowLength);
	// Make a reset...
	ResetGame();

	// For debuging only, it will fill all the first row of block except for the first one...
	#ifdef FILL_FIRST_ROW_EXECPT_ONE
	for(int TmpCpt = 1; TmpCpt < PreCalc.LinesSpaceSize; TmpCpt++)
		pSpaceMem[TmpCpt] = true;
	#endif
}



//-----------------------------------------------------------------------------
// Name : ReallocAllSpaceMem()
// Desc : Intialise all needed variables there for first time.
//-----------------------------------------------------------------------------
void Tetrix3DEngine::ReallocDynamicsArrays()
{
	//Delete old arrays
	//Delete dynamic array
	SAFE_DELETE_ARRAY(pLinesToRemove);
	SAFE_DELETE_ARRAY(pTmpSpaceMem);
	SAFE_DELETE_ARRAY(pSpaceMem);
	SAFE_DELETE_ARRAY(pSpaceMemX);
	SAFE_DELETE_ARRAY(pSpaceMemY);
	SAFE_DELETE_ARRAY(pSpaceMemZ);
	SAFE_DELETE_ARRAY(pSpaceMemColor);
	SAFE_DELETE_ARRAY(pZ_Score);
	//SAFE_DELETE_ARRAY(pLineDonePerc);
	//Create the arrays
	pSpaceMem      = new bool  [PreCalc.GameSpaceSize];
	pSpaceMemX     = new float [PreCalc.GameSpaceSize];
	pSpaceMemY     = new float [PreCalc.GameSpaceSize];
	pSpaceMemZ     = new float [PreCalc.GameSpaceSize];
	pSpaceMemColor = new int   [PreCalc.GameSpaceSize];
	pTmpSpaceMem   = new bool  [PreCalc.GameSpaceSize];
	pLinesToRemove = new bool  [Z_RowLength];
	pZ_Score       = new int   [Z_RowLength];
	//pLineDonePerc  = new float [Z_RowLength];
}

//-----------------------------------------------------------------------------
// Name : InitializeTextureSystem()
// Desc : Intialise all needed variables there for first time.
//-----------------------------------------------------------------------------
bool Tetrix3DEngine::InitializeTextureSystem()
{
	//Load the cube	textures
	Textures.LoadTexture(CUBE_TEX_NAME, &TexID.g_CubeTexture, false, false);
	//Load the 3D font texture
	Textures.LoadTexture(INTRO_FONT_TEX_FNAME, &TexID.g_3DIntroTextTexture, false, false);
	#ifndef NO_BACKGROUND
	//Load the backgroud texture
	Textures.LoadTexture(INTRO_BCK_TEX_FNAME, &TexID.g_IntroBckTexture, true, false);	
	Textures.LoadTexture(GAME_BCK_TEX_FNAME, &TexID.g_GameBckTexture, true, false);
	Textures.LoadTexture(OPTIONS_BCK_TEX_FNAME, &TexID.g_OptionsBckTexture, false, false);	
	Textures.LoadTexture(KEYCONFIG_BCK_TEX_FNAME, &TexID.g_KeyConfigBckTexture, false, false);	
	#endif
	return true;
}


//-----------------------------------------------------------------------------
// Name : InitializeModelsArrays()
// Desc : Intialise all needed variables there for first time.
//-----------------------------------------------------------------------------
bool Tetrix3DEngine::InitializeModelsArrays()
{
	//Load the cube model
	char c[256];
	for(int Cpt = 0; Cpt <= 2; Cpt++){
		sprintf(c, Cube3dsFName, Cpt+1);
		BlockModel[Cpt].LoadModelFromFile(c);
	}

	//Load the "3D" model
	The3DTextModel.LoadModelFromFile(GL3DTextFName);
	
	// This will build our VBO(if possible)
	BuildVBO();

	return true;
}

//-----------------------------------------------------------------------------
// Name : Init_GameOver_Pattern()
// Desc : Set up an array that will be used to move the 
//        camera a little to give an impression of shaking.
//-----------------------------------------------------------------------------
void Tetrix3DEngine::Init_GameOver_Pattern()
{
	ShakePts.ShakingPoint[0].X = -0.1f;
	ShakePts.ShakingPoint[0].Y = -0.1f;
	ShakePts.ShakingPoint[1].X = 0.1f;
	ShakePts.ShakingPoint[1].Y = -0.1f;
	ShakePts.ShakingPoint[2].X = -0.1f;
	ShakePts.ShakingPoint[2].Y = 0.1f;
	ShakePts.ShakingPoint[3].X = 0.1f;
	ShakePts.ShakingPoint[3].Y = 0.1f;
}


//-----------------------------------------------------------------------------
// Name : RegistryReaderWriter(int Reg_Mode, int *Width, int *Height, /*int *BitsPerPixel, */bool *FullScreen , int *CubeDetail, bool *ShowTexture, bool *SoundSwitch, int *X_RowLength, int *Y_RowLength, int *Z_RowLength, UCHAR *GameKey, int *HiScore)
// Desc : Read and write to the regristry 
//        some initialisation value.
//-----------------------------------------------------------------------------
void Tetrix3DEngine::RegistryReaderWriter(int Reg_Mode, int *Width, int *Height, /*int *BitsPerPixel, */bool *FullScreen , int *CubeDetail, bool *ShowTexture, bool *SoundSwitch, bool *MusicSwitch, int *X_RowLength, int *Y_RowLength, int *Z_RowLength, UCHAR *GameKey, int *HiScore)
{
	CRegisterWIN32 *Reg;             
	//Help to shorten code size...
	static const LPSTR OptionsStr[] = {"Width", "Height", /*"BitsPerPixel", */"Full Screen", "Detail", "Show Texture", "SoundSwitch", "MusicSwitch", "Horizontal Grid Size", "Vertical Grid Size", "Depht Grid Size", "Hi Score"};
	static const LPSTR KeyBoardStr[] = {"Left", "Right", "Up", "Down", "Fall", "RotX1", "RotX2", "RotY1", "RotY2", "RotZ1", "RotZ2", "Pause", "Quit", "Sound", "Music", "Detail", "Show Texture", "FPS", "Invert Mouse", "Show Ghost"};
	static const int DEF_KEYBOARD_VALUE[] = {37, 39, 38, 40, 32, 97, 100, 98, 101, 99, 102, 80, 81, 83, 77, 68, 84, 70, 73, 71};

	int Cpt = -1;
	Reg = new CRegisterWIN32;
	Reg->Open(HKEY_CURRENT_USER, DEF_OPTIONS_KEY, false);
	if(Reg_Mode == READTOREG){

		*Width = Reg->ReadInt(OptionsStr[++Cpt], 800);
		if(*Width != 320 && *Width != 640 && *Width != 800){*Width = 800;Reg->WriteInt(OptionsStr[Cpt], *Width);}

		*Height = Reg->ReadInt(OptionsStr[++Cpt], 600);
		if(*Height != 240 && *Height != 480 && *Height != 600){*Height = 600;Reg->WriteInt(OptionsStr[Cpt], *Height);}
		
		//*BitsPerPixel = Reg->ReadInt(OptionsStr[++Cpt],32);
		//if(*BitsPerPixel != 16 && *BitsPerPixel != 24 && *BitsPerPixel != 32){*BitsPerPixel = 32;Reg->WriteInt(OptionsStr[Cpt],*BitsPerPixel);}

		*FullScreen = IntToBool(Reg->ReadInt(OptionsStr[++Cpt], 1));
		if(*FullScreen != false && *FullScreen != true){*FullScreen = true;Reg->WriteInt(OptionsStr[Cpt], *FullScreen);}
		
		*CubeDetail = Reg->ReadInt(OptionsStr[++Cpt], Hi_Res);
		if(*CubeDetail != Lo_Res && *CubeDetail != Med_Res && *CubeDetail != Hi_Res){*CubeDetail = Hi_Res;Reg->WriteInt(OptionsStr[Cpt], *CubeDetail);}
		
		*ShowTexture = IntToBool(Reg->ReadInt(OptionsStr[++Cpt], 1));
		if(*ShowTexture != 0 && *ShowTexture != 1){*ShowTexture = 1;Reg->WriteInt(OptionsStr[Cpt], *ShowTexture);}
		
		*SoundSwitch = IntToBool(Reg->ReadInt(OptionsStr[++Cpt], 1));
		if(*SoundSwitch != 0 && *SoundSwitch != 1){*SoundSwitch = true;Reg->WriteInt(OptionsStr[Cpt], BoolToInt(*SoundSwitch));}
		
		*MusicSwitch = IntToBool(Reg->ReadInt(OptionsStr[++Cpt], 1));
		if(*MusicSwitch != 0 && *MusicSwitch != 1){*MusicSwitch = true;Reg->WriteInt(OptionsStr[Cpt], BoolToInt(*MusicSwitch));}
		
		*X_RowLength = Reg->ReadInt(OptionsStr[++Cpt], 8);
		if(*X_RowLength < XMinRowLengthValue || *X_RowLength > XMaxRowLengthValue){*X_RowLength = 8;Reg->WriteInt(OptionsStr[Cpt], *X_RowLength);}
		*Y_RowLength = Reg->ReadInt(OptionsStr[++Cpt], 8);
		if(*Y_RowLength < YMinRowLengthValue || *Y_RowLength > YMaxRowLengthValue){*Y_RowLength = 8;Reg->WriteInt(OptionsStr[Cpt], *Y_RowLength);}
		*Z_RowLength = Reg->ReadInt(OptionsStr[++Cpt], 10);
		if(*Z_RowLength < ZMinRowLengthValue || *Z_RowLength > ZMaxRowLengthValue){*Z_RowLength = 10;Reg->WriteInt(OptionsStr[Cpt], *Z_RowLength);}

		*HiScore = Reg->ReadInt(OptionsStr[++Cpt], 0);
		if(*HiScore < 0){*HiScore = 0;Reg->WriteInt(OptionsStr[Cpt], *HiScore);}

		Reg->Close();
		Reg->Open(HKEY_CURRENT_USER, DEF_KEYBOARD_KEY, false);

		for(Cpt = 0; Cpt < KEY_COUNT; Cpt++)
		{
			GameKey[Cpt] = Reg->ReadInt(KeyBoardStr[Cpt], DEF_KEYBOARD_VALUE[Cpt]);
			if(!ValidTetrix3DKey(GameKey[Cpt])){GameKey[Cpt] = DEF_KEYBOARD_VALUE[Cpt]; Reg->WriteInt(KeyBoardStr[Cpt], GameKey[Cpt]);}
		}
	} else {
		Reg->WriteInt(OptionsStr[++Cpt], *Width);
		Reg->WriteInt(OptionsStr[++Cpt], *Height);
		//Reg->WriteInt(OptionsStr[++Cpt], *BitsPerPixel);
		Reg->WriteInt(OptionsStr[++Cpt], BoolToInt(*FullScreen));
		Reg->WriteInt(OptionsStr[++Cpt], *CubeDetail);
		Reg->WriteInt(OptionsStr[++Cpt], BoolToInt(*ShowTexture));
		Reg->WriteInt(OptionsStr[++Cpt], BoolToInt(*SoundSwitch));
		Reg->WriteInt(OptionsStr[++Cpt], BoolToInt(*MusicSwitch));
		Reg->WriteInt(OptionsStr[++Cpt], *X_RowLength);
		Reg->WriteInt(OptionsStr[++Cpt], *Y_RowLength);
		Reg->WriteInt(OptionsStr[++Cpt], *Z_RowLength);
		Reg->WriteInt(OptionsStr[++Cpt], *HiScore);
		Reg->Close();

		Reg->Open(HKEY_CURRENT_USER, DEF_KEYBOARD_KEY, false);
		for(Cpt = 0; Cpt < KEY_COUNT; Cpt++)
			Reg->WriteInt(KeyBoardStr[Cpt],GameKey[Cpt]);
	}
	Reg->Close();
	SAFE_DELETE_OBJECT(Reg);
}

//-----------------------------------------------------------------------------
// Name : ValidTetrix3DKey(int Num)
// Desc : Used to check if we can use the given keyboard key.
//-----------------------------------------------------------------------------
bool Tetrix3DEngine::ValidTetrix3DKey(int Num)
{
	switch(Num)
	{
	VALID_TETRIX3D_KEY: return true;  break;
	default:            return false; break;
	}
}


//-----------------------------------------------------------------------------
// Name : Init_GameSpaceStuff()
// Desc : That will create some variable(much used as const)
//        used to shortened and speed this code. 
//-----------------------------------------------------------------------------
void Tetrix3DEngine::Init_GameSpaceStuff()
{
	PreCalc.MinXGridVal = ((float)(0-X_RowLength)/2);
	PreCalc.MaxXGridVal = ((float)(0+X_RowLength)/2);
	PreCalc.MinYGridVal = ((float)(0-Y_RowLength)/2);
	PreCalc.MaxYGridVal = ((float)(0+Y_RowLength)/2);
	PreCalc.MinZGridVal = 0.0f;
	PreCalc.MaxZGridVal = (float)Z_RowLength;
	PreCalc.LinesSpaceSize = (X_RowLength*Y_RowLength);
	PreCalc.GameSpaceSize = (PreCalc.LinesSpaceSize*Z_RowLength);
	PreCalc.X_RowLengthMinus1 = X_RowLength-1;
	PreCalc.Y_RowLengthMinus1 = Y_RowLength-1;
	PreCalc.Z_RowLengthMinus1 = Z_RowLength-1;
	PreCalc.X_RowLengthDiv2 = ((float)(X_RowLength)/2);
	PreCalc.Y_RowLengthDiv2 = ((float)(Y_RowLength)/2);
	PreCalc.Z_RowLengthDiv2 = ((float)(Z_RowLength)/2);
	PreCalc.X_RowLengthPlus1Div2 = ((float)(X_RowLength+1)/2);
	PreCalc.Y_RowLengthPlus1Div2 = ((float)(Y_RowLength+1)/2);
	PreCalc.Z_RowLengthPlus1Div2 = ((float)(Z_RowLength+1)/2);
	PreCalc.X_RowLengthMinus1Div2 = ((float)(X_RowLength-1)/2);
	PreCalc.Y_RowLengthMinus1Div2 = ((float)(Y_RowLength-1)/2);
	PreCalc.Z_RowLengthMinus1Div2 = ((float)(Z_RowLength-1)/2);
    float PreviewViewportSizeRatio = g_ScreenDat.Width / 640.0f;
	PreCalc.PreviewRect.left = (502*PreviewViewportSizeRatio);
	PreCalc.PreviewRect.top = ((480-348)*PreviewViewportSizeRatio);
	PreCalc.PreviewRect.right = (106*PreviewViewportSizeRatio);
	PreCalc.PreviewRect.bottom = (106*PreviewViewportSizeRatio);
}

//-----------------------------------------------------------------------------
// Name : OthersSpaceMemInit()
// Desc : Fill SpaceMemInit with false(or 0). 
//-----------------------------------------------------------------------------
void Tetrix3DEngine::OthersSpaceMemInit()
{
	int CptX, CptY, CptZ;
	for(CptZ = 0; CptZ < Z_RowLength; CptZ++){
		for(CptY = 0; CptY < Y_RowLength; CptY++){
			for(CptX = 0; CptX < X_RowLength; CptX++){
				pSpaceMemColor[XYZ_TO_I(CptX, CptY, CptZ)] = (CptZ % MAX_COLOR_MINUS_1)+1;
				pSpaceMemX[XYZ_TO_I(CptX, CptY, CptZ)] = CptX -  PreCalc.X_RowLengthMinus1Div2;
				pSpaceMemY[XYZ_TO_I(CptX, CptY, CptZ)] = (0-(CptY -  PreCalc.Y_RowLengthMinus1Div2));
				pSpaceMemZ[XYZ_TO_I(CptX, CptY, CptZ)] = CptZ + 0.5f;
			}
		}
	}
	// Generate a score for each z position
	GenZScore();
}

//-----------------------------------------------------------------------------
// Name : SpaceMemInit()
// Desc : Fill SpaceMemInit with false(or 0). 
//-----------------------------------------------------------------------------
void Tetrix3DEngine::SpaceMemInit()
{
	//Fill SpaceMem with 0(false)
	ZeroMemory(pSpaceMem, PreCalc.GameSpaceSize);
}

//-----------------------------------------------------------------------------
// Name : GenZScore()
// Desc : Fill an array that represent the pts 
//        for the AI depending on the Z position
//-----------------------------------------------------------------------------
void Tetrix3DEngine::GenZScore()
{	
	for(int Cpt = 0; Cpt < Z_RowLength; Cpt++)
		pZ_Score[Cpt] = (Z_RowLength - Cpt) * AI_FLOOR_STEP_PTS;
}

//-----------------------------------------------------------------------------
// Name : InitTimerSpeed()
// Desc : Initialise the array that will hold all
//        the different timer speed value we can have. 
//-----------------------------------------------------------------------------
void Tetrix3DEngine::InitTimerSpeed()
{	//init value for the TimerSpeed array(that will control the speed of the game)
	TimerSpeed[0] = INREDRAW_TIMER_SPEED;
	for(int Cpt = 1; Cpt <= 10; Cpt++)
		TimerSpeed[Cpt]	= MAX_TIMER_SPEED-(((MAX_TIMER_SPEED-MIN_TIMER_SPEED)/10)*(Cpt-1));
}

//-----------------------------------------------------------------------------
// Name : GameCleanup()
// Desc : This will cleanup all the stuff created with new
//        in InitialiseGame()
//-----------------------------------------------------------------------------
void Tetrix3DEngine::GameCleanup()
{
	CleanUpSoundSystem();

	//Delete dynamic array
	SAFE_DELETE_ARRAY(pLinesToRemove);
	SAFE_DELETE_ARRAY(pTmpSpaceMem);
	SAFE_DELETE_ARRAY(pSpaceMem);
	SAFE_DELETE_ARRAY(pSpaceMemX);
	SAFE_DELETE_ARRAY(pSpaceMemY);
	SAFE_DELETE_ARRAY(pSpaceMemZ);
	SAFE_DELETE_ARRAY(pSpaceMemColor);
	SAFE_DELETE_ARRAY(pZ_Score);
}

//-----------------------------------------------------------------------------
// Name : CleanUpOpenGL()
// Desc : Clear all the Opengl stuff
//-----------------------------------------------------------------------------
void Tetrix3DEngine::CleanUpOpenGL()
{
	// Delete the grid list
	glDeleteLists(g_Grid.ListIndex, 1);

	//Destroy font 			
	glDeleteLists(g_GLFonts.FirstFontList, (g_GLFonts.LastFontList-g_GLFonts.FirstFontList));// Delete All 96 Characters

	if(g_hRC){											
		wglMakeCurrent(NULL, NULL);						// This frees our rendering memory and sets everything back to normal
		wglDeleteContext(g_hRC);						// Delete our OpenGL Rendering Context	
	}

	if(g_hDC) 
		ReleaseDC(g_hWnd, g_hDC);						// Release our HDC from memory

	if(g_ScreenDat.FullScreen)
		ChangeDisplaySettings(NULL,0);	
}


//-----------------------------------------------------------------------------
// Name : CleanUpSoundSystem()
// Desc : Clear the sound system
//-----------------------------------------------------------------------------
void Tetrix3DEngine::CleanUpSoundSystem()
{
	FModObj.StopUpdateThread();
	FModObj.FreeTunes();
	FModObj.FreeSounds();
	FModObj.ShutDownFMod();
}


//-----------------------------------------------------------------------------
// Name : DestroyGameWindow()
// Desc : Unregister the game window class
//-----------------------------------------------------------------------------
void Tetrix3DEngine::DestroyGameWindow()
{
    // Here we unregister the window class with the OS.
	if(g_ScreenDat.Registered)
		UnregisterClass(CLASS_NAME, g_hInstance);
}


//-----------------------------------------------------------------------------
// Name : SetNewShapeData(int Indx)
// Desc : Do all we need to init. a new pcx
//-----------------------------------------------------------------------------
void Tetrix3DEngine::SetNewShapeData(int Indx)
{
#ifndef USE_ALWAYS_THIS_BLOCK
	// This will happen the first time of a new game...
	if(pNextShape->ShapeIndx == 0)
		*pNextShape = g_Shape.VectShapeStruct[RandomShape];

	// This copy the original data into the pointer data
	*pCurrentShape = g_Shape.VectShapeStruct[pNextShape->ShapeIndx];
	*pNextShape = g_Shape.VectShapeStruct[Indx];
	
#else
	// This will happen the first time of a new game...
	if(pNextShape->ShapeIndx == 0)
		*pNextShape = g_Shape.VectShapeStruct[USE_ALWAYS_THIS_BLOCK];

	// This copy the original data into the pointer data
	*pCurrentShape = g_Shape.VectShapeStruct[USE_ALWAYS_THIS_BLOCK];
	*pNextShape = g_Shape.VectShapeStruct[USE_ALWAYS_THIS_BLOCK];

#endif
	//Update the ghost postion
	UpdateGhost();

	//
	if(Is_AI_Playing)
		MakeAIPlay();
}	   


//-----------------------------------------------------------------------------
// Name : ResetGame()
// Desc : This is all the code we need to reset the game.
//-----------------------------------------------------------------------------
void Tetrix3DEngine::ResetGame()
{	//Reset all data that need to 
	g_InRedraw = false;			    
	g_GameState = GAME_NOT_STARTED;
	pCurrentShape->ShapeIndx = 0;
	pNextShape->ShapeIndx = 0;
	SpaceMemInit();
	g_CurrentTimerSpeed = 1;
	g_Lines = 0;
	g_RenderIntervalRatio = ((float)TimerSpeed[g_CurrentTimerSpeed] / 1000.0f);
	ResetCamPos();
}


//-----------------------------------------------------------------------------
// Name : RotateCoord(CThreeFltpoint CoordVal, CThreeFltpoint CenterVal, int Dir)
// Desc : Rotate the given CThreeFltpoint and return it
//-----------------------------------------------------------------------------
CThreeFltpoint Tetrix3DEngine::RotateCoord(CThreeFltpoint CoordVal, CThreeFltpoint CenterVal, int Dir)
{	
	//Convert center format
	CenterVal.X -= PreCalc.X_RowLengthPlus1Div2;
	CenterVal.Y -= PreCalc.Y_RowLengthPlus1Div2;
	CenterVal.Z -= PreCalc.Z_RowLengthPlus1Div2;
	//Convert coord format
	CoordVal.X -= PreCalc.X_RowLengthPlus1Div2;
	CoordVal.Y -= PreCalc.Y_RowLengthPlus1Div2;
	CoordVal.Z -= PreCalc.Z_RowLengthPlus1Div2;
	//Move the rotation pts TO 0,0,0
	CoordVal.X -= CenterVal.X;
	CoordVal.Y -= CenterVal.Y;
	CoordVal.Z -= CenterVal.Z;

	//Rotate...
	static float Tmp;
	switch(Dir)
	{
	case X_ClkWse: 
		Tmp = CoordVal.Y;
		CoordVal.Y = 0.0f-CoordVal.Z;
		CoordVal.Z = Tmp;
		break;
	case X_CntClkWse: 
		Tmp = CoordVal.Y;
		CoordVal.Y = CoordVal.Z;
		CoordVal.Z = 0.0f-Tmp;
		break;
	case Y_ClkWse: 
		Tmp = CoordVal.X;
		CoordVal.X = CoordVal.Z;
		CoordVal.Z = 0.0f-Tmp;
		break;
	case Y_CntClkWse: 
		Tmp = CoordVal.X;
		CoordVal.X = 0.0f-CoordVal.Z;
		CoordVal.Z = Tmp;
		break;
	case Z_ClkWse: 
		Tmp = CoordVal.X;
		CoordVal.X = 0.0f-CoordVal.Y;
		CoordVal.Y = Tmp;
		break;
	case Z_CntClkWse: 
		Tmp = CoordVal.X;
		CoordVal.X = CoordVal.Y;
		CoordVal.Y = 0.0f-Tmp;
		break;
	}

	//Move the rotation pts to origin(not to center)
	CoordVal.X += CenterVal.X;
	CoordVal.Y += CenterVal.Y;
	CoordVal.Z += CenterVal.Z;
	//Re-convert coord format
	CoordVal.X += PreCalc.X_RowLengthPlus1Div2;
	CoordVal.Y += PreCalc.Y_RowLengthPlus1Div2;
	CoordVal.Z += PreCalc.Z_RowLengthPlus1Div2;
	//Return values
	return CoordVal;
}
 

//-----------------------------------------------------------------------------
// Name : CanMoveGhost(CShapeStruct *pGhostShape)
// Desc : Return TRUE if the pcx can move
//-----------------------------------------------------------------------------
bool Tetrix3DEngine::CanMoveGhost(CShapeStruct *pGhostShape)
{
	static int Cpt, SpaceMemIndx;

	/*for(Cpt = 0; Cpt < pCurrentShape->ShapeVect.size(); Cpt++){
		SpaceMemIndx = XYZ_TO_I((int)pGhostShape->ShapeVect[Cpt].X, (int)pGhostShape->ShapeVect[Cpt].Y, (int)pGhostShape->ShapeVect[Cpt].Z);
		if(MovDir != Dir_Z_Down && pSpaceMem[SpaceMemIndx] == true)
			return false;
	}*/

	//check all the block of the piece
	for(Cpt = 0; Cpt < pGhostShape->ShapeVect.size(); Cpt++){

		//return false if we are trying to move outside the valid range of the game
		if(!ValidTetrixCoord((int)pGhostShape->ShapeVect[Cpt].X, (int)pGhostShape->ShapeVect[Cpt].Y, (int)pGhostShape->ShapeVect[Cpt].Z-1, false))
			return false;
		//skip the next condition if the next pos. of block have a higher Z pos than the game(could not be affected by anything since we have check it si not on the boundarie of the game)
		SpaceMemIndx = XYZ_TO_I((int)pGhostShape->ShapeVect[Cpt].X, (int)pGhostShape->ShapeVect[Cpt].Y, (int)pGhostShape->ShapeVect[Cpt].Z-1);
		if(SpaceMemIndx >= PreCalc.GameSpaceSize)
			continue; 
		else if(pSpaceMem[SpaceMemIndx] == true)
			return false;
	}
	return true;
}


//-----------------------------------------------------------------------------
// Name : MoveGhost(CShapeStruct *pGhostShape)
// Desc : Move the ghost shape
//-----------------------------------------------------------------------------
void Tetrix3DEngine::MoveGhost(CShapeStruct *pGhostShape)
{
	//move the block one by one...
	for(int Cpt = 0; Cpt < pGhostShape->ShapeVect.size(); Cpt++)
		pGhostShape->ShapeVect[Cpt].Z--;
	//Move the center of the block(if we dont want to rotate around the same point after moving!)
	pGhostShape->ShapeCenter.Z--;
}


//-----------------------------------------------------------------------------
// Name : CanMove(int MovDir)
// Desc : Return TRUE if the pcx can move
//-----------------------------------------------------------------------------
bool Tetrix3DEngine::CanMove(int MovDir)
{	
	static int Cpt, MoveX, MoveY, MoveZ, SpaceMemIndx;

	/*for(Cpt = 0; Cpt < pCurrentShape->ShapeVect.size(); Cpt++){
		SpaceMemIndx = XYZ_TO_I((int)pCurrentShape->ShapeVect[Cpt].X, (int)pCurrentShape->ShapeVect[Cpt].Y, (int)pCurrentShape->ShapeVect[Cpt].Z);
		if(MovDir != Dir_Z_Down && pSpaceMem[SpaceMemIndx] == true)
			return false;
	}*/

	//set moveX,Y,Z to tell where to check
	switch(MovDir)
	{
	case Dir_Up    : MoveX = 0;  MoveY = -1; MoveZ = 0;  break;
	case Dir_Down  : MoveX = 0;  MoveY = 1;  MoveZ = 0;  break;
	case Dir_Right : MoveX = 1;  MoveY = 0;  MoveZ = 0;  break;
	case Dir_Left  : MoveX = -1; MoveY = 0;  MoveZ = 0;  break;
	case Dir_Z_Down: MoveX = 0;  MoveY = 0;  MoveZ = -1; break;
	}
	//check all the block of the piece
	for(Cpt = 0; Cpt < pCurrentShape->ShapeVect.size(); Cpt++){

		//if the block is in the valid range
		if(ValidTetrixCoord((int)pCurrentShape->ShapeVect[Cpt].X, (int)pCurrentShape->ShapeVect[Cpt].Y, (int)pCurrentShape->ShapeVect[Cpt].Z, true)){
			//if a block is already where is this block...
			SpaceMemIndx = XYZ_TO_I((int)pCurrentShape->ShapeVect[Cpt].X, (int)pCurrentShape->ShapeVect[Cpt].Y, (int)pCurrentShape->ShapeVect[Cpt].Z);
			//if the player's game over!!!...
			if(MovDir == Dir_Z_Down && pSpaceMem[SpaceMemIndx] == true){
				DoGameOver();
				//Return value
				return false;  
			}
		}
		//return false if we are trying to move outside the valid range of the game
		if(!ValidTetrixCoord((int)pCurrentShape->ShapeVect[Cpt].X+MoveX, (int)pCurrentShape->ShapeVect[Cpt].Y+MoveY, (int)pCurrentShape->ShapeVect[Cpt].Z+MoveZ, false))
			return false;
		//finally, return false if another block is below the one we are checking
		SpaceMemIndx = XYZ_TO_I((int)pCurrentShape->ShapeVect[Cpt].X+MoveX, (int)pCurrentShape->ShapeVect[Cpt].Y+MoveY, (int)pCurrentShape->ShapeVect[Cpt].Z+MoveZ);
		//skip the next condition if the next pos. of block have a higher Z pos than the game(could not be affected by anything since we have check it si not on the boundarie of the game)
		if(SpaceMemIndx >= PreCalc.GameSpaceSize)
			continue; 
		else if(pSpaceMem[SpaceMemIndx] == true)
			return false;
	}
	return true;
}	   


//-----------------------------------------------------------------------------
// Name : Move(int MovDir)
// Desc : Move the current shape
//-----------------------------------------------------------------------------
void Tetrix3DEngine::Move(int MovDir)
{
	static int MoveX,MoveY,MoveZ;
	//move the block one by one...
	for(int Cpt = 0; Cpt < pCurrentShape->ShapeVect.size(); Cpt++)
	{
		//Check witch dir to move
		switch(MovDir)
		{
		case Dir_Up: MoveX = 0; MoveY = -1; MoveZ = 0; break;
		case Dir_Down: MoveX = 0; MoveY = 1; MoveZ = 0; break;
		case Dir_Right: MoveX = 1; MoveY = 0; MoveZ = 0; break;
		case Dir_Left: MoveX = -1; MoveY = 0; MoveZ = 0; break;
		case Dir_Z_Down: MoveX = 0; MoveY = 0; MoveZ = -1; break;
		}
		//Move the block
		pCurrentShape->ShapeVect[Cpt].X += MoveX;
		pCurrentShape->ShapeVect[Cpt].Y += MoveY;
		pCurrentShape->ShapeVect[Cpt].Z += MoveZ;
	}
	//Move the center of the block(if we dont want to rotate around the same point after moving!)
	pCurrentShape->ShapeCenter.X += MoveX;
	pCurrentShape->ShapeCenter.Y += MoveY;
	pCurrentShape->ShapeCenter.Z += MoveZ;
	//Update the ghost postion
	UpdateGhost();
}


//-----------------------------------------------------------------------------
// Name : CanRotate(int RotDir)
// Desc : Return TRUE if the pcx can rotate
//-----------------------------------------------------------------------------
bool Tetrix3DEngine::CanRotate(int RotDir)
{
	static CThreeFltpoint XYZ;
	static int Cpt;
	//check all the block of the piece
	for(Cpt = 0; Cpt < pCurrentShape->ShapeVect.size(); Cpt++)
	{
		//find what will be the coordonate of the current block if we rotate it around his center
		XYZ = RotateCoord(pCurrentShape->ShapeVect[Cpt], pCurrentShape->ShapeCenter, RotDir);
		//Check if the pos of the block is valid or it will retrun false
		if(!ValidTetrixCoord(XYZ.X, XYZ.Y, XYZ.Z, false))
			return false;
		//Continue if the Z pos of the block is greater than the game
		if((int)XYZ.Z >= Z_RowLength)
			continue;
		//Return false if another block is already there
		if(pSpaceMem[XYZ_TO_I((int)XYZ.X, (int)XYZ.Y, (int)XYZ.Z)] == true)
			return false;
	}
	//if all the block pass the test, retrun true
	return true;
}	   

//-----------------------------------------------------------------------------
// Name : Rotate(int RotDir)
// Desc : Rotate the current shape
//-----------------------------------------------------------------------------
void Tetrix3DEngine::Rotate(int RotDir)
{	//rotate all the block(...we don't need to rotate the rotation point!!!)
	static int Cpt;
	for(Cpt = 0; Cpt < pCurrentShape->ShapeVect.size(); Cpt++)
		pCurrentShape->ShapeVect[Cpt] = RotateCoord(pCurrentShape->ShapeVect[Cpt], pCurrentShape->ShapeCenter, RotDir);
	//Update the ghost postion
	UpdateGhost();
}


//-----------------------------------------------------------------------------
// Name : ValidTetrixCoord(int XCrd,int YCrd,int ZCrd, bool CheckMaxZ)
// Desc : Tell if the given x,y,z coord are valid(if we can move a block here...)
//-----------------------------------------------------------------------------
bool Tetrix3DEngine::ValidTetrixCoord(int XCrd,int YCrd,int ZCrd, bool CheckMaxZ)
{	//check if the given position is a valid one
	if(CheckMaxZ)
		return ((XCrd > -1 && XCrd < X_RowLength) && (YCrd > -1 && YCrd < Y_RowLength) && (ZCrd > -1 && ZCrd < Z_RowLength));
	else
		return ((XCrd > -1 && XCrd < X_RowLength) && (YCrd > -1 && YCrd < Y_RowLength) && (ZCrd > -1));
}


//-----------------------------------------------------------------------------
// Name : ResetCamPos()
// Desc : That will reset the camera position to it's origin
//-----------------------------------------------------------------------------
void Tetrix3DEngine::ResetCamPos()
{
	xRotation = yRotation = 0;
	g_Camera.EyeX = g_Camera.EyeY = 0.0f;
	g_Camera.EyeZ = StartingZCamPos;
	g_ResetTimer = false;//???(necessary???)
}


//-----------------------------------------------------------------------------
// Name : GameUpdate()
// Desc : Do all the game decision here(this is like the brain part!)
//-----------------------------------------------------------------------------
void Tetrix3DEngine::GameUpdate()
{
	//This will hold the number of lines to remove if we need to
	static int NumLinesToRem = 0;
	//Check what state is in g_GameState and take action depending on the value 
	switch(g_GameState)
	{
	case GAME_STARTED:
		//If we are not into the block redrawing(when you do a line)
		if(!g_InRedraw){
			//Select a new shape in g_Shape if nothing is in it 
			if(pCurrentShape->ShapeIndx == 0){
				//Find a new shape
				SetNewShapeData(RandomShape);
				//If we don't, the first bolck will begin 1 line below on the z axis becose it will enter in CanMove and Move...
				return;
			}

			//If we can move the block...
			if(CanMove(Dir_Z_Down)){
				//move it...
				Move(Dir_Z_Down);
			} else {//otherway...
				//OverWriteBlockJmp:
				//Add the piece to SpaceMem
				AddPcxToSpaceMem();
				//Play the stick sound
				#ifndef DISABLE_THE_FUCKING_STICK_SOUND
				if(g_SoundEnabled)
					FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[STICK_SOUND]);
				#endif
				//Tell how many lines we have to remove
				NumLinesToRem = IsSomeLinesToRemove();
				//if we have some line to rem...
				if(NumLinesToRem > 0){
					FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[LINESINC_SOUND]);
					//inc the lines counter
					g_Lines += NumLinesToRem;
					//Set the HiScore to the current g_Lines value(if it is greater than the hiScore, of course!)
					if(g_Lines > g_HiScore)
						g_HiScore = g_Lines;
					//remove the lines 
					RemoveLines();
					#ifdef FAST_REDRAW
						//Find a new shape
						SetNewShapeData(RandomShape);
					#else
						//Copy Spacemem array temporary
						FillTmpSpaceMem();
						//Fill SpaceMem with 0(reset it)
						SpaceMemInit();
						//init the global counter for my block animation
						SMCpt_X = SMCpt_Y = SMCpt_Z = 0;
						//Set the rendering Speed
						g_RenderIntervalRatio = ((float)TimerSpeed[0] / 1000.0f);
						//Set g_InRedraw to true 
						g_InRedraw = true;
					#endif
				} else { //no lines to remove...
					//Find a new shape
					SetNewShapeData(RandomShape);
				}	   
			}
		} else {//if we are in redraw...
			//Check if we must redraw another block...
			g_InRedraw = ShowNextBlock();		
			//...if not...
			if(!g_InRedraw){
				//Find a new shape
				SetNewShapeData(RandomShape);
				//Set the rendering Speed
				g_RenderIntervalRatio = ((float)TimerSpeed[g_CurrentTimerSpeed] / 1000.0f);
			}
		}
		break;
	case GAME_OVER:
		//if we are in the shaking sequence...
		if(g_SakeCount > 0)	
			//dec g_SakeCount
			g_SakeCount--; 
		else//otherwise...
			//Set the rendering Speed
			g_RenderIntervalRatio = ((float)TimerSpeed[g_CurrentTimerSpeed] / 1000.0f);
		break;
	default: break;
	}
}


//-----------------------------------------------------------------------------
// Name : GLChangeColor(int Color)
// Desc : Change the OpenGL current color to predefined one
//        without an alpha value
//-----------------------------------------------------------------------------
void Tetrix3DEngine::GLChangeColor(int Color)
{
	switch(Color)
	{
	case Col_RED   : glColor3ub(glRed3b);    break;
	case Col_GREEN : glColor3ub(glGreen3b);  break;
	case Col_BLUE  : glColor3ub(glBlue3b);   break;
	case Col_YELLOW: glColor3ub(glYellow3b); break;
	case Col_PURPLE: glColor3ub(glPurple3b); break;
	case Col_ORANGE: glColor3ub(glOrange3b); break;
	case Col_WHITE : glColor3ub(glWhite3b);  break;
	case Col_BLACK : glColor3ub(glBlack3b);  break;
	case Col_GRAY  : glColor3ub(glGray3b);   break;
	}
}


//-----------------------------------------------------------------------------
// Name : GLChangeColor(int Color, float Alpha)
// Desc : Change the OpenGL current color to predefined one
//        with an alpha value
//-----------------------------------------------------------------------------
void Tetrix3DEngine::GLChangeColor(int Color, float Alpha)
{
	switch(Color)
	{
	case Col_RED   : glColor4f(1.0f, 0.0f, 0.0f, Alpha); break;
	case Col_GREEN : glColor4f(0.0f, 1.0f, 0.0f, Alpha); break;
	case Col_BLUE  : glColor4f(0.0f, 0.0f, 1.0f, Alpha); break;
	case Col_YELLOW: glColor4f(1.0f, 1.0f, 0.0f, Alpha); break;
	case Col_PURPLE: glColor4f(1.0f, 0.0f, 1.0f, Alpha); break;
	case Col_ORANGE: glColor4f(1.0f, 0.5f, 0.0f, Alpha); break;
	case Col_WHITE : glColor4f(1.0f, 1.0f, 1.0f, Alpha); break;
	case Col_BLACK : glColor4f(0.0f, 0.0f, 0.0f, Alpha); break;
	case Col_GRAY  : glColor4f(0.5f, 0.5f, 0.5f, Alpha); break;
	}
}


//-----------------------------------------------------------------------------
// Name : AddPcxToSpaceMem()
// Desc : This will write into SpaceMem all the blocks position	
//        of the current shape
//-----------------------------------------------------------------------------
void Tetrix3DEngine::AddPcxToSpaceMem()
{
	//add all the block to corresponding place of the SpaceMem array
	for(int Cpt = 0; Cpt < pCurrentShape->ShapeVect.size(); Cpt++)
	{
		//if((int)g_Shape.pCurrentShape->ShapeVect[Cpt].X > -1 && (int)g_Shape.pCurrentShape->ShapeVect[Cpt].X < X_RowLength && (int)g_Shape.pCurrentShape->ShapeVect[Cpt].Y > -1 && (int)g_Shape.pCurrentShape->ShapeVect[Cpt].Y < Y_RowLength && (int)g_Shape.pCurrentShape->ShapeVect[Cpt].Z > -1 && (int)g_Shape.pCurrentShape->ShapeVect[Cpt].Z < Z_RowLength)
		//Check if the pieces is into the boundarie of the game(including greater Z then the game)
		if(ValidTetrixCoord((int)pCurrentShape->ShapeVect[Cpt].X,(int)pCurrentShape->ShapeVect[Cpt].Y,(int)pCurrentShape->ShapeVect[Cpt].Z,true))
		{	//if yes...  add the block...
			int TmpX = (int)pCurrentShape->ShapeVect[Cpt].X;
			int TmpY = (int)pCurrentShape->ShapeVect[Cpt].Y;
			int TmpZ = (int)pCurrentShape->ShapeVect[Cpt].Z;
			int SpaceMemIndx = XYZ_TO_I(TmpX, TmpY, TmpZ);
			pSpaceMem[SpaceMemIndx] = true;
			//SpaceCol[(int)g_Shape.pCurrentShape->ShapeVect[Cpt].X][(int)g_Shape.pCurrentShape->ShapeVect[Cpt].Y][(int)g_Shape.pCurrentShape->ShapeVect[Cpt].Z] = g_Shape.pCurrentShape->m_ShapeColor;
		} else { //else 
/******the next if statment if really nessecerry??????????************/
			if(pCurrentShape->ShapeVect[Cpt].Z >= Z_RowLength){
				DoGameOver();
				break;
			}
		}
	}

} 


//-----------------------------------------------------------------------------
// Name : IsSomeLinesToRemove()
// Desc : Tell how many lines we need to remove, if some.
//-----------------------------------------------------------------------------
int Tetrix3DEngine::IsSomeLinesToRemove()
{
	static bool BoolRes;
	static int CptX, CptY, CptZ, Res;
	Res = 0;
	//Check for lines to remove starting from the logic order to check
	CptZ = 0;
	while(CptZ < Z_RowLength){
	CptY = 0;
		while(CptY < Y_RowLength){
			CptX = 0;
			while(CptX < X_RowLength){
				BoolRes = true; //Set the return value to true and will set it to false if we cannot make a line on the current one we are checking 
				//if this field don't have a block...
				int SpaceMemIndx = XYZ_TO_I(CptX,CptY,CptZ);
				if(pSpaceMem[SpaceMemIndx] == false){
					BoolRes = false;	 //set the return value to false
					goto Jumper;	 //Jump to Jumper:
				}
				CptX++;
			}
			CptY++;	
		}
		Res++;	//This will not happen if BoolRes == false
		Jumper: //faster the process
		pLinesToRemove[CptZ] = BoolRes; //used in remove_lines func. see further explanation bellow...
		CptZ++;
	}
	return Res;
}


//-----------------------------------------------------------------------------
// Name : RemoveLines()
// Desc : Remove the lines that are full and make the
//        upper one go down.
//-----------------------------------------------------------------------------
void Tetrix3DEngine::RemoveLines()
{
	int CptCurLine = 0;
	//Check the line one by one
	for(int CptZ = 0; CptZ < Z_RowLength; CptZ++){		
		//if the line is full...
		Restart:
		if(CptZ + CptCurLine < Z_RowLength && pLinesToRemove[CptZ+CptCurLine] == true){		
			CptCurLine++;
			goto Restart;
		} else {
			//Can we check this variable?
			if(CptZ + CptCurLine < Z_RowLength)
				memcpy(pSpaceMem+(CptZ * PreCalc.LinesSpaceSize), pSpaceMem+((CptZ+CptCurLine) * PreCalc.LinesSpaceSize), PreCalc.LinesSpaceSize);
			else		
				memset(pSpaceMem+(CptZ * PreCalc.LinesSpaceSize), 0, PreCalc.LinesSpaceSize);
		}
	}
}


//-----------------------------------------------------------------------------
// Name : FillTmpSpaceMem()
// Desc : Just copy SpaceMem into another array temporairy.
//-----------------------------------------------------------------------------
void Tetrix3DEngine::FillTmpSpaceMem()
{	//just copy SpaceMem into TmpSpaceMem 
	memcpy(pTmpSpaceMem,pSpaceMem, PreCalc.GameSpaceSize); 
}


//-----------------------------------------------------------------------------
// Name : ShowNextBlock()
// Desc : This will be use when we redraw the cube
//        one by one afther some full lines where done
//        and will find witch is the next block to show.
//-----------------------------------------------------------------------------
bool Tetrix3DEngine::ShowNextBlock()
{	//this is the code that will show the block one by one when you do a lines
	bool Res;
	do{
		//Set return value to true if we find a block at this pos
		Res = pTmpSpaceMem[XYZ_TO_I(SMCpt_X,SMCpt_Y,SMCpt_Z)] == true;
		//Fill erased SpaceMem by the way with the new value
		pSpaceMem[XYZ_TO_I(SMCpt_X,SMCpt_Y,SMCpt_Z)] = Res;
		//Counters's control flow...
		SMCpt_X++; 
		if(SMCpt_X >= X_RowLength){
			SMCpt_X = 0;
			SMCpt_Y++;
			if(SMCpt_Y >= Y_RowLength){
				SMCpt_Y = 0;
				SMCpt_Z++;
			}
		}
		//see the next explanation below...
		if(SMCpt_X == 0 && SMCpt_Y == 0 && SMCpt_Z == Z_RowLength) 
			return false;
	}while(!Res); //Search 'till we find a block or return if we finish to check all the TmpSpaceMem array
	
	return Res;
}

//-----------------------------------------------------------------------------
// Name : IsVBOExtensionSupported(char* szTargetExtension)
// Desc : Check if the VBO is Supported
//-----------------------------------------------------------------------------
bool Tetrix3DEngine::IsVBOExtensionSupported(char* szTargetExtension)
{
	const unsigned char *pszExtensions = NULL;
	const unsigned char *pszStart;
	unsigned char *pszWhere, *pszTerminator;

	// Extension names should not have spaces
	pszWhere = (unsigned char *) strchr( szTargetExtension, ' ' );
	if( pszWhere || *szTargetExtension == '\0' )
		return false;

	// Get Extensions String
	pszExtensions = glGetString( GL_EXTENSIONS );

	// Search The Extensions String For An Exact Copy
	pszStart = pszExtensions;
	while(1){
		pszWhere = (unsigned char *) strstr((const char *)pszStart, szTargetExtension);
		if(!pszWhere)
			break;
		pszTerminator = pszWhere + strlen(szTargetExtension);
		if(pszWhere == pszStart || *( pszWhere - 1 ) == ' ')
			if(*pszTerminator == ' ' || *pszTerminator == '\0')
				return true;
		pszStart = pszTerminator;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Name : BuildVBO()
// Desc : Build the VBO, if supported
//-----------------------------------------------------------------------------
void Tetrix3DEngine::BuildVBO()
{
	// Check if the VBO is extension is suppported
	#ifndef NO_VBO
	g_VBO = IsVBOExtensionSupported("GL_ARB_vertex_buffer_object");
	if(g_VBO){
		// Get Pointers To The GL Functions
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
		glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");
		//Load the Models into the graphics card memory
		GenAndBindVBO(&The3DTextModel);
		for(int Cpt= 0; Cpt < 3; Cpt++)
			GenAndBindVBO(&BlockModel[Cpt]);
	}

	#else
	g_VBO = false;
	#endif
}

//-----------------------------------------------------------------------------
// Name : VBO_GenAndBind(UINT *Normal_VBO_ID, UINT *TexVertex_VBO_ID, UINT *Vertex_VBO_ID, CThreeFltpoint *pNormal, CTwoFltpoint *pTexVerts, CThreeFltpoint *pVerts, int VertexCount)
// Desc : Gen. and bind the VBO with data
//-----------------------------------------------------------------------------
void Tetrix3DEngine::GenAndBindVBO(E3DModel *pE3DModel)
{
	GenAndBindBufferMacro(pE3DModel->VertsVBO_ID,    pE3DModel->NumOfVerts, float, 3, pE3DModel->pVertsArray,    GL_STATIC_DRAW_ARB)
	GenAndBindBufferMacro(pE3DModel->TexVertsVBO_ID, pE3DModel->NumOfVerts, float, 2, pE3DModel->pTexVertsArray, GL_STATIC_DRAW_ARB)
	GenAndBindBufferMacro(pE3DModel->NormalVBO_ID,   pE3DModel->NumOfVerts, float, 3, pE3DModel->pNormalsArray,  GL_STATIC_DRAW_ARB)
}

//-----------------------------------------------------------------------------
// Name : BindxxxArray(E3DModel *pE3DModel)
// Desc : Bind the Vertex Arrays
//-----------------------------------------------------------------------------
void Tetrix3DEngine::BindVertsArray(E3DModel *pE3DModel)
{
	if(g_VBO){
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, pE3DModel->VertsVBO_ID);
		glVertexPointer(3, GL_FLOAT, 0, (char *)NULL);
	} else
		glVertexPointer(3, GL_FLOAT, 0, pE3DModel->pVertsArray);
}
//-----------------------------------------------------------------------------
void Tetrix3DEngine::BindTexVertsArray(E3DModel *pE3DModel)
{
	if(g_VBO){
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, pE3DModel->TexVertsVBO_ID);
		glTexCoordPointer(2, GL_FLOAT, 0, (char *)NULL);
	} else
		glTexCoordPointer(2, GL_FLOAT, 0, pE3DModel->pTexVertsArray);
}
//-----------------------------------------------------------------------------
void Tetrix3DEngine::BindNormalsArray(E3DModel *pE3DModel)
{
	if(g_VBO){
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, pE3DModel->NormalVBO_ID);
		glNormalPointer(GL_FLOAT, 0, (char *)NULL);
	} else
		glNormalPointer(GL_FLOAT, 0, pE3DModel->pNormalsArray);
}

//-----------------------------------------------------------------------------
// Name : LoadFont()
// Desc : COOL! Load font from file in memory 
//        withont having to install them in windoz!
//-----------------------------------------------------------------------------
bool Tetrix3DEngine::LoadFont()
{
	// Load the font in windoz, this avoid the installation 
	// of the font in the windoz font directory
	if(!AddFontResource(DigitFontFileName)){return false;}
	if(!AddFontResource(XBANDFontFileName)){return false;}

	//return sucssec to the caller
	return true;
}

//-----------------------------------------------------------------------------
// Name : FreeFont()
// Desc : Free the font loaded with LoadFont()
//-----------------------------------------------------------------------------
void Tetrix3DEngine::FreeFont()
{
	// Delete the loaded font of windoz
	RemoveFontResource(DigitFontFileName);
	RemoveFontResource(XBANDFontFileName);
}

//-----------------------------------------------------------------------------
// Name : DoGameOver()
// Desc : Prepare and Set the game to the GAME_OVER state
//-----------------------------------------------------------------------------
void Tetrix3DEngine::DoGameOver()
{
	if(g_GameState != GAME_OVER){
		// Change game state
		g_GameState = GAME_OVER;
		// Play the busted sound
		FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[BUSTED_SOUND]);
		//set the number of shake 
		g_SakeCount = DEF_SHAKING_COUNT;
		//set the rendering speed
		g_RenderIntervalRatio = ((float)INSHAKING_TIMER_SPEED / 1000.0f);
	}
}


//-----------------------------------------------------------------------------
// Name : UpdateGhost()
// Desc : This just move the ghost shape to the lowest place
//-----------------------------------------------------------------------------
void Tetrix3DEngine::UpdateGhost()
{
	//Update the ghost
	GhostShape = *pCurrentShape;
	while(CanMoveGhost(&GhostShape))
		MoveGhost(&GhostShape);
}

//-----------------------------------------------------------------------------
// Name : ChangeGameState()
// Desc : Just to call Tetrix3DEngine::ChangeGameState() 
//        from the Tetrix3DFader class
//-----------------------------------------------------------------------------
void CTetrix3DFader::ChangeGameState()
{
	//pTetrix3DEngine->g_GameState = GetNewGameState();
	pTetrix3DEngine->ChangeGameState();
}


//-----------------------------------------------------------------------------
// Name : ChangeGameState()
// Desc : Almost the same with OnKeyDown, this is used by 
//        the fader especialy
//-----------------------------------------------------------------------------
void Tetrix3DEngine::ChangeGameState()
{
	if(g_GameState == GAME_PAUSED && Fader->KeyPressed == g_GameKey[T3D_PAUSE])
		goto De_pause;
	
	switch(Fader->KeyPressed)
	{
	case VK_RETURN:
		switch(g_GameState)
		{
		case GAME_INTRO:
			if(IsIntroAnimationFinish){
				switch(g_IntroSelection)
				{
				case INTRO_START:
					g_ResetTimer = true;
					ResizeGameParam();
					/*{
						char c[256] = "c:\\Temp\\TETNIN.ogg";
						LoopSoundTrackThreadHandle = CreateThread(NULL, 0, LoopSoundTrack, (LPVOID)c, 0, &LoopSoundTrackThreadID);
					}*/
					break;
				case INTRO_OPTIONS:
					// Set the true options value into this struct
					OptionsTmpVar.TmpX_RowLength = X_RowLength;
					OptionsTmpVar.TmpY_RowLength = Y_RowLength;
					OptionsTmpVar.TmpZ_RowLength = Z_RowLength;
					OptionsTmpVar.TmpCubeDetail = g_CubeDetail;
					OptionsTmpVar.TmpInvertMouse = g_InvertMouse;
					OptionsTmpVar.TmpShowTexture = g_ShowTexture;
					OptionsTmpVar.TmpShowBck = g_ShowBck;
					OptionsTmpVar.TmpSoundEnabled = g_SoundEnabled;
					OptionsTmpVar.TmpMusicEnabled = g_MusicEnabled;
					//Prepare the menu
					g_OptionsSelection = 0;
					//Change state
					g_GameState = GAME_OPTIONS;
					break;
				case INTRO_KEYBOARD:
					//Copy the true key value into the tmp array
					memcpy(&TmpGameKey[0], &g_GameKey[0], KEY_COUNT);
					//Prepare the menu
					g_KeyConfigWaitKeyPress = false;
					g_KeyConfigSelection = 0;
					//Change state
					g_GameState = GAME_KEYCONFIG;
					break;
				}
			} else {
				g_IntroBlockOffset = 0.0f;
				g_Intro3DTextOffset = 0.0f;
				IsIntroAnimationFinish = true;
			}
			
			break;
			
		case GAME_OPTIONS:
			if(g_OptionsSelection == 9){
				// Update the true key value with the temp one...
				X_RowLength = OptionsTmpVar.TmpX_RowLength;
				Y_RowLength = OptionsTmpVar.TmpY_RowLength;
				Z_RowLength = OptionsTmpVar.TmpZ_RowLength;
				g_CubeDetail = OptionsTmpVar.TmpCubeDetail;
				g_InvertMouse = OptionsTmpVar.TmpInvertMouse;
				g_ShowTexture = OptionsTmpVar.TmpShowTexture;
				g_ShowBck = OptionsTmpVar.TmpShowBck;
				g_SoundEnabled = OptionsTmpVar.TmpSoundEnabled;
				g_MusicEnabled = OptionsTmpVar.TmpMusicEnabled;
				// Used to move the Intro block
				RESET_INTRO;	
				IsIntroAnimationFinish = false;
				Is_AI_Playing = false;
				TimeLeftBeforeAIStart = DEF_TIME_BEFORE_AI_START;
				ResetGame();
				g_GameState = GAME_INTRO;
			}
			break;
		case GAME_KEYCONFIG:
			if(g_KeyConfigSelection == 11){
				//Copy the tmp key value into the true array
				memcpy(&g_GameKey[0], &TmpGameKey[0], KEY_COUNT);
				// Used to move the Intro block
				RESET_INTRO;	
				IsIntroAnimationFinish = false;
				Is_AI_Playing = false;
				TimeLeftBeforeAIStart = DEF_TIME_BEFORE_AI_START;
				ResetGame();
				g_GameState = GAME_INTRO;
			} else {
				g_KeyConfigWaitKeyPress = !g_KeyConfigWaitKeyPress;
			}
			break;
		case GAME_NOT_STARTED:
			//ResetCamPos();
			g_GameState = GAME_STARTED;
			g_ResetTimer = true;
			break;
		case GAME_STARTED:
			g_ResetTimer = false;
			g_GameState = GAME_PAUSED;
			break;
		case GAME_PAUSED:
			De_pause:
			g_ResetTimer = false;
			g_GameState = GAME_STARTED;
			return;
			break;
		case GAME_OVER:
			g_GameState = GAME_NOT_STARTED;
			g_ResetTimer = true;
			ResetGame();
			break;
		case GAME_CONFIRM_EXIT:
			switch(GameConfirmExitSelection)
			{
			case GCE_RESET: 
				ResetGame(); 
				break; 
			case GCE_MAIN_MENU: 
				RESET_INTRO; 
				IsIntroAnimationFinish = false;
				Is_AI_Playing = false;
				TimeLeftBeforeAIStart = DEF_TIME_BEFORE_AI_START;
				g_GameState = GAME_INTRO; 
				break;
			case GCE_EXIT: 
			    PostQuitMessage(0); 
				break;
			}
			break;
		}
		break;
			
	case VK_ESCAPE:

		if(g_GameState == GAME_KEYCONFIG || g_GameState == GAME_OPTIONS){
			RESET_INTRO; 
			IsIntroAnimationFinish = false;
			Is_AI_Playing = false;
			TimeLeftBeforeAIStart = DEF_TIME_BEFORE_AI_START;
			ResetGame();
			g_GameState = GAME_INTRO; // Must be AFTER ResetGame();				
			break;
		}

		switch(g_GameState)
		{
		case GAME_NOT_STARTED:
		case GAME_STARTED:
		case GAME_PAUSED:
		case GAME_OVER:
		case GAME_SUSPENDED:
			GameConfirmExitSelection = 0;
			StateBeforeGameConfirmExit = g_GameState;
			g_GameState = GAME_CONFIRM_EXIT;
			break;
		case GAME_OPTIONS: 
		case GAME_KEYCONFIG: 
			RESET_INTRO;
			g_GameState = GAME_INTRO;
			break;
		case GAME_CONFIRM_EXIT:
			g_GameState = StateBeforeGameConfirmExit;
			break;
		case GAME_INTRO: 
			PostQuitMessage(0); 
			break;
		}	
		break;
	}
}	

/*void Tetrix3DEngine::CommandLineParser()
{
	if(pCmdLinePad->IndexOf("-nosound") != -1)
		g_SoundEnabled = false;
	else if(pCmdLinePad->IndexOf("-nomusic") != -1)
		g_MusicEnabled = false;
	else if(pCmdLinePad->IndexOf("-notexture") != -1)
		g_ShowTexture = false;
	else if(pCmdLinePad->IndexOf("-noghost") != -1)
		g_ShowGhost = false;
	else if(pCmdLinePad->IndexOf("-nobackground") != -1)
		g_ShowBck = false;
	else if(pCmdLinePad->IndexOf("-novbo") != -1)
		;//g_ShowBck = false;
	else if(pCmdLinePad->IndexOf("-fillfirstrowexceptone") != -1)
		;//g_ShowBck = false;	
}*/

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

bool Tetrix3DEngine::CanMoveTestPcx(int MovDir)
{	
	static int Cpt, MoveX, MoveY, MoveZ, SpaceMemIndx;

	//set moveX,Y,Z to tell where to check
	switch(MovDir)
	{
	case Dir_Up    : MoveX = 0;  MoveY = -1; MoveZ = 0;  break;
	case Dir_Down  : MoveX = 0;  MoveY = 1;  MoveZ = 0;  break;
	case Dir_Right : MoveX = 1;  MoveY = 0;  MoveZ = 0;  break;
	case Dir_Left  : MoveX = -1; MoveY = 0;  MoveZ = 0;  break;
	case Dir_Z_Down: MoveX = 0;  MoveY = 0;  MoveZ = -1; break;
	}

	//check all the block of the piece
	for(Cpt = 0; Cpt < TestShapeVect.size(); Cpt++){

		//if the block is in the valid range
		if(ValidTetrixCoord((int)TestShapeVect[Cpt].X, (int)TestShapeVect[Cpt].Y, (int)TestShapeVect[Cpt].Z, true)){
			//if a block is already where is this block...
			SpaceMemIndx = XYZ_TO_I((int)TestShapeVect[Cpt].X, (int)TestShapeVect[Cpt].Y, (int)TestShapeVect[Cpt].Z);
			//if the player's game over!!!...
			if(MovDir == Dir_Z_Down && pSpaceMem[SpaceMemIndx] == true){
				return false;  
			}
		}
		//return false if we are trying to move outside the valid range of the game
		if(!ValidTetrixCoord((int)TestShapeVect[Cpt].X+MoveX, (int)TestShapeVect[Cpt].Y+MoveY, (int)TestShapeVect[Cpt].Z+MoveZ, false))
			return false;
		//finally, return false if another block is below the one we are checking
		SpaceMemIndx = XYZ_TO_I((int)TestShapeVect[Cpt].X+MoveX, (int)TestShapeVect[Cpt].Y+MoveY, (int)TestShapeVect[Cpt].Z+MoveZ);
		//skip the next condition if the next pos. of block have a higher Z pos than the game(could not be affected by anything since we have check it si not on the boundarie of the game)
		if(SpaceMemIndx >= PreCalc.GameSpaceSize)
			continue; 
		else if(pSpaceMem[SpaceMemIndx] == true)
			return false;
	}
	return true;
}	   

///////////////////////////////////////////////////////////////////////////////////////////

void Tetrix3DEngine::MoveTestPcx(int MovDir)
{
	static int MoveX,MoveY,MoveZ;
	//move the block one by one...
	for(int Cpt = 0; Cpt < TestShapeVect.size(); Cpt++)
	{
		//Check witch dir to move
		switch(MovDir)
		{
		case Dir_Up: MoveX = 0; MoveY = -1; MoveZ = 0; break;
		case Dir_Down: MoveX = 0; MoveY = 1; MoveZ = 0; break;
		case Dir_Right: MoveX = 1; MoveY = 0; MoveZ = 0; break;
		case Dir_Left: MoveX = -1; MoveY = 0; MoveZ = 0; break;
		case Dir_Z_Down: MoveX = 0; MoveY = 0; MoveZ = -1; break;
		}
		//Move the block
		TestShapeVect[Cpt].X += MoveX;
		TestShapeVect[Cpt].Y += MoveY;
		TestShapeVect[Cpt].Z += MoveZ;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////

void Tetrix3DEngine::ProcessAICmd(UINT CmdType, UINT CmdParam)
{
	if(Is_AI_Playing){
		switch(CmdType)
		{
		case 0: 
			if(CanRotate(CmdParam)){
				Rotate(CmdParam);
				if(g_SoundEnabled){
					FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[PCXMOVE_SOUND]);
				}
			} else {
				MessageBeep(0);
			}
			break;
		case 1: 
			if(CanMove(CmdParam)){
				Move(CmdParam);
				if(g_SoundEnabled){
					FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[PCXMOVE_SOUND]);
				}	
			} else {
				MessageBeep(0);
			}
			break;
		case 2: 
			while(CanMove(Dir_Z_Down)){
				Move(Dir_Z_Down);
			}
			if(g_GameState == GAME_OVER)
				AddPcxToSpaceMem();
			g_ResetTimer = true;
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////

int Tetrix3DEngine::NumOfNeighbour(int xPos, int yPos, int zPos, bool IsOtherOwnPcxDown)
{
	int SpaceMemIndx = 0;
	int NeighbourCount = 0;

	//Left
	if(xPos > 0){
		SpaceMemIndx = XYZ_TO_I(xPos-1, yPos, zPos);
		if(pSpaceMem[SpaceMemIndx] == true)
			NeighbourCount += AI_NEIGHBOUR_PTS;
	} else
		NeighbourCount += AI_BORDER_PTS;

	//Right
	if(xPos < (X_RowLength-1)){
		SpaceMemIndx = XYZ_TO_I(xPos+1, yPos, zPos);
		if(pSpaceMem[SpaceMemIndx] == true)
			NeighbourCount += AI_NEIGHBOUR_PTS;
	} else
		NeighbourCount += AI_BORDER_PTS;

	//Top
	if(yPos > 0){
		SpaceMemIndx = XYZ_TO_I(xPos, yPos-1, zPos);
		if(pSpaceMem[SpaceMemIndx] == true)
			NeighbourCount += AI_NEIGHBOUR_PTS;
	} else
		NeighbourCount += AI_BORDER_PTS;

	//Down
	if(yPos < (Y_RowLength-1)){
		SpaceMemIndx = XYZ_TO_I(xPos, yPos+1, zPos);
		if(pSpaceMem[SpaceMemIndx] == true)
			NeighbourCount += AI_NEIGHBOUR_PTS;
	} else
		NeighbourCount += AI_BORDER_PTS;


	//Bottom
	if(zPos > 0){
		if(!IsOtherOwnPcxDown){
			SpaceMemIndx = XYZ_TO_I(xPos, yPos, zPos-1);
			if(pSpaceMem[SpaceMemIndx] == false)
				NeighbourCount -= AI_NO_NEIGHBOUR_DOWN_PTS;
		}
	} else 
		NeighbourCount += AI_BORDER_PTS;

	return NeighbourCount;
}

///////////////////////////////////////////////////////////////////////////////////////////

AI_Data_Struct Tetrix3DEngine::Analyser()
{
	int CptBlock = 0;
	int OffSetX = 0, OffSetY = 0;
	int xRot = 0, yRot = 0, zRot = 0;
	int NewScore = 0, LastScore = 0;
	int RotationDone = 0;
	AI_Data_Struct BestScore;

	CThreeFltpoint ShapeCenter = pCurrentShape->ShapeCenter;
	int ShapeIndx = pCurrentShape->ShapeIndx;
	std::vector<CThreeFltpoint> MoveBackup, RotateBackup;
	//std::vector<AI_Data_Struct> BestScoreVect;
	

	//Check if the Pcx as at least 1 block
	if(ShapeIndx != 0){

		//Init ModShape
		TestShapeVect = pCurrentShape->ShapeVect;


		CheckNextRotation:
		if(!TestBit(AI_RotationNeeded[ShapeIndx-1], RotationDone))
			goto SkipThisRotation;
		//This will backup our pcx before making it go down the bottom
		RotateBackup = TestShapeVect;
	

		//Place it on the upper left corner
		while(CanMoveTestPcx(Dir_Left)){
			MoveTestPcx(Dir_Left);
			OffSetX--;
		}
		while(CanMoveTestPcx(Dir_Up)){
			MoveTestPcx(Dir_Up);
			OffSetY--;
		}


		CheckNextMove:
		//This will backup our pcx before making it go down the bottom
		MoveBackup = TestShapeVect;
		
		//Move the pcx down on the z axis the farther we can
		while(CanMoveTestPcx(Dir_Z_Down))
			MoveTestPcx(Dir_Z_Down);


		//Calculate basic score
		NewScore = 0;
		for(CptBlock = 0; CptBlock < TestShapeVect.size(); CptBlock++){

			//Add basic point depending on the block height
			NewScore += pZ_Score[(int)TestShapeVect[CptBlock].Z];
			
			//Check if there is a block below this one
			bool IsOtherOwnPcxDown = false;
			for(int CptBlockBelow = 0; CptBlockBelow < TestShapeVect.size(); CptBlockBelow++){

				//Skip this block...
				if(CptBlockBelow == CptBlock){continue;}
				
				if((int)TestShapeVect[CptBlock].X == (int)TestShapeVect[CptBlockBelow].X && 
				   (int)TestShapeVect[CptBlock].Y == (int)TestShapeVect[CptBlockBelow].Y && 
				   (int)TestShapeVect[CptBlock].Z == (int)TestShapeVect[CptBlockBelow].Z+1){
					IsOtherOwnPcxDown = true;
					break;
				}
				
			}

			//Tweak result a bit...
			NewScore += NumOfNeighbour((int)TestShapeVect[CptBlock].X, (int)TestShapeVect[CptBlock].Y, (int)TestShapeVect[CptBlock].Z, IsOtherOwnPcxDown);
			
			//NewScore += AI_FLOOR_STEP_PTS * (pLineDonePerc[(int)TestShapeVect[CptBlock].Z];
		}




		//Set BestScore
		if(NewScore > LastScore){

			BestScore.Score = NewScore;
			BestScore.RotCount  = RotationDone;
			BestScore.Offset.X = OffSetX;
			BestScore.Offset.Y = OffSetY;
			BestScore.xRot = xRot;
			BestScore.yRot = yRot;
			BestScore.zRot = zRot;

			//BestScoreVect.push_back(BestScore);
			LastScore = NewScore;
		}

		//Restore the pcx before moving bottom down
		TestShapeVect = MoveBackup;

		//Move to the next possible position...
		if(CanMoveTestPcx(Dir_Right)){        //Move the pcx right
			MoveTestPcx(Dir_Right);
			OffSetX++;
			goto CheckNextMove;
		} else if(CanMoveTestPcx(Dir_Down)){  //Move the pcx down
			MoveTestPcx(Dir_Down);
			OffSetY++;
			while(CanMoveTestPcx(Dir_Left)){  //Move the pcx Left-Most
				MoveTestPcx(Dir_Left);
				OffSetX--;
			}
			goto CheckNextMove;
		} else {              //Rotate the pcx, and restart the process
			//Restore the pcx like it was the last rotation
			TestShapeVect = RotateBackup;
			OffSetX = 0;
			OffSetY = 0;

			SkipThisRotation:
			//Increment the Axis counter and rotate the ModShape accordingly
			xRot++;
			for(CptBlock = 0; CptBlock < TestShapeVect.size(); CptBlock++)
				TestShapeVect[CptBlock] = RotateCoord(TestShapeVect[CptBlock], ShapeCenter, X_ClkWse); 
			if(xRot == 4){
				xRot = 0;
				yRot++;
				for(CptBlock = 0; CptBlock < TestShapeVect.size(); CptBlock++)
					TestShapeVect[CptBlock] = RotateCoord(TestShapeVect[CptBlock], ShapeCenter, Y_ClkWse); 
				if(yRot == 4){
					yRot = 0;
					zRot++;
					for(CptBlock = 0; CptBlock < TestShapeVect.size(); CptBlock++)
						TestShapeVect[CptBlock] = RotateCoord(TestShapeVect[CptBlock], ShapeCenter, Z_ClkWse); 
				}
			}

			RotationDone++;
			if(RotationDone >= 32)
				goto JmpToEnd;
			goto CheckNextRotation;
		}

	}

	JmpToEnd:
	return BestScore;
}

///////////////////////////////////////////////////////////////////////////////////////////

/*void Tetrix3DEngine::MakeAIPlay()
{
	int RotCpt = 0;
	int CptMovX = 0, CptMovY = 0;
	int CptRotX = 0, CptRotY = 0, CptRotZ = 0;

	//Rotate the pcx
	while(RotCpt < AI_Dat.RotCount){
		
		//Increment the Axis counter and rotate the ModShape accordingly
		CptRotX++;
		ROTATE_GAME_KEY_MACRO(X_ClkWse)
		if(CptRotX == 4){
			CptRotX = 0;
			CptRotY++;
			ROTATE_GAME_KEY_MACRO(Y_ClkWse)
			if(CptRotY == 4){
				CptRotY = 0;
				CptRotZ++;
				ROTATE_GAME_KEY_MACRO(Z_ClkWse)
			}
		}

		RotCpt++;
	}

	//Move the pcx along the X axis
	CptMovX = AI_Dat.Offset.X;
	if(CptMovX < 0){
		for(int Cpt = CptMovX; Cpt < 0; Cpt++){
			MOVE_GAME_KEY_MACRO(Dir_Left)
		}
	} else if(CptMovX > 0){
		for(int Cpt = CptMovX; Cpt > 0; Cpt--){
			MOVE_GAME_KEY_MACRO(Dir_Right)
		}
	}

	//Move the pcx along the Y axis
	CptMovY = AI_Dat.Offset.Y;
	if(CptMovY < 0){
		for(int Cpt = CptMovY; Cpt < 0; Cpt++){
			MOVE_GAME_KEY_MACRO(Dir_Up)
		}
	} else if(CptMovY > 0){
		for(int Cpt = CptMovY; Cpt > 0; Cpt--){
			MOVE_GAME_KEY_MACRO(Dir_Down)
		}
	}

	FALL_GAME_KEY_MACRO
}*/

///////////////////////////////////////////////////////////////////////////////////////////

void Tetrix3DEngine::TestFunc1()
{
	//Init local variables
	int CptBlock = 0, CptLoop = 0;
	int CptX = 0, CptY = 0, CptZ = 0;
	char c[64];

	//Pcx data vector
	std::vector<CThreeFltpoint> OriginalShape, ModShape, ResVec;
	OriginalShape = pCurrentShape->ShapeVect;
	
	//Create and Open the TestLog.txt file
	FILE *pTestLog = fopen("TestLog.txt", "wb+");

	//Check if the Pcx as at least 1 block
	if(pCurrentShape->ShapeIndx != 0){

		//Init ModShape
		ModShape = OriginalShape;
		
		//Check all the possibility
		for(int CptLoop = 0; CptLoop < 64; CptLoop++){
			//Write new block data in file
			ZeroMemory(c, 64);
			sprintf(c, "%d%d%d\r\n", CptX, CptY, CptZ);
			fputs(c, pTestLog);
			//Rotate one cube at a time for each pcx
			for(CptBlock = 0; CptBlock < ModShape.size(); CptBlock++){
				
				//
				ZeroMemory(c, 64);
				sprintf(c, "MiniTestLog\\MiniTest%2.2d.dat", CptLoop);
				FILE *pMiniTestLog = fopen(c, CptBlock == 0 ? "wb+" : "ab");
				fwrite(&ModShape[CptBlock], 1, sizeof(float)*3, pMiniTestLog);
				fclose(pMiniTestLog);

				//

				//
				ZeroMemory(c, 64);
				sprintf(c, "X = %2.2f - Y = %2.2f - Z = %2.2f\r\n", ModShape[CptBlock].X, ModShape[CptBlock].Y, ModShape[CptBlock].Z);
				fputs(c, pTestLog);
			}
			//Space the result
			sprintf(c, "\r\n");
			fputs(c, pTestLog);

			//


			//Increment the Axis counter and rotate the ModShape accordingly
			CptX++;
			for(CptBlock = 0; CptBlock < ModShape.size(); CptBlock++)
				ModShape[CptBlock] = RotateCoord(ModShape[CptBlock], pCurrentShape->ShapeCenter, X_ClkWse); 
			if(CptX == 4){
				CptX = 0;
				CptY++;
				for(CptBlock = 0; CptBlock < ModShape.size(); CptBlock++)
					ModShape[CptBlock] = RotateCoord(ModShape[CptBlock], pCurrentShape->ShapeCenter, Y_ClkWse); 
				if(CptY == 4){
					CptY = 0;
					CptZ++;
					for(CptBlock = 0; CptBlock < ModShape.size(); CptBlock++)
						ModShape[CptBlock] = RotateCoord(ModShape[CptBlock], pCurrentShape->ShapeCenter, Z_ClkWse); 
				}
			}

		}
	}

	//Close the file
	fclose(pTestLog);
	MessageBeep(0);
}

///////////////////////////////////////////////////////////////////////////////////////////

void Tetrix3DEngine::TestFunc2()
{
	#ifdef USE_ALWAYS_THIS_BLOCK
	const UINT BufSize = 48;
	char c[64];
	BYTE MainBuf[BufSize], SecBuf[BufSize];
	int CptLoop = 0;
	bool CheckedFile[64];
	ZeroMemory(CheckedFile, 64);

	//Create the output file
	ZeroMemory(c, 64);
	sprintf(c, "TestResult%d.txt", USE_ALWAYS_THIS_BLOCK);
	FILE *pTestRes = fopen(c, "wb+");


	//loop throught each files
	for(CptLoop = 0; CptLoop < 64; CptLoop++){
		
		//Skip this file if it's a duplicated one...
		if(CheckedFile[CptLoop] == true)
			continue;

		//Write this file number to the output file
		ZeroMemory(c, 64);
		sprintf(c, "%2.2d", CptLoop);
		fputs(c, pTestRes);

		//Open the file
		ZeroMemory(c, 64);
		sprintf(c, "MiniTestLog\\MiniTest%2.2d.dat", CptLoop);
		FILE *f = fopen(c, "rb");
		//Put content in the main buffer
		ZeroMemory(MainBuf, BufSize);
		fread(MainBuf, 1, BufSize, f);
		//Close the file
		fclose(f);

		//Loop throught the second file to compare with the other one
		for(int CptLoop2 = CptLoop+1; CptLoop2 < 64; CptLoop2++){
			
			//Open the file
			ZeroMemory(c, 64);
			sprintf(c, "MiniTestLog\\MiniTest%2.2d.dat", CptLoop2);
			FILE *f = fopen(c, "rb");
			//Put content in the secondary buffer
			ZeroMemory(SecBuf, BufSize);
			fread(SecBuf, 1, BufSize, f);
			//Close the file
			fclose(f);		

			//If the 2 buffer(files) are the same
			if(memcmp(MainBuf, SecBuf, BufSize) == 0){
				//Write this file number to the output file
				ZeroMemory(c, 64);
				sprintf(c, "-%2.2d", CptLoop2);
				fputs(c, pTestRes);
				//This file is a duplicated one...
				CheckedFile[CptLoop2] = true;
			}

		}

		//add an endline...
		fputs("\r\n", pTestRes);
		//This file been checked...
		CheckedFile[CptLoop] = true;
	}

	//Close the output file
	fclose(pTestRes);
	MessageBeep(0);
	#endif
}


