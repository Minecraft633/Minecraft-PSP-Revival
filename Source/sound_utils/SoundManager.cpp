#include "SoundManager.h"

using namespace Aurora;

SoundManager SoundManager::m_SoundManager;

SoundManager* SoundManager::Instance()
{
	return &m_SoundManager;
}

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
	pgeWavStopAll();

	pgeWavUnload(rainSound);
	pgeWavUnload(buttonSound);
	pgeWavUnload(tnt);
	pgeWavUnload(doorOpen);
	pgeWavUnload(doorClose);
	pgeWavUnload(breakSound);
	pgeWavUnload(bow);
	pgeWavUnload(plopSound);
	pgeWavUnload(zombieDieSound);
	pgeWavUnload(pigDieSound);
	pgeWavUnload(pigSaySound);
	pgeWavUnload(cowSaySound);
	pgeWavUnload(fuse);
	pgeWavUnload(fiss);
	pgeWavUnload(splash);
	pgeWavUnload(glass);
	pgeWavUnload(zombieSaySound);
	pgeWavUnload(shear);
	pgeWavUnload(flint);
	pgeWavUnload(xp);
	pgeWavUnload(eatSound);
	pgeWavUnload(noteSounds);
	for (char i = 0; i <= 1; i++)
	{
		pgeWavUnload(grassSounds[i]);
		pgeWavUnload(gravelSounds[i]);
		pgeWavUnload(stoneSounds[i]);
		pgeWavUnload(woodSounds[i]);
		pgeWavUnload(sandSounds[i]);
		pgeWavUnload(snowSounds[i]);

		pgeWavUnload(hitSounds[i]);

		pgeWavUnload(fallSounds[i]);
		pgeWavUnload(cowHurtSounds[i]);
		pgeWavUnload(creeperHurtSounds[i]);
		pgeWavUnload(zombieHurtSounds[i]);
		pgeWavUnload(sheepHurtSounds[i]);
	}
	for (char i = 0; i <= 2; i++)
	{
		pgeWavUnload(digSounds[i]);
	}
}

