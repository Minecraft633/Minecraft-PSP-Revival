#ifndef AUDIO_H
#define AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSL_SOUND	
{
	char filename[64];					//!< Original file name, so that the file can be reopen after a resume from stand-by. Used only if the song is streamed.
	void *data;							//!< User sound data (depends on the sound file format).
	void *dataplus;						//!< Extended user sound data.
	int baseoffset;						//!< Offset from the beginning of the file (without the header). Used to resume the sound after stand-by.
	int format;							//!< -
	int divider;						//!< -
	int size;							//!< -
	int mono;							//!< 0x10 for Mono, 0 for Stereo output
	int isStreamed;						//!< Holds whether the sound is streamed or not.
	u16 volumeLeft;						//!< Volume of the left channel (set volumeLeft == volumeRight for normal operation, another value creates a panning effect)
	u16 volumeRight;					//!< Volume of the right channel
	int suspendNumber;					//!< -
	int (*endCallback)(struct OSL_SOUND*, int);			//!< Function called when the sound has finished to play
	u8 userdata[32];					//!< Custom user data
	int numSamples;						//!< Number of samples per read. Default is osl_audioDefaultNumSamples, that is, 512.
	
	void (*playSound)(struct OSL_SOUND*);		//!< Custom function called when the sound must be played
	void (*stopSound)(struct OSL_SOUND*);		//!< Custom function called when the sound must be stopped (not paused, completely stopped)
	int (*audioCallback)(unsigned int, void*, unsigned int);		//!< Custom function which must feed a buffer with a certain number of samples
	VIRTUAL_FILE* (*standBySound)(struct OSL_SOUND*);				//!< Function called when the PSP enters in stand by mode
	VIRTUAL_FILE** (*reactiveSound)(struct OSL_SOUND*, VIRTUAL_FILE*);		//!< Function called when the sound must be reactivated (after a stand by).
	void (*deleteSound)(struct OSL_SOUND*);		//!< Custom function called to destroy the sound.
} OSL_SOUND;

typedef struct
{
  int active;
  int threadhandle;
  int handle;
  void (*callback)(unsigned int channel, void *buf, unsigned int reqn);
  int inProgress;
} osl_audio_channelinfo;

typedef struct		
{
	void *data, *dataplus;
	int format, divider, size, mono, isStreamed;
	int numSamples;
	OSL_SOUND *sound;
} OSL_AUDIO_VOICE;

extern int  oslInitAudio();
extern void oslDeinitAudio();
extern void oslInitAudioME(int formats);
#define OSL_NUM_AUDIO_CHANNELS 8
#define OSL_VOLUME_MAX 0x8000
#define oslAudioSetDefaultSampleNumber(num)			(osl_audioDefaultNumSamples = num)
extern int osl_audioDefaultNumSamples;
extern OSL_SOUND *oslLoadSoundFile(const char *filename, int stream);
extern OSL_SOUND *oslLoadSoundFileWAV(const char *filename, int stream);
extern OSL_SOUND *oslLoadSoundFileBGM(const char *filename, int stream);
OSL_SOUND *oslLoadSoundFileMOD(const char *filename, int stream);
OSL_SOUND *oslLoadSoundFileMP3(const char *filename, int stream);
OSL_SOUND *oslLoadSoundFileAT3(const char *filename, int stream);
void oslSetModSampleRate(int freq, int stereo, int shift);
extern void oslPlaySound(OSL_SOUND *s, int voice);
extern void oslStopSound(OSL_SOUND *s);
extern void oslPauseSound(OSL_SOUND *s, int pause);
extern void oslDeleteSound(OSL_SOUND *s);
extern void oslAudioVSync();
#define oslSetSoundEndCallback(s, fct)  (s->endCallback = (fct))
#define oslSetSoundLoop(s,loop)			oslSetSoundEndCallback(s, (loop)?oslSoundLoopFunc:NULL)

enum {OSL_FMT_NONE=0};
enum {OSL_FMT_MASK=0xff};
enum {OSL_FMT_MONO=0, OSL_FMT_STEREO=0x200, OSL_FMT_STREAM=0x400};
enum {OSL_FMT_44K=0, OSL_FMT_22K=1, OSL_FMT_11K=2};
enum oslInitAudioME_formats	
{
	OSL_FMT_AT3 = 1,				//!< Atrac3 and Atrac3+
	OSL_FMT_MP3 = 2,				//!< Mpeg Audio-Layer 3
	OSL_FMT_ALL = 3,				//!< All formats
};
extern int oslGetSoundChannel(OSL_SOUND *s);
extern int oslAudioCreateChannel(int i, int format, int numSamples, OSL_SOUND *s);
extern int oslAudioRecreateChannel(int i, int format, int numSamples, OSL_SOUND *s);
extern void oslAudioDeleteChannel(int i);
extern int  oslAudioOutBlocking(unsigned int channel, unsigned int vol1, unsigned int vol2, void *buf);
typedef int (* oslAudioThreadfunc_t)(int args, void *argp);

extern volatile int osl_audioActive[OSL_NUM_AUDIO_CHANNELS], osl_audioBusy[OSL_NUM_AUDIO_CHANNELS];
extern int osl_suspendNumber;
extern OSL_AUDIO_VOICE osl_audioVoices[OSL_NUM_AUDIO_CHANNELS];
extern int oslSoundLoopFunc(OSL_SOUND *s, int voice);

#ifdef __cplusplus
}
#endif
#endif