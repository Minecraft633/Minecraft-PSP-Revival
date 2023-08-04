#include "PIG.h"
#include <Source/Aurora/Graphics/TextureManager.h>

#define DEG_TO_RAD (PI / 180.0f)
#define PIXEL 1.0f / 16.0f
#define GRAVITY -6.8f
#define JUMPVELOCITY 4.6f

PIG::PIG(Vector3 position1, int number1) : Mob(position1, number1)
{
	legs = Vector3(0, PIXEL * -6, 0);

	SPD = 1.0f;
	HP = 5.0f;

	SetCollideBox(Vector3(PIXEL * 7, PIXEL * 17, PIXEL * 7), Vector3(PIXEL * 7, PIXEL * 10, PIXEL * 7));
	bBox = BoundingBox(Vector3(0, 0, 0), Vector3(0, 0, 0));

	animLegAngle = 0.0f;
	animLegAngleT = 0.0f;

	headAngleY = 0.0f;

	rLegAngle = PI;
	lLegAngle = PI;

	mainAngle = (rand() % 360) / 180.0f * PI;
	deathAngle = 0.0f;

	senselessJumpsTimer = 0.0f;
	senselessJumpsTimerMax = 60.0f + (rand() % 11) * 3;
	senselessJumps = false;

	rotate1 = false;

	handItemId = -1;

	UpdateBBox();
	myLight = 1.0f;

	lastCommand = 0;
	freezedTimerMax = 500.0f;

	mainTexture = TextureHelper::Instance()->GetTexture(TextureHelper::PIGTexture);
}

PIG::~PIG()
{

}

void PIG::BoxCollision()
{
	for (int x = 0; x < world->mPIGs.size(); x++)
	{
		PIG* TestPIG2 = world->mPIGs[x];
		if (TestPIG2->number != number && (TestPIG2->position.x != position.x || TestPIG2->position.z != position.z || TestPIG2->position.y != position.y))
		{
			if (TestPIG2->bBox.intersect(bBox))
			{
				float angle1 = ATAN2_fast(position.x - TestPIG2->position.x, position.z - TestPIG2->position.z); // super secret function

				TryToMove(Vector3(sinf(angle1), 0.0f, cosf(angle1)), SPD * 0.05f);
			}
		}
	}
}

void PIG::TakeDamage(float damage, float power, float dt)
{
	HP -= damage;

	kicked = true;
	damaged = true;
	damagedTimer = power * 0.6f + 0.15;
	velocity.y = (power + 0.15f) * JUMPVELOCITY;
	walkingOnGround = false;

	position += velocity * dt;
	playHurtSound = true;

	lastKickType = 1; //by player
}

void PIG::TakeDamageFromPointExplosion(float damage, float power, float dt, Vector3 point)
{
	explosionPos = point;

	HP -= damage;

	kicked = true;
	damaged = true;
	damagedTimer = power * 0.6f + 0.15;
	velocity.y = (power + 0.25f) * JUMPVELOCITY;
	walkingOnGround = false;

	position += velocity * dt;
	playHurtSound = true;

	lastKickType = 2; //by explosion
}

void PIG::sceGumScale(const ScePspFVector3* v)
{

}

