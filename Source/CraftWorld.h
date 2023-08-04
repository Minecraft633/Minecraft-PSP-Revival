#ifndef CraftWorld_H
#define CraftWorld_H

#include <string>
#include <math.h>
#include <psptypes.h>

#include <algorithm>

#include <Source/Aurora/Math/Frustum.h>
#include <Source/Aurora/Math/Vector4.h>
#include <Source/Aurora/Graphics/Effects/Drop.h>
#include <Source/Aurora/Graphics/Effects/TNT.h>
#include <Source/Aurora/Graphics/Effects/WeatherSystem.h>

#include "Blocks.h"
#include "Items.h"
#include "SimpleMeshChunk.h"
#include "Mobs/Zombie.h"
#include "Mobs/Cow.h"
#include "Mobs/PIG.h"
#include "Mobs/Creeper.h"
#include "Mobs/Sheep.h"
#include "Mobs/Mob.h"
#include "Furnace.h"
#include "Chest.h"
#include "DirectionBlock.h"
#include "NoteBlockEntity.h"
#include "JukeboxEntity.h"
#include "MonsterSpawnerEntity.h"
#include "ItemFrameEntity.h"
#include "InputHelper.h"

#include "Functions.h"
#include <sys/time.h>
#include <Aurora/Utils/Logger.h>
#include <Aurora/Utils/Timer.h>

#include <fstream>
#include <ostream>

class Cow;
class PIG;
class Zombie;
class Creeper;
class Sheep;
class Mob;

class Drop;
class TNT;
class ItemFrameEntity;
class WeatherSystem;

using namespace std;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora;

class CraftWorld
{
public:

	double TestWriteChunk(float dt);

	char WORLD_HEIGHT;

	enum BlockSettings
	{
		 OpFirstShadowBit = 16,
		 OpSecondShadowBit = 32,
		 OpThirdShadowBit = 64,
		 OpFourthShadowBit = 128
	};

	CraftWorld();
	~CraftWorld();

    typedef struct Statistics
    {
        unsigned short blockPlaced;
        unsigned short blockDestroyed;
        unsigned short daysInGame;
        unsigned short minutesPlayed;
        unsigned short itemsCrafted;
        unsigned short itemsSmelted;
        unsigned short jumps;
        unsigned short dies;
        unsigned short foodEaten;
        unsigned short badlyFalls;
        unsigned short blocksWalked;
        unsigned short treesGrowned;
        unsigned short cropsGrowned;
        unsigned short soilPlowed;
        unsigned short timeInWater;
        unsigned short timeInAir;
        unsigned short damageRecieved;
    } st;

    typedef struct Options
    {
        bool useMipsTexturing;
        bool detailedSky;
        bool fastRendering;
        bool mipMapTexturing;
        bool freezeDayTime;
        bool sounds;
        bool music;
        bool autoJump;
        bool worldBlockAnimation;
        bool fogRendering;
        bool smoothLighting;
        bool particles;
        bool guiDrawing;

        float fov;
        char horizontalViewDistance;
        char verticalViewDistance;
        int difficult;
    } opt;

    typedef struct WorldVariables
    {
        int worldDiamondOre;
        int worldIronOre;
        int worldCoalOre;
        int worldGoldenOre;
        int worldRedstoneOre;
        int worldLapisOre;
        int worldTrees;
        int worldFlowers;
        int worldClay;
    } wvars;

    WeatherSystem* worldWeather;

    float updateChunkTimer;
    float updateTransparentChunkTimer2;
    float updateChunkTimer2;
    float updateTransparentChunkTimer;
    float updateFastChunkTimer;

    float fetchNearestChunksTimer;

    bool chunksWasSetToUpdate;
	bool PhysicsUpdate;

    st mainStatistics;
    opt mainOptions;
    wvars mainWorldVariables;

    char gameModeWorld;
    bool gameLocked;

    void InitBlockVector();
    void InitItemVector();

