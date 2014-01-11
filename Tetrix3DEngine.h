#ifndef _CTETRIX3DENGINE_H_
#define _CTETRIX3DENGINE_H_
#ifdef __cplusplus

// Ooo please god help me...
#pragma warning (disable : 4786)//what the hell are these warning???

#pragma comment(lib, "winmm.lib")        //Windoz stuff
#pragma comment(lib, "opengl32.lib")     //OpenGL stuff
#pragma comment(lib, "glu32.lib")	     //OpenGL stuff

#define WIN32_LEAN_AND_MEAN // Trim libraies.
#define VC_LEANMEAN         // Trim even further.
//
#include <Windows.h>        // Windoz header
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include "CAllShapesStruct.h"    // This init the shapes vectors
#include "FModTunes.h"
#include "GLFont.h"	             // A font class i've made
#include "Grid.h"	             // The grid class
#include "TextureObjects.h"      // This class i've mod. load the texture i need in OpenGL and can build an Alpha Channel...
#include <SafeKill.h>            // Help to destroy dynamically created object
#include <ColorDef.h>            // Some color define for OpenGL
#include "Timer.h"               // My little timer class
#include <Time.h>			     // Used for srand
#include <VectorStringList.h>	 // A kind of delphi stringlist i've made
#include "Reg\\RegisterWIN32.h"	 // A Registry class i found somewhere on http\\www.planet-soure-code.com(very usefull too...)
#include "Fader.h"
#include "KeyConfigString.h"
#include "E3DModel.h"    
#include "Common.h"              // This file contain all the define, enum and other stuff like that
#include "Resource.h"

// No need of these in the release version
#ifdef _DEBUG
	// This force no VBO to be used
	//#define NO_VBO
	// This will fill all the row of the first line 
	// execpt for the first one in the upper left
	//#define FILL_FIRST_ROW_EXECPT_ONE
	// This will be usefull for testing, 
	// since it force to not load and use any background 
	//#define NO_BACKGROUND
	// This macro tell that we must ignore key repetion
	//#define IGNORE_KEY_REPETITION  
	// This macro will disable the GameUpdate function call
	//#define NO_GAME_UPDATE
	// This macro will force the same pcx 
	// to be use instead of random
	// ***CAREFULL, NO OVERFLOW TEST***
	//#define USE_ALWAYS_THIS_BLOCK  USE_Z2
#endif //_DEBUG

// This macro will skip the long and boring redraw
#define FAST_REDRAW
// ...
#define DISABLE_THE_FUCKING_STICK_SOUND


// VBO Extension Definitions, From glext.h
#define GL_ARRAY_BUFFER_ARB  0x8892
#define GL_STATIC_DRAW_ARB   0x88E4
typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC)    (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC)    (GLsizei n, GLuint *buffers);
typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC)    (GLenum target, int size, const GLvoid *data, GLenum usage);



//Two little struct for 3d and 2d coordinate
struct float2 {float X,Y;};
struct float3 {float X,Y,Z;};
struct INT2   {int X,Y;};


class CGameWindowDat {
public:
	int Left;
	int Top;
	int Width;
	int Height;
	bool FullScreen;
	//int BitsPerPixel;
	bool Registered;
	CGameWindowDat(){
		GAME_WINDOW_DAT_HELPER(100, 100, 400, 400, false, /*32, */false);
	};
	CGameWindowDat(int L, int T, int W, int H, bool FS, /*int BBP, */bool R)
	{
		GAME_WINDOW_DAT_HELPER(L, T, W, H, FS, /*BBP, */R);		
	};
};

