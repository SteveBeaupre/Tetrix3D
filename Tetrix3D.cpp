#include "Tetrix3D.h"

//-----------------------------------------------------------------------------
// Name : WinMain() (Application Entry Point)
// Desc : Entry point for program, App flow starts here, end here.
//-----------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Check if the game isn't running right now!
	if(!pGameEngine->OneInstance(ONE_INSTANCE_MUTEX_NAME)){
		MessageBox(NULL, "Another Instance found. Exiting Now!", "Error!", 0);
		return false;
	}

	//Init the pointer of a pad(vector of string)
	pCmdLinePad = &CmdLinePad;
	//Parse the cmd line into the pad
	pCmdLinePad->CmdlParser(lpCmdLine);


	//Create the engine
	pGameEngine = new Tetrix3D();
	pGameEngine->pCmdLinePad = pCmdLinePad;

	//Read from the registry
	pGameEngine->RegistryReaderWriter(READTOREG, 
					&pGameEngine->g_ScreenDat.Width, &pGameEngine->g_ScreenDat.Height, /*&WND_BBP, */&pGameEngine->g_ScreenDat.FullScreen,
					&pGameEngine->g_CubeDetail, &pGameEngine->g_ShowTexture, &pGameEngine->g_SoundEnabled,  &pGameEngine->g_MusicEnabled, 
					&pGameEngine->X_RowLength, &pGameEngine->Y_RowLength, &pGameEngine->Z_RowLength, 
					&pGameEngine->g_GameKey[0],
					&pGameEngine->g_HiScore);

	// Tweak our data with the command line
	//pGameEngine->CommandLineParser();

	//A struct to create our window
	CGameWindowDat GameWindowDat(0, 0, pGameEngine->g_ScreenDat.Width, pGameEngine->g_ScreenDat.Height, pGameEngine->g_ScreenDat.FullScreen, /*pGameEngine->g_ScreenDat.BitsPerPixel, */false);

	//Init. the Engine
	if(pGameEngine->InitializeGameEngine(&hInstance, &GameWindowDat))
		pGameEngine->EnterMessageLoop();

	//Destroy the object when the user press escape
	SAFE_DELETE_OBJECT(pGameEngine);

	//return 0 to WinMain
	return 0;
}


//-----------------------------------------------------------------------------
// Name : OneInstance(char *MutexName)
// Desc : This is called to see if another instance of 
//        the app. is found 
//-----------------------------------------------------------------------------
HANDLE Tetrix3D::OneInstance(char *MutexName)
{
	HANDLE ret = NULL; //create a null handle for return
	HANDLE hMutex = CreateMutex(NULL, FALSE, MutexName); //attempt to create MuteX
	bool Running = (GetLastError() == ERROR_ALREADY_EXISTS || GetLastError() == ERROR_ACCESS_DENIED); //See if it was successful
	Running ? ret = NULL : ret = hMutex; //Get the return value
	if(hMutex != NULL) ReleaseMutex(hMutex); //release rhe mutex
	return ret;
}

