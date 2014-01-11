//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Desc: Define some new tokens
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#define TETRIX3D
//The registry Keys name
#define DEF_OPTIONS_KEY   "Software\\Tetrix3D\\Options"
#define DEF_KEYBOARD_KEY  "Software\\Tetrix3D\\Keyboard"
//
#define BLOCK_FNAME      "Block\\Block.Blk"
//This will defiene the Cubes files names
#define Cube3dsFName     "E3DModels\\GLCube%d.E3D"
#define GL3DTextFName    "E3DModels\\3DTex.E3D"
//This will define the background file name
#define INTRO_FONT_TEX_FNAME     "Textures\\3DTex.bmp"
#define LOAD_BCK_TEX_FNAME       "Textures\\LoadBck.tga"
#define INTRO_BCK_TEX_FNAME      "Textures\\IntroBck.tga"
#define OPTIONS_BCK_TEX_FNAME    "Textures\\OptionsBck.tga"
#define KEYCONFIG_BCK_TEX_FNAME  "Textures\\KeyConfig.tga"
#define GAME_BCK_TEX_FNAME       "Textures\\GameBck.tga"
#define CUBE_TEX_NAME            "Textures\\CubeTex.bmp"
//define default sound ID
#define MenuItemSnd "SoundFX\\MenuItem.wav"
#define MenuSelSnd  "SoundFX\\MenuSel.wav"
#define MenuChgSnd  "SoundFX\\MenuChg.wav"
#define MenuEscSnd  "SoundFX\\MenuEsc.wav"
#define PcxMoveSnd  "SoundFX\\PcxMove.wav"
#define StickSnd    "SoundFX\\Stick.wav"
#define LinesIncSnd "SoundFX\\Lines++.wav"
#define BustedSnd   "SoundFX\\Busted.wav"
//define default font file name
#define DigitFontFileName     "Fonts\\DIGIFAW.ttf"
#define XBANDFontFileName     "Fonts\\XBand Rough.ttf"
//Define some font name...
#define XBANDFontName         "XBAND Rough"	
#define DigitFontName         "DigifaceWide"	
//Define the default perspective and ortho2d glu command
#define DEF_GL_DEPTH         100.0f
#define DEF_GL_INTRO_DEPTH   1000.0f
#define GLU_INTRO_PERSPECTIVE_MACRO 	        gluPerspective(45, 1.33333333f, 0.1f, DEF_GL_INTRO_DEPTH)        
#define GLU_SQUARE_PERSPECTIVE_MACRO 	        gluPerspective(45, 1.0f, 0.1f, DEF_GL_DEPTH)        
#define GLU_NORMAL_PERSPECTIVE_MACRO 	        gluPerspective(45, 1.33333333f, 0.1f, DEF_GL_DEPTH)        
#define GLU_PREVIEWNEXTBLOCK_PERSPECTIVE_MACRO 	gluPerspective(45, 1.0f, 0.1f, DEF_GL_DEPTH)        
#define ORTHO2D_MACRO 	        gluOrtho2D(0, g_ScreenDat.Width, 0, g_ScreenDat.Height)        
//This define the app. & class name and the main window caption
#define APP_NAME       "Tetrix3D"
#define CLASS_NAME     "Tetrix3D GameEngine Class"
#define CLASS_CAPTION  "Tetrix3D - Created By Vortex666"
//Define the text to be draw on screen
#define IntroStartStr       "START"	
#define IntroOptionsStr     "OPTIONS"	
#define KeyboardOptionsStr  "KEYBOARD"	
#define LinesStr            "LINES"	
#define PauseStr            "Pause"	
#define PressStartStr       "Press \"Enter\""
#define GameOverStr         "Game Over !"
#define ConfirmExitStr      "Exit now?"
//the Mutex name(if another app in the world have this mutex, my name is Elvis Graton!!!)
#define ONE_INSTANCE_MUTEX_NAME "Tetrix3D_6765457154987_LeavE_ThIs_Lines_Of_COdE_Into_sOmThing_UniQUe_Like_This_LINes!_98741321654"
// Used to define the different block
#define USE_BAR     1
#define USE_CUBE    2
#define USE_L1      3
#define USE_L2      4 
#define USE_Z1      5
#define USE_Z2      6
#define USE_T       7
// Used to define the detail level of the cube
#define Lo_Res                 0
#define Med_Res                1
#define Hi_Res                 2
//Define maximum array size
#define XMinRowLengthValue     5
#define XMaxRowLengthValue     20
#define YMinRowLengthValue     5
#define YMaxRowLengthValue     20
#define ZMinRowLengthValue     5
#define ZMaxRowLengthValue     25
//These macro will be used for describe witch of the 3 button of the mouse have been pressed
#define LEFT_MOUSE_BUTTON       0
#define MIDDLE_MOUSE_BUTTON     1
#define RIGHT_MOUSE_BUTTON      2
//define two macro for StartOrOptions
#define INTRO_START             0
#define INTRO_OPTIONS           1
#define INTRO_KEYBOARD          2
//
#define GCE_RESET               0
#define GCE_MAIN_MENU           1
#define GCE_EXIT                2
//define Timer Speed
#define MIN_TIMER_SPEED			500
#define MAX_TIMER_SPEED			1000
#define INREDRAW_TIMER_SPEED	100
#define INSHAKING_TIMER_SPEED	50
//define How Sake We do
#define DEF_SHAKING_COUNT       50
//Tell how many fast the camera	will move(or will seem to...)
//#define ROTATION_SPEED          1.0f
//Used to read or write to the registry
#define READTOREG               1
#define WRITETOREG	            2
// Tell how many time we want to fade
#define DEF_FADE_TIME_LENGTH     0.25f
// This define how many time the message font must be on the screen before been erased
#define DEF_MESSAGE_TIME         1.0f

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Desc : Some Macros
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Used to make a number always positive
//#define ABS(X)		  ((X) < 0 ? (-X) : (X))
//This macro will set a full RECT struct 
//#define SET_RECT_MACRO(m, l, t, r, b)   (m).left = (l); (m).top = (t); (m).right = (r); (m).bottom = (b)
//CGameWindowDat helper
#define GAME_WINDOW_DAT_HELPER(L,T,W,H,FS,/*BPP,*/R)  this->Left = (L); this->Top = (T); this->Width = (W); this->Height = (H); this->FullScreen = (FS); /*this->BitsPerPixel = (BPP); */this->Registered = (R)
//Used for setting the camera setting in one shot
#define CCAMDAT_MACRO(Obj,lEyeX,lEyeY,lEyeZ,lLookAtX,lLookAtY,lLookAtZ,lUpX,lUpY,lUpZ)  (Obj).EyeX = (lEyeX); (Obj).EyeY = (lEyeY); (Obj).EyeZ = (lEyeZ); (Obj).LookAtX = (lLookAtX); (Obj).LookAtY = (lLookAtY); (Obj).LookAtZ = (lLookAtZ); (Obj).UpX = (lUpX); (Obj).UpY = (lUpY); (Obj).UpZ = (lUpZ)
//Calcul the z of the Initial camera position(becose the grid can be biggter or smaller if you want)
#define INIT_CAMERA_MACRO(RowLength) ((Z_RowLength+11)+(11*((double)(RowLength-8)/8)))
//A Set of case that said witch key could be used for input in the game
#define VALID_TETRIX3D_KEY case 8:case 13:case 16:case 17:case 18:case 20:case 27:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 45:case 46:case 48:case 49:case 50:case 51:case 52:case 53:case 54:case 55:case 56:case 57:case 65:case 66:case 67:case 68:case 69:case 70:case 71:case 72:case 73:case 74:case 75:case 76:case 77:case 78:case 79:case 80:case 81:case 82:case 83:case 84:case 85:case 86:case 87:case 88:case 89:case 90:case 96:case 97:case 98:case 99:case 100:case 101:case 102:case 103:case 104:case 105:case 106:case 107:case 109:case 110:case 111:case 149:case 187:case 189:case 192
//define random Shape and Color macros
#define RandomShape ((rand() % /*MAX_SHAPE*/g_Shape.NumOfBlockLoaded)+1)
#define RandomColor ((rand() % MAX_COLOR_MINUS_1)+1)
//This will convert the 3 float into the good index number 
#define XYZ_TO_I(x,y,z)	 (((z) * PreCalc.LinesSpaceSize) + ((y) * X_RowLength) + (x))
//Used by SetNewShapeData(int Indx, int Col)
#define XYZ_VECTOR_SHAPEMAKER(s, x, y, z)	XYZ.X = (x); XYZ.Y = (y); XYZ.Z = (z); (s)->ShapeVect.push_back(XYZ)
#define XYZ_VECTOR_SHAPEMAKER2(s, x, y, z)	XYZ.X = (x); XYZ.Y = (y); XYZ.Z = (z); (s).ShapeVect.push_back(XYZ)
#define XYZ_CENTER_SHAPEMAKER(s, x, y, z)	XYZ.X = (x); XYZ.Y = (y); XYZ.Z = (z); (s)->ShapeCenter = XYZ
#define XYZ_CENTER_SHAPEMAKER2(s, x, y, z)	XYZ.X = (x); XYZ.Y = (y); XYZ.Z = (z); (s).ShapeCenter = XYZ
// These macro will shorten the code in OnKeyDown
#define MOVE_GAME_KEY_MACRO(dir)      if(CanMove(dir)){Move(dir);if(g_SoundEnabled){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[PCXMOVE_SOUND]);}g_ResetTimer = false;}
#define ROTATE_GAME_KEY_MACRO(dir)    if(CanRotate(dir)){Rotate(dir);if(g_SoundEnabled){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[PCXMOVE_SOUND]);}g_ResetTimer = false;}
#define FALL_GAME_KEY_MACRO           while(CanMove(Dir_Z_Down)){Move(Dir_Z_Down);}if(g_GameState == GAME_OVER){AddPcxToSpaceMem();}g_ResetTimer = true;/*if(g_SoundEnabled){PlayRes(FallSnd);}*/
// This one too...
#define CHANGE_DETAIL_GAME_KEY_MACRO  switch(g_CubeDetail){case Lo_Res:case Med_Res:g_CubeDetail++;break;case Hi_Res:g_CubeDetail = Lo_Res;break;}					
// This macro will avoid some code repetition, it enable or not the texture
#define TEXTURE_SWITCH(tex)   if(g_ShowTexture){glEnable(GL_TEXTURE_2D);glBindTexture(GL_TEXTURE_2D, tex);}else{glDisable(GL_TEXTURE_2D);}
// This define the amount of time left before firing the AI
#define DEF_TIME_BEFORE_AI_START   5
// Reset the Intro moving block sequence variables
#define RESET_INTRO  g_IntroSelection=0;g_IntroBlockOffset = 3.0f;g_Intro3DTextOffset = 3.0f;TimeLeftBeforeAIStart = DEF_TIME_BEFORE_AI_START