void SoundManager::Init()
{
	//init audio engine
	VirtualFileInit();
	oslInitAudio();
	pgeWavInit();

	//set default values
	currentWalkSound = 0;
	lastWalkSound = 0;
	playerSounds = true;
	lastAmbientSound = 0;
	currentAmbientSound = 0;
	ambientSoundsEnabled = true;
	srand(time(NULL));

	buttonSound = pgeWavLoad("Assets/Sounds/other/button.wav");

	tnt = pgeWavLoad("Assets/Sounds/other/tnt.wav");
	fuse = pgeWavLoad("Assets/Sounds/other/fuse.wav");
	fiss = pgeWavLoad("Assets/Sounds/other/fizz.wav");
	splash = pgeWavLoad("Assets/Sounds/other/splash.wav");
	flint = pgeWavLoad("Assets/Sounds/other/flint.wav");
	xp = pgeWavLoad("Assets/Sounds/other/experience.wav");

	doorOpen = pgeWavLoad("Assets/Sounds/other/door_open.wav");
	doorClose = pgeWavLoad("Assets/Sounds/other/door_close.wav");

	eatSound = pgeWavLoad("Assets/Sounds/eat/eat.wav");

	breakSound = pgeWavLoad("Assets/Sounds/other/break.wav");

	rainSound = pgeWavLoad("Assets/Sounds/other/rain.wav");

	bow = pgeWavLoad("Assets/Sounds/other/bow.wav");

	shear = pgeWavLoad("Assets/Sounds/mobs/sheep/shear.wav");

	//plop sound
	plopSound = pgeWavLoad("Assets/Sounds/other/plop.wav");
	pgeWavVolume(plopSound, 50, 50);

	//fall sounds
	fallSounds[0] = pgeWavLoad("Assets/Sounds/Damage/fallbig.wav");
	fallSounds[1] = pgeWavLoad("Assets/Sounds/Damage/fallsmall.wav");

	//note sounds
	noteSounds = pgeWavLoad("Assets/Sounds/other/note.wav");

	//zombie sounds
	zombieHurtSounds[0] = pgeWavLoad("Assets/Sounds/mobs/zombie/hurt1.wav");
	zombieHurtSounds[1] = pgeWavLoad("Assets/Sounds/mobs/zombie/hurt2.wav");
	zombieSaySound = pgeWavLoad("Assets/Sounds/mobs/zombie/say1.wav");
	zombieDieSound = pgeWavLoad("Assets/Sounds/mobs/zombie/death.wav");

	//cow sounds
	cowHurtSounds[0] = pgeWavLoad("Assets/Sounds/mobs/cow/hurt1.wav");
	cowHurtSounds[1] = pgeWavLoad("Assets/Sounds/mobs/cow/hurt2.wav");
	cowSaySound = pgeWavLoad("Assets/Sounds/mobs/cow/say1.wav");

	//pig sounds
	pigSaySound = pgeWavLoad("Assets/Sounds/mobs/pig/say1.wav");
	pigDieSound = pgeWavLoad("Assets/Sounds/mobs/pig/death.wav");

	//creeper sounds
	creeperHurtSounds[0] = pgeWavLoad("Assets/Sounds/mobs/creeper/hurt1.wav");
	creeperHurtSounds[1] = pgeWavLoad("Assets/Sounds/mobs/creeper/hurt2.wav");

	//sheep sounds
	sheepHurtSounds[0] = pgeWavLoad("Assets/Sounds/mobs/sheep/say1.wav");
	sheepHurtSounds[1] = pgeWavLoad("Assets/Sounds/mobs/sheep/say2.wav");

	//grass sounds
	grassSounds[0] = pgeWavLoad("Assets/Sounds/walking/grass/grass1.wav");
	grassSounds[1] = pgeWavLoad("Assets/Sounds/walking/grass/grass2.wav");

	//gravel
	gravelSounds[0] = pgeWavLoad("Assets/Sounds/walking/gravel/gravel1.wav");
	gravelSounds[1] = pgeWavLoad("Assets/Sounds/walking/gravel/gravel2.wav");

	//stone
	stoneSounds[0] = pgeWavLoad("Assets/Sounds/walking/stone/stone1.wav");
	stoneSounds[1] = pgeWavLoad("Assets/Sounds/walking/stone/stone2.wav");

	//wood
	woodSounds[0] = pgeWavLoad("Assets/Sounds/walking/wood/wood1.wav");
	woodSounds[1] = pgeWavLoad("Assets/Sounds/walking/wood/wood2.wav");

	//sand
	sandSounds[0] = pgeWavLoad("Assets/Sounds/walking/sand/sand1.wav");
	sandSounds[1] = pgeWavLoad("Assets/Sounds/walking/sand/sand2.wav");

	//snow
	snowSounds[0] = pgeWavLoad("Assets/Sounds/walking/snow/snow1.wav");
	snowSounds[1] = pgeWavLoad("Assets/Sounds/walking/snow/snow2.wav");

	//glass
	glass = pgeWavLoad("Assets/Sounds/dig/glass1.wav");

	digSounds[0] = pgeWavLoad("Assets/Sounds/dig/grass1.wav");
	digSounds[1] = pgeWavLoad("Assets/Sounds/dig/gravel1.wav");
	digSounds[2] = pgeWavLoad("Assets/Sounds/dig/stone1.wav");

	// Hit sounds
	hitSounds[0] = pgeWavLoad("Assets/Sounds/Damage/hit1.wav");
	hitSounds[1] = pgeWavLoad("Assets/Sounds/Damage/hit2.wav");

	ambientTrack = NULL;
	ambientTrackName = " ";

	diskTrack = NULL;
	diskTrackName = " ";

	delayAmbient = false;
}

void SoundManager::AudioSync()
{

	oslAudioVSync();
}

void SoundManager::PlayMenuSound()
{
	pgeWavPlay(buttonSound);
}

void SoundManager::PlayPlopSound()
{
	if (playerSounds)
		pgeWavPlay(plopSound);
}