void PIG::Update(CraftWorld* myWorld, Vector3 playerPos, float dt)
{
	world = myWorld;
	relPlayerPos = playerPos;
	BoundingBox playerHitBox = BoundingBox(Vector3(playerPos.x - 0.35f, playerPos.y - 1.65f, playerPos.z - 0.35f), Vector3(playerPos.x + 0.35f, playerPos.y + 0.2f, playerPos.z + 0.35f));

	distanceToPlayer = DistanceToPlayer();
	fastDistanceToPlayer = FastDistanceToPlayer();

	if (killed == true)
	{
		if (deathAngle < PI / 2)
		{
			deathAngle += (PI / 2) * dt * 1.75 + ((PI / 2 - deathAngle) / 24);
		}

		if (deathAngle > PI / 2)
		{
			deathAngle = PI / 2;
		}

		deathTimer -= dt;
	}

	if (toDestroy == false)
	{
		relPlayerPos = playerPos;
		if (fastDistanceToPlayer < world->mainOptions.horizontalViewDistance * 14 + 5)
		{
			freezedTimer = 0.0f;
			if (HP <= 0.0f && killed == false)
			{
				killed = true;
				playDieSound = true;
				if (distanceToPlayer <= 5.0f)
				{
					XP_Earned = true;
				}
				SetCollideBox(Vector3(PIXEL * 7, PIXEL * 6, PIXEL * 7), Vector3(PIXEL * 7, PIXEL * 10, PIXEL * 7));
				return;
			}
			UpdateBBox();

			//TIMERS
			if (dt < 0.1f)
			{
				if (DistanceToPlayer() < 65.0f)
				{
					jumpTimer += dt;
					if (jumpTimer > 0.8f)
					{
						jumpReady = true;
						jumpTimer = 0.0f;
					}

					if (damaged == true)
					{
						if (damagedTimer > 0.0f)
						{
							damagedTimer -= dt;
						}
						else
						{
							damagedTimer = 0.0f;
							damaged = false;
						}
					}

					if (playerHitBox.intersect(bBox))
					{
						float angle2 = ATAN2_fast(position.x - relPlayerPos.x, position.z - relPlayerPos.z);
						TryToMove(Vector3(sinf(angle2), 0.0f, cosf(angle2)), 1.0 * dt);
					}
					if (distanceToPlayer < 6)
					{
						LookTimer += dt;
						if (LookTimer > 2.0f)
						{
							if (rand() % 2 == 0)
							{
								float playerAngle = ATAN2_fast(position.x - playerPos.x, position.z - playerPos.z) + PI;
								if (mainAngle != playerAngle)
								{
									mainAngle += sinf(playerAngle - mainAngle) * 0.07f;
									headAngleY = playerAngle - mainAngle;
								}
							}
							LookTimer = 0.0f;
						}
					}
					if (distanceToPlayer < 1.5)
					{
						nearToPlayer = true;
					}
					else
					{
						nearToPlayer = false;
					}
					//SWIMING
					if (world->PlayerInWater(position) == true && jumpReady == true && killed == false)
					{
						TryToJump();
						jumpReady = false;
					}

					//HAUNTING
					moving = false;

					if (killed == false)
					{
						senselessJumpsTimer += dt;
						if (senselessJumpsTimer >= senselessJumpsTimerMax)
						{
							senselessJumpsTimer = 0.0f;
							if (senselessJumps == true)
							{
								senselessJumps = false;
								senselessJumpsTimerMax = 60.0f + (rand() % 21) * 1.5f;

								if (rand() % 3 == 0)
								{
									senselessJumpsTimerMax = 60.0f + rand() % 80;
								}
							}
							else
							{
								senselessJumps = true;
								senselessJumpsTimerMax = 2.0f + (rand() % 21) / 10.0f;
							}
						}

						if (senselessJumps && jumpReady)
						{
							TryToJump();
							jumpReady = false;
						}

						if (handItemId == SnowBallItem::getID() || fastDistanceToPlayer >= 2 && 10 >= fastDistanceToPlayer)
						{
							Throw_Item = true;
						}
						else
						{
							Throw_Item = false;
						}

						if (handItemId != Carrot::getID() || fastDistanceToPlayer >= 10)
						{
							headAngleY = 0.0f;
							if (hauntingTimer < hauntingTimerMax)
							{
								hauntingTimer += dt;
							}
							if (hauntingTimer >= hauntingTimerMax)
							{
								rotate1 = false;
								hauntingTimer = 0.0f;

								int percent = 50;
								if (lastCommand == 0)
								{
									percent = 75 + rand() % 10;
								}
								else
								{
									percent = 30 + rand() % 20;
								}

								if (rand() % 100 < percent)
								{
									hauntingTimerMax = 15.0f + rand() % 4;

									hauntingPos.x = position.x - 6 + ((rand() % 120) / 10.0f);
									hauntingPos.z = position.z - 6 + ((rand() % 120) / 10.0f);

									if (hauntingPos.x > 252)
									{
										hauntingPos.x = 252;
									}
									if (hauntingPos.x < 2)
									{
										hauntingPos.x = 2;
									}

									if (hauntingPos.z > 252)
									{
										hauntingPos.z = 252;
									}
									if (hauntingPos.z < 2)
									{
										hauntingPos.z = 2;
									}

									hauntingAngle = ATAN2_fast(position.x - hauntingPos.x, position.z - hauntingPos.z) + PI;

									lastCommand = 0;
								}
								else // just rotate about its own position
								{
									hauntingTimerMax = 10.0f + (rand() % 30) / 10.0f;
									hauntingAngle = (rand() % 360) / 180.0f * PI;

									hauntingPos = Vector3(-1, -1, -1);

									lastCommand = 1;
								}
							}

							if (hauntingPos.x != 0 && hauntingPos.z != 0)
							{
								if (DistanceToHauntingPos() > 0.5f)
								{
									if (hauntingPos != Vector3(-1, -1, -1))
									{
										if (mainAngle != hauntingAngle)
										{
											mainAngle += sinf(hauntingAngle - mainAngle) * 0.07f;
										}
										TryToMove(Vector3(sinf(mainAngle), 0.0f, cosf(mainAngle)), SPD * dt);
									}
									else
									{
										if (mainAngle != hauntingAngle)
										{
											mainAngle += sinf(hauntingAngle - mainAngle) * 0.07f;
										}
									}
								}
								else
								{
									hauntingPos.x = 0;
									hauntingPos.z = 0;
								}
							}
						}
						else
						{
							hauntingTimer = 0.0f;

							float playerAngle = ATAN2_fast(position.x - playerPos.x, position.z - playerPos.z) + PI;
							if (mainAngle != playerAngle)
							{
								mainAngle += sinf(playerAngle - mainAngle) * 0.07f;
							}
							headAngleY = playerAngle - mainAngle;

							if (distanceToPlayer > 2.3f)
							{
								TryToMove(Vector3(sinf(mainAngle), 0.0f, cosf(mainAngle)), SPD * dt);
							}
						}
					}

					if (kicked == true || handItemId == Carrot::getID())
					{
						checkCollisionTimer += dt;
						if (checkCollisionTimer >= 0.1f)
						{
							BoxCollision();
							checkCollisionTimer = 0.0f;
						}
					}

					//MOVING WHEN KICKED
					if (kicked == true)
					{
						haunting = false;
						hauntingTimer = 0.0f;
						hauntingPos.x = 0;
						hauntingPos.z = 0;

						float mainAngle2; // super secret function
						if (lastKickType != 2)
						{
							mainAngle2 = ATAN2_fast(position.x - relPlayerPos.x, position.z - relPlayerPos.z) + PI;
						}
						else
						{
							mainAngle2 = ATAN2_fast(position.x - explosionPos.x, position.z - explosionPos.z) + PI;
						}
						TryToMoveWithoutJump(Vector3(sinf(mainAngle2 + PI), 0.0f, cosf(mainAngle2 + PI)), (SPD + 0.18f) * 9.2f * (damagedTimer * damagedTimer) * dt);

						if ((walkingOnGround == true || footInLava || footInWater) && damagedTimer <= 0.17f)
						{
							kicked = false;

							hauntingTimerMax = 3.0f + rand() % 3;

							hauntingPos.x = position.x - 5 + (rand() % 100) / 10.0f;
							hauntingPos.z = position.z - 5 + (rand() % 100) / 10.0f;

							if (hauntingPos.x > world->WORLD_SIZE - 3)
							{
								hauntingPos.x = world->WORLD_SIZE - 3;
							}
							if (hauntingPos.x < 2)
							{
								hauntingPos.x = 2;
							}

							if (hauntingPos.z > world->WORLD_SIZE - 3)
							{
								hauntingPos.z = world->WORLD_SIZE - 3;
							}
							if (hauntingPos.z < 2)
							{
								hauntingPos.z = 2;
							}
						}
					}

					if (killed == false)
					{
						if (moving == true)
						{
							animLegAngleT += PI * dt * 1.8;
							if (animLegAngleT >= PI * 2)
							{
								animLegAngleT = 0.0f;
							}
						}
						else
						{
							if (animLegAngleT > 0 && animLegAngleT < PI / 2)
							{
								animLegAngleT -= animLegAngleT * 0.3f;
								if (animLegAngleT < 0.05)
								{
									animLegAngleT = 0;
								}
							}
							if (animLegAngleT >= PI / 2 && animLegAngleT < PI)
							{
								animLegAngleT += (PI - animLegAngleT) * 0.3f;
								if (animLegAngleT > PI - 0.05)
								{
									animLegAngleT = 0;
								}
							}
							if (animLegAngleT > PI&& animLegAngleT < PI * 1.5)
							{
								animLegAngleT -= (animLegAngleT - PI) * 0.3f;
								if (animLegAngleT < PI + 0.05)
								{
									animLegAngleT = 0;
								}
							}
							if (animLegAngleT >= PI * 1.5 && animLegAngleT < PI * 2)
							{
								animLegAngleT += (2 * PI - animLegAngleT) * 0.3f;
								if (animLegAngleT > 2 * PI - 0.05)
								{
									animLegAngleT = 0;
								}
							}
						}
						animLegAngle = sinf(animLegAngleT) * 0.7f;
					}
					animLegAngle = sinf(animLegAngleT) * 0.7f;
					animLegAngle = sinf(animLegAngleT) * 0.8f;

					//SIMEPLE MOB PHYSICS
					Vector3 footPosition = position + foot;
					Vector3 legsPosition = position + legs;
					Vector3 headPosition = position + head;

					if (world->PlayerInWater(legsPosition))
						footInWater = true;
					else
						footInWater = false;

					if (world->PlayerInWater(headPosition))
					{
						if (velocity.y > -3)
						{
							velocity.y += (GRAVITY / 3.0f) * dt;
						}
						else
						{
							velocity.y = -3;
						}
						headInWater = true;
					}
					else
					{
						headInWater = false;
						velocity.y += (GRAVITY * 1.3 * dt) * 1.6f;
					}

					if (world->PlayerInLava(legsPosition))
					{
						footInLava = true;
						HP -= 0.5f;
						damaged = true;
						damagedTimer = 0.55f;
					}
					else
					{
						footInLava = false;
					}

					if (world->PlayerInLava(headPosition))
					{
						velocity.y += (GRAVITY / 3.0f) * dt;
						headInLava = true;
						HP -= 1;
						damaged = true;
						damagedTimer = 0.55f;
					}
					else
					{
						headInLava = false;
						velocity.y += GRAVITY * dt;
					}

					if (world->SolidAtPointForEntity(footPosition, colliderD) || world->SolidAtPointForEntity(headPosition, colliderU))
					{
						walkingOnGround = true;
						if (world->SolidAtPointForEntity(headPosition, colliderU))
						{
							int blockIn = (int)(headPosition.y);
							position.y = (float)(blockIn - colliderU.y - 0.05);
						}
						if (world->SolidAtPointForEntity(footPosition, colliderD))
						{
							int blockOn = (int)(footPosition.y);
							position.y = (float)(blockOn + 1 + colliderD.y - 0.05);
						}

						if (velocity.y != 0.0f) /// FALL DAMAGE
						{
							if (!world->PlayerInWater(footPosition))
							{
								if (velocity.y < -12 && velocity.y > -19)
								{
									HP -= (int)((velocity.y * -1) - 11) / 1.4;

									damaged = true;
									damagedTimer = 0.5f;
									playHurtSound = true;
								}
								if (velocity.y < -19)
								{
									HP -= (int)((velocity.y * -1) - 11) * 1.3f;

									damaged = true;
									damagedTimer = 0.5f;
									playHurtSound = true;
								}
							}
						}

						velocity.y = 0.0f;

						if (jumping == true)
						{
							velocity.y = 1.55f * JUMPVELOCITY;
							jumping = false;
							walkingOnGround = false;
						}
					}
					else
					{
						walkingOnGround = false;
					}
					position += velocity * dt;
				}
			}
		}
		else
		{
			if (dt < 0.1f)
			{
				freezedTimer += dt;
				if (freezedTimer >= freezedTimerMax)
				{
					toDestroy = true;
				}
			}
		}
	}
}
void PIG::Render(Frustum& camFrustum, float dt)
{
	if (DistanceToPlayer() < 65.0f)
	{
		if (toDestroy == false)
		{
			if (fastDistanceToPlayer < world->mainOptions.horizontalViewDistance * 14 && camFrustum.BoxInFrustum(bBox) == Frustum::Intersects)
			{
				sceGumPushMatrix();

				ScePspFVector3 loc22 = { position.x,position.y,position.z };
				sceGumTranslate(&loc22);

				sceGumRotateZ(0.0f);
				sceGumRotateY(mainAngle - PI / 2);

				if (killed == true)
				{
					sceGumRotateX(deathAngle);
				}
				else
				{
					sceGumRotateX(0.0f);
				}

				updateLightTimer += dt;
				if (updateLightTimer > 0.25f)
				{
					updateLightTimer = 0.0f;
					myLight = world->BlockFinalLight(position.x, position.y + 1, position.z);
				}

				if ((damaged == true && damagedTimer > 0.25f) || killed == true)
				{
					sceGuColor(GU_COLOR(0.8f, 0.1, 0.1, 1.0f));
				}
				else
				{
					sceGuColor(GU_COLOR(myLight, myLight, myLight, 1.0f));
				}

				TextureManager::Instance()->SetTextureModeulate(mainTexture);

				sceGuEnable(GU_TEXTURE_2D);
				sceGuEnable(GU_DEPTH_TEST);
				sceGuDisable(GU_BLEND);
				sceGuDisable(GU_ALPHA_TEST);

				//sceGumRotateZ(1.6f); //This rotated the PIGs body!
				//ScePspFVector3 scale1 = { 1.5f, 1.2f, 1.2f };
				//sceGumScale(&scale1);

				sceGumPushMatrix();
				ScePspFVector3 loc = { -0.2f,0,0 };
				sceGumTranslate(&loc);

				sceGumRotateX(0.0f);
				sceGumRotateY(0.0f);
				sceGumRotateZ(0.0f);

				sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 36, 0, world->bodyVertices);
				sceGumPopMatrix();

				sceGumPushMatrix();
				sceGumRotateZ(0.0f);
				ScePspFVector3 loc2 = { PIXEL * 2,-PIXEL * 14,PIXEL * 1 };
				sceGumTranslate(&loc2);
				//ScePspFVector3 scale2 = { 2, 1, 1 };
				//sceGumScale(&scale2);

				sceGumRotateX(0.0f);
				sceGumRotateY(-headAngleY);

				sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 36, 0, world->headVertices);
				sceGumPopMatrix();

				sceGumPushMatrix();
				ScePspFVector3 loc5 = { PIXEL * 4,-PIXEL * 6,PIXEL * 2 };
				sceGumTranslate(&loc5);

				sceGumRotateX(0.0f);
				sceGumRotateY(PI);
				sceGumRotateZ(rLegAngle + animLegAngle);

				sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 36, 0, world->PIGLegVertices);
				sceGumPopMatrix();

				sceGumPushMatrix();
				ScePspFVector3 loc15 = { -PIXEL * 4,-PIXEL * 6,-PIXEL * 2 };
				sceGumTranslate(&loc15);

				sceGumRotateX(0.0f);
				sceGumRotateY(PI);
				sceGumRotateZ(rLegAngle + animLegAngle);

				sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 36, 0, world->PIGLegVertices);
				sceGumPopMatrix();

				sceGumPushMatrix();
				ScePspFVector3 loc6 = { PIXEL * 4,-PIXEL * 6,-PIXEL * 2 };
				sceGumTranslate(&loc6);

				sceGumRotateX(0.0f);
				sceGumRotateY(PI);
				sceGumRotateZ(lLegAngle - animLegAngle);

				sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 36, 0, world->PIGLegVertices);
				sceGumPopMatrix();

				sceGumPushMatrix();
				ScePspFVector3 loc16 = { -PIXEL * 4,-PIXEL * 6,PIXEL * 2 };
				sceGumTranslate(&loc16);

				sceGumRotateX(0.0f);
				sceGumRotateY(PI);
				sceGumRotateZ(lLegAngle - animLegAngle);

				sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 36, 0, world->PIGLegVertices);
				sceGumPopMatrix();

				sceGuDisable(GU_DEPTH_TEST);
				sceGuDisable(GU_TEXTURE_2D);
				sceGuColor(0xFFFFFFFF);

				sceGumPopMatrix();
			}
		}
	}
}