//Just shrink 3 damn line of code into someting more compact 
#define GenAndBindBufferMacro(ID, VCount, Type, NumPerVertex, Data, Mode) if(Data){glGenBuffersARB(1, &ID);glBindBufferARB(GL_ARRAY_BUFFER_ARB, ID);glBufferDataARB(GL_ARRAY_BUFFER_ARB, VCount*NumPerVertex*sizeof(Type), Data, Mode);}
#define DeleteBufferMacro(Model)  if(Model.pVertsArray){glDeleteBuffersARB(1, &Model.VertsVBO_ID);} if(Model.pTexVertsArray){glDeleteBuffersARB(1, &Model.TexVertsVBO_ID);} if(Model.pAuxTexVertsArray){glDeleteBuffersARB(1, &Model.AuxTexVertsVBO_ID);} if(Model.pNormalsArray){glDeleteBuffersARB(1, &Model.NormalVBO_ID);} if(Model.pFogArray){glDeleteBuffersARB(1, &Model.FogVBO_ID);}

#define WM_AI_THREAD_SLEEP_INTERVAL   10
//
#define WM_AI_COMMAND   WM_USER + 125

//
#define AI_FLOOR_STEP_PTS            10
#define AI_NEIGHBOUR_PTS             1
#define AI_BORDER_PTS                2
#define AI_BOTTOM_PTS                2
#define AI_NO_NEIGHBOUR_DOWN_PTS     1000

//
#define AI_LINE_PERC_LOW_RATIO    0.85f
#define AI_LINE_PERC_LOW_MULT     3
#define AI_LINE_PERC_HIGH_RATIO   0.95f
#define AI_LINE_PERC_HIGH_MULT    10

//
//#define AI_THREAD_SLEEP_INTERVAL  100