//-----------------------------------------------------------------------------
// Name : OnKeyDown(UINT KeyPressed)
// Desc : This is called when we press a key 
//-----------------------------------------------------------------------------
void Tetrix3D::OnKeyDown(UINT Key)
{
	//remove this later, it's just a temp protection...
	if(Key == 'Q'){
		PostQuitMessage(0);
		return;
	}
	
	if(Fader->IsFading)
		return;

	//User OnKeyDown even hander
	if(!g_InRedraw){
		
		if(g_GameState == GAME_KEYCONFIG && g_KeyConfigWaitKeyPress){
			TmpGameKey[g_KeyConfigSelection] = Key;
			g_KeyConfigWaitKeyPress = false;
			return;
		}


		if(g_GameState == GAME_PAUSED && Key == g_GameKey[T3D_PAUSE]){
			goto UnPause;
		}
		
		switch(Key)
		{
		case VK_RETURN:
			switch(g_GameState)
			{
			case GAME_INTRO:
				if(IsIntroAnimationFinish){
					if(g_SoundEnabled){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[MENUSEL_SOUND]);}
					switch(g_IntroSelection)
					{
					case INTRO_START:
						Fader->StartFader(g_ScreenDat.Width, g_ScreenDat.Height, 0, 0, DEF_FADE_TIME_LENGTH, DEF_FADE_TIME_LENGTH, Key);
						/*g_ResetTimer = true;
						ResizeGameParam();*/
						break;
					case INTRO_OPTIONS:
						Fader->StartFader(g_ScreenDat.Width, g_ScreenDat.Height, 0, 0, DEF_FADE_TIME_LENGTH, DEF_FADE_TIME_LENGTH, Key);
						// Set the true options value into this struct
						/*OptionsTmpVar.TmpX_RowLength = X_RowLength;
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
						g_GameState = GAME_OPTIONS;*/
						break;
					case INTRO_KEYBOARD:
						Fader->StartFader(g_ScreenDat.Width, g_ScreenDat.Height, 0, 0, DEF_FADE_TIME_LENGTH, DEF_FADE_TIME_LENGTH, Key);
						//Copy the true key value into the tmp array
						/*memcpy(&TmpGameKey[0], &g_GameKey[0], KEY_COUNT);
						//Prepare the menu
						g_KeyConfigWaitKeyPress = false;
						g_KeyConfigSelection = 0;
						//Change state
						g_GameState = GAME_KEYCONFIG;*/
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
					if(g_SoundEnabled){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[MENUSEL_SOUND]);}
					Fader->StartFader(g_ScreenDat.Width, g_ScreenDat.Height, 0, 0, DEF_FADE_TIME_LENGTH, DEF_FADE_TIME_LENGTH, Key);
					/*// Update the true key value with the temp one...
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
					// Change state
					g_GameState = GAME_INTRO;*/
				}
				break;
			case GAME_KEYCONFIG:
				if(g_KeyConfigSelection == 11){
					if(g_SoundEnabled){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[MENUSEL_SOUND]);}
					Fader->StartFader(g_ScreenDat.Width, g_ScreenDat.Height, 0, 0, DEF_FADE_TIME_LENGTH, DEF_FADE_TIME_LENGTH, Key);
					/*//Copy the tmp key value into the true array
					memcpy(&g_GameKey[0], &TmpGameKey[0], KEY_COUNT);
					// Used to move the Intro block
					RESET_INTRO;	
					// Change state
					g_GameState = GAME_INTRO;*/
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
				UnPause:
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
				if(g_SoundEnabled){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[MENUSEL_SOUND]);}
				switch(GameConfirmExitSelection)
				{
				case GCE_RESET    : ResetGame(); break; 
				case GCE_MAIN_MENU: Fader->StartFader(g_ScreenDat.Width, g_ScreenDat.Height, 0, 0, DEF_FADE_TIME_LENGTH, DEF_FADE_TIME_LENGTH, Key); /*RESET_INTRO; g_GameState = GAME_INTRO;*/ break;
				case GCE_EXIT     : Fader->StartFader(g_ScreenDat.Width, g_ScreenDat.Height, 0, 0, DEF_FADE_TIME_LENGTH, DEF_FADE_TIME_LENGTH, Key); /*PostQuitMessage(0);*/ break;
				}
				break;
			}
			break;
		case VK_ESCAPE:

			if(Is_AI_Playing && g_GameState == GAME_STARTED){
				// This will wait and kill the AI thread, if running
				SetEvent(hTerminateAIThreadEvent);
				WaitForSingleObject(hMakeAIPlay, INFINITE);
				ResetEvent(hTerminateAIThreadEvent);
				//
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
				if(g_SoundEnabled){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[MENUESC_SOUND]);}
				Fader->StartFader(g_ScreenDat.Width, g_ScreenDat.Height, 0, 0, DEF_FADE_TIME_LENGTH, DEF_FADE_TIME_LENGTH, Key);
				/*RESET_INTRO;
				g_GameState = GAME_INTRO;*/
				break;
			case GAME_CONFIRM_EXIT:
				g_GameState = StateBeforeGameConfirmExit;
				break;
			case GAME_INTRO: 
				Fader->StartFader(g_ScreenDat.Width, g_ScreenDat.Height, 0, 0, DEF_FADE_TIME_LENGTH, 1000000.0f, Key);
				//PostQuitMessage(0); 
				break;
			}
			break;
		}
		
		if(g_GameState == GAME_STARTED){
			///////////NEW//////////////
			/*if(Key == 123){ //F12
				TestFunc1();
				TestFunc2();
			}*/
			
			/*if(Key == 122){ //F10

			}*/

			/*if(Key == 123){ //F12
				AI_Dat = Analyser();
				//MakeAIPlay();
				hMakeAIPlay = CreateThread(NULL, 0, MakeAIPlayThread, NULL, 0, &dwMakeAIPlayThreadID);	
			}*/
			////////////////////////////

			//The Move key...
			if(!Is_AI_Playing){

				if(Key == g_GameKey[T3D_LEFT]){
					MOVE_GAME_KEY_MACRO(Dir_Left)
				} else if(Key == g_GameKey[T3D_RIGHT]){
					MOVE_GAME_KEY_MACRO(Dir_Right)
				} else if(Key == g_GameKey[T3D_UP]){
					MOVE_GAME_KEY_MACRO(Dir_Up)
				} else if(Key == g_GameKey[T3D_DOWN]){
					MOVE_GAME_KEY_MACRO(Dir_Down)
				} else if(Key == g_GameKey[T3D_FALL]){
					FALL_GAME_KEY_MACRO
				//The Rotate key...
				} else if(Key == g_GameKey[T3D_ROTX1]){
					ROTATE_GAME_KEY_MACRO(X_CntClkWse)
				} else if(Key == g_GameKey[T3D_ROTX2]){
					ROTATE_GAME_KEY_MACRO(X_ClkWse)
				} else if(Key == g_GameKey[T3D_ROTY1]){
					ROTATE_GAME_KEY_MACRO(Y_CntClkWse)
				} else if(Key == g_GameKey[T3D_ROTY2]){
					ROTATE_GAME_KEY_MACRO(Y_ClkWse)
				} else if(Key == g_GameKey[T3D_ROTZ1]){
					ROTATE_GAME_KEY_MACRO(Z_CntClkWse)
				} else if(Key == g_GameKey[T3D_ROTZ2]){
					ROTATE_GAME_KEY_MACRO(Z_ClkWse)
				//...and the others game-related keys
				} else if(Key == g_GameKey[T3D_PAUSE]){
					g_ResetTimer = false;
					g_GameState = GAME_PAUSED;
					g_MessageTimeLeft = DEF_MESSAGE_TIME;
				} else if(Key == g_GameKey[T3D_SHOW_GHOST]){
					g_ShowGhost = !g_ShowGhost;
					g_CurrentMessage = MsgGhost;
					g_MessageTimeLeft = DEF_MESSAGE_TIME;
				}

			}
		}
			
		if(g_GameState == GAME_INTRO){
			if(g_Key[VK_LEFT] && g_IntroSelection > 0) {
				g_IntroSelection--;
				TimeLeftBeforeAIStart = DEF_TIME_BEFORE_AI_START;
				if(g_SoundEnabled){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[MENUCHG_SOUND]);}
			} else if(g_Key[VK_RIGHT] && g_IntroSelection < 2){
				g_IntroSelection++;
				TimeLeftBeforeAIStart = DEF_TIME_BEFORE_AI_START;
				if(g_SoundEnabled){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[MENUCHG_SOUND]);}
			}
		}

		
		if(g_GameState == GAME_OPTIONS){
			//Control the up and down message
			if(g_Key[VK_UP] && g_OptionsSelection > 0) {
				g_OptionsSelection--;
				if(g_SoundEnabled){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[MENUCHG_SOUND]);}
			} else if(g_Key[VK_DOWN] && g_OptionsSelection < 9){
				g_OptionsSelection++;
				if(g_SoundEnabled){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[MENUCHG_SOUND]);}
			}
			//Control the left and right message
			static bool CanPlaySnd;
			CanPlaySnd = false;
			if(g_Key[VK_LEFT]){
				switch(g_OptionsSelection)
				{
				case 0: if(OptionsTmpVar.TmpX_RowLength > XMinRowLengthValue){OptionsTmpVar.TmpX_RowLength--; CanPlaySnd = true;} break;
				case 1: if(OptionsTmpVar.TmpY_RowLength > YMinRowLengthValue){OptionsTmpVar.TmpY_RowLength--; CanPlaySnd = true;} break;
				case 2: if(OptionsTmpVar.TmpZ_RowLength > ZMinRowLengthValue){OptionsTmpVar.TmpZ_RowLength--; CanPlaySnd = true;} break;
				case 3: if(OptionsTmpVar.TmpCubeDetail > Lo_Res){OptionsTmpVar.TmpCubeDetail--; CanPlaySnd = true;} break;
				case 4: OptionsTmpVar.TmpInvertMouse = !OptionsTmpVar.TmpInvertMouse; CanPlaySnd = true; break;
				case 5: OptionsTmpVar.TmpShowTexture = !OptionsTmpVar.TmpShowTexture; CanPlaySnd = true; break;
				case 6: OptionsTmpVar.TmpShowBck = !OptionsTmpVar.TmpShowBck; CanPlaySnd = true; break;
				case 7: OptionsTmpVar.TmpSoundEnabled = !OptionsTmpVar.TmpSoundEnabled; CanPlaySnd = true; break;
				case 8: OptionsTmpVar.TmpMusicEnabled = !OptionsTmpVar.TmpMusicEnabled; CanPlaySnd = true; break;
				}
				if(g_SoundEnabled && CanPlaySnd){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[MENUCHG_SOUND]);}
			} else if(g_Key[VK_RIGHT]){
				switch(g_OptionsSelection)
				{
				case 0: if(OptionsTmpVar.TmpX_RowLength < XMaxRowLengthValue){OptionsTmpVar.TmpX_RowLength++; CanPlaySnd = true;} break;
				case 1: if(OptionsTmpVar.TmpY_RowLength < YMaxRowLengthValue){OptionsTmpVar.TmpY_RowLength++; CanPlaySnd = true;} break;
				case 2: if(OptionsTmpVar.TmpZ_RowLength < ZMaxRowLengthValue){OptionsTmpVar.TmpZ_RowLength++; CanPlaySnd = true;} break;
				case 3: if(OptionsTmpVar.TmpCubeDetail < Hi_Res){OptionsTmpVar.TmpCubeDetail++; CanPlaySnd = true;} break;
				case 4: OptionsTmpVar.TmpInvertMouse = !OptionsTmpVar.TmpInvertMouse; CanPlaySnd = true; break;
				case 5: OptionsTmpVar.TmpShowTexture = !OptionsTmpVar.TmpShowTexture; CanPlaySnd = true; break;
				case 6: OptionsTmpVar.TmpShowBck = !OptionsTmpVar.TmpShowBck; CanPlaySnd = true; break;
				case 7: OptionsTmpVar.TmpSoundEnabled = !OptionsTmpVar.TmpSoundEnabled; CanPlaySnd = true; break;
				case 8: OptionsTmpVar.TmpMusicEnabled = !OptionsTmpVar.TmpMusicEnabled; CanPlaySnd = true; break;
				}
				if(g_SoundEnabled && CanPlaySnd){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[MENUCHG_SOUND]);}
			}
		}
		
		
		if(g_GameState == GAME_KEYCONFIG){
			//Control the up and down message
			if(g_Key[VK_UP] && g_KeyConfigSelection > 0) {
				g_KeyConfigSelection--;
				if(g_SoundEnabled){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[MENUCHG_SOUND]);}
			} else if(g_Key[VK_DOWN] && g_KeyConfigSelection < 11){
				g_KeyConfigSelection++;
				if(g_SoundEnabled){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[MENUCHG_SOUND]);}
			}
		}

		if(g_GameState == GAME_CONFIRM_EXIT){
			if(g_Key[VK_UP] && GameConfirmExitSelection > 0){
				GameConfirmExitSelection--;		
				if(g_SoundEnabled){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[MENUCHG_SOUND]);}
			} else if(g_Key[VK_DOWN] && GameConfirmExitSelection < 2){
				GameConfirmExitSelection++;		
				if(g_SoundEnabled){FSOUND_PlaySound(FSOUND_FREE, FModObj.Sounds[MENUCHG_SOUND]);}
			}
		}
		
		if(Key == 'B' && (g_GameState == GAME_INTRO || g_GameState == GAME_NOT_STARTED || g_GameState == GAME_STARTED || g_GameState == GAME_OVER || g_GameState == GAME_PAUSED))
			g_ShowBck = !g_ShowBck;


			

		if(Key == g_GameKey[T3D_QUIT]){
			PostQuitMessage(0); 
		} else if(Key == g_GameKey[T3D_SOUND]){
			g_SoundEnabled = !g_SoundEnabled;
			g_CurrentMessage = MsgSound;
			g_MessageTimeLeft = DEF_MESSAGE_TIME;
		} else if(Key == g_GameKey[T3D_MUSIC]){
			switch(g_MusicEnabled)
			{
			case false:	FModObj.ChangeTunes(); FModObj.PlayTunes(); FModObj.StartUpdateThread(); break;
			case true:	FModObj.StopUpdateThread(); FModObj.StopTunes();  break;
			}
			g_MusicEnabled = !g_MusicEnabled;
			g_CurrentMessage = MsgMusic;
			g_MessageTimeLeft = DEF_MESSAGE_TIME;
		} else if(Key == g_GameKey[T3D_DETAIL]){
			CHANGE_DETAIL_GAME_KEY_MACRO
				g_CurrentMessage = MsgDetail;
			g_MessageTimeLeft = DEF_MESSAGE_TIME;
		} else if(Key == g_GameKey[T3D_SHOW_TEXTURE]){
			g_ShowTexture = !g_ShowTexture;
			g_CurrentMessage = MsgTexture;
			g_MessageTimeLeft = DEF_MESSAGE_TIME;
		} else if(Key == g_GameKey[T3D_FPS]){
			g_ShowFPS = !g_ShowFPS;
		} else if(Key == g_GameKey[T3D_INVERT_MOUSE]){
			g_InvertMouse = !g_InvertMouse;
			g_CurrentMessage = MsgInvMouse;
			g_MessageTimeLeft = DEF_MESSAGE_TIME;
		}

	}
}

