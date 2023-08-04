#ifndef WEATHERSYSTEM_H_
#define WEATHERSYSTEM_H_

#include <Source/Aurora/Graphics/Vertex.h>
#include <Source/Aurora/Graphics/RenderManager.h>
#include <pspmath.h>
#include <Source/Aurora/Math/Frustum.h>
#include <Source/CraftWorld.h>
#include <Source/Functions.h>
#include <Source/Aurora/Math/BoundingBox.h>
#include <Source/TextureHelper.h>

class CraftWorld;

class WeatherSystem
{
public:
    WeatherSystem();
    ~WeatherSystem();

    void SetWeatherDuration(float duration);
    void SetWeatherType(int type);

    float GetWeatherDuration();
    int GetWeatherType();

    void Update(CraftWorld* mWorld, float dt);
    void Render(CraftWorld* mWorld, Frustum &camFrustum, float camera_angle, Vector3 playerPosition, float dt);
    void BuildPlanesVertex();

    int raindropsCount;
    bool itIsRaining;
    bool itIsSnowing;

private:


    Vector3 playerTempPosition;

    int weatherType;
    float weatherDuration;

    TexturesPSPVertex *plane;

    int planeY[28];

    float rainTextureOffset;
    float snowTextureOffset;
    float textureAlpha;
    float size;

    Vector3 GetPlaneCoord(int planeID);

    void RenderPlane(int shiftX, int shiftY, int shiftZ);
    void RenderRightPlane();
    void RenderLeftPlane();
    void RenderBackPlane();
    void RenderFrontPlane();
    void RenderDiagonalePlane(int type);
};


#endif

