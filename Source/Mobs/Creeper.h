#ifndef CREEPER_H_
#define CREEPER_H_

#include <Source/Aurora/Graphics/Vertex.h>
#include <Source/Aurora/Graphics/RenderManager.h>
#include <pspmath.h>
#include <psptypes.h>
#include <Source/Aurora/Math/Frustum.h>

#include <Source/Aurora/Math/BoundingBox.h>
#include "Mob.h"

class CraftWorld;

class Creeper : public Mob
{
public:

    Creeper(Vector3 position1, int number1);
    ~Creeper();

    void BoxCollision();
    void Update(CraftWorld *myWorld, Vector3 playerPos, float dt);
    void Render(Frustum &camFrustum, float dt);
    void TakeDamage(float damage, float power, float dt);
    void TakeDamageFromPointExplosion(float damage, float power, float dt, Vector3 point);

    bool exploded;
    bool playFuseSound;

    int handItemId;

private:

    bool startExplosion;
    float timeBeforeExplosion;
    float blowingScale;
    float animationTime;

    bool boredom;
    float boredomTime;

    int lastCommand;

    float rLegAngle;
    float lLegAngle;

    float headAngleZ;
    float headAngleY;

    float animLegAngle;
    float animLegAngleT;

    float explosionRadius;
};
#endif