//create 1 function to respond key up and down
//-----------------------------------------------------------------------------
// Name : OnKeyUp(UINT KeyPressed)
// Desc : This is called when we release a key 
//-----------------------------------------------------------------------------
void Tetrix3D::OnKeyUp(UINT Key)
{
	// If the user presses the escape key then exit the application.
	/*if(Key == VK_ESCAPE){
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
			g_SkipLastFrame = true;
			break;
		}		
	}*/
}


//-----------------------------------------------------------------------------
// Name : OnMouseDown()
// Desc : This is called when we left-click	the mouse 
//-----------------------------------------------------------------------------
void Tetrix3D::OnMouseDown(int ButtonPressed)
{
	if(ButtonPressed == MIDDLE_MOUSE_BUTTON) 		
		ResetCamPos();

	if(ButtonPressed == LEFT_MOUSE_BUTTON && g_GameState == GAME_STARTED) {
		//set the gameState suspended
		g_GameState = GAME_SUSPENDED;
		//change the rendering speed(and keep the previous one in memory)
		g_RenderIntervalRatioMem = g_RenderIntervalRatio;
		g_RenderIntervalRatio = 0.001f;
	}
}

//-----------------------------------------------------------------------------
// Name : OnMouseUp()
// Desc : This is called when we release a left-click
//-----------------------------------------------------------------------------
void Tetrix3D::OnMouseUp(int ButtonPressed)
{
	//check if gamestate is suspended
	if(ButtonPressed == LEFT_MOUSE_BUTTON && g_GameState == GAME_SUSPENDED){
		//set the gameState started
		g_GameState = GAME_STARTED;
		//change the rendering speed
		g_RenderIntervalRatio = g_RenderIntervalRatioMem;
	}
}

//-----------------------------------------------------------------------------
// Name : OnMouseMouve()
// Desc : This is called when we move the mouse
//-----------------------------------------------------------------------------
int Tetrix3D::OnMouseMove(int X, int Y)
{
	static int Last_X = X;
	static int Last_Y = Y;
	static bool Will_Loop = false;
	static const int DEF_WIDTH_SIZE_M1 = (g_ScreenDat.Width-1); 
	static const int DEF_HEIGHT_SIZE_M1 = (g_ScreenDat.Height-1);
	
	//This code simulate no wrap for the mouse on the screen
	if(Will_Loop)
	{
		Will_Loop = false; 
		return 0;
	} 
	
	if(X == 0 || X == DEF_WIDTH_SIZE_M1){
		X = DEF_WIDTH_SIZE_M1 - X;  
		if(X == 0){Last_X = X - (DEF_WIDTH_SIZE_M1 - Last_X);}else{Last_X = X + Last_X;}
		Will_Loop = TRUE; 
		SetCursorPos(X, Y);
	}	
	if(Y == 0 || Y == DEF_HEIGHT_SIZE_M1){
		Y = DEF_HEIGHT_SIZE_M1 - Y; 
		if(Y == 0){Last_Y = Y - (DEF_HEIGHT_SIZE_M1 - Last_Y);}else{Last_Y = Y + Last_Y;}
		Will_Loop = TRUE; 
		SetCursorPos(X, Y);
	}

	//this code set different variables for rotating, moving or zooming the game
	if(g_MouseButtonState[LEFT_MOUSE_BUTTON]) {
		//if CTRL is pressed and SHIFT is NOT pressed, rotate the game
		if(!g_Key[VK_CONTROL] && !g_Key[VK_SHIFT]){		
			xRotation += X - Last_X/*((float)(X - Last_X) / ROTATION_SPEED)*/;
			if(xRotation >= 360){xRotation -= 360;}
			if(xRotation < 0){xRotation += 360;}
			switch(g_InvertMouse)
			{
			case false: yRotation += Y - Last_Y/*((float)(Y - Last_Y) / ROTATION_SPEED)*/; break;
			case true:  yRotation -= Y - Last_Y/*((float)(Y - Last_Y) / ROTATION_SPEED)*/; break;
			}
			if(yRotation >= 360) yRotation -= 360;
			if(yRotation < 0) yRotation += 360;
		}
		//if CTRL is down, translate the game
		if(g_Key[VK_CONTROL]){
			g_Camera.EyeX += X - Last_X/*((float)(X - Last_X) / ROTATION_SPEED)*/;
			switch(g_InvertMouse)
			{
			case false: g_Camera.EyeY += Y - Last_Y/*((float)(Y - Last_Y) / ROTATION_SPEED)*/; break;
			case true:  g_Camera.EyeY -= Y - Last_Y/*((float)(Y - Last_Y) / ROTATION_SPEED)*/; break;
			}
			
		}
		//if SHIFT is down, zoom in the game
		if(g_Key[VK_SHIFT])
			g_Camera.EyeZ += Y - Last_Y/*((float)(Y - Last_Y) / ROTATION_SPEED)*/;
		
	}

	//Set these var. for the next call
	Last_X = X;
	Last_Y = Y;

	return 0;
}


