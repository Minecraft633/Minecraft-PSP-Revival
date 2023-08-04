#include "TextureHelper.h"
#include <Aurora/Graphics/RenderManager.h>

TextureHelper::TextureHelper()
{
	defaulPatch = "Assets/Textures/";
	defaultFolder = "Minecraft/";
	texturePatch = defaulPatch+defaultFolder;
}

TextureHelper::~TextureHelper()
{
}

void TextureHelper::Init()
{
    LoadTexturePackPath();
    texturePatch = defaulPatch+defaultFolder;

	FastLoadTexture("gui/mojang_splash_screen.png");
    FastLoadTexture("environment/moon.png");
    FastLoadTexture("gui/utils.png");
	FastLoadTexture("gui/xpbar.png");
    FastLoadTexture("terrain.png");
    FastLoadTexture("terrainMipMapLevel2.png");
	FastLoadTexture("terrainMipMapLevel3.png");
    FastLoadTexture("animated/water.png");
    FastLoadTexture("animated/lava.png");
	FastLoadTexture("animated/fire_0.png");
    FastLoadTexture("items.png");
    FastLoadTexture("gui/blue.png");
    FastLoadTexture("environment/sun.png");
    FastLoadTexture("environment/moon.png");
    FastLoadTexture("environment/stars.png");
    FastLoadTexture("entity/snowball.png");
    FastLoadTexture("entity/zombie/zombie.png");
    FastLoadTexture("entity/skin.png");
	FastLoadTexture("gui/loading.png");
    FastLoadTexture("gui/choose_rectangles.png");
    FastLoadTexture("gui/menu_elements.png");
    FastLoadTexture("entity/char.png");
    FastLoadTexture("gui/red.png");
    FastLoadTexture("gui/black.png");
    FastLoadTexture("gui/pumpkinblur.png");
    FastLoadTexture("environment/clouds1.png");
	FastLoadTexture("environment/clouds2.png");
	FastLoadTexture("gui/dark_background.png");
	FastLoadTexture("gui/menu_buttons.png");
	FastLoadTexture("gui/menu1.png");
	FastLoadTexture("gui/menu2.png");
	FastLoadTexture("gui/menu3.png");
	FastLoadTexture("gui/game_buttons.png");
    FastLoadTexture("gui/inventory1.png");
    FastLoadTexture("gui/inventory_creative.png");
    FastLoadTexture("gui/crafting_table.png");
    FastLoadTexture("gui/chest.png");
    FastLoadTexture("gui/furnace.png");
    FastLoadTexture("gui/cursor.png");
    FastLoadTexture("gui/slider.png");
    FastLoadTexture("gui/fire.png");
	FastLoadTexture("gui/arrow.png");
	FastLoadTexture("gui/cell.png");
	FastLoadTexture("armor/cloth_1.png");
	FastLoadTexture("armor/cloth_2.png");
	FastLoadTexture("armor/chain_1.png");
	FastLoadTexture("armor/chain_2.png");
	FastLoadTexture("armor/iron_1.png");
	FastLoadTexture("armor/iron_2.png");
	FastLoadTexture("armor/diamond_1.png");
	FastLoadTexture("armor/diamond_2.png");
	FastLoadTexture("armor/gold_1.png");
	FastLoadTexture("armor/gold_2.png");
	FastLoadTexture("entity/cow/cow.png");
	FastLoadTexture("entity/pig/pig.png");
    FastLoadTexture("particle/experience_orb.png");
	FastLoadTexture("particle/smoke.png");
	FastLoadTexture("particle/flame.png");
	FastLoadTexture("particle/note.png");
	FastLoadTexture("particle/bubble.png");
	FastLoadTexture("particle/explosion.png");
	FastLoadTexture("item/clock.png");
	FastLoadTexture("item/compass.png");
	FastLoadTexture("environment/sky_color.png");
	FastLoadTexture("environment/rain.png");
	FastLoadTexture("environment/snow.png");
	FastLoadTexture("entity/creeper/creeper.png");
	FastLoadTexture("entity/sheep/sheep.png");
	FastLoadTexture("entity/sheep/sheep_fur.png");
	FastLoadTexture("gui/panorama.png");
	FastLoadTexture("entity/shadow.png");
}

