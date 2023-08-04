#ifndef STATEPLAYCREATIVE_H_
#define STATEPLAYCREATIVE_H_

#include <Aurora/Utils/StateManager.h>
#include <Aurora/Utils/GameState.h>

#include <math.h>
#include <psptypes.h>

#include <Source/Aurora/Graphics/RenderManager.h>
#include <Source/Aurora/Utils/Logger.h>
#include <Source/Aurora/Utils/Timer.h>
#include <Source/Aurora/System/SystemManager.h>
#include <Source/Aurora/Graphics/Models/ObjModel.h>
#include <Source/Aurora/Graphics/Camera.h>
#include <Source/Aurora/Graphics/Sprite.h>

#include <Source/Aurora/Graphics/Effects/SkyLight.h>
#include <Source/Aurora/Graphics/Effects/SnowBall.h>
#include <Source/Aurora/Graphics/Effects/SkyBox.h>
#include <Source/Aurora/Graphics/Effects/Destroyer.h>
#include <Source/Aurora/Graphics/Effects/StarsBox.h>
#include <Source/Aurora/Graphics/Effects/Drop.h>
#include <Source/Aurora/Graphics/Effects/TNT.h>
#include <Source/Aurora/Graphics/Effects/Particle.h>

#include "ChunkDataProvider.h"

#include "Mobs/Zombie.h"
#include "Mobs/Cow.h"
#include "Mobs/PIG.h"
#include "Mobs/Creeper.h"
#include "Mobs/Sheep.h"
#include "Mobs/Mob.h"

#include "StateOptions.h"
#include "CraftWorld.h"
#include "InputHelper.h"
#include "SoundManager.h"
#include "Chest.h"
#include "Furnace.h"
#include "NoteBlockEntity.h"
#include "JukeboxEntity.h"
#include "MonsterSpawnerEntity.h"
#include "ItemFrameEntity.h"

#include "Functions.h"

using namespace Aurora::Graphics;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora;

class StatePlayCreative : public CGameState
{
public:
	StatePlayCreative();
	virtual ~StatePlayCreative();

	void Init();
	void InitParametric(bool makeTrees, bool makeWater, bool makeCaves, unsigned int seedIII, int worldType, char gameMode);
	void Enter();
	void CleanUp();

	void Pause();
	void Resume();
	void LoadTextures();

	void HandleEvents(StateManager* sManager);
	void Update(StateManager* sManager);
	void Draw(StateManager* sManager);

	void LoadMap(std::string fileName, bool compressed);
	void SetWorldAndSaveName(std::string worldName, std::string fileName);
	void InitCamera();

	//keys helpers
	bool keyPressed(int currentKey);
	bool keyHold(int currentKey);
	void PutInInventory(int id, int num, bool st);

private:

	void advancedBlit(int sx, int sy, int sw, int sh, int dx, int dy, int slice);
	bool TryToMove(Vector3 moveVector, float dt);
	void CraftItem2x2();
	void CraftItem3x3();

	void CheckForFurnFuel(Furnace* Fur);
	void CheckForFurnWorking(Furnace* Fur);
	void ReadyFurnSmelting(Furnace* Fur);

	int FindFurnaceId(int x, int y, int z);
	int FindChestId(int x, int y, int z);
private:
	StateOptions* mOptions;

	RenderManager* mRender;
	SystemManager* mSystemMgr;
	SoundManager* mSoundMgr;
	InputHelper* mIhelper;
	Camera* fppCam;

	std::string saveFileName;
	int freeMemory;
	float freeMemoryTimer;

	float cloudsOffset;

	CraftWorld* mWorld;

	ObjModel* Far_Land;
	SkyLight* skyLight;
	SkyLight* skyMoonLight;
	SkyBox* skyBox;
	Destroyer* destroyer;
	StarsBox* starsBox;

	//short WORLD_SIZE;

	float starsAngle;
	bool check;
	bool cycle;

	bool startDt;
	float dT;
	float dET;
	char dStd;

	bool StartSprint;
	bool hurt;
	float hurt_time;
	float dtt;

	float furnaceTimes;

	Vector3 testPos1;

	Vector3 cubePos;
	bool showCube;

	float sleepMessageTime;
	int sleepMessageType;
	float sleepTime;

	short craftSlotId[4];
	short craftSlotAm[4];
	bool craftSlotSt[4];

	short craftSlotId3[9];
	short craftSlotAm3[9];
	bool craftSlotSt3[9];

