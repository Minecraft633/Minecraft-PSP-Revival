#ifndef __PGEWAV_H__
#define __PGEWAV_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	unsigned long channels;		/**<  Number of channels */
	unsigned long sampleRate;	/**<  Sample rate */
	unsigned long sampleCount;	/**<  Sample count */
	unsigned long dataLength;	/**<  Data length */
	unsigned long rateRatio;	/**<  Rate ratio (sampleRate / 44100 * 0x10000) */
	unsigned long playPtr;		/**<  Internal */
	unsigned long playPtr_frac;	/**<  Internal */
	unsigned int loop;			/**<  Loop flag */
	unsigned char *data;		/**< A pointer to the actual WAV data */
	unsigned long id;			/**<  The ID of the WAV */
	unsigned int bitPerSample;	/**<  The bit rate of the WAV */
    float leftvolume;			/**<  The left volume of the WAV */
	float rightvolume;			/**<  The right volume of the WAV */

} pgeWav;

int pgeWavInit(void);
void pgeWavShutdown(void);
pgeWav *pgeWavLoad(const char *filename);
pgeWav *pgeWavLoadMemory(const unsigned char *buffer, int size);
void pgeWavUnload(pgeWav *wav);
int pgeWavPlay(pgeWav *wav);
void pgeWavStop(pgeWav *wav);
void pgeWavStopAll(void);
void pgeWavLoop(pgeWav *wav, unsigned int loop);
void pgeWavVolume(pgeWav *wav, unsigned char leftvolume, unsigned char rightvolume);
void pgeWavPitch(pgeWav *wav, float pitch);
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __PGEWAV_H__