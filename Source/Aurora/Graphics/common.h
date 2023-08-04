#ifndef AURORA_COMMONPSP_H
#define AURORA_COMMONPSP_H

#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>

#ifdef __cplusplus
extern "C"
{
#endif
int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks(void);

#ifdef __cplusplus
}
#endif
#endif