    block_t _GetBlock(const int x, const int y, const int z);
    block_t _GetBlockNoCheck(const int x, const int y, const int z);

	block_t& GetBlock (const int x, const int y, const int z) ;
	block_t& GetBlockNoCheck (const int x, const int y, const int z) ;
	block_t& GetBlockSettings (const int x, const int y, const int z);
	block_t& GetBlockSettingsNoCheck (const int x, const int y, const int z);

	int GetShadowLevel (const int x, const int y, const int z);
	int GetShadowLevelNoCheck (const int x, const int y, const int z);
	void SetShadowLevel (const int x, const int y, const int z, int level);
	void SetShadowLevelNoCheck (const int x, const int y, const int z, int level);

	int GetLightLevel(const int x, const int y, const int z);
	int GetLightLevelNoCheck(const int x, const int y, const int z);
	void SetLightLevel(const int x, const int y, const int z, unsigned int level);
	void SetLightLevelNoCheck(const int x, const int y, const int z, unsigned int level);

	block_t BlockAtPoint(Vector3 pos);

    void BuildWorldBlockPlane(BaseBlock *blockType, int x,int y, int z, int &iVertex, SimpleMeshChunk* MeshChunk, block_t Block, bool transparent, char side, Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4);
    void BuildWorldBlockPlane(BaseBlock *blockType, int x,int y, int z, int &iVertex, SimpleMeshChunk* MeshChunk, block_t Block, bool transparent, char side, Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4, bool alwaysBuild, bool constantColor);
    void BuildWorldBlockPlane(BaseBlock *blockType, int x,int y, int z, int &iVertex, SimpleMeshChunk* MeshChunk, block_t Block, bool transparent, char side, Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4, bool alwaysBuild, bool constantColor, int texturePlane);
    void BuildWorldBlockPlane(BaseBlock *blockType, int x,int y, int z, int &iVertex, SimpleMeshChunk* MeshChunk, block_t Block, bool transparent, char side, Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4, bool alwaysBuild, bool constantColor, int texturePlane, int textureRotation);
    void BuildWorldBlockPlaneNoCheck(BaseBlock *blockType, int x,int y, int z, int &iVertex, SimpleMeshChunk* MeshChunk, block_t Block, bool transparent, char side, Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4);
    void BuildGUIBlockPlane(BaseBlock* blockType, char side, Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4);

    float LengthDirX(float horAngle, float verAngle);
    float LengthDirY(float horAngle, float verAngle);
    float LengthDirZ(float horAngle, float verAngle);

    float TaxicabDistance2d(float x1, float y1, float x2, float y2);
    unsigned int FastDistance2d(unsigned int dx, unsigned int dy);
    float Distance2d(float x1, float y1, float x2, float y2);
    float Distance3d(float x1, float y1, float z1, float x2, float y2, float z2);

	//map creation
	void initWorld(short worldSize, char worldHeight, short chunkSize);
	void initWorldBlocksLight();

	void initPutBlocksLight(const int xx, const int zz);
	void initPutBlocksLight(const int xx, const int yy, const int zz);

	void GetSpecialBlockVerts(int i,BaseBlock *blockType);
	void GetNormalBlockVerts(int i,BaseBlock *blockType);
	void GetItemVerts(int i,BaseItem *itemType);
	void GetCloudsVerts(BaseBlock *blockType);

	void buildblocksVerts();
	void buildcloudsVerts();
	void buildmobVerts();
	void buildZombieVerts();

    void setTextureSize(short texture, short chunk, short animTexture, short animChunk, short itemTexture, short itemChunk);

    void PutInInventory(int id, int num, bool st);
    void PutInInventory(int id, int num, bool st, int &slot);
    bool InventoryIsFull(int id, int num, bool st);

	void CreateFullMeshChunk(const int StartX, const int StartY, const int StartZ);