	short creativeSlotId[27];

	short craftItemId;
	short craftItemAm;
	bool craftItemSt;

	short craftItemId3;
	short craftItemAm3;
	bool craftItemSt3;

	float musicTimeGo;
	float musicTime;

	char chestId;
	char furnaceId;

	Chest* UseChest;
	Furnace* UseFurnace;

	float GRAVITY;
	float JUMPVELOCITY;
	float CLIMBVELOCITY;

	Vector3 playerVelocity;
	Vector3 playerPosition;
	Vector3 newPlayerPos;
	Vector3 oldPlayerPos;

	bool moveForward;
	bool walkingOnGround;
	bool moving;
	bool jumping;
	bool headInWater;
	bool footInWater;
	bool headInLava;
	bool footInLava;

	bool invEn;
	bool upEn;
	bool craft3xEn;
	bool chestEn;
	bool furnaceEn;

	bool usingSlider; // craive vars
	bool onDestroySlot;

	bool showSlotName;
	float showSlotTimer;
	float showSlotTimerMax;
	int slotId;

	char barPosition;
	char invXPosition;
	char invYPosition;

	char tempXFurnace;
	char tempYFurnace;
	bool tempUpFurnace;

	char tempXInv;
	char tempYInv;
	bool tempUpInv;

	char tempXCT;
	char tempYCT;
	bool tempUpCT;

	char tempXChest;
	char tempYChest;
	bool tempUpChest;

	int ram1;
	int ram2;

	float tickPhysics;
	float tickCave;
	float tickHunger;
	float tickHealth;
	float tickOS;
	float tickChunk;
	float tickFlymode;
	float tickSprintmode;
	float tickShowSlotName;
	float rainSoundFrequency;

	float inputDiskNameTimer;
	std::string inputDiskName;

	short chunks; // 0 - 7;

	int chunkId;

	Timer mTimer;
	float dt;

	int barItems;
	int texture;
	int texture_mips2;
	int texture_mips3;
	int waterAnimation;

	unsigned char blue;
	unsigned char red;
	unsigned char black;
	unsigned char stars;

	int zombieTex;
	int cowTex;
	int snowBall4;
	int clouds1Tex;
	int clouds2Tex;
	int suntex;
	int moontex;
	int grassColorTex;

	unsigned char currentTexture;
	unsigned char timeTexture;

	float pre_fps;
	int average_fps;
	float tick_fps;
	int ticks;
	Sprite* waterScreen;

	Sprite* gameSprite;
	Sprite* menuSprite;
	Sprite* invCellSprite;
	Sprite* invSprite;
	Sprite* sliderSprite;
	Sprite* sliderLightedSprite;
	Sprite* crtSprite;
	Sprite* chtSprite;
	Sprite* selectInvSprite;
	Sprite* barSprite;
	Sprite* crossSprite;
	Sprite* selectSprite;

	Sprite* furArrowSprite[22];
	Sprite* furFireSprite[14];
	Sprite* furSprite;
	Sprite* toolPointSprite[14];

	float cameraSpeed;
	float cameraMoveSpeed;

	//menu sprites
	Sprite* buttonSprite;
	Sprite* sbuttonSprite;
	Sprite* nbuttonSprite;
	Sprite* moverSprite;

	int menuState;//0 game,1 menu
	bool menuOptions;
	int optionsMenuPos;
	int selectPos;

	char statisticsPage;
	//some settings
	bool canFly;
	bool canSprint;
	bool canCrouch;
	bool devMode;
	bool analogLeft, analogRight, analogUp, analogDown;

	//for sound of walkinng
	float walkSoundAccu;
	bool  isWalking;

	//Headbob
	bool canHeadBob;
	float bobCycle;
	bool bobType;
	float cubeLight;

	double shift_x;
	double shift_y;
	double changeY;

	float animDest;
	float animSpeed;
	bool animGo;

	bool anim[3];
	bool makeScreen;
	bool dieFactor;

	float angleFactor;

	void InitCreativeInventory();

	void DrawText(int x, int y, unsigned int color, float size, const char* message, ...);
	void DrawText2(int x, int y, unsigned int color, float size, const char* message, ...);

	std::vector<SnowBall2*> mSnowBalls;
	std::vector<int> inventoryItems;
	std::vector<Particle*> mParticles;

	int creativePage;
	int creativePageMax;
};
#endif