//Struct used to store 2 float
struct CShakingPoint{float X,Y;};
//This struct is just an array of CShakingPoint for shaking effect
struct CShakingStruct{CShakingPoint ShakingPoint[4];};
//This struct store many number pre-calculated for optimization
struct CPreCalc {
	int   GameSpaceSize, LinesSpaceSize;
	int   X_RowLengthMinus1, Y_RowLengthMinus1, Z_RowLengthMinus1;
	float X_RowLengthPlus1Div2, Y_RowLengthMinus1Div2, Z_RowLengthMinus1Div2;
	float X_RowLengthMinus1Div2, Y_RowLengthPlus1Div2, Z_RowLengthPlus1Div2;
	float MinXGridVal, MaxXGridVal, MinYGridVal, MaxYGridVal, MinZGridVal, MaxZGridVal;
	float X_RowLengthDiv2, Y_RowLengthDiv2, Z_RowLengthDiv2;
	RECT  PreviewRect;
};
//A struct to hold the Intro blocks positions
struct CIntroBlockStruct {
	CThreeFltpoint XYZ;
	bool Positive;
};

//This struct will hold all the CXFont we need
struct CTetrix3DFonts{
	int FirstFontList;
	int LastFontList;
	float GLFontRatio;
	GLFont GLPressStartFont;
	GLFont GLPauseFont;
	GLFont GLGameOverFont;
	GLFont GLFPSFont;
	GLFont GLMessageFont;
	GLFont GLCurrentScore;
	GLFont GLHiScore;
	GLFont GLIntroFont;
	GLFont GLOptionsFont;
	GLFont GLKeyConfigFont;
	GLFont GLConfirmExitFont;
	//
	GLFont GLAI_MoveDone_Font;
	GLFont GLAI_Score_Font;
	GLFont GLAI_RotNeeded_Font;
	GLFont GLAI_MovNeeded_Font;
}; 
//This will hold hour camera data
struct CCamDat{
	float EyeX, EyeY, EyeZ;
	float LookAtX, LookAtY, LookAtZ;
	float UpX, UpY, UpZ;
};

//
struct COptionsTmpVar {
	int TmpX_RowLength, TmpY_RowLength, TmpZ_RowLength;
	int TmpCubeDetail;
	bool TmpInvertMouse, TmpShowTexture, TmpShowBck, TmpSoundEnabled, TmpMusicEnabled;
};

struct AI_Data_Struct {
	int RotCount;
	int xRot, yRot, zRot;
	INT2 Offset;
	int Score;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Desc: Some enum
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//This is a set for telling the prog in what game state we are
enum GameStateDef {
	GAME_INTRO = 1,
	GAME_OPTIONS,
	GAME_KEYCONFIG,
	GAME_NOT_STARTED,
	GAME_STARTED,
	GAME_PAUSED,
	GAME_OVER,
	GAME_SUSPENDED,
	GAME_CONFIRM_EXIT
};
//Tell witch way to move
enum DirDef {
	Dir_Up = 1,
	Dir_Down,
	Dir_Left,
	Dir_Right,
	Dir_Z_Down
};
//Tell witch way rotate
enum RotDirDef {
	X_ClkWse = 1,
	X_CntClkWse,
	Y_ClkWse,
	Y_CntClkWse,
	Z_ClkWse,
	Z_CntClkWse
};
//Used to represent all the game possible key
enum KEY_DEF {
	T3D_LEFT = 0,
	T3D_RIGHT,
	T3D_UP,
	T3D_DOWN,
	T3D_FALL,
	T3D_ROTX1,
	T3D_ROTX2,
	T3D_ROTY1,
	T3D_ROTY2,
	T3D_ROTZ1,
	T3D_ROTZ2,
	T3D_PAUSE,
	T3D_QUIT,
	T3D_SOUND,
	T3D_MUSIC,
	T3D_DETAIL,
	T3D_SHOW_TEXTURE,
	T3D_FPS, 
	T3D_INVERT_MOUSE, 
	T3D_SHOW_GHOST, 
};
#define KEY_COUNT  20

enum MessageDef {
	MsgNoMsg = -1,
	MsgDetail,
	MsgSound,
	MsgMusic,
	MsgTexture,
	MsgInvMouse,
	MsgGhost,
};

struct CTetrix3DTextureID {
	UINT g_CubeTexture, 
		 g_GameBckTexture, 
		 g_IntroBckTexture, 
		 g_OptionsBckTexture, 
		 g_KeyConfigBckTexture, 
		 g_3DIntroTextTexture;		// This holds the texture info, referenced by an ID
};


class CTetrix3DFader: public Fader
{
public:
  //This virtual func. can be added(then modified...) in your code to fit your need
  void ChangeGameState();
};



class Tetrix3DEngine {
public:
	Tetrix3DEngine();
	~Tetrix3DEngine();