	void RebuildFullMeshChunk(int id);
	void RebuildOpaqueMeshChunk(int id);
	void RebuildTransparentMeshChunk(int id);

	void rebuildNearestChunks(int id,Vector3 pos);
	void rebuildNearestTransparentChunks(int id, Vector3 pos);

	void rebuildNearestChunksForLight(int id,Vector3 pos);
	void createWorldChunkss();
	void SetAllChunksToUpdate();
    int getChunkId(Vector3 pos);
    bool ChunksEqual(Vector3 pos1, Vector3 pos2);

	void RebuildVisibleChunks();

	void GetSpecialBlock(int x,int y, int z,int &iVertex,SimpleMeshChunk* MeshChunk,block_t Block,bool transparent);
	void GetNormalBlock(int x,int y, int z,int &iVertex,SimpleMeshChunk* MeshChunk,block_t Block,bool transparent);

	//load/save options
	void SaveCompressedWorld(std::string filename);
	void LoadWorld(const char *filename);
	void LoadCompressedWorld(std::string filename);

	//lightening
	void InitLightValues();
	void SetWorldTime(float time);
	void LightTravel(int x,int y,int z,int steps,int lightLevel);

	void buildMap();
	void destroyMap();

	//rendering
	void FetchNearestChunks();
	void drawWorld(Frustum &camFrustum,bool camUpdate);
	void drawCubes(int i, float light);
	void drawHudCubes(int i);
	void drawWhiteCube(int i);
	void drawHand();
	void drawClouds();
	void drawDropItems(int i);
	void drawItems(int i);
	void drawHudItems(int i);
	void drawFull3DItem(int i,float light);
	void drawHandItems(int i, float light);
	void drawArmor(int i, float light);
	void drawShoulder(int chestplateId, float light);
	void drawBody(int leggingsId, float light);

	//pickin,collision etc
	int groundHeightFromBottom (const int x, const int y, const int z);
	int groundHeight (const int x, const int z);
	int groundHeight (const int x, const int y, const int z);
	int groundHeightWater (const int x, const int z);
	int groundHeightExcept (const int x, const int z, int bl);
	int BlockSoundAtPos(Vector3 pos);

    bool SolidAtPointForPlayerHead(Vector3 pos);
	bool SolidAtPointForPlayer(Vector3 pos);
	bool SolidAtPointForMovingPlayer(Vector3 pos);
	bool SolidAtPointForPlayerOnGround(Vector3 pos);
	bool SolidAtPointForEntity(Vector3 pos, Vector3 collider1);
	bool SolidAtPoint(Vector3 pos);
	bool HighStairAtPoint(Vector3 pos);

	bool PlayerOnHalfBlock(Vector3 pos);
	bool PlayerOnStairsBlock(Vector3 pos);
	bool PlayerOnHighStair(Vector3 pos);

	bool PlayerInWater(Vector3 pos);
	bool PlayerInLava(Vector3 pos);
	bool PlayerInBlock(Vector3 pos);
	bool PlayerCollisionWithLadder(Vector3 pos);
	bool PlayerCollisionWithCactus(Vector3 pos);

	bool BlockTransparentOrSpecial(const int x, const int y, const int z);
	bool BlockTransparentOrLightSource(const int x, const int y, const int z);
	bool BlockTransparentOrLightSourceNoCheck(const int x, const int y, const int z);
	bool BlockTransparent(const int x, const int y, const int z);
	bool BlockTransparentNoCheck(const int x, const int y, const int z);
	bool BlockAnimated(const int x, const int y, const int z);
	char BlockSpecial(const int x, const int y, const int z);
	char BlockSpecialNoCheck(const int x, const int y, const int z);
	bool BlockEditable(const int x, const int y, const int z);
	bool BlockAllowLight(const int x, const int y, const int z);
	float BlockFinalLight(int x, int y, int z);
	float BlockFinalLightNoCheck(int x, int y, int z);
	char BlockRefraction(const int x, const int y, const int z);
	short BlockLoot(const int x, const int y, const int z);
	short BlockMaterial(const int x, const int y, const int z);
	bool BlockUpdate2(const int x, const int y, const int z);
	bool BlockSolid(const int x, const int y, const int z);

