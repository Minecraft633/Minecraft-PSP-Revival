#ifndef STATEPLAY_H_
#define STATEPLAY_H_

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

#include <Source/Aurora/Graphics/Effects/InventoryPlayer.h>
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

class StatePlay : public CGameState
{
public:
	StatePlay();
	virtual ~StatePlay();

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
	void SetDayTimeAfterLoad();
	void CraftItem2x2();
	void CraftItem3x3();

	void CheckForFurnFuel(Furnace* Fur);
	void CheckForFurnWorking(Furnace* Fur);
	void ReadyFurnSmelting(Furnace* Fur);

	int FindFurnaceId(int x, int y, int z);
	int FindChestId(int x, int y, int z);

	void HungerTime();
	void HealthTime();
	void OxygenTime();
	void HurtPlayer(float damage);
	void HurtPlayerConsiderArmor(float damage);

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
	InventoryPlayer* invPlayer;

	double temp_total_time_ms;

	//short WORLD_SIZE;

	bool cycle;
	bool loadReady;
	bool startDt;
	bool tryToDt;
	float dT;
	float dET;
	char dStd;
	float last_HP;

	float sleepMessageTime;
	int sleepMessageType;
	float sleepTime;

	bool StartSprint;
	bool hurt;
	float hurt_time;
	float dtt;

	float furnaceTimes;

	Vector3 testPos1;
	Vector3 cubePos;

	bool showCube;

	short craftSlotId[4];
	short craftSlotAm[4];
	bool craftSlotSt[4];

	short craftSlotId3[9];
	short craftSlotAm3[9];
	bool craftSlotSt3[9];

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
	bool moving;
	bool walkingOnGround;
	bool jumping;
	bool headInWater;
	bool footInWater;
	bool headInLava;
	bool footInLava;
	bool touchingCactus;

	bool invEn;
	bool upEn;
	bool armorEn;
	bool craft3xEn;
	bool chestEn;
	bool furnaceEn;

	char barPosition;
	char invXPosition;
	char invYPosition;
	float invSteveAngle;

	int ram1;
	int ram2;

	float tickPhysics;
	float tickCave;
	float tickHunger;
	float tickHealth;
	float tickChunk;
	float tickLavaDamage;
	float tickCactusDamage;
	float tickOS;
	float tickShowSlotName;
	float tickSprintmode;
	short chunks;

	float rainSoundFrequency;

	float inputDiskNameTimer;
	std::string inputDiskName;

	int chunkId;

	Timer mTimer;
	float dt;

	int barItems;
	int _Destroyer;
	int texture;
	int texture_mips2;
	int texture_mips3;
	int waterAnimation;

	unsigned char blue;
	unsigned char red;
	unsigned char black;
	unsigned char stars;

	int Steve;
	int zombieTex;
	int cowTex;
	int PIGTex;
	int snowBall4;
	int clouds1Tex;
	int clouds2Tex;
	int suntex;
	int moontex;
	int rainTex;

	unsigned char currentTexture;
	unsigned char timeTexture;

	float pre_fps;
	int average_fps;
	float tick_fps;
	int ticks;

	Sprite* waterScreen;
	Sprite* pumpkinMask;

	Sprite* invCellSprite;
	Sprite* gameSprite;
	Sprite* menuSprite;
	Sprite* invSprite;
	Sprite* crtSprite;
	Sprite* chtSprite;
	Sprite* selectInvSprite;
	Sprite* barSprite;
	Sprite* xpSprite;
	Sprite* crossSprite;
	Sprite* selectSprite;

	Sprite* furArrowSprite[22];
	Sprite* furFireSprite[14];
	Sprite* furSprite;

	Sprite* hpCellSprite;
	Sprite* hpCellSpriteW;
	Sprite* hpSprite;
	Sprite* hpSpriteW;
	Sprite* hpHardSprite;
	Sprite* hpHardSpriteW;
	Sprite* hpHalfSprite;
	Sprite* hpHalfSpriteW;
	Sprite* hpHardHalfSprite;
	Sprite* hpHardHalfSpriteW;

	Sprite* hgCellSprite;
	Sprite* hgSprite;
	Sprite* hgHalfSprite;
	Sprite* bubbleSprite;

	Sprite* arCellSprite;
	Sprite* arSprite;
	Sprite* arHalfSprite;

	Sprite* toolPointSprite[14];

	float hpAnim;

	float cameraSpeed;
	float cameraMoveSpeed;

	//menu sprites
	Sprite* buttonSprite;
	Sprite* sbuttonSprite;
	Sprite* nbuttonSprite;
	Sprite* smbuttonSprite;
	Sprite* moverSprite;

	bool menuOptions;
	char menuState;
	char optionsMenuPos;
	char selectPos;

	char statisticsPage;

	//some settings
	bool canSprint;
	bool canFly;
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
	float changeScale;
	int slotForChangeScale;

	float animDest;
	float animSpeed;
	bool animGo;

	bool anim[3];
	bool makeScreen;
	bool dieFactor;

	float angleFactor;

	void DrawText(int x, int y, unsigned int color, float size, const char* message, ...);
	void DrawText2(int x, int y, unsigned int color, float size, const char* message, ...);
	void DrawAmount(int x, int y, int amount);

	std::vector<SnowBall2*> mSnowBalls;
	std::vector<Particle*> mParticles;
};
#endif