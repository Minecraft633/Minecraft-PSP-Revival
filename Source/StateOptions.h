#ifndef STATEOPTIONS_H_
#define STATEOPTIONS_H_

#include <stdlib.h>

#include <Source/Aurora/Utils/StateManager.h>
#include <Source/Aurora/Utils/GameState.h>

#include <Source/Aurora/Graphics/RenderManager.h>
#include <Source/Aurora/Utils/Logger.h>
#include <Source/Aurora/Utils/Timer.h>
#include <Source/Aurora/System/SystemManager.h>
#include <Source/Aurora/Graphics/Models/ObjModel.h>
#include <Source/Aurora/Graphics/Camera.h>
#include <Source/Aurora/Graphics/Sprite.h>

#include "SoundManager.h"
#include "CraftWorld.h"

using namespace Aurora::Graphics;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora;

class StateOptions : public CGameState
{
public:
	StateOptions();
	virtual ~StateOptions();

	void Init();
	void Enter();
	void CleanUp();

	void Pause();
	void Resume();

	void HandleEvents(StateManager* sManager);
	void Update(StateManager* sManager);
	void Draw(StateManager* sManager);
	void DrawText(int x, int y, unsigned int color, float size, const char* message, ...);

private:
	Sprite* menu1Sprite;
	Sprite* backgroundSprite;
	float bx;
	bool background;
	bool directionx;
	Sprite* smbuttonSprite;
	Sprite* dark_backgroundSprite;
	Sprite* buttonSprite;
	Sprite* sbuttonSprite;
	Sprite* nbuttonSprite;
	Sprite* buttonSmallSprite;
	Sprite* sbuttonSmallSprite;
	Sprite* buttonLargeSprite;
	Sprite* sbuttonLargeSprite;
	Sprite* moverSprite;
	Sprite* backSprite;

	RenderManager* mRender;
	SystemManager* mSystemMgr;
	SoundManager* mSoundMgr;

	int selectPos;
	int menuState;//0 main,1 load,2 options

	//controls
	int controlPos;
	int controlStart;
	int controlEnd;

	//choosing key
	bool chooseKeyState;
	bool configChanged;
	int currentKey;
	int newKey;

	//analog stick
	int currentAnalogPos;
};
#endif