void SoundManager::TNTSound()
{
	if (playerSounds)
		pgeWavPlay(tnt);
}

void SoundManager::PlayFuseSound()
{
	if (playerSounds)
		pgeWavPlay(fuse);
}

void SoundManager::PlayFissSound()
{
	if (playerSounds)
		pgeWavPlay(fiss);
}

void SoundManager::PlayShearSound()
{
	if (playerSounds)
		pgeWavPlay(shear);
}

void SoundManager::PlayFlintSound()
{
	if (playerSounds)
		pgeWavPlay(flint);
}

void SoundManager::PlayXPSound()
{
	if (playerSounds)
		pgeWavPlay(xp);
}

void SoundManager::PlaySplashSound()
{
	if (playerSounds)
	{
		char volume = 88 + rand() % 10;
		pgeWavVolume(splash, volume, volume);
		pgeWavPlay(splash);
	}
}

void SoundManager::PlayNoteSound(unsigned char noteType, char pitch)
{
	if (playerSounds)
	{
		pgeWavPitch(noteSounds, pitch);
		pgeWavPlay(noteSounds);
		pgeWavPitch(noteSounds, 1.0f);
	}
}

void SoundManager::PlayDoorSound(bool open)
{
	if (playerSounds)
	{
		open == true ? pgeWavPlay(doorOpen) : pgeWavPlay(doorClose);
	}
}

void SoundManager::PlayGlassSound()
{
	if (playerSounds)
		pgeWavPlay(glass);
}

void SoundManager::PlayRainSound(char volume)
{
	if (playerSounds)
	{
		pgeWavVolume(rainSound, volume, volume);
		pgeWavPlay(rainSound);
		pgeWavVolume(rainSound, 100, 100);
	}
}

void SoundManager::PlayEndDigSound(char type)
{
	if (playerSounds)
	{
		switch (type)
		{
			case 0://grass
			{
				pgeWavPlay(digSounds[0]);
			}
			break;
			case 1://gravel
			{
				pgeWavPlay(digSounds[1]);
			}
			break;
			case 2://stone
			{
				pgeWavPlay(digSounds[2]);
			}
			break;
			case 3://wood
			{
				pgeWavPlay(woodSounds[rand() % 3]);
			}
			break;
			case 4://cloth
			{
				pgeWavPlay(snowSounds[rand() % 3]);
			}
			break;
			case 5://sand
			{
				pgeWavPlay(sandSounds[rand() % 3]);
			}
			break;
			case 6://snow
			{
				pgeWavPlay(snowSounds[rand() % 3]);
			}
			break;
		}
	}
}

void SoundManager::PlayBreakSound()
{
	if (playerSounds)
		pgeWavPlay(breakSound);
}

void SoundManager::PlayBowSound()
{
	if (playerSounds)
		pgeWavPlay(bow);
}

void SoundManager::PlayEatSound()
{
	pgeWavPlay(eatSound);
}

void SoundManager::PlayZombieHurtSound(char distanceToPlayer)
{
	if (playerSounds)
	{
		char i = rand() % 2;
		if (distanceToPlayer < 15.0f)
		{
			distanceToPlayer -= 2.0f;
			if (distanceToPlayer < 0)
			{
				distanceToPlayer = 0.0f;
			}
			char volume = (15.0f - distanceToPlayer) / 15.0f * 100.0f;
			pgeWavVolume(zombieHurtSounds[i], volume, volume);
			pgeWavPlay(zombieHurtSounds[i]);
		}
	}
}

void SoundManager::PlayZombieDieSound(char distanceToPlayer)
{
	if (playerSounds)
	{
		if (distanceToPlayer < 15.0f)
		{
			distanceToPlayer -= 2.0f;
			if (distanceToPlayer < 0)
			{
				distanceToPlayer = 0.0f;
			}
			char volume = (15.0f - distanceToPlayer) / 15.0f * 100.0f;
			pgeWavVolume(zombieDieSound, volume, volume);
			pgeWavPlay(zombieDieSound);
		}
	}
}


