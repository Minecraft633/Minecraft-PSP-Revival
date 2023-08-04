#include "oslib.h"
#include <mikmod.h>
#include <mmio.h>

static int osl_modPlaying = 0;
static int osl_modInitialized = 0;
static int osl_modFrequency = 11025, osl_modShift = 0, osl_modStereo = 1;

static BOOL PSP_IsThere(void)
{
	return 1;
}

static BOOL PSP_Init(void)
{
	if (VC_Init())
		return 1;
	return 0;
}

static void PSP_Exit(void)
{
	VC_Exit();
}

static void PSP_Update(void)
{
}

static BOOL PSP_Reset(void)
{
	VC_Exit();
	return VC_Init();
}

static BOOL PSP_PlayStart(void)
{
	VC_PlayStart();
	osl_modPlaying = 1;
	return 0;
}

static void PSP_PlayStop(void)
{
	osl_modPlaying = 0;
	VC_PlayStop();
}

MDRIVER drv_psp =
{
	NULL,
	"PSP Audio",
	"PSP Output Driver v1.0 - by Jim Shaw",
	0,255,
	PSP_IsThere,
	(void*)VC_SampleLoad,
	VC_SampleUnload,
	VC_SampleSpace,
	VC_SampleLength,
	PSP_Init,
	PSP_Exit,
	PSP_Reset,
	VC_SetNumVoices,
	PSP_PlayStart,
	PSP_PlayStop,
	PSP_Update,
	VC_VoiceSetVolume,
	VC_VoiceSetFrequency,
	VC_VoiceSetPanning,
	VC_VoicePlay,
	VC_VoiceStop,
	VC_VoiceStopped,
	VC_VoiceReleaseSustain,
	VC_VoiceGetPosition,
	VC_VoiceRealVolume
};

#if 0
static void MODStop(OSL_SOUND *s)	
{
	UNIMOD *mod = (UNIMOD*)s->data;
    mod->forbid = 1;
}

static int MODActive(OSL_SOUND *s)		
{
	UNIMOD *mod = (UNIMOD*)s->data;
	return (!(mod->sngpos >= mod->numpos));
}

static void MODStart(OSL_SOUND *s)	
{
	UNIMOD *mod = (UNIMOD*)s->data;

    if(!MikMod_Active())
    {
        MikMod_EnableOutput();
    }
    mod->forbid = 0;
}
#endif

void oslAudioCallback_PlaySound_MOD(OSL_SOUND *s)	
{
	s->mono = 0x10;

	Player_Stop();
	Player_Start((UNIMOD*)s->data);
	Player_SetPosition(0);
}

void oslAudioCallback_StopSound_MOD(OSL_SOUND *s)	
{
	Player_Stop();
}

int oslAudioCallback_AudioCallback_MOD(unsigned int i, void* buf, unsigned int length)			
{
	if (osl_modPlaying)
		VC_WriteBytes(buf, length << 2);
	else
		memset(buf, 0, length << 2);

	if (!Player_Active())
		return 0;
	return 1;
}

VIRTUAL_FILE **oslAudioCallback_ReactiveSound_MOD(OSL_SOUND *s, VIRTUAL_FILE *f)		
{
	return NULL;
}

VIRTUAL_FILE *oslAudioCallback_StandBy_MOD(OSL_SOUND *s)	
{
	return NULL;
}

void oslAudioCallback_DeleteSound_MOD(OSL_SOUND *s)	
{
	MikMod_FreeSong((UNIMOD*)s->data);
}

void my_error_handler(void)
{
	return;
}

void oslSetModSampleRate(int freq, int stereo, int shift)	
{
	osl_modFrequency = freq;
	osl_modStereo = stereo;
	osl_modShift = shift;
}

OSL_SOUND *oslLoadSoundFileMOD(const char *filename, int stream)	
{
	OSL_SOUND *s;
	UNIMOD *mf;

	if (!osl_modInitialized)		
	{
		MikMod_RegisterAllLoaders();
		MikMod_RegisterDriver(drv_psp);
		MikMod_Init();

		osl_modInitialized = 1;
	}

	s = (OSL_SOUND*)malloc(sizeof(OSL_SOUND));
	if (s)			
	{
		memset(s, 0, sizeof(OSL_SOUND));
		mf = MikMod_LoadSong((char*)filename, 128);
		if (mf)		{
			s->data = (void*)mf;

			s->endCallback = NULL;
			s->volumeLeft = s->volumeRight = OSL_VOLUME_MAX;
			s->format = 0;
			s->mono = 0x10;
			s->divider = OSL_FMT_11K;
			s->isStreamed = 0;
			s->numSamples = 0;
			s->audioCallback = oslAudioCallback_AudioCallback_MOD;
			s->playSound = oslAudioCallback_PlaySound_MOD;
			s->stopSound = oslAudioCallback_StopSound_MOD;
			s->standBySound = oslAudioCallback_StandBy_MOD;
			s->reactiveSound = oslAudioCallback_ReactiveSound_MOD;
			s->deleteSound = oslAudioCallback_DeleteSound_MOD;
		}
		else		
		{
			free(s);
			s = NULL;
		}
	}
	return s;
}