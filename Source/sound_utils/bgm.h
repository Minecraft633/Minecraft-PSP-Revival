#ifndef _OSL_BGM_H_
#define _OSL_BGM_H_

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct
	{
		char strVersion[11];
		int format;
		int sampleRate;
		unsigned char nbChannels;
		unsigned char reserved[32];
	} BGM_FORMAT_HEADER;

#ifdef __cplusplus
}
#endif
#endif