void SoundManager::PlayPigDieSound(char distanceToPlayer)
{
	if (playerSounds)
	{
		if (distanceToPlayer < 15.0f)
		{
			distanceToPlayer -= 2.0f;
			if (distanceToPlayer < 0)
			{
				distanceToPlayer = 0.0f;
			}
			char volume = (15.0f - distanceToPlayer) / 15.0f * 100.0f;
			pgeWavVolume(pigDieSound, volume, volume);
			pgeWavPlay(pigDieSound);
		}
	}
}

void SoundManager::PlayPigSaySound(char distanceToPlayer)
{
	if (playerSounds)
	{
		if (distanceToPlayer < 15.0f)
		{
			distanceToPlayer -= 2.0f;
			if (distanceToPlayer < 0)
			{
				distanceToPlayer = 0.0f;
			}
			char volume = (15.0f - distanceToPlayer) / 15.0f * 100.0f;
			pgeWavVolume(pigSaySound, volume, volume);
			pgeWavPlay(pigSaySound);
		}
	}
}

void SoundManager::PlayCowHurtSound(char distanceToPlayer)
{
	if (playerSounds)
	{
		char i = rand() % 2;
		if (distanceToPlayer < 15.0f)
		{
			distanceToPlayer -= 2.0f;
			if (distanceToPlayer < 0)
			{
				distanceToPlayer = 0.0f;
			}
			char volume = (15.0f - distanceToPlayer) / 15.0f * 100.0f;
			pgeWavVolume(cowHurtSounds[i], volume, volume);
			pgeWavPlay(cowHurtSounds[i]);
		}
	}
}

void SoundManager::PlayCreeperHurtSound(char distanceToPlayer)
{
	if (playerSounds)
	{
		char i = rand() % 2;
		if (distanceToPlayer < 15.0f)
		{
			distanceToPlayer -= 2.0f;
			if (distanceToPlayer < 0)
			{
				distanceToPlayer = 0.0f;
			}
			char volume = (15.0f - distanceToPlayer) / 15.0f * 100.0f;
			pgeWavVolume(creeperHurtSounds[i], volume, volume);
			pgeWavPlay(creeperHurtSounds[i]);
		}
	}
}

void SoundManager::PlaySheepHurtSound(char distanceToPlayer)
{
	if (playerSounds)
	{
		char i = rand() % 2;
		if (distanceToPlayer < 15.0f)
		{
			distanceToPlayer -= 2.0f;
			if (distanceToPlayer < 0)
			{
				distanceToPlayer = 0.0f;
			}
			char volume = (15.0f - distanceToPlayer) / 15.0f * 100.0f;
			pgeWavVolume(sheepHurtSounds[i], volume, volume);
			pgeWavPlay(sheepHurtSounds[i]);
		}
	}
}

void SoundManager::PlayZombieSaySound(char distanceToPlayer)
{
	if (playerSounds)
	{
		if (distanceToPlayer < 22.0f)
		{
			distanceToPlayer -= 2.0f;
			if (distanceToPlayer < 0)
			{
				distanceToPlayer = 0.0f;
			}
			char volume = (22.0f - distanceToPlayer) / 22.0f * 100.0f;
			pgeWavVolume(zombieSaySound, volume, volume);
			pgeWavPlay(zombieSaySound);
		}
	}
}

void SoundManager::PlayCowSaySound(char distanceToPlayer)
{
	if (playerSounds)
	{
		if (distanceToPlayer < 20.0f)
		{
			distanceToPlayer -= 2.0f;
			if (distanceToPlayer < 0)
			{
				distanceToPlayer = 0.0f;
			}
			char volume = (20.0f - distanceToPlayer) / 20.0f * 100.0f;
			pgeWavVolume(cowSaySound, volume, volume);
			pgeWavPlay(cowSaySound);
		}
	}
}

void SoundManager::PlayHitSound()
{
	if (playerSounds)
	{
		pgeWavPlay(hitSounds[rand() % 2]);
	}
}

void SoundManager::PlayFallSound(char i)
{
	if (playerSounds)
	{
		if (i < -9 && i > -21)
		{
			pgeWavPlay(fallSounds[1]);
		}
		if (i < -21)
		{
			pgeWavPlay(fallSounds[0]);
		}
	}
}

