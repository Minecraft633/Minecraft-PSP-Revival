#ifndef common_vram_h
#define common_vram_h

#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

void* getStaticVramBuffer(unsigned int width, unsigned int height, unsigned int psm);
void* getStaticVramTexture(unsigned int width, unsigned int height, unsigned int psm);

#ifdef __cplusplus
}
#endif
#endif