//-----------------------------------------------------------------------------
// Name : RenderGameLoading()
// Desc : Render the loading part of the game
//-----------------------------------------------------------------------------
/*inline void Tetrix3D::RenderGameLoading()
{

}*/

//-----------------------------------------------------------------------------
// Name : RenderGameOptions()
// Desc : Render the options part of the game
//-----------------------------------------------------------------------------
inline void Tetrix3D::RenderGameOptions()
{
	static Cpt = 0;

	//2D Stuffs
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ORTHO2D_MACRO;
	glMatrixMode(GL_MODELVIEW);
	
	
	// Print Start and Option 
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);  
	glDisable(GL_TEXTURE_2D);
	
	
	
	#ifndef NO_BACKGROUND
	if(g_ShowBck)//Cut the fucking fps a whole fucking way down!!!
	{
		glEnable(GL_TEXTURE_2D);							
		//bind the texture
		glBindTexture(GL_TEXTURE_2D, TexID.g_OptionsBckTexture);
		
		glBegin(GL_QUADS);
		glColor3ub(glWhite3b);
		
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(g_ScreenDat.Width, 0.0f);
		
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(g_ScreenDat.Width, g_ScreenDat.Height);
		
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.0f, g_ScreenDat.Height);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	#endif 
	
	
	{
		int TopVar = 29;
		static const LPSTR OptionsStr[] = {"Width", "Height", "Depth", "Cube Detail", "Invert Mouse", "Texture", "Background", "Sound FX", "Music", "Exit"};
		static const LPSTR OffOrOn[2] = {"Off", "  On"};
		static const int ToAdd = 18;
		//Draw the options to the screen
		for(Cpt = 0; Cpt < 10; Cpt++){
			if(g_OptionsSelection == Cpt)
				g_GLFonts.GLOptionsFont.ChangeFontColor(0, 255, 0);    //Selected
			else
				g_GLFonts.GLOptionsFont.ChangeFontColor(255, 0, 0);  //Other 
			
			g_GLFonts.GLOptionsFont.Top = (g_ScreenDat.Height-(TopVar*g_GLFonts.GLFontRatio));
			switch(Cpt)
			{
			case 2:	
			case 3:
			case 8:	TopVar += (ToAdd + (5 * g_GLFonts.GLFontRatio))-1;
				break;
			default: TopVar += ToAdd; 
				break;
			}
			//This one is special so we will isolate it
			if(Cpt == 9){
				g_GLFonts.GLOptionsFont.Left = (142 * g_GLFonts.GLFontRatio);
				g_GLFonts.GLOptionsFont.glPrintTxt(OptionsStr[Cpt]);
				continue;
			}
			//Print the options title
			g_GLFonts.GLOptionsFont.Left = (15 * g_GLFonts.GLFontRatio);
			g_GLFonts.GLOptionsFont.glPrintTxt(OptionsStr[Cpt]);
			//Print the value of this options
			g_GLFonts.GLOptionsFont.Left = (280 * g_GLFonts.GLFontRatio);
			switch(Cpt)
			{
			case 0: g_GLFonts.GLOptionsFont.glPrintArg("  %2.2d", OptionsTmpVar.TmpX_RowLength); break;
			case 1: g_GLFonts.GLOptionsFont.glPrintArg("  %2.2d", OptionsTmpVar.TmpY_RowLength); break;
			case 2: g_GLFonts.GLOptionsFont.glPrintArg("  %2.2d", OptionsTmpVar.TmpZ_RowLength); break;
			case 3: g_GLFonts.GLOptionsFont.glPrintArg("  %2.2d", OptionsTmpVar.TmpCubeDetail); break;
			case 4: g_GLFonts.GLOptionsFont.glPrintTxt(OffOrOn[(int)OptionsTmpVar.TmpInvertMouse]); break;
			case 5: g_GLFonts.GLOptionsFont.glPrintArg(OffOrOn[(int)OptionsTmpVar.TmpShowTexture]); break;
			case 6: g_GLFonts.GLOptionsFont.glPrintArg(OffOrOn[(int)OptionsTmpVar.TmpShowBck]); break;
			case 7: g_GLFonts.GLOptionsFont.glPrintArg(OffOrOn[(int)OptionsTmpVar.TmpSoundEnabled]); break;
			case 8: g_GLFonts.GLOptionsFont.glPrintArg(OffOrOn[(int)OptionsTmpVar.TmpMusicEnabled]); break;
			}
			
		}
	}

	//Draw the fps
	if(g_ShowFPS){g_GLFonts.GLFPSFont.glPrintArg("%2.2f FPS", g_FPS);}	
}


