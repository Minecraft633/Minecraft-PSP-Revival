#include "LoadingScreen.h"
#include "TextureHelper.h"
#include <pspthreadman.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspdisplay.h>

#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Graphics/Sprite.h>

#define normal_size 0.44

using namespace Aurora::Graphics;

Sprite* backgroundSprite;
float bx;
bool directionx;
Sprite* loadingSprite;
Sprite* loadSprite;

int LoadingScreen::readiness = 0;
int LoadingScreen::stateName = 0;

LoadingScreen::LoadingScreen()
{
	// the loadingscreen is loaded as a thread
	thid_ = sceKernelCreateThread("LoadingThread", RunLoadingScreen, 0x18, 0x10000, THREAD_ATTR_VFPU | THREAD_ATTR_USER, NULL);
	// start the thread
	sceKernelStartThread(thid_, 0, 0);
}


void LoadingScreen::KillLoadingScreen()
{
	// shut down the loading screen again.
	sceKernelTerminateDeleteThread(thid_);
	// free the mem space of the images
	delete backgroundSprite;
	delete loadSprite;
	delete loadingSprite;
}


int LoadingScreen::RunLoadingScreen(SceSize args, void* argp)
{
	loadingSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Loading));//200
	loadingSprite->SetPosition(240, 136);

	loadSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Load), 0, 0, 16, 8);
	loadSprite->Scale(32, 1.2f);

	backgroundSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::background));
	backgroundSprite->Scale(4.25f, 4.25f);
	backgroundSprite->SetPosition(240, 136);

	int tip = rand() % 5;
	float loadingProcess = 0.0f;

	while (1)
	{
		RenderManager::InstancePtr()->StartFrame(0.466, 0.72, 1);

		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));

		if (readiness == 0)
		{
			if (loadingProcess < 160)
			{
				loadingProcess += 0.65f;
			}
		}
		else
		{
			if (loadingProcess <= readiness / 100.0f * 160)
			{
				loadingProcess += 0.65f;
			}
		}
		bool background = true;
		if (background == true)
		{
			if (bx <= -515)
			{
				bx = 1020;
			}
			bx -= 1.0f / 6.0f;

			backgroundSprite->SetPosition(bx, 136);
			backgroundSprite->DrawLinear();
		}
		loadSprite->NewScale(loadingProcess);
		loadSprite->SetPosition(284, 120.5f);
		loadingSprite->Draw();
		loadSprite->Draw();

		sceGuDisable(GU_BLEND);
		sceGuEnable(GU_DEPTH_TEST);

		RenderManager::InstancePtr()->SetFontStyle(normal_size, 0xFFFFFFFF, 0, 0x00000200 | 0x00000000);
		switch (stateName)
		{
		case 0:
			RenderManager::InstancePtr()->DebugPrint(113, 120, "Loading spawn area");
			break;
		case 1:
			RenderManager::InstancePtr()->DebugPrint(189, 120, "Finding Seed for the World Generator", (readiness * 2) - 1);
			break;
		}
		switch (tip)
		{
		case 0:
			RenderManager::InstancePtr()->DebugPrint(240, 205, "DON'T LEAVE TREES FLOATING!");
			break;
		case 1:
			RenderManager::InstancePtr()->DebugPrint(240, 205, "Use a bed to set your respawn point during");
			RenderManager::InstancePtr()->DebugPrint(240, 220, "the day.");
			break;
		case 2:
			RenderManager::InstancePtr()->DebugPrint(240, 205, "MINECON takes place every year! Look out");
			RenderManager::InstancePtr()->DebugPrint(240, 220, "for the next one!");
			break;
		case 3:
			RenderManager::InstancePtr()->DebugPrint(240, 205, "Protect baby turtles from hostile mobs!");
			break;
		case 4:
			RenderManager::InstancePtr()->DebugPrint(240, 205, "Emerald ores are the rarest ores in Minecraft!");
			RenderManager::InstancePtr()->DebugPrint(240, 220, "They can only be found in extreme hill biomes!");
			break;
		case 5:
			RenderManager::InstancePtr()->DebugPrint(240, 205, "Before you mine diamonds, redstone or gold,");
			RenderManager::InstancePtr()->DebugPrint(240, 220, "make sure you're using an iron or diamond");
			RenderManager::InstancePtr()->DebugPrint(240, 235, "pickaxe or the ore won't drop.");
			break;
		}			
		RenderManager::InstancePtr()->EndFrame();
	}
	return 0;
}