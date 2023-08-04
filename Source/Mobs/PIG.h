#ifndef PIG_H_
#define PIG_H_

#include <Source/Aurora/Graphics/Vertex.h>
#include <Source/Aurora/Graphics/RenderManager.h>
#include <pspmath.h>
#include <psptypes.h>
#include <Source/Aurora/Math/Frustum.h>

#include <Source/Aurora/Math/BoundingBox.h>
#include "Mob.h"
#include "Blocks.h"
#include "CraftWorld.h"

class CraftWorld;

class PIG : public Mob
{
public:

	PIG(Vector3 position1, int number1);
	~PIG();

	void sceGumScale(const ScePspFVector3* v);
	void BoxCollision();
	void Update(CraftWorld* myWorld, Vector3 playerPos, float dt);
	void Render(Frustum& camFrustum, float dt);
	void TakeDamage(float damage, float power, float dt);
	void TakeDamageFromPointExplosion(float damage, float power, float dt, Vector3 point);

	bool rotate1;
	int handItemId;

private:

	int lastCommand;
	float rLegAngle;
	float lLegAngle;

	float headAngleY;

	float animLegAngle;
	float animLegAngleT;

	Vector3 hauntingPos;

	bool senselessJumps;
	float senselessJumpsTimer;
	float senselessJumpsTimerMax;
};

#endif