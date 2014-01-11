#ifndef _CTETRIX3D_H_
#define _CTETRIX3D_H_
#ifdef __cplusplus

#include "Tetrix3DEngine.h"
#include <Windows.h>  // A kind of delphi StringList c++ imitation i've made(very usefull)
#include <SafeKill.h> //Usefull to delete a pointer with one line of code
#include <VectorStringList.h>  // A kind of delphi StringList imitation i've made(very usefull)

#include "Common.h"


DWORD  WINAPI MakeAIPlayThread(void*);


// The Tetrix Intro Position array is here!
static const int IntroBlockY = 9;
static const CIntroBlockStruct IntroBlockStruct[] = {{-13, IntroBlockY, 0, 0}, {-12, IntroBlockY, 0, 0}, {-11, IntroBlockY, 0, 0}, {-10, IntroBlockY, 0, 0},  {-9, IntroBlockY, 0, 0},  {-7, IntroBlockY, 0, 0},  {-6, IntroBlockY, 0, 0},  {-5, IntroBlockY, 0, 0},  {-3, IntroBlockY, 0, 0},  {-2, IntroBlockY, 0, 0},  {-1, IntroBlockY, 0, 0},   {0, IntroBlockY, 0, 0},   {1, IntroBlockY, 0, 0},   {3, IntroBlockY, 0, 0},   {4, IntroBlockY, 0, 0},   {7, IntroBlockY, 0, 0},   {8, IntroBlockY, 0, 0},   {9, IntroBlockY, 0, 0},  {11, IntroBlockY, 0, 0},  {13, IntroBlockY, 0, 0},    {-11, IntroBlockY-1, 0, 1}, {-7, IntroBlockY-1, 0, 1}, {-1, IntroBlockY-1, 0, 1},  {3, IntroBlockY-1, 0, 1},  {5, IntroBlockY-1, 0, 1},  {8, IntroBlockY-1, 0, 1}, {11, IntroBlockY-1, 0, 1}, {13, IntroBlockY-1, 0, 1},    {-11, IntroBlockY-2, 0, 0}, {-7, IntroBlockY-2, 0, 0}, {-6, IntroBlockY-2, 0, 0},  {-5, IntroBlockY-2, 0, 0},  {-1, IntroBlockY-2, 0, 0},  {3, IntroBlockY-2, 0, 0},  {4, IntroBlockY-2, 0, 0},  {8, IntroBlockY-2, 0, 0}, {12, IntroBlockY-2, 0, 0},    {-11, IntroBlockY-3, 0, 1}, {-7, IntroBlockY-3, 0, 1}, {-1, IntroBlockY-3, 0, 1},  {3, IntroBlockY-3, 0, 1},  {5, IntroBlockY-3, 0, 1},  {8, IntroBlockY-3, 0, 1}, {11, IntroBlockY-3, 0, 1}, {13, IntroBlockY-3, 0, 1},    {-11, IntroBlockY-4, 0, 0}, {-7, IntroBlockY-4, 0, 0}, {-6, IntroBlockY-4, 0, 0}, {-5, IntroBlockY-4, 0, 0}, {-1, IntroBlockY-4, 0, 0},  {3, IntroBlockY-4, 0, 0},  {5, IntroBlockY-4, 0, 0},  {7, IntroBlockY-4, 0, 0},  {8, IntroBlockY-4, 0, 0},  {9, IntroBlockY-4, 0, 0}, {11, IntroBlockY-4, 0, 0}, {13, IntroBlockY-4, 0, 0}};
static const int IntroBlockStructSize = (sizeof(IntroBlockStruct)/sizeof(CIntroBlockStruct));

bool CallOneInstance();

CVSL *pCmdLinePad, CmdLinePad;

DWORD TmpGlobalValue;

class Tetrix3D : public Tetrix3DEngine {
public:
	HANDLE OneInstance(char *MutexName);
protected:
	void OnKeyDown(UINT Key);
	void OnKeyUp(UINT Key);
	void OnMouseDown(int ButtonPressed);
	void OnMouseUp(int ButtonPressed);
	int  OnMouseMove(int X, int Y);
	//void RenderGameLoading();	
	void RenderGameOptions();
	void RenderKeyConfig();
	void RenderIntro();
	void RenderDefault();
	void RenderScene();
	void UpdateScene();
	//
	void PrintAIDebugInfo();
	void MakeAIPlay(void);
};
Tetrix3D *pGameEngine; 


#endif
#endif //_CTETRIX3D_H_/*