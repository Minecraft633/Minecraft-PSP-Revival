#ifndef TEXTUREHELPER_H_
#define TEXTUREHELPER_H_

#include <string>
#include <pspiofilemgr.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

class TextureHelper
{
private:

	std::string defaulPatch;
	std::string fileName;

public:

	enum Textures
	{
         Genesis,
		 Logo,
		 Loading,
		 Utils,
		 Xpbar,
		 Terrain1,
		 Terrain2,
         Terrain3,
		 WaterAnimation,
		 LavaAnimation,
		 FireAnimation,
         Items1,
		 Blue,
		 Sun,
         Moon,
         Stars,
         SnowBall3,
         zombieTexture,
         Steve,
		 Buttons,
		 Rectangles,
		 Load,
		 Red,
		 Black,
		 PumpkinMask,
         clouds1,
		 clouds2,
         dark_background,
		 menu,
		 menu1,
		 menu2,
		 menu3,
		 game,
         inv,
         invCreative,
         crt,
         cht,
         fur,
         cursor,
         invSlider,
         furFire,
         furArrow,
         invCell,
         leatherArmor1,
         leatherArmor2,
         chainmailArmor1,
         chainmailArmor2,
         ironArmor1,
         ironArmor2,
         diamondArmor1,
         diamondArmor2,
         goldenArmor1,
         goldenArmor2,
         cowTexture,
		 PIGTexture,
		 xp_orbTexture,
         smokeTexture,
         flameTexture,
         noteTexture,
         bubbleTexture,
         explosionTexture,
         clockTexture,
         compassTexture,
         skyColorTexture,
         rainTexture,
         snowTexture,
         creeperTexture,
         sheepTexture,
         sheepFurTexture,
         background,
		 shadow,
	};
    std::string texturePatch;
    std::string defaultFolder;

	static TextureHelper m_TextureHelper;
	static TextureHelper* Instance();


	TextureHelper();
	~TextureHelper();

	int GetTexture(Textures texture);
    void Init();
    void FastRemoveTexture(std::string name);
    void FastLoadTexture(std::string name);
	void SetTextureFolderName(std::string name);
	void SetTexturePack(std::string name);

    void RemoveConstTextures();

    void LoadTexturePackPath();
    void SaveTexturePackPath();
    inline bool fileExists(const std::string& name);
};
#endif