void SoundManager::PlayWalkSound(char type)
{
	if (playerSounds)
	{
		switch (type)
		{
		case 0://grass
		{
			currentWalkSound = rand() % 2;
			pgeWavVolume(grassSounds[currentWalkSound], 20, 20);
			pgeWavStop(grassSounds[lastWalkSound]);
			pgeWavPlay(grassSounds[currentWalkSound]);
			lastWalkSound = currentWalkSound;
		}
		break;
		case 1://gravel
		{
			currentWalkSound = rand() % 2;
			pgeWavVolume(gravelSounds[currentWalkSound], 20, 20);
			pgeWavStop(gravelSounds[lastWalkSound]);
			pgeWavPlay(gravelSounds[currentWalkSound]);
			lastWalkSound = currentWalkSound;
		}
		break;
		case 2://stone
		{
			currentWalkSound = rand() % 2;
			pgeWavVolume(stoneSounds[currentWalkSound], 20, 20);
			pgeWavStop(stoneSounds[lastWalkSound]);
			pgeWavPlay(stoneSounds[currentWalkSound]);
			lastWalkSound = currentWalkSound;
		}
		break;
		case 3://wood
		{
			currentWalkSound = rand() % 2;
			pgeWavVolume(woodSounds[currentWalkSound], 20, 20);
			pgeWavStop(woodSounds[lastWalkSound]);
			pgeWavPlay(woodSounds[currentWalkSound]);
			lastWalkSound = currentWalkSound;
		}
		break;
		case 4://cloth
		{
			currentWalkSound = rand() % 2;
			pgeWavVolume(snowSounds[currentWalkSound], 20, 20);
			pgeWavStop(snowSounds[lastWalkSound]);
			pgeWavPlay(snowSounds[currentWalkSound]);
			lastWalkSound = currentWalkSound;
		}
		break;
		case 5://sand
		{
			currentWalkSound = rand() % 2;
			pgeWavVolume(sandSounds[currentWalkSound], 20, 20);
			pgeWavStop(sandSounds[lastWalkSound]);
			pgeWavPlay(sandSounds[currentWalkSound]);
			lastWalkSound = currentWalkSound;
		}
		break;
		case 6://snow
		{
			currentWalkSound = rand() % 2;
			pgeWavVolume(snowSounds[currentWalkSound], 20, 20);
			pgeWavStop(snowSounds[lastWalkSound]);
			pgeWavPlay(snowSounds[currentWalkSound]);
			lastWalkSound = currentWalkSound;
		}
		break;
		}
	}
}

void SoundManager::PlayDigSound(char type)
{
	if (playerSounds)
	{
		switch (type)
		{
		case 0://grass
		{
			currentWalkSound = rand() % 2;
			pgeWavVolume(grassSounds[currentWalkSound], 100, 100);
			pgeWavStop(grassSounds[lastWalkSound]);
			pgeWavPlay(grassSounds[currentWalkSound]);

			//set last sound as current
			lastWalkSound = currentWalkSound;
		}
		break;
		case 1://gravel
		{
			currentWalkSound = rand() % 2;
			pgeWavVolume(gravelSounds[currentWalkSound], 100, 100);
			pgeWavStop(gravelSounds[lastWalkSound]);
			pgeWavPlay(gravelSounds[currentWalkSound]);

			//set last sound as current
			lastWalkSound = currentWalkSound;
		}
		break;
		case 2://stone
		{
			currentWalkSound = rand() % 2;
			pgeWavVolume(stoneSounds[currentWalkSound], 100, 100);
			pgeWavStop(stoneSounds[lastWalkSound]);
			pgeWavPlay(stoneSounds[currentWalkSound]);

			//set last sound as current
			lastWalkSound = currentWalkSound;
		}
		break;
		case 3://wood
		{
			currentWalkSound = rand() % 2;
			pgeWavVolume(woodSounds[currentWalkSound], 100, 100);
			pgeWavStop(woodSounds[lastWalkSound]);
			pgeWavPlay(woodSounds[currentWalkSound]);

			//set last sound as current
			lastWalkSound = currentWalkSound;
		}
		break;
		case 4://cloth
		{
			currentWalkSound = rand() % 2;
			pgeWavVolume(snowSounds[currentWalkSound], 100, 100);
			pgeWavStop(snowSounds[lastWalkSound]);
			pgeWavPlay(snowSounds[currentWalkSound]);

			//set last sound as current
			lastWalkSound = currentWalkSound;
		}
		break;
		case 5://sand
		{
			currentWalkSound = rand() % 2;
			pgeWavVolume(sandSounds[currentWalkSound], 100, 100);
			pgeWavStop(sandSounds[lastWalkSound]);
			pgeWavPlay(sandSounds[currentWalkSound]);

			//set last sound as current
			lastWalkSound = currentWalkSound;
		}
		break;
		case 6://snow
		{
			currentWalkSound = rand() % 2;
			pgeWavVolume(snowSounds[currentWalkSound], 100, 100);
			pgeWavStop(snowSounds[lastWalkSound]);
			pgeWavPlay(snowSounds[currentWalkSound]);

			//set last sound as current
			lastWalkSound = currentWalkSound;
		}
		break;
		}
	}
}

