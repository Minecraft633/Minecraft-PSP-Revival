#include <pspkernel.h>
#include <psppower.h>
#include <pspdisplay.h>
#include <pspsdk.h>
#include <psputility.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>

#include <Source/Aurora/Utils/pge.h>

#ifdef __PSP__
	PSP_MODULE_INFO("Phoenix Game Engine", PSP_MODULE_USER, 1, PGE_VERSION);
	PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER|PSP_THREAD_ATTR_VFPU);
#endif

static int running = 1;

static int pgeExitCallback(int arg1, int arg2, void *common)
{
	(void)arg1;
	(void)arg2;
	(void)common;

	running = 0;

	return 0;
}

static int pgeCallbackThread(SceSize args, void *argp)
{
	(void)args;
	(void)argp;

	int cbid;

	cbid = sceKernelCreateCallback("PgeExitCallback", pgeExitCallback, NULL);
	sceKernelRegisterExitCallback(cbid);

	sceKernelSleepThreadCB();

	return 0;
}

void pgeExit(void)
{
	running = 0;

	sceKernelExitGame();
}

void pgeDelay(unsigned int delay)
{
	sceKernelDelayThread(delay);
}

void *pgeMalloc(int size)
{
	void *ptr = malloc(size);
	return ptr;
}

void pgeFree(void *ptr)
{
	if(ptr != NULL)
		free(ptr);
}

void *pgeRealloc(void *ptr, int size)
{
	return realloc(ptr, size);
}