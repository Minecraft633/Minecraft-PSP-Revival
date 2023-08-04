#include <Source/Aurora/Graphics/vram.h>
#include <pspge.h>
#include <pspgu.h>
#include <stdio.h>

#define __MEM_SIZE 0x00200000
#define __MEM_START 0x04000000
#define __BLOCK_SIZE 512
#define __MEM_BLOCKS (__MEM_SIZE/__BLOCK_SIZE)
#define __BLOCKS(x) ((x+__BLOCK_SIZE-1)/__BLOCK_SIZE)
#define __BLOCKSIZE(x) ((x+__BLOCK_SIZE-1)&~(__BLOCK_SIZE-1))
#define __BLOCK_GET_SIZE(x)    ((x & 0x7FFF))
#define __BLOCK_GET_PREV(x)    ((x >> 15) & 0x7FFF)
#define __BLOCK_GET_FREE(x)    ((x >> 31))
#define __BLOCK_GET_BLOCK(x)   ((x >> 30) & 0x1)
#define __BLOCK_SET_SIZE(x,y)  x=((x & ~0x7FFF) | ((y) & 0x7FFF))
#define __BLOCK_ADD_SIZE(x,y)  x=((x & ~0x7FFF) | (((x & 0x7FFF)+((y) & 0x7FFF)) & 0x7FFF))
#define __BLOCK_SET_PREV(x,y)  x=((x & ~0x3FFF8000) | (((y) & 0x7FFF)<<15))
#define __BLOCK_SET_FREE(x,y)  x=((x & 0x7FFFFFFF) | (((y) & 0x1)<<31))
#define __BLOCK_SET_BLOCK(x,y) x=((x & 0xBFFFFFFF) | (((y) & 0x1)<<30))
#define __BLOCK_MAKE(s,p,f,n)   (((f & 0x1)<<31) | ((n & 0x1)<<30) | (((p) & 0x7FFF)<<15) | ((s) & 0x7FFF))
#define __BLOCK_GET_FREEBLOCK(x) ((x>>30) & 0x3)
#define __BLOCK0 ((__MEM_BLOCKS) | (1<<31) | (1<<30))

static unsigned int staticOffset = 0;

static unsigned int getMemorySize(unsigned int width, unsigned int height, unsigned int psm)
{
	switch (psm)
	{
		case GU_PSM_T4:
			return (width * height) >> 1;

		case GU_PSM_T8:
			return width * height;

		case GU_PSM_5650:
		case GU_PSM_5551:
		case GU_PSM_4444:
		case GU_PSM_T16:
			return 2 * width * height;

		case GU_PSM_8888:
		case GU_PSM_T32:
			return 4 * width * height;

		default:
			return 0;
	}
}

void* getStaticVramBuffer(unsigned int width, unsigned int height, unsigned int psm)
{
	unsigned int memSize = getMemorySize(width,height,psm);
	void* result = (void*)staticOffset;
	staticOffset += memSize;

	return result;
}

void* getStaticVramTexture(unsigned int width, unsigned int height, unsigned int psm)
{
	void* result = getStaticVramBuffer(width,height,psm);
	return (void*)(((unsigned int)result) + ((unsigned int)sceGeEdramGetAddr()));
}