//-----------------------------------------------------------------------------
// Name : RenderKeyConfig()
// Desc : Render the Key Config. part of the game
//-----------------------------------------------------------------------------
inline void Tetrix3D::RenderKeyConfig()
{
	//2D Stuffs
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ORTHO2D_MACRO;
	glMatrixMode(GL_MODELVIEW);
	
	
	// Print Start and Option 
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);  
	glDisable(GL_TEXTURE_2D);
	
	
	
	#ifndef NO_BACKGROUND
	if(g_ShowBck)//Cut the fucking fps a whole fucking way down!!!
	{
		glEnable(GL_TEXTURE_2D);							
		//bind the texture
		glBindTexture(GL_TEXTURE_2D, TexID.g_KeyConfigBckTexture);
		
		glBegin(GL_QUADS);
		glColor3ub(glWhite3b);
		
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(g_ScreenDat.Width, 0.0f);
		
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(g_ScreenDat.Width, g_ScreenDat.Height);
		
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.0f, g_ScreenDat.Height);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	#endif
	
	//Draw the fps
	//if(g_ShowFPS){g_GLFonts.GLFPSFont.glPrintArg("%2.2f FPS", g_FPS);}	
	{
		static int FontWidth = 18;
		static const int KeyConfigKeyToCheck[] = {T3D_LEFT, T3D_RIGHT, T3D_UP, T3D_DOWN, T3D_FALL, T3D_ROTX1, T3D_ROTX2, T3D_ROTY1, T3D_ROTY2, T3D_ROTZ1, T3D_ROTZ2};
		static const LPSTR KeyConfigStr[] = {"Left Key", "Right Key", "Up Key", "Down Key", "Fall Key", "X CntClkWse", "X ClkWse", "Y CntClkWse", "Y ClkWse", "Z CntClkWse", "Z ClkWse", "Exit"};
		int KeyConfigLeftCoord[] = {70, 435, 70, 435, 250, 70, 435, 70, 435, 70, 435, 361};
		int KeyConfigTopCoord[] = {517, 517, 460, 460, 403, 290, 290, 233, 233, 176, 176, 60};
		
		for(int KeyConfigCpt = 0; KeyConfigCpt < 12; KeyConfigCpt++){
			//if(KeyConfigCpt == 4)continue;
			if(KeyConfigCpt == 11){
				g_KeyConfigSelection == KeyConfigCpt ? g_GLFonts.GLKeyConfigFont.ChangeFontColor(0, 255, 0) : g_GLFonts.GLKeyConfigFont.ChangeFontColor(255, 0, 0);
				g_GLFonts.GLKeyConfigFont.ChangeFontPosition(KeyConfigLeftCoord[KeyConfigCpt], KeyConfigTopCoord[KeyConfigCpt]);
				g_GLFonts.GLKeyConfigFont.glPrintTxt(KeyConfigStr[KeyConfigCpt]);
				break;
			}
			(g_KeyConfigSelection == KeyConfigCpt && !g_KeyConfigWaitKeyPress) ? g_GLFonts.GLKeyConfigFont.ChangeFontColor(0, 255, 0) : g_GLFonts.GLKeyConfigFont.ChangeFontColor(255, 0, 0);
			g_GLFonts.GLKeyConfigFont.ChangeFontPosition(KeyConfigLeftCoord[KeyConfigCpt], KeyConfigTopCoord[KeyConfigCpt]);
			g_GLFonts.GLKeyConfigFont.glPrintTxt(KeyConfigStr[KeyConfigCpt]);
			char c[128];
			sprintf(c, "%s", KeyConfigString(TmpGameKey[KeyConfigKeyToCheck[KeyConfigCpt]]));
			int cLen = strlen(c) - 1;
			(g_KeyConfigSelection == KeyConfigCpt && g_KeyConfigWaitKeyPress) ? g_GLFonts.GLKeyConfigFont.ChangeFontColor(0, 255, 0) : g_GLFonts.GLKeyConfigFont.ChangeFontColor(255, 0, 0);
			g_GLFonts.GLKeyConfigFont.Left += 280 - (cLen*FontWidth);
			g_GLFonts.GLKeyConfigFont.glPrintArg("%s", c);
		}
	}
}
	

//-----------------------------------------------------------------------------
// Name : RenderIntro()
// Desc : Render the Intro part of the game
//-----------------------------------------------------------------------------
inline void Tetrix3D::RenderIntro()
{
	static Cpt = 0;


	//3D stuffs...
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLU_INTRO_PERSPECTIVE_MACRO;
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);  
	
	// Set the viewport the whole screen
	glViewport(0, 0, g_ScreenDat.Width, g_ScreenDat.Height);// Make our viewport the whole window					
	
	//Draw Tetrix3D on the screen
	glPushMatrix();
	
	// Set the camera position
	gluLookAt(0.0f,-1.0f,30.0f, 0.0f,-1.0f,0.0f, 0.0f,1.0f,0.0f);
	
	// Enable Pointers
	glEnableClientState(GL_NORMAL_ARRAY);						// Enable Vertex Arrays
	glEnableClientState(GL_VERTEX_ARRAY);						// Enable Vertex Arrays
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);				// Enable Texture Coord Arrays

	// Bind the 3D model
	BindVertsArray(&The3DTextModel);
	BindTexVertsArray(&The3DTextModel);
	BindNormalsArray(&The3DTextModel);

	//Check if a texture will be applied to the model
	TEXTURE_SWITCH(TexID.g_3DIntroTextTexture)
		
		
	//Draw 3D(in 3d, of course...(confused?))
	glPushMatrix();
	// Make the 3D Model move	
	glTranslatef(0.0f, 0.0f, 0-(g_Intro3DTextOffset*300));
	// Make the 3D Model rotate	
	glRotatef(g_Intro3DTextOffset * 720.0f,0.0f,1.0f,0.0f);
	// Set the color to white
	glColor3f(1.0f, 1.0f, 1.0f);
	//Draw the model
	//glDrawArrays(GL_TRIANGLES, 0, The3DTextModel.NumOfVerts);	// Draw All Of The Triangles At Once
	glDrawElements(GL_TRIANGLES, The3DTextModel.IndexArraySize, GL_UNSIGNED_INT, The3DTextModel.pIndexArray);

	glPopMatrix();
				
				
	// Bind the cube model
	BindVertsArray(&BlockModel[g_CubeDetail]);
	BindTexVertsArray(&BlockModel[g_CubeDetail]);
	BindNormalsArray(&BlockModel[g_CubeDetail]);
				
	//Check if a texture will be applied to the model
	TEXTURE_SWITCH(TexID.g_CubeTexture)
					
	// Loop through each intro cube
	for(Cpt = 0; Cpt < IntroBlockStructSize; Cpt++){
		//Draw the Intro blocks
		GLChangeColor(RandomColor);
		glPushMatrix();
		//Check if the row will move from the letf or the right
		switch(IntroBlockStruct[Cpt].Positive)
		{
		case true:  glTranslatef(IntroBlockStruct[Cpt].XYZ.X + (g_IntroBlockOffset*10), IntroBlockStruct[Cpt].XYZ.Y, IntroBlockStruct[Cpt].XYZ.Z); break;
		case false:	glTranslatef(IntroBlockStruct[Cpt].XYZ.X - (g_IntroBlockOffset*10), IntroBlockStruct[Cpt].XYZ.Y, IntroBlockStruct[Cpt].XYZ.Z); break;
		}
		//Draw a cube
		//glDrawArrays(GL_TRIANGLES, 0, BlockModel[g_CubeDetail].NumOfVerts);
		glDrawElements(GL_TRIANGLES, BlockModel[g_CubeDetail].IndexArraySize, GL_UNSIGNED_INT, BlockModel[g_CubeDetail].pIndexArray);

		glPopMatrix();
	}
	
	// Disable Pointers
	glDisableClientState(GL_NORMAL_ARRAY);					// Disable Vertex Arrays
	glDisableClientState(GL_VERTEX_ARRAY);					// Disable Vertex Arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);			// Disable Texture Coord Arrays

	// End the 3d drawing 
	glPopMatrix();
	
	
	//2D Stuffs
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ORTHO2D_MACRO;
	glMatrixMode(GL_MODELVIEW);
	
	// Print Start and Option 
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);  
	glDisable(GL_TEXTURE_2D);
	
	//Draw the fps
	if(g_ShowFPS){g_GLFonts.GLFPSFont.glPrintArg("%2.2f FPS", g_FPS);}	
	
	//Draw "Start" on the bottom left of the screen
	g_IntroSelection == INTRO_START ? g_GLFonts.GLIntroFont.ChangeFontColor(glGreen3b) : g_GLFonts.GLIntroFont.ChangeFontColor(glRed3b);;
	g_GLFonts.GLIntroFont.ChangeFontPosition(48 * g_GLFonts.GLFontRatio, 62 * g_GLFonts.GLFontRatio);
	g_GLFonts.GLIntroFont.glPrintTxt(IntroStartStr);	
	//Draw "Option" on the bottom right of the screen
	g_IntroSelection == INTRO_OPTIONS ? g_GLFonts.GLIntroFont.ChangeFontColor(glGreen3b) : g_GLFonts.GLIntroFont.ChangeFontColor(glRed3b);;
	g_GLFonts.GLIntroFont.ChangeFontPosition(123 * g_GLFonts.GLFontRatio, 41 * g_GLFonts.GLFontRatio);
	g_GLFonts.GLIntroFont.glPrintTxt(IntroOptionsStr);	
	//Draw "Keyboard" on the bottom right of the screen
	g_IntroSelection == INTRO_KEYBOARD ? g_GLFonts.GLIntroFont.ChangeFontColor(glGreen3b) : g_GLFonts.GLIntroFont.ChangeFontColor(glRed3b);;
	g_GLFonts.GLIntroFont.ChangeFontPosition(194 * g_GLFonts.GLFontRatio, 20 * g_GLFonts.GLFontRatio);
	g_GLFonts.GLIntroFont.glPrintTxt(KeyboardOptionsStr);	
	
	

	#ifndef NO_BACKGROUND
	if(g_ShowBck){
		glEnable(GL_TEXTURE_2D);							
		glAlphaFunc(GL_GREATER, 0.0f);
		glEnable(GL_ALPHA_TEST);
		//bind the texture
		glBindTexture(GL_TEXTURE_2D, TexID.g_IntroBckTexture);
		
		glBegin(GL_QUADS);
		glColor3ub(glWhite3b);
		
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(g_ScreenDat.Width, 0.0f);
		
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(g_ScreenDat.Width, g_ScreenDat.Height);
		
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.0f, g_ScreenDat.Height);
		glEnd();
		glDisable(GL_ALPHA_TEST);
	}
	#endif
}