	virtual void	UpdateScene()                  {}
	virtual void	RenderScene()                  {}

	virtual void    OnKeyDown(UINT KeyPressed)     {}
	virtual void    OnKeyUp(UINT KeyPressed)       {}
	virtual void    OnMouseDown(int ButtonPressed) {}
	virtual void    OnMouseUp(int ButtonPressed)   {}
	virtual int     OnMouseMove(int X, int Y)      {return 0;}
public:
	HWND               g_hWnd;		 // The hWnd of the game window
	HDC                g_hDC;
	HGLRC              g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
	HINSTANCE          g_hInstance;  // The hInstance of the app.
	
	bool CreateGameWindow();
	bool InitializeGameEngine(HINSTANCE *hInstance, CGameWindowDat *ScreenDat); //the destructor will cleanup this later automaticaly...
	void EnterMessageLoop();
	virtual LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	bool IntToBool(int Val){return Val > 0;}
	int BoolToInt(bool Val){return Val == true;}
public:
	PFNGLGENBUFFERSARBPROC    glGenBuffersARB;				// VBO Name Generation Procedure
	PFNGLBINDBUFFERARBPROC    glBindBufferARB;				// VBO Bind Procedure
	PFNGLBUFFERDATAARBPROC 	  glBufferDataARB;				// VBO Data Loading Procedure
	PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;			// VBO Deletion Procedure
public:
	CVSL *pCmdLinePad;

	E3DModel BlockModel[3], The3DTextModel;
	CTetrix3DTextureID TexID;

    CGameWindowDat g_ScreenDat;
	CCamDat g_Camera;
	
	int X_RowLength, Y_RowLength, Z_RowLength;
	bool g_SoundEnabled;
	bool g_MusicEnabled;
	bool g_ShowTexture;
	int CurListIndx;
	int g_HiScore;
	int g_CubeDetail;
	bool g_ShowFPS;	                         
	bool g_ShowBck;
	bool g_ShowGhost;

	bool g_Key[256];
	UCHAR g_GameKey[KEY_COUNT], TmpGameKey[KEY_COUNT];
	bool ValidTetrix3DKey(int Num);
	bool g_MouseButtonState[3];

	bool SetupPixelFormatDescriptor(HDC hdc); 
	bool InitializeOpenGL();
	void InitializeFontSystem();
	bool InitializeSoundSystem();
	bool InitializeTextureSystem();
	bool InitializeModelsArrays();
	void Init_GameOver_Pattern();
	void InitTimerSpeed();
	void Init_GameSpaceStuff();
	void OthersSpaceMemInit();
	void SpaceMemInit();
	void ResetGame();
	void DoGameOver();
	void ResizeGLScreen(int w, int h);

	CTetrix3DFonts g_GLFonts;
	CGrid g_Grid;
	CPreCalc PreCalc;
	CTextures Textures;
	CTimer g_Timer;
	CAllShapesStruct g_Shape;
	CShapeStruct *pCurrentShape, *pNextShape, GhostShape;
	CShakingStruct ShakePts;
	COptionsTmpVar OptionsTmpVar;
	CTetrix3DFader *Fader;

	FMod FModObj;

	// The block SpaceMem
	bool *pSpaceMem, *pTmpSpaceMem;
	float *pSpaceMemX, *pSpaceMemY, *pSpaceMemZ;
	int *pSpaceMemColor;
	// Another dyn. array
	bool *pLinesToRemove;

