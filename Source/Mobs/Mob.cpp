#include "Mob.h"
#include "CraftWorld.h"
#include <Source/Aurora/Graphics/TextureManager.h>

#define DEG_TO_RAD (PI / 180.0f)
#define PIXEL 1.0f / 16.0f
#define GRAVITY -6.8f
#define JUMPVELOCITY 4.6f

Mob::Mob(Vector3 mobPosition, int mobNumber)
{
    world = NULL;

    position = mobPosition;
    number = mobNumber;

    colliderD = Vector3(0,0,0);
    colliderU = Vector3(0,0,0);

    legs = Vector3(0,0,0);
    body = Vector3(0,0,0);
    head = Vector3(0,0,0);
    foot = Vector3(0,0,0);

    SetCollideBox(Vector3(-1,1,-1),Vector3(1,1,1));
    bBox = BoundingBox(Vector3(0,0,0),Vector3(0,0,0));

    relPlayerPos = Vector3(0,0,0);
    velocity = Vector3(0,0,0);
    explosionPos = Vector3(-1,-1,-1);

    deathAngle = 0.0f;
    mainAngle = (rand()%360)/180.0f*PI;

    SPD = 1.0f;
    HP = 20.0f;

    toDestroy = false;
    freezed = false;
    angred = false;
    moving = false;
    jumping = false;
    jumpReady = true;
    footInWater = false;
    footInLava = false;
    headInWater = false;
    headInLava = false;
    walkingOnGround = true;
    kicked = false;
    damaged = false;
    nearToPlayer = false;
    killed = false; 
	XP_Earned = false;
    haunting = false;

    friendly = true;

    playDieSound = false;
    playHurtSound = false;
    playSaySound = false;

    kickTimer = 0.0f;
    damagedTimer = 0.0f;
    freezedTimer = 0.0f;
    freezedTimerMax = 60.0f;
    jumpTimer = 0.0f;
    checkCollisionTimer = 0.0f;
    deathTimer = 0.75f;
    sayTimer = 10.0f + (rand() % 160)/10.0f;
	LookTimer = 10.0f + (rand() % 160) / 10.0f;
    updateLightTimer = 0.3f;

    hauntingTimer = 0.0f;
    hauntingTimerMax = 0.0f;
    hauntingAngle = 0.0f;
    hauntingPos = Vector3(0,0,0);

    fastDistanceToPlayer = 999.0f;
    distanceToPlayer = 0.0f;

    UpdateBBox();

    tickCactusDamage = 0.0f;
    touchingCactus = false;

    myLight = 1.0f;

    lastKickType = -1;
    mainTexture = -1;
}

Mob::~Mob()
{

}

float Mob::DistanceToPlayer()
{
    return abs(relPlayerPos.x-position.x) + abs(relPlayerPos.z-position.z) + abs(relPlayerPos.y-0.7-position.y)*0.89f;
}

unsigned int Mob::FastDistanceToPlayer()
{
    return world->FastDistance2d(abs(position.x-world->playerPos.x),abs(position.z-world->playerPos.z)) + abs(relPlayerPos.y-(position.y-0.5))*0.7f;
}

float Mob::PreciseDistanceToPlayer() /// REAL DISTANCE
{
    return sqrt((relPlayerPos.x-position.x)*(relPlayerPos.x-position.x) + (relPlayerPos.z-position.z)*(relPlayerPos.z-position.z));
}

float Mob::DistanceToHauntingPos() /// TAXICAB DISTANCE
{
    return abs(hauntingPos.x-position.x) + abs(hauntingPos.z-position.z);
}

void Mob::TryToJump()
{
    if (headInWater || headInLava)
    {
        if (walkingOnGround)
        {
            jumping = true;
        }
        else
        {
            velocity.y = 0.2 * JUMPVELOCITY;
            walkingOnGround = false;
        }
    }
    if ((!headInWater || !headInLava) && !walkingOnGround && (footInWater || footInLava))
    {
        velocity.y = 1.25 * JUMPVELOCITY;
        walkingOnGround = false;
    }
    if(walkingOnGround)
    {
        jumping = true;
    }
    else
    {
        jumping = false;
    }
}