    int LootBlock(int id);
	bool LightSourceBlock(int id);
	int DurabilityPointsItem(int id);
	bool StackableItem(int id);
	string NameBlock(int id);

	bool ItemHaveTerrainTexture(int id);
	char ItemType(int id);

    bool CheckForTorchSupport(const int x, const int y, const int z,int blockID);
	bool CheckForLadderSupport(const int x, const int y, const int z);

	bool DestroyAroundTrapdoors(const int x, const int y, const int z);
	bool DestroyAroundItemFrames(const int x, const int y, const int z);

	bool CanPutBlockHere(const int x, const int y, const int z,int blockID);
	void SetLigtSourcePosition(const int x, const int y, const int z,int blockID);
	void RemoveLigtSourceAtPosition(const int x, const int y, const int z,int blockID);
    void AddChunkToUpdate(int id);
    void AddChunkToFastUpdate(int id);
    void AddNearestChunk(int id);
	void UpdateLightAreaIn(Vector3 pos);

	void AddChunkToFloodUpdate(int id);

	void SpawnZombie(float xx, float yy, float zz);
	void DestroyZombie(int id);
	void DestroyAllZombies();

	void SpawnCreeper(float xx, float yy, float zz);
	void SpawnCreeper(float xx, float yy, float zz, float freezedTimerMax);
	void DestroyCreeper(int id);
	void DestroyAllCreepers();

	void DestroyItemFrameEntity(Vector3 position);

    void SpawnCow(float xx, float yy, float zz);
	void DestroyCow(int id);

	void SpawnPIG(float xx, float yy, float zz);
	void DestroyPIG(int id);

	void SpawnSheep(float xx, float yy, float zz);
	void DestroySheep(int id);

    void CollisionWithOtherZombies(Zombie *Zombie1);

    void SpawnTNTentity(float xx, float yy, float zz);
    void SpawnTNTentity(float xx, float yy, float zz, float time);
    void DestroyTNTentity(int id);

    void DropThis(int id, Vector3 position);
    void DropThis(int id, int am, Vector3 position);
    void DropThis(int id, int am, bool st, Vector3 position);

    void DropThisNoAlign(int id, int am, bool st, Vector3 position, Vector3 velocity);
    void DestroyDrop(int id);

    int FetchArmorPoints(int bootsId, int leggingsId, int chestId, int helmetId);
    float FetchDamagePoints(int weaponId, bool &weapon);
    bool HaveItemInBarSlots(int id);

	int GetDrawntTrianglesCount() { return drawnTriangles; }

	Vector3 GetPlayerPos();

	void UpdatePlayerZoneBB(Vector3 playerPosition);
	void UpdateWorldTime(float dt);
	void UpdateWorldProcesses(float dt);
	void UpdateWorldAnimation(float dt);
	void UpdateClockTexture();
	void UpdateCompassTexture(float cameraAngle);
	void SpawnMobs(float dt);

	int clockFrame;
	int lastClockFrame;

	int compassFrame;
	int lastCompassFrame;
	bool haveCompass;

	int GetBlockTypesCount();
	int GetItemTypesCount();

	void UpdateChunkBlocks(int id);
	void UpdateBlockPhysics(int id);
	void UpdateChunkGrowingBlocks(int id, int vectorId);

	int FindDirId(int x, int y, int z);

	void GetTexturesIds();
	void GetSkyColor();

    short armorId[4];
    short armorAm[4];
    bool armorSt[4];
    short invId[36];
    short invAm[36];
    bool invSt[36];
    short mId;
    short mAm;
    bool mSt;
    float playerGrowth;
    float playerCollisionSize;

    std::vector<BaseBlock> blockTypes;
	std::vector<BaseItem> itemTypes;

