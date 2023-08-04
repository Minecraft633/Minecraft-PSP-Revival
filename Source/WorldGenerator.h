#ifndef WorldGenerator_H
#define WorldGenerator_H

#include <Source/Aurora/Math/Frustum.h>
#include <math.h>
#include <psptypes.h>

#include <Source/3libs/noisepp/Noise.h>
#include <Source/3libs/noisepp/NoiseUtils.h>
#include <Source/3libs/noisepp/NoiseBuilders.h>

#include <pspiofilemgr.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <zlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

#include "Source/SimplexNoise.h"
#include "Chest.h"
#include "MonsterSpawnerEntity.h"
#include "InputHelper.h"

#include <pspgu.h>
#include <pspgum.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <psppower.h>
#include <psptypes.h>
#include <pspge.h>
#include <psputils.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <stdarg.h>

class CraftWorld;
using namespace noisepp;

class WorldGenerator
{
public:
	InputHelper* mIhelper;
	void initRandompMap(short worldSize, char worldHeight, short chunkSize, CraftWorld* world, bool makeTrees, bool makeWater, bool makeCaves, unsigned short seedII, int worldType);
	void initTrees(short WORLD_SIZE, CraftWorld* world, PerlinModule* perlin, short seed);
	void initRandomTrees(short WORLD_SIZE, CraftWorld* world);

	void initPumpkins(short WORLD_SIZE, CraftWorld* world);

	void initFlowers(short WORLD_SIZE, CraftWorld* world);
	void initGrass(short WORLD_SIZE, CraftWorld* world);
	void initOtherVegetation(short WORLD_SIZE, CraftWorld* world);

	void initLavaPools(short WORLD_SIZE, CraftWorld* world);
	void initBiome(short WORLD_SIZE, CraftWorld* world, PerlinModule* perlin, short seed);

	void initCanes(short WORLD_SIZE, CraftWorld* world, char waterLevel);
	void initBedrock(short WORLD_SIZE, CraftWorld* world);
	void initBeachSand(short WORLD_SIZE, CraftWorld* world);

	void initOre(short WORLD_SIZE, CraftWorld* world, int oreID, float oreConcentration, int oreLowestBoundary, int oreHighestBoundary, int variations);

	void initDungeons(short WORLD_SIZE, CraftWorld* world);

	void initClay(short WORLD_SIZE, CraftWorld* world);
	void initDirt(short WORLD_SIZE, CraftWorld* world);
	void initGravel(short WORLD_SIZE, CraftWorld* world);
	void initLavaLakes(short WORLD_SIZE, CraftWorld* world);
	void initWaterLakes(short WORLD_SIZE, CraftWorld* world);

	int GenerateOreVine(short WORLD_SIZE, CraftWorld* world, int x, int y, int z, int oreID, int type);

	void GeneratePileOfLeaves(CraftWorld* world, int x, int y, int z);
	void GenerateClassicTree(short WORLD_SIZE, CraftWorld* world, int x, int y, int z, int trunkBlock, int leavesBlock);
	void GenerateLargeTree(short WORLD_SIZE, CraftWorld* world, int x, int y, int z, int trunkBlock, int leavesBlock);
	void GenerateSpruceTree(short WORLD_SIZE, CraftWorld* world, int x, int y, int z);

	float InterpolateBiomeNoise(int wx, int wz);
	float InterpolateMoistureNoise(int wx, int wz);
	float InterpolateElevationNoise(int wx, int wz);
	float InterpolateRoughnessNoise(int wx, int wz);

	SimplexNoise func1, func2, func3;

	float GetValue3D(int wx, int wy, int wz, int octaves, float startFrequency, float startAmplitude);
	float GetValue2D(int wx, int wz, int octaves, float startFrequency, float startAmplitude);
	float GetValue2D2(int wx, int wz, int octaves, float startFrequency, float startAmplitude);
	float GetValue2D3(int wx, int wz, int octaves, float startFrequency, float startAmplitude);
	void SetSeed(short seed);
};
#endif