void Mob::TryToMove(Vector3 movePosition1, float speed)
{
    Vector3 movePosition = position + Vector3(movePosition1.x*(speed+0.2),0.0f,movePosition1.z*(speed+0.2));
    Vector3 movePositionHead = movePosition + head;
    Vector3 movePositionFoot = movePosition + Vector3(foot.x,foot.y+0.1,foot.z);

    if (!world->SolidAtPointForEntity(movePosition,colliderD) && !world->SolidAtPointForEntity(movePositionHead,colliderU) && !world->SolidAtPointForEntity(movePositionFoot,colliderD))
    {
        Vector3 collisionPosition = position + Vector3(sinf(mainAngle)*(speed+0.25),0.0f,cosf(mainAngle)*(speed+0.25));
        position += Vector3(movePosition1.x*speed,0.0f,movePosition1.z*speed);

        moving = true;
    }
    else
    {
        if(jumpReady == true && (!world->SolidAtPointForEntity(movePositionHead,colliderU) || friendly))
        {
            TryToJump();
            jumpReady = false;
        }
    }

    movePosition = position + Vector3(movePosition1.x*(speed+0.2),0.0f,0.0f);
    movePositionHead = movePosition + head;
    movePositionFoot = movePosition + Vector3(foot.x,foot.y+0.1,foot.z);

    if (!world->SolidAtPointForEntity(movePosition,colliderD) && !world->SolidAtPointForEntity(movePositionHead,colliderU) && !world->SolidAtPointForEntity(movePositionFoot,colliderD))
    {
        position += Vector3(movePosition1.x*speed,0.0f,0);
    }
    else
    {
        if(jumpReady == true && (!world->SolidAtPointForEntity(movePositionHead,colliderU) || friendly))
        {
            TryToJump();
            jumpReady = false;
        }
    }

    movePosition = position + Vector3(0.0f,0.0f,movePosition1.z*(speed+0.2));
    movePositionHead = movePosition + head;
    movePositionFoot = movePosition + Vector3(foot.x,foot.y+0.1,foot.z);

    if (!world->SolidAtPointForEntity(movePosition,colliderD) && !world->SolidAtPointForEntity(movePositionHead,colliderU) && !world->SolidAtPointForEntity(movePositionFoot,colliderD))
    {
        position += Vector3(0.0f,0.0f,movePosition1.z*speed);
    }
    else
    {
        if(jumpReady == true && (!world->SolidAtPointForEntity(movePositionHead,colliderU) || friendly))
        {
            TryToJump();
            jumpReady = false;
        }
    }
}

void Mob::TryToMoveWithoutJump(Vector3 movePosition1, float speed)
{
    Vector3 movePosition = position + Vector3(movePosition1.x*(speed+0.2),0.0f,movePosition1.z*(speed+0.2));
    Vector3 movePositionHead = movePosition + head;
    Vector3 movePositionFoot = movePosition + Vector3(foot.x,foot.y+0.1,foot.z);

    if (!world->SolidAtPointForEntity(movePosition,colliderD) && !world->SolidAtPointForEntity(movePositionHead,colliderU) && !world->SolidAtPointForEntity(movePositionFoot,colliderD))
    {
        Vector3 collisionPosition = position + Vector3(sinf(mainAngle)*(speed+0.25),0.0f,cosf(mainAngle)*(speed+0.25));
        position += Vector3(movePosition1.x*speed,0.0f,movePosition1.z*speed);

        moving = true;
    }
    else
    {
        moving = false;
    }

    movePosition = position + Vector3(movePosition1.x*(speed+0.2),0.0f,0.0f);
    movePositionHead = movePosition + head;
    movePositionFoot = movePosition + Vector3(foot.x,foot.y+0.1,foot.z);

    if (!world->SolidAtPointForEntity(movePosition,colliderD) && !world->SolidAtPointForEntity(movePositionHead,colliderU) && !world->SolidAtPointForEntity(movePositionFoot,colliderD))
    {
        position += Vector3(movePosition1.x*speed,0.0f,0);
    }

    movePosition = position + Vector3(0.0f,0.0f,movePosition1.z*(speed+0.2));
    movePositionHead = movePosition + head;
    movePositionFoot = movePosition + Vector3(foot.x,foot.y+0.1,foot.z);

    if (!world->SolidAtPointForEntity(movePosition,colliderD) && !world->SolidAtPointForEntity(movePositionHead,colliderU) && !world->SolidAtPointForEntity(movePositionFoot,colliderD))
    {
        position += Vector3(0.0f,0.0f,movePosition1.z*speed);
    }
}

void Mob::SetPosition(Vector3 newPosition)
{
    position = newPosition;
}

void Mob::SetCollideBox(Vector3 collider1, Vector3 collider2)
{
    colliderD = collider1;
    colliderU = collider2;

    foot = Vector3(0,-collider1.y,0);
    head = Vector3(0,collider2.y,0);
}

void Mob::UpdateBBox()
{
    bBox = BoundingBox(Vector3(position.x-colliderD.x*1.45,position.y-colliderD.y*1.3,position.z-colliderD.z*1.45),Vector3(position.x+colliderU.x*1.45,position.y+colliderU.y*1.3,position.z+colliderU.z*1.45));
}

void Mob::BoxCollision()
{

}

void Mob::TakeDamage(float damage, float power, float dt)
{

}

void Mob::TakeDamageFromPointExplosion(float damage, float power, float dt, Vector3 point)
{

}

void Mob::Update(CraftWorld *myWorld, Vector3 playerPos, float dt)
{

}

void Mob::Render(Frustum &camFrustum, float dt)
{

}