int TextureHelper::GetTexture(Textures texture)
{
	switch(texture)
	{
		case Genesis:
		{
			fileName = texturePatch + "gui/mojang_splash_screen.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Loading:
		{
			fileName = texturePatch + "gui/loading.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		case Logo:
		{
			fileName = texturePatch + "environment/moon.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Utils:
		{
			fileName = texturePatch + "gui/utils.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Xpbar:
		{
			fileName = texturePatch + "gui/xpbar.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Terrain1:
		{
			fileName = texturePatch + "terrain.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case Terrain2:
		{
			fileName = texturePatch + "terrainMipMapLevel2.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Terrain3:
		{
			fileName = texturePatch + "terrainMipMapLevel3.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case WaterAnimation:
		{
			fileName = texturePatch + "animated/water.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case LavaAnimation:
		{
			fileName = texturePatch + "animated/lava.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case FireAnimation:
		{
			fileName = texturePatch + "animated/fire_0.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case Items1:
		{
			fileName = texturePatch + "items.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Blue:
		{
			fileName = texturePatch + "gui/blue.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Sun:
		{
			fileName = texturePatch + "environment/sun.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case Moon:
		{
			fileName = texturePatch + "environment/moon.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case Stars:
		{
			fileName = texturePatch + "environment/stars.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case SnowBall3:
		{
			fileName = texturePatch + "entity/snowball.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case zombieTexture:
		{
			fileName = texturePatch + "entity/zombie/zombie.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case Steve:
		{
			fileName = texturePatch + "entity/skin.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Buttons:
		{
			fileName = texturePatch + "gui/menu_elements.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case Rectangles:
		{
			fileName = texturePatch + "gui/choose_rectangles.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Load:
		{
			fileName = texturePatch + "entity/char.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Red:
		{
			fileName = texturePatch + "gui/red.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case Black:
		{
			fileName = texturePatch + "gui/black.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case PumpkinMask:
		{
			fileName = texturePatch + "gui/pumpkinblur.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case clouds1:
		{
			fileName = texturePatch + "environment/clouds1.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case clouds2:
		{
			fileName = texturePatch + "environment/clouds2.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case dark_background:
		{
			fileName = texturePatch + "gui/dark_background.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case menu:
		{
			fileName = texturePatch + "gui/menu_buttons.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case menu1:
		{
			fileName = texturePatch + "gui/menu1.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case menu2:
		{
			fileName = texturePatch + "gui/menu2.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case menu3:
		{
			fileName = texturePatch + "gui/menu3.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case game:
		{
			fileName = texturePatch + "gui/game_buttons.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case inv:
		{
			fileName = texturePatch + "gui/inventory1.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case invCreative:
		{
			fileName = texturePatch + "gui/inventory_creative.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case crt:
		{
			fileName = texturePatch + "gui/crafting_table.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case cht:
		{
			fileName = texturePatch + "gui/chest.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case fur:
		{
			fileName = texturePatch + "gui/furnace.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case cursor:
		{
			fileName = texturePatch + "gui/cursor.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case invSlider:
		{
			fileName = texturePatch + "gui/slider.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case furFire:
		{
			fileName = texturePatch + "gui/fire.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case furArrow:
		{
			fileName = texturePatch + "gui/arrow.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case invCell:
		{
			fileName = texturePatch + "gui/cell.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case leatherArmor1:
		{
			fileName = texturePatch + "armor/cloth_1.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case leatherArmor2:
		{
			fileName = texturePatch + "armor/cloth_2.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case chainmailArmor1:
		{
			fileName = texturePatch + "armor/chain_1.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case chainmailArmor2:
		{
			fileName = texturePatch + "armor/chain_2.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case ironArmor1:
		{
			fileName = texturePatch + "armor/iron_1.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case ironArmor2:
		{
			fileName = texturePatch + "armor/iron_2.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case diamondArmor1:
		{
			fileName = texturePatch + "armor/diamond_1.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case diamondArmor2:
		{
			fileName = texturePatch + "armor/diamond_2.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case goldenArmor1:
		{
			fileName = texturePatch + "armor/gold_1.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case goldenArmor2:
		{
			fileName = texturePatch + "armor/gold_2.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case cowTexture:
		{
			fileName = texturePatch + "entity/cow/cow.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case PIGTexture:
		{
			fileName = texturePatch + "entity/pig/pig.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case xp_orbTexture:
		{
			fileName = texturePatch + "particle/experience_orb.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case smokeTexture:
		{
			fileName = texturePatch + "particle/smoke.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case flameTexture:
		{
			fileName = texturePatch + "particle/flame.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case noteTexture:
		{
			fileName = texturePatch + "particle/note.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case bubbleTexture:
		{
			fileName = texturePatch + "particle/flame.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
        case explosionTexture:
		{
			fileName = texturePatch + "particle/explosion.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case clockTexture:
		{
			fileName = texturePatch + "item/clock.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case compassTexture:
		{
			fileName = texturePatch + "item/compass.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case skyColorTexture:
		{
			fileName = texturePatch + "environment/sky_color.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case rainTexture:
		{
			fileName = texturePatch + "environment/rain.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case snowTexture:
		{
			fileName = texturePatch + "environment/snow.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case creeperTexture:
		{
			fileName = texturePatch + "entity/creeper/creeper.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case sheepTexture:
		{
			fileName = texturePatch + "entity/sheep/sheep.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case sheepFurTexture:
		{
			fileName = texturePatch + "entity/sheep/sheep_fur.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case background:
		{
			fileName = texturePatch + "gui/panorama.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
		case shadow:
		{
			fileName = texturePatch + "entity/shadow.png";
			return TextureManager::Instance()->GetTextureNumber(fileName);
		}
		break;
	}
	return 0;
}

void TextureHelper::SetTexturePack(std::string name)
{
    TextureManager::Instance()->RemoveTextures();

	SetTextureFolderName(name+"/");

    FastLoadTexture("environment/moon.png");
    FastLoadTexture("gui/utils.png");
	FastLoadTexture("gui/xpbar.png");
    FastLoadTexture("terrain.png");
    FastLoadTexture("terrainMipMapLevel2.png");
	FastLoadTexture("terrainMipMapLevel3.png");
    FastLoadTexture("animated/water.png");
    FastLoadTexture("animated/lava.png"); 
	FastLoadTexture("animated/fire_0.png");
    FastLoadTexture("items.png");
    FastLoadTexture("gui/blue.png");
    FastLoadTexture("environment/sun.png");
    FastLoadTexture("environment/moon.png");
    FastLoadTexture("environment/stars.png");
    FastLoadTexture("entity/snowball.png");
    FastLoadTexture("entity/zombie/zombie.png");
    FastLoadTexture("entity/skin.png");
	FastLoadTexture("gui/loading.png");
    FastLoadTexture("gui/choose_rectangles.png");
    FastLoadTexture("gui/menu_elements.png");
    FastLoadTexture("entity/char.png");
    FastLoadTexture("gui/red.png");
    FastLoadTexture("gui/black.png");
    FastLoadTexture("gui/pumpkinblur.png");
    FastLoadTexture("environment/clouds1.png");
	FastLoadTexture("environment/clouds2.png");
	FastLoadTexture("gui/dark_background.png");
	FastLoadTexture("gui/menu_buttons.png");
	FastLoadTexture("gui/menu1.png");
	FastLoadTexture("gui/menu2.png");
	FastLoadTexture("gui/menu3.png");
	FastLoadTexture("gui/game_buttons.png");
    FastLoadTexture("gui/inventory1.png");
    FastLoadTexture("gui/inventory_creative.png");
    FastLoadTexture("gui/crafting_table.png");
    FastLoadTexture("gui/chest.png");
    FastLoadTexture("gui/furnace.png");
    FastLoadTexture("gui/cursor.png");
    FastLoadTexture("gui/slider.png");
    FastLoadTexture("gui/fire.png");
	FastLoadTexture("gui/arrow.png");
	FastLoadTexture("gui/cell.png");
	FastLoadTexture("armor/cloth_1.png");
	FastLoadTexture("armor/cloth_2.png");
	FastLoadTexture("armor/chain_1.png");
	FastLoadTexture("armor/chain_2.png");
	FastLoadTexture("armor/iron_1.png");
	FastLoadTexture("armor/iron_2.png");
	FastLoadTexture("armor/diamond_1.png");
	FastLoadTexture("armor/diamond_2.png");
	FastLoadTexture("armor/gold_1.png");
	FastLoadTexture("armor/gold_2.png");
	FastLoadTexture("entity/cow/cow.png");
	FastLoadTexture("entity/pig/pig.png");
	FastLoadTexture("particle/experience_orb.png");
	FastLoadTexture("particle/smoke.png");
	FastLoadTexture("particle/flame.png");
	FastLoadTexture("particle/note.png");
	FastLoadTexture("particle/bubble.png");
	FastLoadTexture("particle/explosion.png");
	FastLoadTexture("item/clock.png");
	FastLoadTexture("item/compass.png");
    FastLoadTexture("environment/sky_color.png");
    FastLoadTexture("environment/rain.png");
    FastLoadTexture("environment/snow.png");
    FastLoadTexture("entity/creeper/creeper.png");
    FastLoadTexture("entity/sheep/sheep.png");
    FastLoadTexture("entity/sheep/sheep_fur.png");
    FastLoadTexture("gui/panorama.png");
	FastLoadTexture("entity/shadow.png");
}

void TextureHelper::FastRemoveTexture(std::string name)
{

}

void TextureHelper::RemoveConstTextures()
{
    TextureManager::Instance()->RemoveConstTextures();
}

void TextureHelper::FastLoadTexture(std::string name)
{
    std::string endName = texturePatch+name;
    TextureManager::Instance()->LoadTexture(endName);
}

void TextureHelper::SetTextureFolderName(std::string name)
{
	defaultFolder = name;
	texturePatch = defaulPatch + defaultFolder;
	SaveTexturePackPath();
}

void TextureHelper::SaveTexturePackPath()
{
	unsigned int pathLength = defaultFolder.size();

	FILE * pFile;
	pFile = fopen("Save/texturepack.cf","wb");

	if(pFile == NULL)
		return;

    fwrite(&pathLength,sizeof(unsigned int),1,pFile);
	fwrite(defaultFolder.c_str(),1,pathLength,pFile);

	//close file
	fclose(pFile);
}

void TextureHelper::LoadTexturePackPath()
{
	unsigned int pathLength = 0;

	FILE * pFile;
	pFile = fopen("Save/texturepack.cf","rb");

    if(pFile == NULL)
    {
        defaultFolder = "Minecraft/";
        return;
    }

    fread(&pathLength,sizeof(unsigned int),1,pFile);
	char* c = new char[pathLength+1];
    fread(c,1,pathLength,pFile);
    c[pathLength] = '\0';
    defaultFolder = c;
    delete[] c;

	//close file
	fclose(pFile);

	if(fileExists(defaulPatch+defaultFolder+"terrain.png") == false) // if this texture pack doesn't exist at the moment
    {
        defaultFolder = "Minecraft/"; // return to default one
    }
}

inline bool TextureHelper::fileExists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}


TextureHelper TextureHelper::m_TextureHelper;

TextureHelper* TextureHelper::Instance()
{
	return &m_TextureHelper;
}