//-----------------------------------------------------------------------------
// Name : RenderDefault()
// Desc : Render the Default part of the game
//-----------------------------------------------------------------------------
inline void Tetrix3D::RenderDefault()
{
	static Cpt = 0;


	//Skip the 3d part if we are in pause
	if(g_GameState == GAME_PAUSED || g_GameState == GAME_CONFIRM_EXIT)
		goto Skip3D;
	
	//3D stuffs...
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLU_SQUARE_PERSPECTIVE_MACRO;
	glMatrixMode(GL_MODELVIEW);
	
	glEnable(GL_DEPTH_TEST);
	
	// Set the viewport square
	glViewport(0, 0, g_ScreenDat.Height, g_ScreenDat.Height);// Make our viewport the whole window					
	
	//Push the matrix
	glPushMatrix();
	
	//Set the camera position
	if(g_GameState == GAME_OVER && g_SakeCount != 0)
		gluLookAt(ShakePts.ShakingPoint[g_SakeCount % 4].X,ShakePts.ShakingPoint[g_SakeCount % 4].Y,StartingZCamPos,0,0,0,0,1,0);
	else
		gluLookAt(g_Camera.EyeX,g_Camera.EyeY,g_Camera.EyeZ, g_Camera.EyeX,g_Camera.EyeY,g_Camera.EyeZ-1, 0,1,0);
	
	//Rotate Matrix
	glRotatef(xRotation, 0.0f, 1.0f, 0.0f);
	glRotatef(yRotation, 1.0f, 0.0f, 0.0f);
	
	//draw the grid(lightning and texture must be off)
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glCallList(g_Grid.ListIndex);
	glEnable(GL_LIGHTING);
	
	// Enable Pointers
	glEnableClientState(GL_NORMAL_ARRAY);						// Enable Vertex Arrays
	glEnableClientState(GL_VERTEX_ARRAY);						// Enable Vertex Arrays
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);				// Enable Texture Coord Arrays

	// Bind the cube model
	BindVertsArray(&BlockModel[g_CubeDetail]);
	BindTexVertsArray(&BlockModel[g_CubeDetail]);
	BindNormalsArray(&BlockModel[g_CubeDetail]);
	
	//Check if a texture will be applied to the model
	TEXTURE_SWITCH(TexID.g_CubeTexture)
		
	//Draw SpaceMem
	for(Cpt = 0; Cpt < PreCalc.GameSpaceSize; Cpt++){
		GLChangeColor(pSpaceMemColor[Cpt]);
		if(pSpaceMem[Cpt] == true){
			glPushMatrix();				
			glTranslatef(pSpaceMemX[Cpt], pSpaceMemY[Cpt], pSpaceMemZ[Cpt]);
			//glDrawArrays(GL_TRIANGLES, 0, BlockModel[g_CubeDetail].NumOfVerts);	// Draw All Of The Triangles At Once
			glDrawElements(GL_TRIANGLES, BlockModel[g_CubeDetail].IndexArraySize, GL_UNSIGNED_INT, BlockModel[g_CubeDetail].pIndexArray);
			glPopMatrix();				
		}
	}
	
	//Draw current Pcx(if not 0) 
	if(!g_InRedraw && (g_GameState ==  GAME_STARTED || g_GameState ==  GAME_SUSPENDED)){
		glColor3ub(glGray3b);
		// Loop through each cube
		for(Cpt = 0; Cpt < pCurrentShape->ShapeVect.size(); Cpt++){					
			glPushMatrix();
			glTranslatef(pCurrentShape->ShapeVect[Cpt].X - PreCalc.X_RowLengthMinus1Div2, (0-(pCurrentShape->ShapeVect[Cpt].Y - PreCalc.Y_RowLengthMinus1Div2)), (pCurrentShape->ShapeVect[Cpt].Z + 0.5f));
			//glDrawArrays(GL_TRIANGLES, 0, BlockModel[g_CubeDetail].NumOfVerts);	// Draw All Of The Triangles At Once
			glDrawElements(GL_TRIANGLES, BlockModel[g_CubeDetail].IndexArraySize, GL_UNSIGNED_INT, BlockModel[g_CubeDetail].pIndexArray);
			glPopMatrix();
		}
		
		// This code will "ghost" the current pcx
		if(g_ShowGhost){
			
			// Enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);				
			
			// Loop through each cube
			for(Cpt = 0; Cpt < GhostShape.ShapeVect.size(); Cpt++){					
				glPushMatrix();
				GLChangeColor(pSpaceMemColor[XYZ_TO_I(1, 1, (int)GhostShape.ShapeVect[Cpt].Z)], 0.6666f);
				glTranslatef(GhostShape.ShapeVect[Cpt].X - PreCalc.X_RowLengthMinus1Div2, (0-(GhostShape.ShapeVect[Cpt].Y - PreCalc.Y_RowLengthMinus1Div2)), (GhostShape.ShapeVect[Cpt].Z + 0.5f));
				//glDrawArrays(GL_TRIANGLES, 0, BlockModel[g_CubeDetail].NumOfVerts);	// Draw All Of The Triangles At Once
				glDrawElements(GL_TRIANGLES, BlockModel[g_CubeDetail].IndexArraySize, GL_UNSIGNED_INT, BlockModel[g_CubeDetail].pIndexArray);
				glPopMatrix();
			}
			
			//  Disable blending
			glDisable(GL_BLEND);
		}
		
	}
	
	//Replace the camera to is origin
	if(g_GameState ==  GAME_OVER && g_SakeCount != 0)
		ResetCamPos();
	
	
	//Pop the matrix
	glPopMatrix();
	
	//This is the preview of the next block code...
	if(g_GameState ==  GAME_STARTED || g_GameState ==  GAME_SUSPENDED){
		// Set the viewport square
		glViewport(PreCalc.PreviewRect.left, PreCalc.PreviewRect.top, PreCalc.PreviewRect.right, PreCalc.PreviewRect.bottom);// Make our viewport the preview size
		//Push the matrix
		glPushMatrix();
		
		gluLookAt(0,0,6, 0,0,0, 0,1,0);
		
		glRotatef(YPreviewBlockRotation, 0.0f, 1.0f, 0.0f);
		
		glColor3ub(glGray3b);
		
		// Bind the cube model
		BindVertsArray(&BlockModel[g_CubeDetail]);
		BindTexVertsArray(&BlockModel[g_CubeDetail]);
		BindNormalsArray(&BlockModel[g_CubeDetail]);
		
		// Draw the next Pcx
		for(Cpt = 0; Cpt < pNextShape->ShapeVect.size(); Cpt++){					
			glPushMatrix();
			glTranslatef(pNextShape->NextShapeVect[Cpt].X, pNextShape->NextShapeVect[Cpt].Y, pNextShape->NextShapeVect[Cpt].Z);
			//glDrawArrays(GL_TRIANGLES, 0, BlockModel[g_CubeDetail].NumOfVerts);	// Draw All Of The Triangles At Once
			glDrawElements(GL_TRIANGLES, BlockModel[g_CubeDetail].IndexArraySize, GL_UNSIGNED_INT, BlockModel[g_CubeDetail].pIndexArray);
			glPopMatrix();
		}
		
		// Disable Pointers
		glDisableClientState(GL_NORMAL_ARRAY);					// Disable Vertex Arrays
		glDisableClientState(GL_VERTEX_ARRAY);					// Disable Vertex Arrays
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);			// Disable Texture Coord Arrays
		
		//Pop the matrix
		glPopMatrix();
	}
	
	
	Skip3D:
	//2D Stuffs
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ORTHO2D_MACRO;
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, g_ScreenDat.Width, g_ScreenDat.Height);// Make our viewport the whole window					
	
	
	// Print GL Text To The Screen
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);  
	glDisable(GL_TEXTURE_2D);
	
	// Draw the fps
	if(g_ShowFPS)
		g_GLFonts.GLFPSFont.glPrintArg("%2.2f FPS", g_FPS);
	
	// Draw message(if one(later...))
	static LPSTR DatMsgStr[] = {"Low", "Med", "Hi"};
	if(g_CurrentMessage >= 0){
		switch(g_CurrentMessage)
		{
		case MsgDetail   : g_GLFonts.GLMessageFont.glPrintArg("Detail Level: %s", DatMsgStr[g_CubeDetail]);                                                         break;
		case MsgSound    : g_SoundEnabled == false ? g_GLFonts.GLMessageFont.glPrintTxt("Sound Disabled") : g_GLFonts.GLMessageFont.glPrintTxt("Sound Enabled");    break;
		case MsgMusic    : g_MusicEnabled == false ? g_GLFonts.GLMessageFont.glPrintTxt("Music Disabled") : g_GLFonts.GLMessageFont.glPrintTxt("Music Enabled");    break;
		case MsgTexture  : g_ShowTexture == false ? g_GLFonts.GLMessageFont.glPrintTxt("Texture Disabled") : g_GLFonts.GLMessageFont.glPrintTxt("Texture Enabled"); break;
		case MsgInvMouse : g_InvertMouse == false ? g_GLFonts.GLMessageFont.glPrintTxt("Mouse Normal") : g_GLFonts.GLMessageFont.glPrintTxt("Mouse Inverted");      break;
		case MsgGhost    : g_ShowGhost == false ? g_GLFonts.GLMessageFont.glPrintTxt("Ghost Disabled") : g_GLFonts.GLMessageFont.glPrintTxt("Ghost Enabled");       break;
		}
	}


	// Draw the current score and hi score
	g_GLFonts.GLCurrentScore.glPrintArg("%6.6d", g_Lines);	
	g_GLFonts.GLHiScore.glPrintArg("%6.6d", g_HiScore);	
	
	//Draw the text that match the current game state
	switch(g_GameState)
	{
	case GAME_NOT_STARTED: 
		g_GLFonts.GLPressStartFont.glPrintTxt(PressStartStr);	
		break;
	case GAME_PAUSED: 
		g_GLFonts.GLPauseFont.glPrintTxt(PauseStr);	
		break;
	case GAME_OVER: 
		g_GLFonts.GLGameOverFont.glPrintTxt(GameOverStr);	
		break;
	case GAME_CONFIRM_EXIT: 
		GameConfirmExitSelection == 0 ? g_GLFonts.GLConfirmExitFont.ChangeFontColor(0, 255, 0) : g_GLFonts.GLConfirmExitFont.ChangeFontColor(255, 0, 0);
		g_GLFonts.GLConfirmExitFont.ChangeFontPosition(70*g_GLFonts.GLFontRatio, 145*g_GLFonts.GLFontRatio);
		g_GLFonts.GLConfirmExitFont.glPrintTxt("Reset");	
		
		GameConfirmExitSelection == 1 ? g_GLFonts.GLConfirmExitFont.ChangeFontColor(0, 255, 0) : g_GLFonts.GLConfirmExitFont.ChangeFontColor(255, 0, 0);
		g_GLFonts.GLConfirmExitFont.ChangeFontPosition(40*g_GLFonts.GLFontRatio, 105*g_GLFonts.GLFontRatio);
		g_GLFonts.GLConfirmExitFont.glPrintTxt("Main Menu");	
		
		GameConfirmExitSelection == 2 ? g_GLFonts.GLConfirmExitFont.ChangeFontColor(0, 255, 0) : g_GLFonts.GLConfirmExitFont.ChangeFontColor(255, 0, 0);
		g_GLFonts.GLConfirmExitFont.ChangeFontPosition(81*g_GLFonts.GLFontRatio, 65*g_GLFonts.GLFontRatio);
		g_GLFonts.GLConfirmExitFont.glPrintTxt("Exit");	
		
		break;
	}
	
	#ifndef NO_BACKGROUND
	if(g_ShowBck){//Cut the fucking fps a whole fucking way down!!!(not if my nVIDIa driver are installed...60 fucking FPS!!!)
		glEnable(GL_TEXTURE_2D);							
		glAlphaFunc(GL_GREATER, 0.0f);
		glEnable(GL_ALPHA_TEST);
		//bind the texture
		glBindTexture(GL_TEXTURE_2D, TexID.g_GameBckTexture);
		
		glBegin(GL_QUADS);
		glColor3ub(glWhite3b);
		
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(g_ScreenDat.Width, 0.0f);
		
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(g_ScreenDat.Width, g_ScreenDat.Height);
		
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.0f, g_ScreenDat.Height);
		glEnd();
		glDisable(GL_ALPHA_TEST);
	}
	#endif

	//
	if(Is_AI_Playing && (g_GameState == GAME_STARTED || g_GameState == GAME_OVER)){
		g_GLFonts.GLAI_MoveDone_Font.glPrintArg("Moves Played: %d", AI_MovesCounter);	
		g_GLFonts.GLAI_Score_Font.glPrintArg("Score: %d", AI_Dat.Score);	
		g_GLFonts.GLAI_RotNeeded_Font.glPrintArg("Rot: %d", AI_Dat.RotCount);	
		g_GLFonts.GLAI_MovNeeded_Font.glPrintArg("X: %d - Y: %d", AI_Dat.Offset.X, AI_Dat.Offset.Y);	
	}
}