    float brightFactor;
    float starsFactor;
    float dawnSunsetFactor;
    Vector3	fogColor;
	Vector3 startSkyColor;
	Vector3 endSkyColor;
	Vector3 dawnSunsetSkyColor;

	Vector3 startClearSkyColor;
	Vector3 endClearSkyColor;
	Vector3 dawnSunsetClearSkyColor;

	Vector3 startRainySkyColor;
	Vector3 endRainySkyColor;
	Vector3 dawnSunsetRainySkyColor;

	float rainyColorAlpha;

	float blockZConstLight;
	float blockXConstLight;

    unsigned short worldSeed;
	short Earned;
    short HP;
    float HG;
    short AP;
    char OS;

    bool kickedStart;
    int kickedBy;
    int kickedDamage;
    float kickedAngle;
    float kickedTimer;

    float animationTimer;
    float animationTimerEnd;
    char animationWaterFrame;
    char animationLavaFrame;
    bool animationLavaStep;
	char animationFireFrame;

    float spawnZombiesTimer;
    float spawnCowsTimer;
	float spawnPIGsTimer;
    float spawnCreeperTimer;
    float spawnSheepTimer;

    int textureWaterAnimationId;
    int textureLavaAnimationId;
	int textureFireAnimationId;
    int textureItemsId;
    int textureTerrainId;
    int textureTerrainMipsId;
    int textureLeatherArmor1Id;
    int textureLeatherArmor2Id;
    int textureChainArmor1Id;
    int textureChainArmor2Id;
    int textureIronArmor1Id;
    int textureIronArmor2Id;
    int textureDiamondArmor1Id;
    int textureDiamondArmor2Id;
    int textureGoldenArmor1Id;
    int textureGoldenArmor2Id;
    int textureClockId;
    int textureCompassId;
    int textureSkyColorId;

	char worldName[50];
	int createdChunksCount;

	float worldDayTime;
	float worldTime;

	float sunTime;
	int worldVersion;
	float lightAngle;

	Vector3 playerZoneSize;
	Vector3 playerSpawnPointPosition;

	Vector3 snowBiomePosition;
	Vector3 desertBiomePosition;
	float snowBiomeRadius;
	float desertBiomeRadius;

    std::vector<Chest*> mChests;
    std::vector<Furnace*> mFurnaces;
    std::vector<DirectionBlock*> mDirects;
    std::vector<Zombie*> mZombies;
    std::vector<Cow*> mCows;
	std::vector<PIG*> mPIGs;
    std::vector<Creeper*> mCreepers;
    std::vector<Sheep*> mSheeps;
    std::vector<Drop*> mDrops;
    std::vector<TNT*> mTNTs;
    std::vector<NoteBlockEntity*> mNoteBlocks;
    std::vector<JukeboxEntity*> mJukeboxes;
    std::vector<MonsterSpawnerEntity*> mMonsterSpawners;
    std::vector<ItemFrameEntity*> mItemFrames;

    /// Gui block properties
    std::vector<Vector3*> mPositionLeftRight;
    std::vector<Vector2*> mtexturesLeftRight;
    std::vector<Vector3*> mTriangleLeftRight;

    std::vector<Vector3*> mPositionBackFront;
    std::vector<Vector2*> mtexturesBackFront;
    std::vector<Vector3*> mTriangleBackFront;

    std::vector<Vector3*> mPositionBottomTop;
    std::vector<Vector2*> mtexturesBottomTop;
    std::vector<Vector3*> mTriangleBottomTop;

    int iVertexLeftRight;
    int pointsLeftRight;

    int iVertexBackFront;
    int pointsBackFront;

    int iVertexBottomTop;
    int pointsBottomTop;