	int SMCpt_X,SMCpt_Y,SMCpt_Z;	
	int xRotation, yRotation;
	int TimerSpeed[11];
	int g_CurrentTimerSpeed;
	bool IsIntroAnimationFinish;
	bool g_ResetTimer;
	bool g_InRedraw;
	int g_Lines;
	int g_SakeCount;
	bool g_InvertMouse;
	float StartingZCamPos;
	float g_IntroBlockOffset, g_Intro3DTextOffset;
	int g_GameState;
	bool g_VBO;
	float g_RenderIntervalRatio, g_RenderIntervalRatioMem;
	float YPreviewBlockRotation;
	float g_FPS;
	int g_IntroSelection, g_OptionsSelection, g_KeyConfigSelection, GameConfirmExitSelection;
	int StateBeforeGameConfirmExit;
	bool g_KeyConfigWaitKeyPress;
	float g_MessageTimeLeft;
	int g_CurrentMessage;

	void GameCleanup();
	void CleanUpOpenGL();
	void CleanUpSoundSystem();
	void DestroyGameWindow();

	void GameUpdate();

	bool CanMoveGhost(CShapeStruct *pGhostShape);
	void MoveGhost(CShapeStruct *pGhostShape);
	bool CanMove(int MovDir);
	bool CanRotate(int RotDir);
	void Move(int MovDir);
	void Rotate(int MovDir);
	void UpdateGhost();
	CThreeFltpoint RotateCoord(CThreeFltpoint CoordVal, CThreeFltpoint CenterVal, int Dir);

	bool LoadFont();
	void FreeFont();

	void AddPcxToSpaceMem();
	int IsSomeLinesToRemove();
	void RemoveLines();
	void FillTmpSpaceMem();

	void ReallocDynamicsArrays();
	void ResizeGameParam(bool IncGridPtr = false);

	void BuildVBO();
	void GenAndBindVBO(E3DModel *pModel);
	bool IsVBOExtensionSupported(char* szTargetExtension);
	void BindVertsArray(E3DModel *pE3DModel);
	void BindTexVertsArray(E3DModel *pE3DModel);
	void BindNormalsArray(E3DModel *pE3DModel);

	void RegistryReaderWriter(int Reg_Mode, int *Width, int *Height, /*int *BBP, */bool *FullScreen , int *CubeDetail, bool *ShowTexture, bool *SoundSwitch, bool *MusicSwitch, int *X_RowLength, int *Y_RowLength, int *Z_RowLength, UCHAR *GameKey, int *HiScore);

	void SetNewShapeData(int Indx);
	bool ShowNextBlock();
	void GLChangeColor(int Color);
	void GLChangeColor(int Color, float Alpha);
	void ResetCamPos();
	bool ValidTetrixCoord(int XCrd,int YCrd,int ZCrd, bool CheckMaxZ);
	void ChangeGameState();

	//void CommandLineParser();

	
	//////////////////////*****NEW STUFF*****//////////////////////

	DWORD  dwMakeAIPlayThreadID;
	HANDLE hMakeAIPlay;
	HANDLE hTerminateAIThreadEvent; 

	bool  Is_AI_Playing;
	float TimeLeftBeforeAIStart;
	int   AI_MovesCounter;

	AI_Data_Struct AI_Dat;
	std::vector<CThreeFltpoint> TestShapeVect;
	UINT AI_RotationNeeded[MAX_SHAPE];

	int  *pZ_Score;
	void GenZScore();
	
	bool TestBit(UINT bitField, char bitNum){return (bitField & (1<<bitNum)) > 0;}

	bool CanMoveTestPcx(int MovDir);
	void MoveTestPcx(int MovDir);

	int  NumOfNeighbour(int xPos, int yPos, int zPos, bool IsOtherOwnPcxDown);
	AI_Data_Struct Analyser();
	void ProcessAICmd(UINT CmdType, UINT CmdParam);
	virtual void MakeAIPlay(void){}

	///////////////////////////////////////////////////////////////
	
	void TestFunc1();
	void TestFunc2();
	
	///////////////////////////////////////////////////////////////
};

#endif
#endif //_CTETRIX3DENGINE_H_/*