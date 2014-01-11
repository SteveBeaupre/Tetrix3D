#include "FModTunes.h"

FMod *pTunes;

DWORD WINAPI UpdateThread(void*);
DWORD dwUpdateThreadID;
HANDLE UpdateThreadHandle;


FMod::FMod()
{
	//VAriable Init...
	pTunes = this;
	for(int Cpt = 0; Cpt < SOUNDS_COUNT; Cpt++)
		Sounds[Cpt] = NULL;
	CurrentTune = 2;
	TunesLoadedCount = 0;
	bStopUpdateThread = false;
}

FMod::~FMod()
{

}

bool FMod::InitFMod()
{
	// Init FMod
	if(!FSOUND_Init(44100, 32, 0))
		return false;
	return true;
}

void FMod::ShutDownFMod()
{
	// Close FSound 
    FSOUND_Close();
}

bool FMod::LoadSounds()
{
	// Load all the sounds
	for(int Cpt = 0; Cpt < SOUNDS_COUNT; Cpt++){
		// Format the sound file name
		char c[1024];
		sprintf(c, "SoundFX\\%s", SoundsName[Cpt]);
		// Load the sound
		Sounds[Cpt] = FSOUND_Sample_Load(Cpt, c, FSOUND_NORMAL, 0, 0);
		//If something wrong's...
		if(!Sounds[Cpt])
			return false;
	}
	return true;
}

void FMod::FreeSounds()
{
	//Stop all the sounds from playing
	FSOUND_StopSound(FSOUND_ALL);
	//Free all sounds from memory
	for(int Cpt = 0; Cpt < SOUNDS_COUNT; Cpt++)
		if(Sounds[Cpt])FSOUND_Sample_Free(Sounds[Cpt]);	
}

bool FMod::LoadTunes()
{
	// Reset the tune loaded counter
	TunesLoadedCount = 0;
	// Load the tunes 
	for(int Cpt = 0; Cpt < TUNES_COUNT; Cpt++){
		// Format the tune name
		char c[256];
		sprintf(c, "SoundTrack\\Toune%2.2d.mid", Cpt);
		// Load a tune
		Tounes[Cpt] = FMUSIC_LoadSong(c);
		// If something wrong's...
		if(!Tounes[Cpt])
			return false;
		// Tell that we dont want to loop our tune
		FMUSIC_SetLooping(Tounes[Cpt], false);
		// Inc the tune loaded counter
		TunesLoadedCount++;
	}

	return true;
}

void FMod::FreeTunes()
{
	// Stop all midi file before shutdown
	FMUSIC_StopAllSongs();
	// Clear all the tunes from memory
	for(int Cpt = 0; Cpt < TUNES_COUNT; Cpt++)
		FMUSIC_FreeSong(Tounes[Cpt]);
	// Reset the tune loaded counter
	TunesLoadedCount = 0;
}

void FMod::PlayTunes()
{
	// If this tune aren't playing now
	if(!FMUSIC_IsPlaying(Tounes[CurrentTune]))
		// Play the tune
		FMUSIC_PlaySong(Tounes[CurrentTune]);
}

void FMod::StopTunes()
{
	// If this tune aren't playing now
	if(FMUSIC_IsPlaying(Tounes[CurrentTune]))
		// Play the tune
		FMUSIC_StopSong(Tounes[CurrentTune]);
}

void FMod::ChangeTunes()
{
	// Stop the tune 
	FMUSIC_StopAllSongs();
	// Inc CurrentTune or set it to zero
	CurrentTune < (TUNES_COUNT-1) ? CurrentTune++ : CurrentTune = 0;
	// Play the tune
	FMUSIC_PlaySong(Tounes[CurrentTune]);
}

bool FMod::IsPlaying()
{
	//Just check if a tune is playing
	if(!FMUSIC_IsPlaying(Tounes[CurrentTune]))
		return false;
	return true;
}

void FMod::StartUpdateThread()
{
	// If the thread is not active and there is at least 1 tune to play...
	if(!UpdateThreadHandle && TunesLoadedCount > 0)
		// Creaate the thread
		UpdateThreadHandle = CreateThread(NULL, 0, UpdateThread, NULL, 0, &dwUpdateThreadID);
}

void FMod::StopUpdateThread()
{
	// If the thread is active
	if(UpdateThreadHandle){
		// This will stop the thread
		bStopUpdateThread = true;
		// Wait for the thread to return 
		WaitForSingleObject(UpdateThreadHandle, INFINITE);
		// Close the thread handle
		CloseHandle(UpdateThreadHandle);
		// Crear the thread handle
		UpdateThreadHandle = NULL;
	}

}

DWORD WINAPI UpdateThread(void*)
{
	// Set bStopUpdateThread to false 
	pTunes->bStopUpdateThread = false;
	
	// This will loop till we set bStopUpdateThread to true
	while(!pTunes->bStopUpdateThread){
		// If the tune is finished
		if(FMUSIC_IsFinished(pTunes->Tounes[pTunes->CurrentTune])){
			// Stop all songs
			FMUSIC_StopAllSongs();
			// Wait 2 seconds
			Sleep(2000);
			// Change the tune
			pTunes->ChangeTunes();		
			// Play the tune
			pTunes->PlayTunes();		
		}
	}

	return 0;
}