    int mChestsize;
    int mFurnacesize;
    int mDirectsize;
    int mZombiesize;
    int mCowsize;
	int mPIGsize;
    int mSheepsize;
    int mCreepersize;
    int mTNTsize;
    int mNoteBlocksize;
    int mJukeboxsize;
    int mMonsterSpawnersize;
    int mItemFramesize;

    int spawner_x;
    int spawner_z;

    int zombieNum;
    int dropNum;
    int TNTnum;

    Vector3 lightColor;
    Vector3	ambientColor;
    float westLight;
    float eastLight;

    float skyTime;

    int FindFurnaceID(Vector3 pos);
    int FindChestID(Vector3 pos);
    int FindNoteBlockID(Vector3 pos);
    int FindJukeboxID(Vector3 pos);
    int FindMonsterSpawnerID(Vector3 pos);
    int FindItemFrameID(Vector3 pos);

    void SetPlayerSpawnPosition(Vector3 newPosition);

    void BuildExplodeSphere(float radius, int X, int Y, int Z);
    void BuildBlockSphere(float radius, block_t block, int X, int Y, int Z);

    void BuildBlockSphere(float radius, block_t block, int X, int Y, int Z, block_t blockToChange);

    void BuildBlockEllipsoid(float w_radius, float h_radius, block_t block, int X, int Y, int Z);

    std::vector<SimpleMeshChunk*> mChunks;
    std::vector<int> toUpdate;
    std::vector<int> toFastUpdate;

    std::vector<Vector3> floodPos;
    std::vector<int> floodChunks;

    float updatingChunksTimer;
    float updatingChunksTimerMax;
    unsigned int updatingChunksPosition;

    std::vector<int> updatingChunks;

    // for humanoids
    TexturesPSPVertex *bodyVertices;
    TexturesPSPVertex *headVertices;
    TexturesPSPVertex *hatVertices;
    TexturesPSPVertex *legVertices;
    TexturesPSPVertex *handVertices;

    // for creeper
    TexturesPSPVertex *creeperLegVertices;

    // for cows
    TexturesPSPVertex *cowBodyVertices;
    TexturesPSPVertex *cowHeadVertices;
    TexturesPSPVertex *cowLegVertices;
    TexturesPSPVertex *cowHornsVertices;

	// for PIGs
	TexturesPSPVertex* PIGLegVertices;

    // for sheep
    TexturesPSPVertex *sheepBodyVertices;
    TexturesPSPVertex *sheepHeadVertices;
    TexturesPSPVertex *sheepHeadOPTVertices;
    TexturesPSPVertex *sheepHeadFurVertices;
    TexturesPSPVertex *sheepLegOPTVertices;
    TexturesPSPVertex *sheepLegFurVertices;

    // for armor
    TexturesPSPVertex *helmetVertices;
    TexturesPSPVertex *shoulderVertices;
    TexturesPSPVertex *chestplateVertices;
    TexturesPSPVertex *bellyVertices;
    TexturesPSPVertex *leggingVertices;
    TexturesPSPVertex *bootVertices;

    Vector3 playerPos;
    bool playerContactLadder;

    //player zone aabb
	BoundingBox playerZoneBB;

	short WORLD_SIZE;
	short CHUNK_SIZE;

	std::vector<int> mNearestChunksIds;

private:
    InputHelper* mIhelper;
	int texutreSize;
	int textureChunk;
	float percent;
	float percent2;
	float percent3;

	std::vector<SimpleMeshChunk*> mTransparentChunks;

	block_t* m_BlockSettings;
    block_t* m_Blocks;

	int chunksCreatedInFrameCount;
	int transparentchunksCreatedInFrameCount;
	int chunksCreatedInFrameCount2;
	int transparentchunksCreatedInFrameCount2;

	float minuteTimer;
	float allChunksUpdateTimer;
	bool updateChunksSwitch;

	float worldHour;// 1 hour in the game is the same as 50 seconds; 24 hours = 20 minutes

	//information
	int drawnTriangles;
	float lightShadowFactor;
	Vector3 lightFactor;
};
#endif