//-----------------------------------------------------------------------------
// Name : RenderScene()
// Desc : Draw the current game state on the screen
//-----------------------------------------------------------------------------
void Tetrix3D::RenderScene()
{
	static int Cpt = 0;

	// Clear The Screen And The Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	switch(g_GameState)
	{
	/*case GAME_LOADING:
		RenderGameLoading();
		break;*/
	case GAME_OPTIONS:
		RenderGameOptions();
		break;
	case GAME_KEYCONFIG:
		RenderKeyConfig();
		break;
	case GAME_INTRO:
		RenderIntro();
		break;
		//The default drawing code
	default:
		RenderDefault();
		break;
	}
	
	if(Fader->IsFading)
		Fader->Fade();

	//swap the drawing buffer with the display one
	SwapBuffers(g_hDC);
}

//-----------------------------------------------------------------------------
// Name : UpdateScene()
// Desc : Update the game data before rendering it
//-----------------------------------------------------------------------------
void Tetrix3D::UpdateScene()
{
	//Hold the number of msec. passed since the last frame
	static float FrameInterval = 0.0f; 

	// Set and calculate the elapsed time
	g_Timer.Tick();


	// Count the FPS
	static float FPSInterval = 0.0f; 
	static int Frame = 0;	
	Frame++;
	FPSInterval += g_Timer.g_ElapsedTime;        //ABS REMOVED
	if(FPSInterval > 1.0f)
	{
		g_FPS = (float)Frame / FPSInterval;
		Frame = 0;
		while(FPSInterval >= 1.0f)
			FPSInterval -= 1.0f;
	}


	if(Fader->IsFading){
		Fader->UpdateFader(g_Timer.g_ElapsedTime);
		//g_Alpha = Fader->Alpha;
	}
	
	
	if(g_MessageTimeLeft > 0.0f){
		g_MessageTimeLeft -= g_Timer.g_ElapsedTime;
		if(g_MessageTimeLeft <= 0.0f){
			g_CurrentMessage = MsgNoMsg;
			g_MessageTimeLeft = 0.0f;
		}
	}
	
	static float YPrevInterval = 0.0f; 
	static float RotationTime = 3.0f;
	static float AngleScale = 360.0f / RotationTime;
	YPrevInterval += g_Timer.g_ElapsedTime;
	while(YPrevInterval >= RotationTime)
		YPrevInterval -= RotationTime;
	YPreviewBlockRotation = YPrevInterval * AngleScale;


	//Move the Intro Stuff!
	if(g_GameState == GAME_INTRO){
		//Move the intro block
		if(g_IntroBlockOffset > 0.0f){
			g_IntroBlockOffset -= g_Timer.g_ElapsedTime;
			if(g_IntroBlockOffset < 0.0f)
				g_IntroBlockOffset = 0.0f;
		}
		//Move the intro block
		if(g_Intro3DTextOffset > 0.0f){
			g_Intro3DTextOffset -= g_Timer.g_ElapsedTime;
			if(g_Intro3DTextOffset < 0.0f)
				g_Intro3DTextOffset = 0.0f;
		} 
		// This will tell us that the animatin is finish when we press start(or you will have to press two time enter...)
		if(g_IntroBlockOffset == 0.0f && g_Intro3DTextOffset == 0.0f)
			IsIntroAnimationFinish = true;

		//
		if(IsIntroAnimationFinish){
			if(TimeLeftBeforeAIStart > 0.0f){
				TimeLeftBeforeAIStart -= g_Timer.g_ElapsedTime;
				if(TimeLeftBeforeAIStart <= 0.0f){
					TimeLeftBeforeAIStart = 0.0f;
					AI_MovesCounter = 0;
					Is_AI_Playing = true;
					
					g_GameState = GAME_STARTED;
					g_ResetTimer = true;
					
					//OnKeyDown(13);
				}
			}
		}
	}


	//Check if we must go in gameloop
	if(g_GameState == GAME_STARTED || g_GameState == GAME_OVER){

		//add the time passed with the last value 
		FrameInterval += g_Timer.g_ElapsedTime;   //ABS REMOVED

		//do gameloop if m_ResetTimer is true and reset g_FInterval value to 0
		if(g_ResetTimer){
			FrameInterval = 0.0f;
			GameUpdate();
			g_ResetTimer = false;

			//

		}
		
		//do normal gameloop if it's the time to...
		while(FrameInterval >= g_RenderIntervalRatio){
			FrameInterval -= g_RenderIntervalRatio;
			#ifndef NO_GAME_UPDATE
				GameUpdate();
			#endif
		}
	}

}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