void SoundManager::StopAmbient()
{
	if (ambientTrack != NULL)
	{
		oslStopSound(ambientTrack);
		oslDeleteSound(ambientTrack);
		ambientTrack = NULL;

		ambientTrackName = "";
	}
}

char SoundManager::PlayMenuAmbient()
{
	switch (rand() % 2)
	{
	case 0: PlayAmbient("menu1"); break;
	case 1: PlayAmbient("menu2"); break;
	}
}

char SoundManager::PlayRandomAmbient()
{
	switch (rand() % 12)
	{
	case 0: PlayAmbient("calm1"); break;
	case 1: PlayAmbient("calm2"); break;
	case 2: PlayAmbient("calm3"); break;
	case 3: PlayAmbient("piano1"); break;
	case 4: PlayAmbient("piano2"); break;
	case 5: PlayAmbient("piano3"); break;
	case 6: PlayAmbient("nuance1"); break;
	case 7: PlayAmbient("nuance2"); break;
	case 8: PlayAmbient("hal1"); break;
	case 9: PlayAmbient("hal2"); break;
	case 10: PlayAmbient("hal3"); break;
	case 11: PlayAmbient("hal4"); break;
	}
	char delay = 540 + rand() % 121;
	return delay;
}


void SoundManager::PlayAmbient(std::string name)
{
	StopAmbient();

	ambientTrackName = name;
	std::string trackPath = "Assets/Sounds/ambient/" + name + ".bgm";
	ambientTrack = oslLoadSoundFile(trackPath.c_str(), OSL_FMT_STREAM);

	oslPlaySound(ambientTrack, 2);
}

void SoundManager::StopDiskTrack()
{
	if (diskTrack != NULL)
	{
		oslStopSound(diskTrack);
		oslDeleteSound(diskTrack);
		diskTrack = NULL;

		diskTrackName = "";
	}
}

void SoundManager::PlayDiskTrack(char diskNumber)
{
	StopAmbient();
	StopDiskTrack();

	std::string name = "";
	switch (diskNumber)
	{
	case 0: name = "13"; break;
	case 1: name = "cat"; break;
	case 2: name = "blocks"; break;
	case 3: name = "chirp"; break;
	case 4: name = "far"; break;
	case 5: name = "mall"; break;
	case 6: name = "mellohi"; break;
	case 7: name = "stal"; break;
	case 8: name = "strad"; break;
	case 9: name = "ward"; break;
	case 10: name = "11"; break;
	case 11: name = "wait"; break;
	}
	diskTrackName = name;
	std::string trackPath = "Assets/Sounds/disk/" + name + ".bgm";
	diskTrack = oslLoadSoundFile(trackPath.c_str(), OSL_FMT_STREAM);

	oslPlaySound(diskTrack, 2);
}