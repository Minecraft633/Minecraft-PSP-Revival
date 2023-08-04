#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

extern "C"
{
#include "sound_utils/pgeWav.h"
#include "sound_utils/oslib.h"
}

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <Aurora/Utils/Logger.h>

#include<fstream>
#include<iostream>

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	static SoundManager m_SoundManager;
	static SoundManager* Instance();

	void Init();
	void AudioSync();

	void PlayBreakSound();
	void PlayHitSound();
	void PlayEatSound();

	void PlayZombieHurtSound(char distanceToPlayer);
	void PlayZombieDieSound(char distanceToPlayer);
	void PlayZombieSaySound(char distanceToPlayer);

	void PlayCowHurtSound(char distanceToPlayer);
	void PlayCowSaySound(char distanceToPlayer);

	void PlayPigSaySound(char distanceToPlayer);
	void PlayPigDieSound(char distanceToPlayer);

	void PlayCreeperHurtSound(char distanceToPlayer);
	void PlaySheepHurtSound(char distanceToPlayer);

	void PlayRainSound(char volume);

	void PlayFissSound();
	void PlaySplashSound();
	void PlayFuseSound();
	void PlayShearSound();
	void PlayFlintSound();
	void PlayXPSound();
	void PlayBowSound();
	void PlayMenuSound();
	void PlayPlopSound();
	void PlayWalkSound(char type);
	void PlayDigSound(char type);
	void PlayEndDigSound(char type);
	void TNTSound();
	void PlayDoorSound(bool open);
	void PlayFallSound(char i);
	void PlayGlassSound();
	void PlayNoteSound(unsigned char noteType, char pitch);

	void PlayDiskTrack(char diskNumber);
	void StopDiskTrack();

	char PlayRandomAmbient();
	char PlayMenuAmbient();

	void StopAmbient();
	void PlayAmbient(std::string name);

	bool playerSounds;
	bool ambientSoundsEnabled;

	char currentAmbientSound;
	char lastAmbientSound;

	bool delayAmbient;

private:

	char currentWalkSound;
	char lastWalkSound;

	pgeWav* buttonSound;
	pgeWav* plopSound;

	pgeWav* grassSounds[2];
	pgeWav* gravelSounds[2];
	pgeWav* stoneSounds[2];
	pgeWav* woodSounds[2];
	pgeWav* sandSounds[2];
	pgeWav* snowSounds[2];

	pgeWav* eatSound;

	pgeWav* hitSounds[2];
	pgeWav* fallSounds[2];

	pgeWav* zombieHurtSounds[2];
	pgeWav* zombieSaySound;
	pgeWav* zombieDieSound;

	pgeWav* cowHurtSounds[2];
	pgeWav* cowSaySound;

	pgeWav* pigSaySound;
	pgeWav* pigDieSound;

	pgeWav* creeperHurtSounds[2];

	pgeWav* sheepHurtSounds[2];

	pgeWav* doorOpen;
	pgeWav* doorClose;
	pgeWav* tnt;
	pgeWav* bow;
	pgeWav* breakSound;
	pgeWav* fuse;
	pgeWav* fiss;
	pgeWav* splash;
	pgeWav* glass;
	pgeWav* shear;
	pgeWav* flint;
	pgeWav* xp;
	pgeWav* noteSounds;
	pgeWav* rainSound;
	pgeWav* digSounds[3];

	OSL_SOUND* ambientTrack;
	std::string ambientTrackName;

	OSL_SOUND* diskTrack;
	std::string diskTrackName;
};
#endif