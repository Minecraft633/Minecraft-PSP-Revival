#ifndef LOADINGSCREEN_H_
#define LOADINGSCREEN_H_

#include <Source/Aurora/Graphics/RenderManager.h>
#include <Source/Aurora/Graphics/Sprite.h>
#include <Source/Aurora/Utils/Timer.h>
#include "TextureHelper.h"

using namespace Aurora::Graphics;
using namespace Aurora::Utils;

class SplashScreen
{
public:

	SplashScreen(const char* filename,int startW,int startH,int endW,int endH,float splashTime);
	SplashScreen(int textureNumber,int startW,int startH,int endW,int endH,float splashTime);
	~SplashScreen();

	void ShowSplash();

private:

	float finishTime;
	float timer;
	float dt;
	Sprite* sprite;
	Timer mTimer;
};
#endif