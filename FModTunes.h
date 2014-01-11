#ifndef _FMOD_TUNES_H_
#define _FMOD_TUNES_H_
#ifdef __cplusplus

#pragma comment(lib, "fmodvc.lib")  // link to fmod liberary file (for visual c++)

#include "Windows.h"
#include "stdio.h"
#include <fmod.h>

// These macro tell how many tunes and sounds we have to load
#define TUNES_COUNT     3
#define SOUNDS_COUNT    8

// An array that hold the sound file name
const LPSTR SoundsName[SOUNDS_COUNT] = {
	"Busted.wav", 
	"Lines++.wav", 
	"MenuChg.wav", 
	"MenuEsc.wav", 
	"MenuItem.wav", 
	"MenuSel.wav", 
	"PcxMove.wav", 
	"Stick.wav"
};
// Some macro that define the sounds ID
#define BUSTED_SOUND    0
#define LINESINC_SOUND  1
#define MENUCHG_SOUND   2
#define MENUESC_SOUND   3
#define MENUITEM_SOUND  4
#define MENUSEL_SOUND   5
#define PCXMOVE_SOUND   6
#define STICK_SOUND     7

 
class FMod {
private:
public:    
	FMod();  // Constructor
	~FMod(); // Destructor

	int CurrentTune;        // The current tune index
	int TunesLoadedCount;   // The number of tune loaded
	bool bStopUpdateThread; // Used to force UpdateThread to break is loop

	FMUSIC_MODULE *Tounes[TUNES_COUNT];  // An array of tunes handle
	FSOUND_SAMPLE *Sounds[SOUNDS_COUNT]; // An array of sound handle

	bool InitFMod();     // Init FMod 
	void ShutDownFMod(); // Shutdown FMod

	bool LoadTunes();    // Load tunes
	void FreeTunes();    // Free tunes
	void PlayTunes();    // Play tunes
	void StopTunes();    // Stop tunes
	void ChangeTunes();  // Change tunes
	bool IsPlaying();    // Tell if a tune is playing

	bool LoadSounds();   // Load the sounds
	void FreeSounds();   // Free the sounds

	void StartUpdateThread(); // Start UpdateThread 
	void StopUpdateThread();  // Stop UpdateThread

};

#endif
#endif //_FMOD_TUNES_H_