void Tetrix3D::PrintAIDebugInfo()
{

}

///////////////////////////////////////////////////////////////////////////////////////////

void Tetrix3D::MakeAIPlay()
{
	// Just count how many moves we've done yet
	AI_MovesCounter++;
	
	// if the last thread isn't fisished yet, this will tell it to do so, 
	// and will wait for it to die, then we reset the event...
	SetEvent(hTerminateAIThreadEvent);
	WaitForSingleObject(hMakeAIPlay, INFINITE);
	ResetEvent(hTerminateAIThreadEvent);
	//Lauch the Analyser and make it play the best move
	hMakeAIPlay = CreateThread(NULL, 0, MakeAIPlayThread, NULL, 0, &dwMakeAIPlayThreadID);	
}

///////////////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI MakeAIPlayThread(void*)
{
	DWORD WaitRes;
	HWND hWnd = pGameEngine->g_hWnd;
	int XMoveLeft = 0, YMoveLeft = 0;
	int xRot = 0, yRot = 0, zRot = 0;
	int RotationLeft = 0;

	//Gather info...
	pGameEngine->AI_Dat = pGameEngine->Analyser();
	RotationLeft = pGameEngine->AI_Dat.RotCount;
	XMoveLeft = pGameEngine->AI_Dat.Offset.X;
	YMoveLeft = pGameEngine->AI_Dat.Offset.Y;
	goto SkipWaitJmp;

	WaitJmp:
	//Sleep(WM_AI_THREAD_SLEEP_INTERVAL);   //Pause the thread
	// Make a pause...
	WaitRes = WaitForSingleObject(pGameEngine->hTerminateAIThreadEvent, WM_AI_THREAD_SLEEP_INTERVAL);
	// Stop the thread?
	if(WaitRes == WAIT_OBJECT_0)
		return 0;


	SkipWaitJmp:

	//Rotate the pcx
	if(RotationLeft > 0){
		xRot++;
		PostMessage(hWnd, WM_AI_COMMAND, 0, X_ClkWse);
		if(xRot == 4){
			xRot = 0;
			yRot++;
			PostMessage(hWnd, WM_AI_COMMAND, 0, Y_ClkWse);
			if(yRot == 4){
				yRot = 0;
				zRot++;
				PostMessage(hWnd, WM_AI_COMMAND, 0, Z_ClkWse);
			}
		}
		RotationLeft--;
		goto WaitJmp;
	}
	
	//Move the pcx Left/Right
	if(XMoveLeft < 0){
		XMoveLeft++;
		PostMessage(hWnd, WM_AI_COMMAND, 1, Dir_Left);
		goto WaitJmp;
	} else if(XMoveLeft > 0){
		XMoveLeft--;
		PostMessage(hWnd, WM_AI_COMMAND, 1, Dir_Right);
		goto WaitJmp;
	}
	//Move the pcx Up/Down
	if(YMoveLeft < 0){
		YMoveLeft++;
		PostMessage(hWnd, WM_AI_COMMAND, 1, Dir_Up);
		goto WaitJmp;
	} else if(YMoveLeft > 0){
		YMoveLeft--;
		PostMessage(hWnd, WM_AI_COMMAND, 1, Dir_Down);
		goto WaitJmp;
	}

	//Make the pcx fall down
	PostMessage(hWnd, WM_AI_COMMAND, 2, 0);

	return 0;
}




						   
