#include "StateMenu.h"
#include "StateOptions.h"
#include "StatePlay.h"
#include "StatePlayCreative.h"
#include "LoadingScreen.h"
#include "InputHelper.h"
#include "TextureHelper.h"
#include <Aurora/System/NetworkManager.h>
#include <zlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

// Font Size
#define normal_size 0.44f
#define small_size 0.35f
#define large_size 0.46f

#define SURVIVAL 0
#define CREATIVE 1
#define HARDCORE 2

using namespace Aurora::Graphics;
using namespace Aurora::Utils;
using namespace Aurora::System;
using namespace Aurora;

StateMenu::StateMenu()
{
}

StateMenu::~StateMenu()
{
}

void StateMenu::Init()
{
	newW_width = 0;
	newW_height = 0;
	newW_length = 0;

	newW_gameMode = SURVIVAL;
	newW_deleteCaves = true;
	conversionStage = 0;

	converterPos = 0;
	schematicExists = false;

	lol = "";
	mainStatistics.blockPlaced = 0;
	mainStatistics.blockDestroyed = 0;
	mainStatistics.daysInGame = 0;
	mainStatistics.minutesPlayed = 0;
	mainStatistics.itemsCrafted = 0;
	mainStatistics.itemsSmelted = 0;
	mainStatistics.jumps = 0;
	mainStatistics.dies = 0;
	mainStatistics.foodEaten = 0;
	mainStatistics.badlyFalls = 0;
	mainStatistics.blocksWalked = 0;
	mainStatistics.treesGrowned = 0;
	mainStatistics.cropsGrowned = 0;
	mainStatistics.soilPlowed = 0;
	mainStatistics.timeInAir = 0;
	mainStatistics.timeInWater = 0;
	mainStatistics.damageRecieved = 0;
	mainOptions.detailedSky = 1;
	mainOptions.smoothLighting = true;
	mainOptions.sounds = 1;
	mainOptions.music = 1;
	mainOptions.fastRendering = false;
	mainOptions.horizontalViewDistance = 3;
	mainOptions.verticalViewDistance = 2;
	mainOptions.fogRendering = 1;
	mainOptions.mipMapTexturing = true;
	mainOptions.autoJump = 0;
	mainOptions.fov = 65;
	mainOptions.freezeDayTime = 0;
	mainOptions.worldBlockAnimation = 1;
	mainOptions.particles = 0;
	mainOptions.difficult = 2; // normal
	mainOptions.guiDrawing = 1;

	//set render manager instance
	mRender = RenderManager::InstancePtr();
	mSystemMgr = SystemManager::Instance();
	mSoundMgr = SoundManager::Instance();

	logoSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::logo), 0, 0, 256, 64);
	logoSprite->Scale(1.5f, 1.5f);
	logoSprite->SetPosition(240, 50);

	backgroundSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::panorama));
	backgroundSprite->Scale(4.25f, 4.25f);
	backgroundSprite->SetPosition(240, 136);
	bx = 900;

	rectFilledSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::choose_rectangles), 0, 0, 219, 39);
	rectFilledSprite->SetPosition(240, 150);
	rectFilledSprite->Scale(1.15f, 1.15f);

	rectEmptySprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::choose_rectangles), 0, 39, 219, 39);
	rectEmptySprite->SetPosition(240, 150);
	rectEmptySprite->Scale(1.15f, 1.15f);

	buttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::buttons), 0, 0, 128, 16); // stand
	buttonSprite->SetPosition(240, 150);
	buttonSprite->Scale(1.45f, 1.45f);

	sbuttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::buttons), 0, 16, 128, 16); // stand selected
	sbuttonSprite->SetPosition(240, 150);
	sbuttonSprite->Scale(1.45f, 1.45f);

	nbuttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::buttons), 0, 32, 128, 16); // dark
	nbuttonSprite->SetPosition(240, 150);
	nbuttonSprite->Scale(1.45f, 1.45f);

	// Medium Buttons
	buttonMediumSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::buttons), 0, 0, 128, 16); // stand
	buttonMediumSprite->SetPosition(240, 150);
	buttonMediumSprite->Scale(0.675f, 1.0f);
	buttonMediumSprite->Scale(1.45f, 1.45f);

	sbuttonMediumSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::buttons), 0, 16, 128, 16); // stand selected
	sbuttonMediumSprite->SetPosition(240, 150);
	sbuttonMediumSprite->Scale(0.675f, 1.0f);
	sbuttonMediumSprite->Scale(1.45f, 1.45f);

	// Small Buttons
	buttonSmallSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::buttons), 0, 0, 128, 16); // stand
	buttonSmallSprite->SetPosition(240, 150);
	buttonSmallSprite->Scale(0.495f, 1.0f);
	buttonSmallSprite->Scale(1.45f, 1.45f);

	sbuttonSmallSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::buttons), 0, 16, 128, 16); // stand selected
	sbuttonSmallSprite->SetPosition(240, 150);
	sbuttonSmallSprite->Scale(0.495f, 1.0f);
	sbuttonSmallSprite->Scale(1.45f, 1.45f);

	nbuttonSmallSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::buttons), 0, 32, 128, 16); // dark
	nbuttonSmallSprite->SetPosition(240, 150);
	nbuttonSmallSprite->Scale(0.495f, 1.0f);
	nbuttonSmallSprite->Scale(1.45f, 1.45f);

	// Large
	buttonLargeSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::buttons), 0, 0, 128, 16); // stand
	buttonLargeSprite->SetPosition(240, 150);
	buttonLargeSprite->Scale(1.35f, 1.0f);
	buttonLargeSprite->Scale(1.45f, 1.45f);

	sbuttonLargeSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::buttons), 0, 16, 128, 16); // stand selected
	sbuttonLargeSprite->SetPosition(240, 150);
	sbuttonLargeSprite->Scale(1.35f, 1.0f);
	sbuttonLargeSprite->Scale(1.45f, 1.45f);

	mbuttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::buttons), 0, 48, 128, 16); // gray
	mbuttonSprite->SetPosition(240, 150);
	mbuttonSprite->Scale(1.35f, 1.0f);
	mbuttonSprite->Scale(1.45f, 1.45f);

	smbuttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::buttons), 0, 65, 128, 16); // gray selected
	smbuttonSprite->SetPosition(240, 150);
	smbuttonSprite->Scale(1.35f, 1.0f);
	smbuttonSprite->Scale(1.45f, 1.45f);

	menu1Sprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::menu1));
	menu1Sprite->SetPosition(240, 145);
	menu1Sprite->Scale(1.5f, 1.5f);
	menu2Sprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::menu2));
	menu2Sprite->SetPosition(240, 137);
	menu2Sprite->Scale(1.5f, 1.5f);
	menu3Sprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::menu3));
	menu3Sprite->SetPosition(240, 136);
	menu3Sprite->Scale(1.5f, 1.5f);

	selectPos = 0;

	// load save info
	ScanSaveFiles("Save/World/");
	ScanTexturePacks("Assets/Textures/");

	menuState = -1;
	loadSelectPos = 0;
	loadSavePos = 0;
	aboutPos = 0;

	newWorldName = "New World";
	newWorldNamestr = "";
	newWorldSeed = "";

	size_f = 0.347f;

	//for map generation
	makeTrees = true;
	makeWater = true;
	makeCaves = true;

	terrainBuilder = 0;
	worldType = 0; // Old
	gameMode = SURVIVAL;
	seed_1 = 0;

	SplashNumber = rand() % 40;
	splashSize = 0.0f;

	saveSubmenu = false;
	saveSubMenuSelect = 2;
	generateSelectPose = 0;

	newWorldNamestr = newWorldName.c_str();
	currentVersion = 140;

	//input helper
	InputHelper::Instance()->Init();
	InputHelper::Instance()->Load();

	animationscreen = 1;
	fontcoloroption = 0;
	fontcolor = 0;
	srand(time(0));

	tpCurrent = 0;
	tpMax = 0;
	tpEnd = 0;
	tpStart = 0;
	tpPos = 0;
	tpSelectPos = 0;

	for (int i = 0; i <= 31; i++)
	{
		worldName[i] = ' ';
	}
	analogLeft = analogRight = analogUp = analogDown = false;
	mSoundMgr->PlayMenuAmbient();
	//invPlayer = new InventoryPlayer();
}

void StateMenu::Enter()
{
	mRender->SetOrtho(0, 0, 0, 0, 0, 0);
	selectPos = 0;
}

void StateMenu::CleanUp()
{
	delete logoSprite;
	delete buttonSprite;
	delete sbuttonSprite;
	delete nbuttonSprite;
	delete mbuttonSprite;
	delete smbuttonSprite;
	delete buttonLargeSprite;
	delete sbuttonLargeSprite;
	delete menuSprite;
	delete menu1Sprite;
	delete menu2Sprite;
	delete menu3Sprite;

	delete buttonSmallSprite;
	delete sbuttonSmallSprite;
	delete nbuttonSmallSprite;

	delete backgroundSprite;
	//delete invPlayer;
}

void StateMenu::Pause()
{
}

void StateMenu::Resume()
{
	menuState = -1;
	loadSelectPos = 0;
	loadSavePos = 0;
	mRender->SetOrtho(0, 0, 0, 0, 0, 0);
}

void StateMenu::HandleEvents(StateManager* sManager)
{
	mSystemMgr->InputUpdate();

	if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
	{
		mSoundMgr->PlayMenuSound();
	}
	
	if (analogUp)
	{
		if (mSystemMgr->GetAnalogY() < InputHelper::Instance()->analogYup)
		analogUp = false;
	}
	if (!analogUp)
	{
		if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
		{
			analogUp = true;
		}
	}

	if (analogDown)
	{
		if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYdown)
		analogDown = false;
	}
	if (!analogDown)
	{
		if (mSystemMgr->GetAnalogY() < InputHelper::Instance()->analogYdown)
		{
			analogDown = true;
		}
	}

	
	if (analogLeft)
	{
		if (mSystemMgr->GetAnalogX() > InputHelper::Instance()->analogXleft)
		analogLeft = false;
	}
	if (!analogLeft)
	{
		if (mSystemMgr->GetAnalogX() < InputHelper::Instance()->analogXleft)
		{
			analogLeft = true;
		}
	}

	if (!analogRight)
	{
		if (mSystemMgr->GetAnalogX() > InputHelper::Instance()->analogXright)
		{
			analogRight = true;
		}
	}
	if (analogRight)
	{
		if (mSystemMgr->GetAnalogX() < InputHelper::Instance()->analogXright)
		analogRight = false;
	}
	switch (menuState)
	{
	case 11:// skin Packs or Texture Packs
	{
		//up, down
		if (mSystemMgr->KeyPressed(PSP_CTRL_UP))
		{
			selectPos--;
			if (selectPos < 0)
				selectPos = 3;
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
		{
			selectPos++;
			if (selectPos > 3)
				selectPos = 0;
		}
		if (analogUp)
		{
			if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
			{
				selectPos++;
				if (selectPos > 2)
					selectPos = 0;
				analogUp = false;
			}
		}
		if (!analogUp)
		{
			if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
			{
				analogUp = true;
			}
		}
		if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
		{
			if (selectPos == 0)//Mods
			{
				menuState = 12;
			}
			if (selectPos == 1)//Texture Packs
			{
				menuState = 13;
			}
			if (selectPos == 2)//skin Packs
			{
				menuState = 14;
			}
			if (selectPos == 3)//Cancel
			{
				menuState = -1;
			}
		}
		if (mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
		{
			menuState = -1;
		}
	}
	break;
	case 12://Multiplayer
	{
		if (mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE) || mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
		{
			menuState = -1;
		}
	}
	break;
	case -1://main menu
	{
		if (mSystemMgr->KeyPressed(PSP_CTRL_UP))
		{
			selectPos--;
			if (selectPos < 0)
				selectPos = 4;
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
		{
			selectPos++;
			if (selectPos > 4)
				selectPos = 0;
		}		
		if (analogUp)
		{
			if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
			{
				if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
				{
					selectPos++;
					if (selectPos > 4)
						selectPos = 0;
					analogUp = false;
				}
			}
		}
		if (!analogUp)
		{
			if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
			{
				analogUp = true;
			}
		}
		if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
		{
			if (selectPos == 0)//play state
			{
				ScanSaveFiles("Save/World/");

				loadSavePos = 0;
				loadSaveStart = 0;
				loadSaveEnd = saveFilesList.size();
				loadSaveMax = 3;
				if (loadSaveMax > loadSaveEnd)
					loadSaveMax = loadSaveEnd;

				menuState = 1;
				if (saveFilesList.empty() == false)
				{
					loadSelectPos = 0;
				}
				else
				{
					loadSelectPos = 3;
				}
			}
			if (selectPos == 1)
			{
				menuState = 12;
				//Multiplayer
			}
			if (selectPos == 2)
			{
				//options
				StateOptions* stateOptions = new StateOptions();
				stateOptions->Init();
				sManager->PushState(stateOptions);
			}
			if (selectPos == 3)
			{
				//about
				menuState = 3;
				aboutPos = 0;
			}
			if (selectPos == 4)
			{
				selectPos = 0;
				//textures
				tpCurrent = 0;
				tpMax = 0;
				tpEnd = 0;
				tpStart = 0;
				tpPos = 0;
				tpSelectPos = 0;

				menuState = 11;
				tpEnd = texturePackList.size();
				tpMax = 3;
				ScanTexturePacks("Assets/Textures/");
			}
		}
	}
	break;
	case 1: // Select World
	{
		if (saveSubmenu)
		{
			if (mSystemMgr->KeyPressed(PSP_CTRL_UP))
			{
				saveSubMenuSelect--;
				if (saveSubMenuSelect < 1)
					saveSubMenuSelect = 2;
			}

			if (mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
			{
				saveSubMenuSelect++;
				if (saveSubMenuSelect > 2)
					saveSubMenuSelect = 1;
			}
			if (analogUp)
			{
				if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
				{
					if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
					{
						saveSubMenuSelect++;
						if (saveSubMenuSelect > 2)
							saveSubMenuSelect = 1;
						analogUp = false;
					}
				}
			}
			if (!analogUp)
			{
				if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
				{
					analogUp = true;
				}
			}
			if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
			{
				if (saveSubMenuSelect == 1)//remove file
				{
					//remove file
					if (saveFilesList.size() > 0)
					{
						if (saveFilesList[loadSavePos].compression)
						{
							//remove normal file
							remove(saveFilesList[loadSavePos].fileName.c_str());

							//remove compressed one
							saveFilesList[loadSavePos].fileName += "c";
							remove(saveFilesList[loadSavePos].fileName.c_str());
						}
						else
						{
							remove(saveFilesList[loadSavePos].fileName.c_str());
						}

						ScanSaveFiles("Save/World/");

						loadSavePos = 0;
						loadSaveStart = 0;
						loadSaveEnd = saveFilesList.size();
						loadSaveMax = 3;
						if (loadSaveMax > loadSaveEnd)
							loadSaveMax = loadSaveEnd;

						saveSubmenu = false;
						saveSubMenuSelect = 2;

						if (saveFilesList.empty() == false)
						{
							loadSelectPos = 0;
						}
						else
						{
							loadSelectPos = 3;
						}
					}
				}
				if (saveSubMenuSelect == 2)//return
				{
					saveSubMenuSelect = 2;
					saveSubmenu = false;
				}
			}
		}
		else
		{
			if (mSystemMgr->KeyPressed(PSP_CTRL_UP))
			{
				if (loadSelectPos == 1 || loadSelectPos == 2)
				{
					loadSelectPos = 0;
				}
				if (loadSelectPos == 4)
				{
					loadSelectPos = 3;
				}
			}

			if (mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
			{
				if (loadSelectPos == 0 || loadSelectPos == 3)
				{
					loadSelectPos++;
				}
			}
			if (analogUp)
			{
				if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
				{
					if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
					{
						if (loadSelectPos == 0 || loadSelectPos == 3)
						{
							loadSelectPos++;
						}
						analogUp = false;
					}
				}
			}
			if (!analogUp)
			{
				if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
				{
					analogUp = true;
				}
			}
			if (mSystemMgr->KeyPressed(PSP_CTRL_LEFT))
			{
				if (saveFilesList.empty() == false)
				{
					switch (loadSelectPos)
					{
					case 2:
						loadSelectPos = 1;
						break;
					case 3:
						loadSelectPos = 0;
						break;
					case 4:
						loadSelectPos = 2;
						break;
					}
				}
			}

			if (mSystemMgr->KeyPressed(PSP_CTRL_RIGHT))
			{
				switch (loadSelectPos)
				{
				case 0:
					loadSelectPos = 3;
					break;
				case 1:
					loadSelectPos = 2;
					break;
				case 2:
					loadSelectPos = 4;
					break;
				}
			}

			if (mSystemMgr->KeyPressed(PSP_CTRL_RTRIGGER))
			{
				if (saveFilesList.empty() == false)
				{
					loadSavePos--;

					if (loadSavePos < loadSaveStart)
					{
						loadSaveStart--;
						loadSaveMax--;

						if (loadSaveMax < 3)
						{
							loadSaveStart = 0;
							loadSaveMax = 3;
						}
					}

					if (loadSavePos < 0)
					{
						loadSavePos = saveFilesList.size() - 1;
						loadSaveMax = loadSaveEnd;
						loadSaveStart = loadSaveEnd - 3;
						if (loadSaveStart < 0)
							loadSaveStart = 0;
					}
				}
			}

			if (mSystemMgr->KeyPressed(PSP_CTRL_LTRIGGER))
			{
				if (saveFilesList.empty() == false)
				{
					loadSavePos++;
					if (loadSavePos == loadSaveMax)
					{
						loadSaveStart++;
						loadSaveMax++;
						if (loadSaveMax > loadSaveEnd)
						{
							loadSaveStart = loadSaveEnd - 3;
							if (loadSaveStart < 0)
								loadSaveStart = 0;
							loadSaveMax = loadSaveEnd;
						}
					}
					if (loadSavePos >= saveFilesList.size())
					{
						loadSavePos = 0;
						loadSaveStart = 0;
						loadSaveMax = 3;
						if (loadSaveMax > loadSaveEnd)
							loadSaveMax = loadSaveEnd;
					}
				}
			}

			if (mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
			{
				menuState = -1;
			}

			if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
			{
				if (loadSelectPos == 0)//play selected world
				{
					if (saveFilesList.size() > 0)
					{
						if (saveFilesList[loadSavePos].locked == false)
						{
							if (saveFilesList[loadSavePos].worldGameMode == SURVIVAL || saveFilesList[loadSavePos].worldGameMode == HARDCORE)
							{
								StatePlay* statePlay = new StatePlay();
								statePlay->LoadMap(saveFilesList[loadSavePos].fileName, saveFilesList[loadSavePos].compression);
								statePlay->InitCamera();
								sManager->PushState(statePlay);
							}
							else
							{
								StatePlayCreative* statePlay = new StatePlayCreative();
								statePlay->LoadMap(saveFilesList[loadSavePos].fileName, saveFilesList[loadSavePos].compression);
								statePlay->InitCamera();
								sManager->PushState(statePlay);
							}
						}
					}
				}
				if (loadSelectPos == 1)//rename world
				{
					if (saveFilesList.size() > 0)
					{
						char worldNameTemp[50];

						unsigned short test[128];
						unsigned short opis[10] = { 'W','o','r','l','d',' ','N','a','m','e' };
						if (mSystemMgr->ShowOSK(opis, test, 128) != -1)
						{
							std::string newWorldName = "";
							for (int j = 0; test[j]; j++)
							{
								unsigned c = test[j];

								if (32 <= c && c <= 127) // print ascii only
									newWorldName += c;
							}

							sprintf(worldNameTemp, "%s", newWorldName.c_str());
						}

						unsigned int saveVersionTemp = 3;
						unsigned char worldGameModeTemp = saveFilesList[loadSavePos].worldGameMode;
						bool locked = saveFilesList[loadSavePos].locked;

						FILE* pFile;
						pFile = fopen(saveFilesList[loadSavePos].fileName.c_str(), "wb");

						if (pFile != NULL)
						{
							//version
							fwrite(&saveVersionTemp, sizeof(unsigned int), 1, pFile);

							fwrite(&worldGameModeTemp, sizeof(char), 1, pFile);

							fwrite(&locked, sizeof(bool), 1, pFile);
							//name
							fwrite(worldNameTemp, sizeof(char), 50, pFile);

							fclose(pFile);
						}

						ScanSaveFiles("Save/World/");
					}
				}
				if (loadSelectPos == 2)//delete world
				{
					if (saveFilesList.size() > 0)
					{
						saveSubmenu = true;
						saveSubMenuSelect = 2;
					}
				}
				if (loadSelectPos == 3)//create new world
				{
					generateSelectPose = 0;
					menuState = 5;

					makeTrees = true;
					makeWater = true;
					makeCaves = true;
					terrainBuilder = 0;
					worldType = 0;

					newWorldName = "New World";
					newWorldSeed = "";
				}
				if (loadSelectPos == 4)//cancel
				{
					menuState = -1;
				}
				if (loadSelectPos == 2)
				{
					if (saveFilesList.size() > 0)
					{
						saveSubmenu = true;
						saveSubMenuSelect = 2;
					}
				}
			}
		}
	}
	break;
	case 3://about
	{
		if (mSystemMgr->KeyPressed(PSP_CTRL_UP))
		{
			aboutPos--;
			if (aboutPos < 0)
				aboutPos = 1;
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
		{
			aboutPos++;
			if (aboutPos > 1)
				aboutPos = 0;
		}
		if (analogUp)
		{
			if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
			{
				if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
				{
					aboutPos++;
					if (aboutPos > 1)
						aboutPos = 0;
					analogUp = false;
				}
			}
		}
		if (!analogUp)
		{
			if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
			{
				analogUp = true;
			}
		}
		if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
		{
			if (aboutPos == 1)
			{
				menuState = -1;
			}
			if (aboutPos == 0)
			{
				menuState = 6;
				converterPos = 0;

				schematicExists = fileExists("Save/world.schematic");
				ScanSaveFiles("Save/");
			}
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
		{
			menuState = -1;
		}
	}
	break;
	case 5://parametric terrain
	{

		if (mSystemMgr->KeyPressed(PSP_CTRL_UP))
		{

			if (generateSelectPose == 0)
			{
				generateSelectPose = 4;
				return;
			}

			generateSelectPose--;
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
		{
			if (generateSelectPose == 4)
			{
				generateSelectPose = 0;
				return;
			}
			generateSelectPose++;
		}
		if (analogUp)
		{
			if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
			{
				if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
				{
					if (generateSelectPose == 4)
					{
						generateSelectPose = 0;
						return;
					}
					analogUp = false;
				}
			}
		}
		if (!analogUp)
		{
			if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
			{
				analogUp = true;
			}
		}
		if (mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
		{
			ScanSaveFiles("Save/World/");

			menuState = 1;
			loadSavePos = 0;
			loadSaveStart = 0;
			loadSaveEnd = saveFilesList.size();
			loadSaveMax = 3;
			if (loadSaveMax > loadSaveEnd)
				loadSaveMax = loadSaveEnd;

			loadSelectPos = 3;
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
		{
			if (generateSelectPose == 0)
			{
				newWorldName = "";

				unsigned short test[128];
				unsigned short opis[10] = { 'W','o','r','l','d',' ','N','a','m','e' };
				if (mSystemMgr->ShowOSK(opis, test, 128) != -1)
				{
					for (int j = 0; j < 14; j++)
					{
						unsigned c = test[j];

						if (32 <= c && c <= 127) // print ascii only
							newWorldName += c;
					}
				}
				newWorldNamestr = newWorldName.c_str();
			}

			if (generateSelectPose == 1)
			{
				seed_1 = 0;
				newWorldSeed = "";

				unsigned short test[128];
				unsigned short opis[10] = { 'W','o','r','l','d',' ','s','e','e','d' };
				if (mSystemMgr->ShowOSK(opis, test, 128) != -1)
				{

					for (int j = 0; j < 14; j++)
					{
						unsigned c = test[j];

						if (c >= 32 && c <= 127)
						{
							newWorldSeed += c;
						}
					}
					seed_1 = hash(newWorldSeed.c_str(), 0);
				}
			}

			if (generateSelectPose == 2)
			{
				gameMode += 1;
				if (gameMode == 3)
				{
					gameMode = 0;
				}
			}

			if (generateSelectPose == 3)
			{
				worldType += 1;
				if (worldType > 8)
				{
					worldType = 0;
				}
			}

			if (generateSelectPose == 4)
			{
				if (gameMode == 0 || gameMode == 2)
				{
					StatePlay* statePlay = new StatePlay();
					statePlay->InitParametric(makeTrees, makeWater, makeCaves, seed_1, worldType, gameMode);
					statePlay->InitCamera();
					statePlay->SetWorldAndSaveName(newWorldName, nextSaveFileName);
					sManager->PushState(statePlay);
				}
				else
				{
					StatePlayCreative* statePlay = new StatePlayCreative();
					statePlay->InitParametric(makeTrees, makeWater, makeCaves, seed_1, worldType, gameMode);
					statePlay->InitCamera();
					statePlay->SetWorldAndSaveName(newWorldName, nextSaveFileName);
					sManager->PushState(statePlay);
				}
				seed_1 = 0;
			}
		}
	}
	break;
	case 6://converter
	{
		if (mSystemMgr->KeyPressed(PSP_CTRL_UP))
		{
			converterPos--;
			if (converterPos < 0)
				converterPos = 2;
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
		{
			converterPos++;
			if (converterPos > 2)
				converterPos = 0;
		}
		if (analogUp)
		{
			if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
			{
				if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
				{
					converterPos++;
					if (converterPos > 2)
						converterPos = 0;
					analogUp = false;
				}
			}
		}
		if (!analogUp)
		{
			if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
			{
				analogUp = true;
			}
		}
		if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
		{
			if (converterPos == 0)
			{
				if (newW_gameMode == SURVIVAL)
				{
					newW_gameMode = CREATIVE;
				}
				else
				{
					if (newW_gameMode == CREATIVE)
					{
						newW_gameMode = HARDCORE;
					}
					else
					{
						if (newW_gameMode == HARDCORE)
						{
							newW_gameMode = SURVIVAL;
						}
					}		
				}	
			}
			if (converterPos == 1)
			{
				if (schematicExists)
				{
					unsigned char empty;
					gzFile saveFile;

					/// (1) FETCH SCHEMATIC WIDTH HEIGHT LENGTH AND TAKING ITS BLOCK IDS
					saveFile = gzopen("Save/world.schematic", "rb");
					if (saveFile == 0)
						return;
					//compound
					gzread(saveFile, &empty, sizeof(char));
					gzread(saveFile, &empty, sizeof(char));
					gzread(saveFile, &empty, sizeof(char));

					for (int i = 0; i < 9; i++)
					{
						gzread(saveFile, &empty, sizeof(char));
						lol += empty;
					}

					//height
					gzread(saveFile, &empty, sizeof(char));

					gzread(saveFile, &empty, sizeof(char));
					gzread(saveFile, &empty, sizeof(char));

					for (int i = 0; i < 6; i++)
					{
						gzread(saveFile, &empty, sizeof(char));
						lol += empty;
					}
					gzread(saveFile, &empty, sizeof(char));
					gzread(saveFile, &newW_height, sizeof(char));


					//length
					gzread(saveFile, &empty, sizeof(char));

					gzread(saveFile, &empty, sizeof(char));
					gzread(saveFile, &empty, sizeof(char));

					for (int i = 0; i < 6; i++)
					{
						gzread(saveFile, &empty, sizeof(char));
						lol += empty;
					}
					gzread(saveFile, &empty, sizeof(char));
					gzread(saveFile, &newW_length, sizeof(char));


					//width
					gzread(saveFile, &empty, sizeof(char));

					gzread(saveFile, &empty, sizeof(char));
					gzread(saveFile, &empty, sizeof(char));

					for (int i = 0; i < 5; i++)
					{
						gzread(saveFile, &empty, sizeof(char));
						lol += empty;
					}
					gzread(saveFile, &empty, sizeof(char));
					gzread(saveFile, &newW_width, sizeof(char));

					//blocks
					signed short* m_Array1;
					m_Array1 = new block_t[128 * 98 * 128];
					memset(m_Array1, 0, sizeof(signed short) * 128 * 128 * 98);

					unsigned char need[6];
					bool again = false;
					for (int i = 0; i < 100000000; i++)
					{
						for (int j = 1; j <= 5; j++)
						{
							need[j - 1] = need[j];
						}
						gzread(saveFile, &empty, sizeof(unsigned char));
						need[5] = empty;

						if (need[0] == 'B' &&
							need[1] == 'l' &&
							need[2] == 'o' &&
							need[3] == 'c' &&
							need[4] == 'k' &&
							need[5] == 's')
						{
							i = 100000000;
							break;
						}

						if (need[2] == 'D' &&
							need[3] == 'a' &&
							need[4] == 't' &&
							need[5] == 'a')
						{
							gzread(saveFile, &empty, sizeof(unsigned char));
							gzread(saveFile, &empty, sizeof(unsigned char));
							gzread(saveFile, &empty, sizeof(unsigned char));
							gzread(saveFile, &empty, sizeof(unsigned char));

							gzread(saveFile, m_Array1, sizeof(unsigned char) * (128 * 98 * 128));

							again = true;;
							i = 100000000;
							break;
						}
					}

					if (again)
					{
						memset(m_Array1, 0, sizeof(unsigned char) * 128 * 128 * 98);
						for (int j = 0; j <= 6; j++)
						{
							need[j] = '/0';
						}

						for (int i = 0; i < 100000000; i++)
						{
							for (int j = 1; j <= 5; j++)
							{
								need[j - 1] = need[j];
							}
							gzread(saveFile, &empty, sizeof(unsigned char));
							need[5] = empty;

							if (need[0] == 'B' &&
								need[1] == 'l' &&
								need[2] == 'o' &&
								need[3] == 'c' &&
								need[4] == 'k' &&
								need[5] == 's')
							{
								i = 100000000;
								break;
							}
						}
					}
					gzread(saveFile, &empty, sizeof(unsigned char));
					gzread(saveFile, &empty, sizeof(unsigned char));
					gzread(saveFile, &empty, sizeof(unsigned char));
					gzread(saveFile, &empty, sizeof(unsigned char));

					gzread(saveFile, m_Array1, sizeof(unsigned char) * (128 * 98 * 128));

					gzclose(saveFile);

					// id converter
					std::vector <unsigned char> id_map;
					id_map.push_back(0); //0
					id_map.push_back(StoneBlock::getID());
					id_map.push_back(GrassBlock::getID());
					id_map.push_back(DirtBlock::getID());
					id_map.push_back(CobbleStone::getID());
					id_map.push_back(OakPlanks::getID());
					id_map.push_back(OakSapling::getID());
					id_map.push_back(bedrock::getID());
					id_map.push_back(WaterBlock::getID());
					id_map.push_back(WaterBlock::getID());
					id_map.push_back(Lava::getID());
					id_map.push_back(Lava::getID());
					id_map.push_back(SandBlock::getID());
					id_map.push_back(Gravel::getID());
					id_map.push_back(GoldBlock::getID());
					id_map.push_back(IronOre::getID());
					id_map.push_back(CoalOre::getID());
					id_map.push_back(WoodBlock::getID());
					id_map.push_back(LeavesBlock::getID());
					id_map.push_back(Sponge::getID());
					id_map.push_back(GlassBlock::getID());
					id_map.push_back(LapisOre::getID());
					id_map.push_back(LapisBlock::getID());
					id_map.push_back(0);
					id_map.push_back(SandStone::getID());
					id_map.push_back(NoteBlock::getID());
					id_map.push_back(0); // bed
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(WhiteWoolBlock::getID()); // 35
					id_map.push_back(0);
					id_map.push_back(Flower1::getID());
					id_map.push_back(Flower2::getID());
					id_map.push_back(Mooshroom1::getID());
					id_map.push_back(Mooshroom2::getID());
					id_map.push_back(GoldBlock::getID());
					id_map.push_back(bedrock::getID());
					id_map.push_back(DoubleSlab::getID());
					id_map.push_back(Slab::getID());
					id_map.push_back(BrickBlock::getID());
					id_map.push_back(TNTBlock::getID());
					id_map.push_back(ShelfBlock::getID());
					id_map.push_back(MossyCobblestone::getID());
					id_map.push_back(Obsidian::getID());
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(BirchPlanks::getID());
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(DiamondOre::getID());
					id_map.push_back(Diamond::getID());
					id_map.push_back(CraftingTable::getID());
					id_map.push_back(WheatBlock6::getID());
					id_map.push_back(Soil::getID());
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(CobbleStone::getID());
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(RedstoneOre::getID());
					id_map.push_back(RedstoneOre::getID());
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(Snow2::getID());
					id_map.push_back(IceBlock::getID());
					id_map.push_back(SnowBlock::getID());
					id_map.push_back(CactusBlock::getID());
					id_map.push_back(ClayBlock::getID());
					id_map.push_back(CaneBlock::getID());
					id_map.push_back(JukeBox::getID());
					id_map.push_back(WoodenFence::getID());
					id_map.push_back(Pumpkin1::getID());
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(JackOLantern1::getID());
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(StoneBrick::getID());
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(IronBars::getID());
					id_map.push_back(GlassPanel::getID());
					id_map.push_back(MelonBlock::getID());
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(BrickBlock::getID());
					id_map.push_back(0);
					id_map.push_back(GrassBlock::getID());
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(0);
					id_map.push_back(OakHalfBlock::getID());

					signed short* m_Array2;
					m_Array2 = new block_t[128 * 98 * 128];
					memset(m_Array2, 0, sizeof(signed short) * 128 * 128 * 98);

					for (int x = 0; x < 128; x++)
					{
						for (int z = 0; z < 128; z++)
						{
							for (int y = 0; y < 98; y++)
							{
								int take_adress = x + z * 128 + y * 128 * 128;
								int take_id = m_Array1[take_adress];

								int put_adress = x + y * 128 + z * 128 * 98;
								int put_id = 0;

								if (take_id < id_map.size())
								{
									put_id = id_map[take_id];
								}

								m_Array2[put_adress] = put_id;
							}
						}
					}

					// creating height map
					signed short * m_Heightmap;
					m_Heightmap = new block_t[128 * 128];
					memset(m_Heightmap, 0, sizeof(signed short) * 128 * 128);

					for (unsigned char x = 0; x < 128; x++)
					{
						for (unsigned char z = 0; z < 128; z++)
						{
							char adress = x + z * 128;
							for (unsigned char y = 111; y >= 0; y--)
							{
								unsigned char block_adress = x + y * 128 + z * 128 * 98;
								signed short & block = m_Array2[block_adress];

								if (block == DirtBlock::getID() || block == GrassBlock::getID() || block == SandBlock::getID() || block == StoneBlock::getID() || block == Gravel::getID() || block == SandStone::getID())
								{
									m_Heightmap[adress] = y;
									y = -1;
								}
							}
						}
					}

					// delete caves
					for (unsigned char x = 0; x < 128; x++)
					{
						for (unsigned char z = 0; z < 128; z++)
						{
							unsigned char adress = x + z * 128;
							unsigned char height = m_Heightmap[adress];
							for (unsigned char y = 0; y <= height - 1; y++)
							{
								unsigned char block_adress = x + y * 128 + z * 128 * 98;
								signed short & block = m_Array2[block_adress];

								if (block != 0)
								{
									if (block != bedrock::getID() && block != StoneBlock::getID() && block != DirtBlock::getID() && block != GoldBlock::getID() && block != IronOre::getID() && block != LapisOre::getID() && block != RedstoneOre::getID()
										&& block != Lava::getID() && block != WaterBlock::getID() && block != RedstoneOre::getID() && block != CoalOre::getID() && block != Gravel::getID() && block != DiamondOre::getID()
										&& block != SandStone::getID() && block != SandBlock::getID() && block != Mooshroom1::getID() && block != Mooshroom2::getID())
									{
										y = 100;
										continue;
									}
								}
								if (block == 0 || block == Lava::getID() || block == WaterBlock::getID())
								{
									block = StoneBlock::getID();
								}

							}
						}
					}
					free(m_Heightmap);

					/// (2) FETCH SCHEMATIC DATA
					saveFile = gzopen("Save/world.schematic", "rb");
					if (saveFile == 0)
						return;

					for (int j = 0; j <= 6; j++)
					{
						need[j] = '/0';
					}

					//data
					again = false;
					for (int i = 0; i < 100000000; i++)
					{
						for (int j = 1; j <= 5; j++)
						{
							need[j - 1] = need[j];
						}
						gzread(saveFile, &empty, sizeof(unsigned char));
						need[5] = empty;

						if (need[0] == 'B' &&
							need[1] == 'l' &&
							need[2] == 'o' &&
							need[3] == 'c' &&
							need[4] == 'k' &&
							need[5] == 's')
						{
							again = true;
							i = 100000000;

							gzread(saveFile, &empty, sizeof(unsigned char));
							gzread(saveFile, &empty, sizeof(unsigned char));
							gzread(saveFile, &empty, sizeof(unsigned char));
							gzread(saveFile, &empty, sizeof(unsigned char));

							gzread(saveFile, m_Array1, sizeof(unsigned char) * (128 * 98 * 128));

							break;
						}

						if (need[2] == 'D' &&
							need[3] == 'a' &&
							need[4] == 't' &&
							need[5] == 'a')
						{
							i = 100000000;
							break;
						}
					}

					if (again)
					{
						memset(m_Array1, 0, sizeof(unsigned char) * 128 * 128 * 98);
						for (int j = 0; j <= 6; j++)
						{
							need[j] = '/0';
						}

						for (int i = 0; i < 100000000; i++)
						{
							for (int j = 1; j <= 5; j++)
							{
								need[j - 1] = need[j];
							}
							gzread(saveFile, &empty, sizeof(unsigned char));
							need[5] = empty;

							if (need[2] == 'D' &&
								need[3] == 'a' &&
								need[4] == 't' &&
								need[5] == 'a')
							{
								i = 100000000;
								break;
							}
						}
					}

					gzread(saveFile, &empty, sizeof(unsigned char));
					gzread(saveFile, &empty, sizeof(unsigned char));
					gzread(saveFile, &empty, sizeof(unsigned char));
					gzread(saveFile, &empty, sizeof(unsigned char));

					memset(m_Array1, 0, sizeof(unsigned char) * 128 * 128 * 98);
					gzread(saveFile, m_Array1, sizeof(unsigned char) * (128 * 98 * 128));

					gzclose(saveFile);

					// place correct blocks in our world using schematic blocks data
					for (unsigned char x = 0; x < 128; x++)
					{
						for (unsigned char z = 0; z < 128; z++)
						{
							for (unsigned char y = 0; y < 98; y++)
							{
								unsigned char address = x + y * 128 + z * 128 * 98;
								unsigned char data_address = x + z * 128 + y * 128 * 128;
								signed short & block = m_Array2[address];
								unsigned char data = m_Array1[data_address];

								if (y < 111)
								{
									unsigned char up_address = x + (y + 1) * 128 + z * 128 * 98;
									signed short & up_block = m_Array2[up_address];

									if (up_block == Snow2::getID() && (block == GrassBlock::getID() || block == DirtBlock::getID()))
									{
										block = SnowSoil::getID();
										continue;
									}
								}

								if (block == LeavesBlock::getID())
								{
									if (data == 1 || data == 5 || data == 9 || data == 13)
									{
										block = SpruceLeaves::getID();
									}
									if (data == 2 || data == 6 || data == 10 || data == 14)
									{
										block = BirchLeaves::getID();
									}
									continue;
								}

								if (block == OakPlanks::getID())
								{
									if (data == 1)
									{
										block = SprucePlanks::getID();
									}
									if (data == 2)
									{
										block = BirchPlanks::getID();
									}
									continue;
								}
								if (block == WoodBlock::getID())
								{
									if (data == 1)
									{
										block = DarkWoodBlock::getID();
									}
									if (data == 2)
									{
										block = WhiteWoodBlock::getID();
									}
									continue;
								}
								if (block == WheatBlock6::getID())
								{
									if (data == 0)
									{
										block = WheatBlock1::getID();
									}
									if (data == 1 || data == 2)
									{
										block = WheatBlock2::getID();
									}
									if (data == 3 || data == 4)
									{
										block = WheatBlock3::getID();
									}
									if (data >= 5)
									{
										block = WheatBlock4::getID() + (data - 5);
									}
									continue;
								}

								if (block == DoubleSlab::getID())
								{
									if (data == 1)
									{
										block = SandStone::getID();
									}
									if (data == 2)
									{
										block = OakPlanks::getID();
									}
									if (data == 3)
									{
										block = CobbleStone::getID();
									}
									if (data == 4)
									{
										block = BrickBlock::getID();
									}
									if (data == 5)
									{
										block = StoneBrick::getID();
									}
									continue;
								}
								if (block == Slab::getID())
								{
									if (data == 2)
									{
										block = OakHalfBlock::getID();
									}
									if (data == 3)
									{
										block = CobbleHalfBlock::getID();
									}
									if (data == 4)
									{
										block = BrickHalfBlock::getID();
									}
									if (data == 5)
									{
										block = HalfStoneBrick::getID();
									}
									continue;
								}
								if (block == OakHalfBlock::getID())
								{
									if (data == 1)
									{
										block = SpruceHalfBlock::getID();
									}
									if (data == 2)
									{
										block = BirchHalfBlock::getID();
									}
									continue;
								}
								if (block == StoneBrick::getID())
								{
									if (data == 2)
									{
										block = CrackedStoneBrick::getID();
									}
									if (data == 3)
									{
										block = CarvedStoneBrick::getID();
									}
									continue;
								}
								if (block == Pumpkin1::getID())
								{
									block = Pumpkin1::getID() + data;
									continue;
								}
								if (block == WhiteWoolBlock::getID())
								{
									if (data == 1)
									{
										block = OrangeWoolBlock::getID();
									}
									if (data == 2)
									{
										block = VioletWoolBlock::getID();
									}
									if (data == 3)
									{
										block = LightBlueWoolBlock::getID();
									}
									if (data == 4)
									{
										block = YellowWoolBlock::getID();
									}
									if (data == 5)
									{
										block = GreenWoolBlock::getID();
									}
									if (data == 6)
									{
										block = PinkWoolBlock::getID();
									}
									if (data == 7)
									{
										block = GrayWoolBlock::getID();
									}
									if (data == 8)
									{
										block = LightGrayWoolBlock::getID();
									}
									if (data == 9)
									{
										block = PastelWoolBlock::getID();
									}
									if (data == 10)
									{
										block == VioletWoolBlock::getID();
									}
									if (data == 11)
									{
										block = BlueWoolBlock::getID();
									}
									if (data == 12)
									{
										block = BrownWoolBlock::getID();
									}
									if (data == 13)
									{
										block = DarkGreenWoolBlock::getID();
									}
									if (data == 14)
									{
										block = RedWoolBlock::getID();
									}
									if (data == 15)
									{
										block = BlackWoolBlock::getID();
									}
									continue;
								}
							}
						}
					}

					free(m_Array1);
					saveFile = 0;

					Vector3 playerPos = Vector3(0, 0, 0);
					for (unsigned char y = 111; y >= 40; y--)
					{
						unsigned char address = 98 + y * 128 + 128 * 128 * 98;
						unsigned char block = m_Array2[address];

						if (block != 0)
						{
							playerPos = Vector3(128.5f, y + 1.7f, 128.5f);
							break;
						}
					}

					/// (3) SAVE WORLD
					std::string filename = nextSaveFileName;

					//first save version and name on normal file
					FILE* pFile;
					pFile = fopen(filename.c_str(), "wb");

					if (pFile == NULL)
						return;

					//version
					int _worldVersion = 150;
					fwrite(&_worldVersion, sizeof(unsigned int), 1, pFile);

					//game mode
					fwrite(&newW_gameMode, sizeof(unsigned char), 1, pFile);

					//game mode
					bool locked = 0;
					fwrite(&locked, sizeof(bool), 1, pFile);

					//name
					unsigned char _worldName[50];
					_worldName[0] = 'C';
					_worldName[1] = 'o';
					_worldName[2] = 'n';
					_worldName[3] = 'v';
					_worldName[4] = 'e';
					_worldName[5] = 'r';
					_worldName[6] = 't';
					_worldName[7] = 'e';
					_worldName[8] = 'd';
					_worldName[9] = 1 + rand() % 126;
					_worldName[10] = '\0';
					fwrite(_worldName, sizeof(unsigned char), 50, pFile);

					//close file
					fclose(pFile);


					//now save compressed map info
					filename = filename + "c";

					saveFile = gzopen(filename.c_str(), "wb");
					if (saveFile == 0)
						return;

					//player pos
					gzwrite(saveFile, &playerPos.x, sizeof(float));
					gzwrite(saveFile, &playerPos.y, sizeof(float));
					gzwrite(saveFile, &playerPos.z, sizeof(float));

					//player spawn pos
					Vector3 playerSpawnPointPosition = playerPos;
					gzwrite(saveFile, &playerSpawnPointPosition.x, sizeof(float));
					gzwrite(saveFile, &playerSpawnPointPosition.y, sizeof(float));
					gzwrite(saveFile, &playerSpawnPointPosition.z, sizeof(float));

					//snow biome pos
					Vector3 snowBiomePosition = Vector3(0, 0, 0);
					float snowBiomeRadius = -1;
					gzwrite(saveFile, &snowBiomePosition.x, sizeof(float));
					gzwrite(saveFile, &snowBiomePosition.z, sizeof(float));
					gzwrite(saveFile, &snowBiomeRadius, sizeof(float));

					//desert biome pos
					Vector3 desertBiomePosition = Vector3(0, 0, 0);
					float desertBiomeRadius = -1;
					gzwrite(saveFile, &desertBiomePosition.x, sizeof(float));
					gzwrite(saveFile, &desertBiomePosition.z, sizeof(float));
					gzwrite(saveFile, &desertBiomeRadius, sizeof(float));

					//size
					unsigned char WORLD_SIZE = 256;
					gzwrite(saveFile, &WORLD_SIZE, sizeof(int));

					//size
					unsigned char WORLD_HEIGHT = 98;
					gzwrite(saveFile, &WORLD_HEIGHT, sizeof(int));

					//chunksize
					unsigned char CHUNK_SIZE = 50;
					gzwrite(saveFile, &CHUNK_SIZE, sizeof(int));

					//cubes
					gzwrite(saveFile, m_Array2, sizeof(unsigned char) * (WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE));

					for (unsigned char x = 0; x < 128; x++)
					{
						for (unsigned char z = 0; z < 128; z++)
						{
							for (unsigned char y = 0; y < 98; y++)
							{
								m_Array2[z * 98 * 128 + y * 128 + x] = 0;
							}
						}
					}

					//settings
					gzwrite(saveFile, m_Array2, sizeof(unsigned char) * (WORLD_SIZE * WORLD_HEIGHT * WORLD_SIZE));

					float worldDayTime = 12.0f;
					//world time
					gzwrite(saveFile, &worldDayTime, sizeof(float));

					gzwrite(saveFile, &mainStatistics, sizeof(st));

					gzwrite(saveFile, &mainOptions, sizeof(opt));

					short invId[36];
					short invAm[36];
					bool invSt[36];
					short armorId[4];
					short armorAm[4];
					bool armorSt[4];

					for (unsigned char o = 0; o <= 35; o += 1)
					{
						invAm[o] = -1;
						invId[o] = -1;
						invSt[o] = false;
					}

					for (unsigned char o = 0; o <= 3; o += 1)
					{
						armorId[o] = 324 + o;
						armorAm[o] = -1;
						armorSt[o] = false;
					}
					gzwrite(saveFile, &invId, sizeof(short) * (36));

					gzwrite(saveFile, &invAm, sizeof(short) * (36));

					gzwrite(saveFile, &invSt, sizeof(bool) * (36));

					gzwrite(saveFile, &armorId, sizeof(short) * (4));

					gzwrite(saveFile, &armorAm, sizeof(short) * (4));

					gzwrite(saveFile, &armorSt, sizeof(bool) * (4));

					unsigned short Earned = 0;
					gzwrite(saveFile, &Earned, sizeof(unsigned short));

					unsigned short HP = 20;
					gzwrite(saveFile, &HP, sizeof(unsigned short));

					unsigned int HG = 20;
					gzwrite(saveFile, &HG, sizeof(unsigned int));

					unsigned char OS = 10;
					gzwrite(saveFile, &OS, sizeof(unsigned char));

					float brightFactor = 1.0f;
					gzwrite(saveFile, &brightFactor, sizeof(float));

					float starsFactor = 0.0f;
					gzwrite(saveFile, &starsFactor, sizeof(float));

					int worldSeed = 0;
					gzwrite(saveFile, &worldSeed, sizeof(int));

					float skyTime = 112.0f;
					gzwrite(saveFile, &skyTime, sizeof(float));

					int savedWeatherType = 0;
					gzwrite(saveFile, &savedWeatherType, sizeof(int));

					float savedWeatherDuration = 600 + rand() % 300;
					gzwrite(saveFile, &savedWeatherDuration, sizeof(float));

					int null_var = 0;
					gzwrite(saveFile, &null_var, sizeof(int));

					gzwrite(saveFile, &null_var, sizeof(int));

					gzwrite(saveFile, &null_var, sizeof(int));

					gzwrite(saveFile, &null_var, sizeof(int));

					gzwrite(saveFile, &null_var, sizeof(int));

					gzwrite(saveFile, &null_var, sizeof(int));

					gzwrite(saveFile, &null_var, sizeof(int));

					gzwrite(saveFile, &null_var, sizeof(int));

					gzwrite(saveFile, &null_var, sizeof(int));

					gzwrite(saveFile, &null_var, sizeof(int));

					gzwrite(saveFile, &null_var, sizeof(int));

					//close file
					gzclose(saveFile);
					saveFile = 0;

					free(m_Array2);
				}
			}
			if (converterPos == 2)
			{
				menuState = 3;
			}
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
		{
			menuState = 3;
		}
	}
	break;
	case 10:
	{
		if (mSystemMgr->KeyPressed(PSP_CTRL_UP))
		{
			saveSubMenuSelect == 0 ? saveSubMenuSelect = 1 : saveSubMenuSelect = 0;
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
		{
			saveSubMenuSelect == 1 ? saveSubMenuSelect = 0 : saveSubMenuSelect = 1;
		}
		if (analogUp)
		{
			if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
			{
				if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
				{
					saveSubMenuSelect == 1 ? saveSubMenuSelect = 0 : saveSubMenuSelect = 1;
					analogUp = false;
				}
			}
		}
		if (!analogUp)
		{
			if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
			{
				analogUp = true;
			}
		}
		if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
		{
			if (tpSelectPos == 0)//play state
			{
				if (texturePackList.empty() == false)
				{
					TextureHelper::Instance()->SetTexturePack(texturePackList[tpPos].name);
					tpCurrent = tpPos;
				}
			}
			if (tpSelectPos == 1)//delete save
			{
				menuState = -1;
			}
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
		{
			unsigned char worldNameTemp[50];
			for (unsigned char i = 0; i <= 49; i++)
			{
				worldNameTemp[i] = saveFilesList[loadSavePos].worldName[i];
			}

			unsigned int saveVersionTemp = 3;
			unsigned char worldGameModeTemp = saveSubMenuSelect;
			bool locked = saveFilesList[loadSavePos].locked;

			FILE* pFile;
			pFile = fopen(saveFilesList[loadSavePos].fileName.c_str(), "wb");

			if (pFile != NULL)
			{
				//version
				fwrite(&saveVersionTemp, sizeof(int), 1, pFile);

				fwrite(&worldGameModeTemp, sizeof(char), 1, pFile);

				fwrite(&locked, sizeof(bool), 1, pFile);
				//name
				fwrite(worldNameTemp, sizeof(char), 50, pFile);

				fclose(pFile);
			}

			ScanSaveFiles("Save/World/");

			if (saveFilesList[loadSavePos].locked == true)
			{
				loadSelectPos = 1;
			}

			menuState = 1;
		}
	}
	break;
	case 13://Texture Packs
	{
		if (mSystemMgr->KeyPressed(PSP_CTRL_LEFT))
		{
			tpSelectPos--;
			if (tpSelectPos < 0)
			{
				tpSelectPos = 1;
			}
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_RIGHT))
		{
			tpSelectPos++;
			if (tpSelectPos > 1)
			{
				tpSelectPos = 0;
			}
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_UP))
		{
			tpPos--;
			if (tpPos < tpStart)
			{
				tpStart--;
				tpMax--;

				if (tpMax < 3)
				{
					tpStart = 0;
					tpMax = 3;
				}
			}

			if (tpPos < 0)
			{
				tpPos = texturePackList.size() - 1;
				tpMax = tpEnd;
				tpStart = tpEnd - 3;
				if (tpStart < 0)
					tpStart = 0;
			}
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
		{
			tpPos++;
			if (tpPos == tpMax)
			{
				tpStart++;
				tpMax++;
				if (tpMax > tpEnd)
				{
					tpStart = tpEnd - 3;
					if (tpStart < 0)
					{
						tpStart = 0;
					}
					tpMax = tpEnd;
				}
			}
			if (tpPos >= texturePackList.size())
			{
				tpPos = 0;
				tpStart = 0;
				tpMax = 3;
				if (tpMax > tpEnd)
					tpMax = tpEnd;
			}
		}
		if (analogUp)
		{
			if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
			{
				if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
				{
					tpPos++;
					if (tpPos == tpMax)
					{
						tpStart++;
						tpMax++;
						if (tpMax > tpEnd)
						{
							tpStart = tpEnd - 3;
							if (tpStart < 0)
							{
								tpStart = 0;
							}
							tpMax = tpEnd;
						}
					}
					if (tpPos >= texturePackList.size())
					{
						tpPos = 0;
						tpStart = 0;
						tpMax = 3;
						if (tpMax > tpEnd)
							tpMax = tpEnd;
					}
					analogUp = false;
				}
			}
		}
		if (!analogUp)
		{
			if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)
			{
				analogUp = true;
			}
		}
		if (mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
		{
			menuState = 11;
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
		{
			if (tpSelectPos == 0)//play state
			{
				if (texturePackList.empty() == false)
				{
					TextureHelper::Instance()->SetTexturePack(texturePackList[tpPos].name);
					tpCurrent = tpPos;
				}
			}
			if (tpSelectPos == 1)//delete save
			{
				menuState = -1;
			}
		}
	}
	break;
	case 14://skin Packs
	{
		
	}
	break;
	}
}
void StateMenu::Update(StateManager* sManager)
{
}

void StateMenu::Draw(StateManager* sManager)
{
	//start rendering
	mRender->StartFrame(1, 1, 1);
	bool background = true;
	if (background == true)
	{	
		if (bx <= -515)
		{
			bx = 1020;
		}
		bx -= 1.0f / 6.0f;

		backgroundSprite->SetPosition(bx, 136);
		backgroundSprite->DrawLinear();
	}
	switch (menuState)
	{
	case 11: // Global Resources
	{
		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));

		logoSprite->Draw();
		menu1Sprite->Draw();

		//Mods
		buttonLargeSprite->SetPosition(240, 145);
		buttonLargeSprite->Draw();

		//Texture Packs
		buttonLargeSprite->SetPosition(240, 170);
		buttonLargeSprite->Draw();

		//Slin Packs
		buttonLargeSprite->SetPosition(240, 195);
		buttonLargeSprite->Draw();

		//Cancel
		buttonLargeSprite->SetPosition(240, 220);
		buttonLargeSprite->Draw();

		//selected button
		sbuttonLargeSprite->SetPosition(240, (selectPos * 25) + 145);
		sbuttonLargeSprite->Draw();

		sceGuDisable(GU_BLEND);
		sceGuEnable(GU_DEPTH_TEST);
		mRender->SetFontStyle(0.35, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0, 0x00000000);
		mRender->DebugPrint(117, 87, "Packs in your worlds will be");
		mRender->DebugPrint(117, 100, "overridden by these packs. No one");
		mRender->DebugPrint(117, 113, "else will see the resources you set");
		mRender->DebugPrint(117, 126, "here.");

		mRender->SetFontStyle(normal_size, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0, 0x00000200); //Drop shadow text.
		mRender->DebugPrint(241, 161, "Mods");
		mRender->DebugPrint(241, 186, "Texture Packs");
		mRender->DebugPrint(241, 211, "Skin Packs");
		mRender->DebugPrint(241, 236, "Cancel");

		selectPos == 0 ? DrawText(240, 160, GU_COLOR(1, 1, 0.25, 1), normal_size, "Mods") : DrawText(240, 160, GU_COLOR(1, 1, 1, 1), normal_size, "Mods");
		selectPos == 1 ? DrawText(240, 185, GU_COLOR(1, 1, 0.25, 1), normal_size, "Texture Packs") : DrawText(240, 185, GU_COLOR(1, 1, 1, 1), normal_size, "Texture Packs");
		selectPos == 2 ? DrawText(240, 210, GU_COLOR(1, 1, 0.25, 1), normal_size, "Skin Packs") : DrawText(240, 210, GU_COLOR(1, 1, 1, 1), normal_size, "Skin Packs");
		selectPos == 3 ? DrawText(240, 235, GU_COLOR(1, 1, 0.25, 1), normal_size, "Cancel") : DrawText(240, 235, GU_COLOR(1, 1, 1, 1), normal_size, "Cancel");
		DrawText(240, 73, GU_COLOR(0.25, 0.25, 0.25, 1), normal_size, "Global Resources");

	}
	break;
	case 12: //Multiplayer
	{
		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));

		logoSprite->Draw();
		menu1Sprite->Draw();

		//back
		sbuttonLargeSprite->SetPosition(240, 220);
		sbuttonLargeSprite->Draw();
		DrawText(241, 236, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Back");
		DrawText(240, 235, GU_COLOR(1, 1, 0.25, 1), normal_size, "Back");

		DrawText(240, 151, GU_COLOR(0.25, 0.25, 0.25, 1), large_size, "Not Yet Available");
	}
	break;

	case -1://main menu
	{
		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));

		logoSprite->Draw();
		menuSprite->Draw();

		//Singleplayer
		buttonSprite->SetPosition(240, 90);
		buttonSprite->Draw();

		//Multiplayer
		buttonSprite->SetPosition(240, 120);
		buttonSprite->Draw();

		//World Converter
		buttonSprite->SetPosition(240, 150);
		buttonSprite->Draw();

		//Help & Options
		buttonSprite->SetPosition(240, 180);
		buttonSprite->Draw();

		//Global Resources
		buttonSprite->SetPosition(240, 210);
		buttonSprite->Draw();

		//selected button
		sbuttonSprite->SetPosition(240, (selectPos * 30) + 90);
		sbuttonSprite->Draw();

		sceGuDisable(GU_BLEND);
		sceGuEnable(GU_DEPTH_TEST);

		splashSize += 0.1f;
		if (splashSize > 1.0f * PI)
		{
			splashSize = 0.0f;
		}
		mRender->SetFontStyle(normal_size, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0, 0x00000200); //Drop shadow text.
		mRender->DebugPrint(241, 106, "Singleplayer");
		mRender->DebugPrint(241, 136, "Multiplayer");
		mRender->DebugPrint(241, 166, "Help & Options");
		mRender->DebugPrint(241, 196, "World Converter");
		mRender->DebugPrint(241, 226, "Global Resources");

		selectPos == 0 ? DrawText(240, 105, GU_COLOR(1, 1, 0.25, 1), normal_size, "Singleplayer") : DrawText(240, 105, GU_COLOR(1, 1, 1, 1), normal_size, "Singleplayer"); //White and yellow text.
		selectPos == 1 ? DrawText(240, 135, GU_COLOR(1, 1, 0.25, 1), normal_size, "Multiplayer") : DrawText(240, 135, GU_COLOR(1, 1, 1, 1), normal_size, "Multiplayer");
		selectPos == 2 ? DrawText(240, 165, GU_COLOR(1, 1, 0.25, 1), normal_size, "Help & Options") : DrawText(240, 165, GU_COLOR(1, 1, 1, 1), normal_size, "Help & Options");
		selectPos == 3 ? DrawText(240, 195, GU_COLOR(1, 1, 0.25, 1), normal_size, "World Converter") : DrawText(240, 195, GU_COLOR(1, 1, 1, 1), normal_size, "World Converter");	
		selectPos == 4 ? DrawText(240, 225, GU_COLOR(1, 1, 0.25, 1), normal_size, "Global Resources") : DrawText(240, 225, GU_COLOR(1, 1, 1, 1), normal_size, "Global Resources");

		switch (SplashNumber)
		{
		case 0: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Made by Notch!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Made by Notch!"); break; //Yellow text.

		case 1: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "And my pickaxe!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "And my pickaxe!"); break; //Yellow text.

		case 2: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Made in C++!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Made in C++!"); break; //Yellow text.

		case 3: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "As seen on TV!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "As seen on TV!"); break; //Yellow text.

		case 4: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "A skeleton popped out!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "A skeleton popped out!"); break; //Yellow text.

		case 5: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "More polygons!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "More polygons!"); break; //Yellow text.

		case 6: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Minecraft!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Minecraft!"); break; //Yellow text.

		case 7: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "That's no moon!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "That's no moon!"); break; //Yellow text.

		case 8: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Awesome community!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Awesome community!"); break; //Yellow text.

		case 9: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Pixels!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Pixels!"); break; //Yellow text.

		case 10: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "It's here!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "It's here!"); break; //Yellow text.

		case 11: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Fat free!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Fat free!"); break; //Yellow text.

		case 12: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Minors welcome!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Minors welcome!"); break; //Yellow text.

		case 13: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Cloud computing!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Cloud computing!"); break; //Yellow text.

		case 14: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Now in 3D!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Now in 3D!"); break; //Yellow text.

		case 15: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Yes, sir!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Yes, sir!"); break; //Yellow text.

		case 16: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Bring it on!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Bring it on!"); break; //Yellow text.

		case 17: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Random splash!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Random splash!"); break; //Yellow text.

		case 18: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Ultimate edition!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Ultimate edition!"); break; //Yellow text.

		case 19: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Han shot first!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Han shot first!"); break; //Yellow text.

		case 20: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Han shot first!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Han shot first!"); break; //Yellow text.

		case 21: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Sublime!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Sublime!"); break; //Yellow text.

		case 22: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Fan fiction!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Fan fiction!"); break; //Yellow text.

		case 23: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Uninflammable!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Uninflammable!"); break; //Yellow text.

		case 24: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Turing complete!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Turing complete!"); break; //Yellow text.

		case 25: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Undefeated!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Undefeated!"); break; //Yellow text.

		case 26: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Tyrion would love it!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Tyrion would love it!"); break; //Yellow text.

		case 27: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Read more books!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Read more books!"); break; //Yellow text.

		case 28: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Music by C418!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Music by C418!"); break; //Yellow text.

		case 29: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Tell your friends!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Tell your friends!"); break; //Yellow text.

		case 30: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Ask your doctor!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Ask your doctor!"); break; //Yellow text.

		case 31: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Toilet friendly!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Toilet friendly!"); break; //Yellow text.

		case 32: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Great job!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Great job!"); break; //Yellow text.

		case 33: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Child's Play!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Child's Play!"); break; //Yellow text.

		case 34: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Bringing home the bacon!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Bringing home the bacon!"); break; //Yellow text.

		case 35: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Engage!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Engage!"); break; //Yellow text.

		case 36: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Played by cowboys!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Played by cowboys!"); break; //Yellow text.

		case 37: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Does barrel rolls!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Does barrel rolls!"); break; //Yellow text.

		case 38: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "missingno"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "missingno"); break; //Yellow text.

		case 39: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Falling with style!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Falling with style!"); break; //Yellow text.

		case 40: DrawText(329, 71, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0.5 + sinf(splashSize) * 0.05f, "Funk soul brother!"); //Drop shadow text.
			DrawText(328, 70, GU_COLOR(1, 1, 0.25, 1), 0.5 + sinf(splashSize) * 0.05f, "Funk soul brother!"); break; //Yellow text.
		}
	}
	break;

	case 1://select world
	{
		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));
		bool smallButton = false;
		menu2Sprite->Draw();

		if (saveSubmenu) // delete world
		{
			buttonSprite->SetPosition(240, 202);
			buttonSprite->Draw();

			buttonSprite->SetPosition(240, 225);
			buttonSprite->Draw();

		}
		else
		{
			/// left part
			if (saveFilesList.empty() == false)
			{
				buttonSprite->SetPosition(145, 202); // play selected world
				buttonSprite->Draw();

				buttonSmallSprite->SetPosition(99, 225); // rename
				buttonSmallSprite->Draw();

				buttonSmallSprite->SetPosition(191, 225); // delete
				buttonSmallSprite->Draw();
			}
			else
			{
				nbuttonSprite->SetPosition(145, 202); // play selected world
				nbuttonSprite->Draw();

				nbuttonSmallSprite->SetPosition(99, 225); // rename
				nbuttonSmallSprite->Draw();

				nbuttonSmallSprite->SetPosition(191, 225); // delete
				nbuttonSmallSprite->Draw();
			}

			///right part
			buttonSprite->SetPosition(334, 202); // create new world
			buttonSprite->Draw();

			buttonSprite->SetPosition(334, 225); // cancel
			buttonSprite->Draw();
		}
		if (saveSubmenu) // delete world
		{
			sbuttonSprite->SetPosition(240, (saveSubMenuSelect * 23) + 179);
		}
		else
		{
			switch (loadSelectPos)
			{
			case 0:
				sbuttonSprite->SetPosition(145, 202);// play selected world
				break;
			case 1:
				sbuttonSmallSprite->SetPosition(99, 225);//rename
				smallButton = true;
				break;
			case 2:
				sbuttonSmallSprite->SetPosition(191, 225);//delete
				smallButton = true;
				break;
			case 3:
				sbuttonSprite->SetPosition(334, 202); // Create new world
				break;
			case 4:
				sbuttonSprite->SetPosition(334, 225); //Cancel
				break;
			}
		}
		smallButton == true ? sbuttonSmallSprite->Draw() : sbuttonSprite->Draw();

		//select sprite
		if (saveFilesList.size() > 0)
		{
			//save files
			for (int i = loadSaveStart; i < loadSaveMax; i++)
			{
				if (loadSavePos == i)
				{
					if (saveFilesList[i].worldName[0] != '\0')
					{
						mRender->SetFontStyle(0.5f, GU_COLOR(1, 1, 0, 1), 0, 0x00000000);
						mRender->DebugPrint(122, 83 + (i * 41) - (loadSaveStart * 41), "%s", saveFilesList[i].worldName);
					}
					else
					{
						mRender->SetFontStyle(0.5f, GU_COLOR(0.6, 0.6, 0, 1), 0, 0x00000000);
						mRender->DebugPrint(122, 83 + (i * 41) - (loadSaveStart * 41), "New World");
					}

					mRender->SetFontStyle(0.4f, GU_COLOR(0.5, 0.5, 0, 1), 0, 0x00000000);
					mRender->DebugPrint(122, 91 + (i * 41) - (loadSaveStart * 41), "%s", saveFilesList[i].fileName.c_str());

					mRender->SetDefaultFont();

					switch (saveFilesList[i].worldGameMode)
					{
					case 0:
						mRender->SetFontStyle(0.4f, GU_COLOR(0.5, 0.5, 0, 1), 0, 0x00000000);
						mRender->DebugPrint(122, 103 + (i * 41) - (loadSaveStart * 41), "Survival", saveFilesList[i].saveSize / 1024);
						mRender->DebugPrint(340, 91 + (i * 41) - (loadSaveStart * 41), "%i KB", saveFilesList[i].saveSize / 1024);
						break;
					case 1:
						mRender->SetFontStyle(0.4f, GU_COLOR(0.5, 0.5, 0, 1), 0, 0x00000000);
						mRender->DebugPrint(122, 103 + (i * 41) - (loadSaveStart * 41), "Creative", saveFilesList[i].saveSize / 1024);
						mRender->DebugPrint(340, 91 + (i * 41) - (loadSaveStart * 41), "%i KB", saveFilesList[i].saveSize / 1024);
						break;
					case 2:
						mRender->SetFontStyle(0.4f, GU_COLOR(0.5, 0.5, 0, 1), 0, 0x00000000);
						mRender->DebugPrint(122, 103 + (i * 41) - (loadSaveStart * 41), "Hardcore", saveFilesList[i].saveSize / 1024);
						mRender->DebugPrint(340, 91 + (i * 41) - (loadSaveStart * 41), "%i KB", saveFilesList[i].saveSize / 1024);
						break;
					}
				}
				else
				{

					if (saveFilesList[i].worldName[0] != '\0')
					{
						mRender->SetFontStyle(0.5f, GU_COLOR(1, 1, 1, 1), 0, 0x00000000);
						mRender->DebugPrint(122, 83 + (i * 41) - (loadSaveStart * 41), "%s", saveFilesList[i].worldName);
					}
					else
					{
						mRender->SetFontStyle(0.5f, GU_COLOR(1, 1, 1, 1), 0, 0x00000000);
						mRender->DebugPrint(122, 83 + (i * 41) - (loadSaveStart * 41), "New World");
					}

					mRender->SetFontStyle(0.4f, GU_COLOR(0.8f, 0.8f, 0.8f, 1), 0, 0x00000000);
					mRender->DebugPrint(122, 91 + (i * 41) - (loadSaveStart * 41), "%s", saveFilesList[i].fileName.c_str());

					mRender->SetDefaultFont();


					switch (saveFilesList[i].worldGameMode)
					{
					case 0:
						mRender->SetFontStyle(0.4f, GU_COLOR(0.8f, 0.8f, 0.8f, 1), 0, 0x00000000);
						mRender->DebugPrint(122, 103 + (i * 41) - (loadSaveStart * 41), "Survival", saveFilesList[i].saveSize / 1024);
						mRender->DebugPrint(340, 91 + (i * 41) - (loadSaveStart * 41), "%i KB", saveFilesList[i].saveSize / 1024);
						break;
					case 1:
						mRender->SetFontStyle(0.4f, GU_COLOR(0.8f, 0.8f, 0.8f, 1), 0, 0x00000000);
						mRender->DebugPrint(122, 103 + (i * 41) - (loadSaveStart * 41), "Creative", saveFilesList[i].saveSize / 1024);
						mRender->DebugPrint(340, 91 + (i * 41) - (loadSaveStart * 41), "%i KB", saveFilesList[i].saveSize / 1024);
						break;
					case 2:
						mRender->SetFontStyle(0.4f, GU_COLOR(0.8f, 0.8f, 0.8f, 1), 0, 0x00000000);
						mRender->DebugPrint(122, 103 + (i * 41) - (loadSaveStart * 41), "Hardcore", saveFilesList[i].saveSize / 1024);
						mRender->DebugPrint(340, 91 + (i * 41) - (loadSaveStart * 41), "%i KB", saveFilesList[i].saveSize / 1024);
						break;
					}
				}
			}
		}

		sceGuDisable(GU_BLEND);
		sceGuEnable(GU_DEPTH_TEST);

		if (saveSubmenu)
		{
			saveSubMenuSelect == 0 ? DrawText(240, 198, GU_COLOR(0.25, 0.25, 0.25, 1), small_size, "Are you sure?") : DrawText(240, 198, GU_COLOR(0.25, 0.25, 0.25, 1), small_size, "Are you sure?");
			mRender->SetFontStyle(normal_size, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0, 0x00000200); //Drop shadow text.
			mRender->DebugPrint(241, 218, "Yes");
			mRender->DebugPrint(241, 241, "No");
			saveSubMenuSelect == 1 ? DrawText(240, 217, GU_COLOR(1, 1, 0.25, 1), normal_size, "Yes") : DrawText(240, 217, GU_COLOR(1, 1, 1, 1), normal_size, "Yes");
			saveSubMenuSelect == 2 ? DrawText(240, 240, GU_COLOR(1, 1, 0.25, 1), normal_size, "No") : DrawText(240, 240, GU_COLOR(1, 1, 1, 1), normal_size, "No");
		}
		else
		{
			float buttonTextColor = 1.0f; // for left part
			if (saveFilesList.empty() == true)
			{
				buttonTextColor = 0.5f;
			}
			mRender->SetFontStyle(normal_size, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0, 0x00000200); //Drop shadow text.
			mRender->DebugPrint(147, 218, "Play Selected World");
			mRender->DebugPrint(100, 241, "Rename");
			mRender->DebugPrint(192, 241, "Delete");
			mRender->DebugPrint(335, 218, "Create New World");
			mRender->DebugPrint(335, 241, "Cancel");
			loadSelectPos == 0 ? DrawText(146, 217, GU_COLOR(1, 1, 0.25, 1), normal_size, "Play Selected World") : DrawText(146, 217, GU_COLOR(buttonTextColor, buttonTextColor, buttonTextColor, 1), normal_size, "Play Selected World");
			loadSelectPos == 1 ? DrawText(99, 240, GU_COLOR(1, 1, 0.25, 1), normal_size, "Rename") : DrawText(99, 240, GU_COLOR(buttonTextColor, buttonTextColor, buttonTextColor, 1), normal_size, "Rename");
			loadSelectPos == 2 ? DrawText(191, 240, GU_COLOR(1, 1, 0.25, 1), normal_size, "Delete") : DrawText(191, 240, GU_COLOR(buttonTextColor, buttonTextColor, buttonTextColor, 1), normal_size, "Delete");
			loadSelectPos == 3 ? DrawText(334, 217, GU_COLOR(1, 1, 0.25, 1), normal_size, "Create New World") : DrawText(334, 217, GU_COLOR(1.0f, 1.0f, 1.0f, 1), normal_size, "Create New World");
			loadSelectPos == 4 ? DrawText(334, 240, GU_COLOR(1, 1, 0.25, 1), normal_size, "Cancel") : DrawText(334, 240, GU_COLOR(1.0f, 1.0f, 1.0f, 1), normal_size, "Cancel");
		}
	}
	break;
	case 3://World Converter
	{
		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));

		logoSprite->Draw();
		menu1Sprite->Draw();

		buttonLargeSprite->SetPosition(240, 195);
		buttonLargeSprite->Draw();

		buttonLargeSprite->SetPosition(240, 220);
		buttonLargeSprite->Draw();

		//back
		sbuttonLargeSprite->SetPosition(240, (aboutPos * 25) + 195);
		sbuttonLargeSprite->Draw();

		sceGuDisable(GU_BLEND);
		sceGuEnable(GU_DEPTH_TEST);

		mRender->SetFontStyle(0.35, GU_COLOR(0.25, 0.25, 0.25, 1), 0, 0x00000000);
		mRender->DebugPrint(117, 87, "Using MCEdit, load your PC world.");
		mRender->DebugPrint(117, 100, "Then, change 'Edit Size' to");
		mRender->DebugPrint(117, 113, "'128x98x128' and under 'Edit'");
		mRender->DebugPrint(117, 126, "select 'Delete Entities And Items'.");
		mRender->DebugPrint(117, 139, "Export 'world.schematic' in the");
		mRender->DebugPrint(117, 152, "save folder.");
		mRender->DebugPrint(117, 165, "Worlds must be from Java Edition");
		mRender->DebugPrint(117, 178, "Release 1.2.1 - 1.3.1");

		mRender->SetFontStyle(normal_size, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0, 0x00000200); //Drop shadow text.
		mRender->DebugPrint(241, 211, "Converter");
		mRender->DebugPrint(241, 236, "Cancel");

		aboutPos == 0 ? DrawText(240, 210, GU_COLOR(1, 1, 0.25, 1), normal_size, "Converter") : DrawText(240, 210, GU_COLOR(1, 1, 1, 1), normal_size, "Converter");
		aboutPos == 1 ? DrawText(240, 235, GU_COLOR(1, 1, 0.25, 1), normal_size, "Cancel") : DrawText(240, 235, GU_COLOR(1, 1, 1, 1), normal_size, "Cancel");
		DrawText(240, 73, GU_COLOR(0.25, 0.25, 0.25, 1), normal_size, "World Converter");
	}
	break;
	case 5://paramateric view
	{
		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));

		logoSprite->Draw();
		menu1Sprite->Draw();

		//World Name
		mbuttonSprite->SetPosition(240, 90);
		mbuttonSprite->Draw();
		//Seed
		mbuttonSprite->SetPosition(240, 130);
		mbuttonSprite->Draw();
		//Game Mode
		buttonLargeSprite->SetPosition(240, 170);
		buttonLargeSprite->Draw();
		//World Type
		buttonLargeSprite->SetPosition(240, 195);
		buttonLargeSprite->Draw();
		//Create New World
		buttonLargeSprite->SetPosition(240, 220);
		buttonLargeSprite->Draw();

		if (generateSelectPose > 1)
		{
			sbuttonLargeSprite->SetPosition(240, 170 + (generateSelectPose - 2) * 25);
			sbuttonLargeSprite->Draw();
		}
		if (generateSelectPose <= 1)
		{
			smbuttonSprite->SetPosition(240, 90 + generateSelectPose * 40);
			smbuttonSprite->Draw();
		}

		sceGuDisable(GU_BLEND);
		sceGuEnable(GU_DEPTH_TEST);

		DrawText(240, 72, GU_COLOR(0.25, 0.25, 0.25, 1), normal_size, "Create New World");
		DrawText(240, 86, GU_COLOR(0.25, 0.25, 0.25, 1), small_size, "World Name");
		DrawText(240, 125, GU_COLOR(0.25, 0.25, 0.25, 1), small_size, "Seed");
		DrawText(240, 166, GU_COLOR(0.25, 0.25, 0.25, 1), small_size, "World Options");
		
		//draw subtitles on buttons
		if (gameMode == SURVIVAL)
		{
			DrawText(241, 186, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Game Mode: Survival");
			generateSelectPose == 2 ? DrawText(240, 185, GU_COLOR(1, 1, 0.25, 1), normal_size, "Game Mode: Survival") : DrawText(240, 185, GU_COLOR(1, 1, 1, 1), normal_size, "Game Mode: Survival");
		}
		if (gameMode == CREATIVE)
		{
			DrawText(241, 186, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Game Mode: Creative");
			generateSelectPose == 2 ? DrawText(240, 185, GU_COLOR(1, 1, 0.25, 1), normal_size, "Game Mode: Creative") : DrawText(240, 185, GU_COLOR(1, 1, 1, 1), normal_size, "Game Mode: Creative");
		}
		if (gameMode == HARDCORE)
		{
			DrawText(241, 186, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Game Mode: Hardcore");
			generateSelectPose == 2 ? DrawText(240, 185, GU_COLOR(1, 1, 0.25, 1), normal_size, "Game Mode: Hardcore") : DrawText(240, 185, GU_COLOR(1, 1, 1, 1), normal_size, "Game Mode: Hardcore");
		}

		if (worldType == 0)
		{
			DrawText(241, 212, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "World Type: Old");
			generateSelectPose == 3 ? DrawText(240, 211, GU_COLOR(1, 1, 0.25, 1), normal_size, "World Type: Old") : DrawText(240, 211, GU_COLOR(1, 1, 1, 1), normal_size, "World Type: Old");
		}
		if (worldType == 1)
		{
			DrawText(241, 212, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "World Type: Superflat");
			generateSelectPose == 3 ? DrawText(240, 211, GU_COLOR(1, 1, 0.25, 1), normal_size, "World Type: Superflat") : DrawText(240, 211, GU_COLOR(1, 1, 1, 1), normal_size, "World Type: Superflat");
		}
		if (worldType == 2)
		{
			DrawText(241, 212, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Biome: Forest");
			generateSelectPose == 3 ? DrawText(240, 211, GU_COLOR(1, 1, 0.25, 1), normal_size, "Biome: Forest") : DrawText(240, 211, GU_COLOR(1, 1, 1, 1), normal_size, "Biome: Forest");
		}
		if (worldType == 3)
		{
			DrawText(241, 212, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Biome: Desert Lakes");
			generateSelectPose == 3 ? DrawText(240, 211, GU_COLOR(1, 1, 0.25, 1), normal_size, "Biome: Desert Lakes") : DrawText(240, 211, GU_COLOR(1, 1, 1, 1), normal_size, "Biome: Desert Lakes");
		}
		if (worldType == 4)
		{
			DrawText(241, 212, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Biome: Snowy Taiga");
			generateSelectPose == 3 ? DrawText(240, 211, GU_COLOR(1, 1, 0.25, 1), normal_size, "Biome: Snowy Taiga") : DrawText(240, 211, GU_COLOR(1, 1, 1, 1), normal_size, "Biome: Snowy Taiga");
		}
		if (worldType == 5)
		{
			DrawText(241, 212, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Biome: Flat Desert");
			generateSelectPose == 3 ? DrawText(240, 211, GU_COLOR(1, 1, 0.25, 1), normal_size, "Biome: Flat Desert") : DrawText(240, 211, GU_COLOR(1, 1, 1, 1), normal_size, "Biome: Flat Desert");
		}
		if (worldType == 6)
		{
			DrawText(241, 212, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Biome: Flat Snow");
			generateSelectPose == 3 ? DrawText(240, 211, GU_COLOR(1, 1, 0.25, 1), normal_size, "Biome: Flat Snow") : DrawText(240, 211, GU_COLOR(1, 1, 1, 1), normal_size, "Biome: Flat Snow");
		}
		if (worldType == 7)
		{
			DrawText(241, 212, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Biome: Flat Ocean");
			generateSelectPose == 3 ? DrawText(240, 211, GU_COLOR(1, 1, 0.25, 1), normal_size, "Biome: Flat Ocean") : DrawText(240, 211, GU_COLOR(1, 1, 1, 1), normal_size, "Biome: Flat Ocean");
		}
		if (worldType == 8)
		{
			DrawText(241, 212, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Biome: Flat Nether");
			generateSelectPose == 3 ? DrawText(240, 211, GU_COLOR(1, 1, 0.25, 1), normal_size, "Biome: Flat Nether") : DrawText(240, 211, GU_COLOR(1, 1, 1, 1), normal_size, "Biome: Flat Nether");
		}

		DrawText(241, 236, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Create New World");
		generateSelectPose == 4 ? DrawText(240, 235, GU_COLOR(1, 1, 0.25, 1), normal_size, "Create New World") : DrawText(240, 235, GU_COLOR(1, 1, 1, 1), normal_size, "Create New World");

		mRender->SetFontStyle(normal_size, GU_COLOR(1, 1, 1, 1), 0, 0x00000000 | 0x00004000);
		mRender->DebugPrint(124, 105, "%s", newWorldName.c_str());
		if (seed_1 == 0)
		{
			mRender->SetFontStyle(normal_size, GU_COLOR(0.65, 0.65, 0.65, 1), 999, 0x00000200 | 0x00004000);
			mRender->DebugPrint(240, 145, "random");
		}
		else
		{
			mRender->DebugPrint(124, 145, "%s", newWorldSeed.c_str());
		}
		mRender->SetDefaultFont();
	}
	break;
	case 6:// World Converter
	{
		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));

		logoSprite->Draw();
		menu1Sprite->Draw();

		buttonLargeSprite->SetPosition(240, 170);
		buttonLargeSprite->Draw();

		buttonLargeSprite->SetPosition(240, 195);
		buttonLargeSprite->Draw();

		buttonLargeSprite->SetPosition(240, 220);
		buttonLargeSprite->Draw();

		//back
		int y_pos = 170;
		if (converterPos == 1)
		{
			y_pos = 195;
		}
		if (converterPos == 2)
		{
			y_pos = 220;
		}
		sbuttonLargeSprite->SetPosition(240, y_pos);
		sbuttonLargeSprite->Draw();

		sceGuDisable(GU_BLEND);
		sceGuEnable(GU_DEPTH_TEST);

		if (schematicExists)
		{
			DrawText(240, 120, GU_COLOR(0.1, 0.9, 0.1, 1), large_size, "'world.schematic' is found!");
		}
		else
		{
			DrawText(240, 120, GU_COLOR(0.9, 0.1, 0.1, 1), large_size, "'world.schematic' is missing!");
		}

		if (newW_gameMode == SURVIVAL)
		{
			DrawText(241, 186, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Game Mode: Survival");
			converterPos == 0 ? DrawText(240, 185, GU_COLOR(1, 1, 0.25, 1), normal_size, "Game Mode: Survival") : DrawText(240, 185, GU_COLOR(1, 1, 1, 1), normal_size, "Game Mode: Survival");
		}
		if (newW_gameMode == CREATIVE)
		{
			DrawText(241, 186, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Game Mode: Creative");
			converterPos == 0 ? DrawText(240, 185, GU_COLOR(1, 1, 0.25, 1), normal_size, "Game Mode: Creative") : DrawText(240, 185, GU_COLOR(1, 1, 1, 1), normal_size, "Game Mode: Creative");
		}
		if (newW_gameMode == HARDCORE)
		{
			DrawText(241, 186, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Game Mode: Hardcore");
			converterPos == 0 ? DrawText(240, 185, GU_COLOR(1, 1, 0.25, 1), normal_size, "Game Mode: Hardcore") : DrawText(240, 185, GU_COLOR(1, 1, 1, 1), normal_size, "Game Mode: Hardcore");
		}
		DrawText(241, 211, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Try to convert");
		DrawText(241, 236, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Cancel");
		converterPos == 1 ? DrawText(240, 210, GU_COLOR(1, 1, 0.25, 1), normal_size, "Try to convert") : DrawText(240, 210, GU_COLOR(1, 1, 1, 1), normal_size, "Try to convert");
		converterPos == 2 ? DrawText(240, 235, GU_COLOR(1, 1, 0.25, 1), normal_size, "Cancel") : DrawText(240, 235, GU_COLOR(1, 1, 1, 1), normal_size, "Cancel");

		DrawText(240, 73, GU_COLOR(0.25, 0.25, 0.25, 1), normal_size, "World Converter");
	}
	break;
	case 10: // World Converter (Game Mode)
	{
		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));

		logoSprite->Draw();
		menu2Sprite->Draw();

		buttonLargeSprite->SetPosition(240, 100);
		buttonLargeSprite->Draw();

		buttonLargeSprite->SetPosition(240, 140);
		buttonLargeSprite->Draw();

		sbuttonLargeSprite->SetPosition(240, 100 + saveSubMenuSelect * 40);
		sbuttonLargeSprite->Draw();

		sceGuDisable(GU_BLEND);
		sceGuEnable(GU_DEPTH_TEST);
		DrawText(240, 75, GU_COLOR(1, 1, 1, 1), normal_size, "Choose Game Mode");
		DrawText(241, 116, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Survival");
		DrawText(241, 146, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Creative");
		DrawText(241, 176, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Hardcore");
		saveSubMenuSelect == 0 ? DrawText(240, 115, GU_COLOR(1, 1, 0.25, 1), normal_size, "Survival") : DrawText(240, 115, GU_COLOR(1, 1, 1, 1), normal_size, "Survival");
		saveSubMenuSelect == 1 ? DrawText(240, 145, GU_COLOR(1, 1, 0.25, 1), normal_size, "Creative") : DrawText(240, 145, GU_COLOR(1, 1, 1, 1), normal_size, "Creative");
		saveSubMenuSelect == 2 ? DrawText(240, 175, GU_COLOR(1, 1, 0.25, 1), normal_size, "Hardcore") : DrawText(240, 175, GU_COLOR(1, 1, 1, 1), normal_size, "Hardcore");
	}
	break;
	case 13://Texture Packs
	{
		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));

		menu1Sprite->Draw();

		for (unsigned char i = tpStart; i < tpMax; i++)
		{
			if (i < texturePackList.size())
			{
				sceGuEnable(GU_BLEND);

				rectFilledSprite->SetPosition(240, 87 + (tpCurrent * 43) - (tpStart * 43));

				if (i == tpCurrent)
				{
					rectFilledSprite->Draw();
				}

				rectEmptySprite->SetPosition(240, 87 + (tpPos * 43) - (tpStart * 43));
				if (i == tpPos)
				{
					rectEmptySprite->Draw();
				}

				Sprite* DrawSprite = texturePackList[i].packSprite;
				DrawSprite->SetPosition(133, 88 + (i * 42) - (tpStart * 42));
				DrawSprite->ConstDraw();

				sceGuDisable(GU_BLEND);

				if (i == tpPos)
				{
					mRender->SetFontStyle(normal_size, GU_COLOR(0.4, 0.4, 0, 1), 0, 0x00000000);
					mRender->DebugPrint(152, 88 + (i * 44) - (tpStart * 44), "%s", texturePackList[i].name.c_str());

					mRender->SetFontStyle(normal_size, GU_COLOR(1, 1, 0, 1), 0, 0x00000000);
					mRender->DebugPrint(151, 87 + (i * 44) - (tpStart * 44), "%s", texturePackList[i].name.c_str());

					mRender->SetFontStyle(small_size, GU_COLOR(0.5, 0.5, 0, 1), 0, 0x00000000);
					mRender->DebugPrint(151, 93 + (i * 44) - (tpStart * 44), "%s", texturePackList[i].description.c_str());
				}
				else
				{
					mRender->SetFontStyle(normal_size, GU_COLOR(0.25, 0.25, 0.25, 1), 0, 0x00000000);
					mRender->DebugPrint(152, 88 + (i * 44) - (tpStart * 44), "%s", texturePackList[i].name.c_str());

					mRender->SetFontStyle(normal_size, GU_COLOR(1, 1, 1, 1), 0, 0x00000000);
					mRender->DebugPrint(151, 87 + (i * 44) - (tpStart * 44), "%s", texturePackList[i].name.c_str());

					mRender->SetFontStyle(small_size, GU_COLOR(0.25, 0.25, 0.25, 1), 0, 0x00000000);
					mRender->DebugPrint(151, 93 + (i * 44) - (tpStart * 44), "%s", texturePackList[i].description.c_str());
				}
				mRender->SetDefaultFont();
			}
		}

		buttonMediumSprite->SetPosition(177.5f, 220);
		buttonMediumSprite->Draw();

		buttonMediumSprite->SetPosition(302.5f, 220);
		buttonMediumSprite->Draw();


		sbuttonMediumSprite->SetPosition(177.5f + (tpSelectPos * 125.0f), 220);
		sbuttonMediumSprite->Draw();

		sceGuDisable(GU_BLEND);
		sceGuEnable(GU_DEPTH_TEST);

		DrawText(181, 235, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Select");
		DrawText(302, 235, GU_COLOR(0.25f, 0.25f, 0.25f, 1), normal_size, "Cancel");
		tpSelectPos == 0 ? DrawText(180, 234, GU_COLOR(1, 1, 0.25, 1), normal_size, "Select") : DrawText(180, 234, GU_COLOR(1, 1, 1, 1), normal_size, "Select");
		tpSelectPos == 1 ? DrawText(301, 234, GU_COLOR(1, 1, 0.25, 1), normal_size, "Cancel") : DrawText(301, 234, GU_COLOR(1, 1, 1, 1), normal_size, "Cancel");
	}
	break;
	case 14://skins
	{
		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));

		menu3Sprite->Draw();
		DrawText(281, 225, GU_COLOR(0.25, 0.25, 0.25, 1), normal_size, "Steve");
		DrawText(280, 225, GU_COLOR(1, 1, 1, 1), normal_size, "Steve");

		/*/Steve
		MatrixTranslation(Vector3(220, 85, -14));
		MatrixScale(Vector3(50, 50, 50));
		MatrixAngle(Vector3(-14, invSteveAngle, 180));

		TextureManager::Instance()->SetTextureModeulate(Steve);

		invPlayer->mainAngle = 0.45f;

		DrawSetDepthMask(false);
		DrawSetCulling(true);
		invPlayer->Render(mWorld, dt, invSteveAngle, Steve, mWorld->invId[27 + barPosition]);
		DrawSetCulling(false);
		DrawSetDepthMask(true);

		MatrixPop();*/
	}
	break;
	}
	mRender->EndFrame();
}

//additional functions
void StateMenu::ScanSaveFiles(const char* dirName)
{
	//clear list with save files
	saveFilesList.clear();

	DIR* Dir;
	struct dirent* DirEntry;
	Dir = opendir(dirName);

	while ((DirEntry = readdir(Dir)) != NULL)
	{
		if (DirEntry->d_stat.st_attr & FIO_SO_IFREG) // we found file
		{
			SaveFile newSaveFile;
			std::string plik = dirName;
			plik += DirEntry->d_name;
			//dont load lmsc files
			size_t found = plik.find(".lmsc");
			size_t found2 = plik.find(".LMSc");
			if (found == std::string::npos && found2 == std::string::npos)
			{
				newSaveFile.fileName = plik;
				saveFilesList.push_back(newSaveFile);
			}
		}
	}

	closedir(Dir);


	//now update all info in save files
	for (unsigned int i = 0; i < saveFilesList.size(); i++)
	{
		FILE* pFile;
		pFile = fopen(saveFilesList[i].fileName.c_str(), "rb");

		if (pFile != NULL)
		{
			//version
			fread(&saveFilesList[i].saveVersion, sizeof(int), 1, pFile);

			fread(&saveFilesList[i].worldGameMode, sizeof(char), 1, pFile);

			fread(&saveFilesList[i].locked, sizeof(bool), 1, pFile);
			//name
			fread(saveFilesList[i].worldName, sizeof(char), 50, pFile);

			if (saveFilesList[i].saveVersion >= 2)
				saveFilesList[i].compression = true;
			else
				saveFilesList[i].compression = false;

			fclose(pFile);
		}

		saveFilesList[i].saveSize = fileSize(saveFilesList[i].fileName + "c");
	}

	//set next save name and numer
	nextSaveFileNumber = saveFilesList.size() + 1;
	{
		//ride thru all saves and check if i can use new name
		char liczba[3];
		sprintf(liczba, "%d", nextSaveFileNumber);
		nextSaveFileName = "Save/World/";
		nextSaveFileName += liczba;
		nextSaveFileName += ".lms";

		for (unsigned int i = 0; i < saveFilesList.size(); i++)
		{
			if (nextSaveFileName.compare(saveFilesList[i].fileName) == 0)
			{
				nextSaveFileNumber += 1;
				char liczba[3];
				sprintf(liczba, "%d", nextSaveFileNumber);
				nextSaveFileName = "Save/World/";
				nextSaveFileName += liczba;
				nextSaveFileName += ".lms";
			}
		}
	}
}

void StateMenu::ScanTexturePacks(const char* dirName)
{
	if (texturePackList.empty() != false)
	{
		TextureHelper::Instance()->RemoveConstTextures();
		for (int j = 0; j < texturePackList.size(); j++)
		{
			if (texturePackList[j].packSprite != NULL)
			{
				delete texturePackList[j].packSprite;
			}
		}
		texturePackList.clear();
	}
	texturePackList.clear();

	TextureManager::Instance()->LoadConstTexture("Assets/unknown_pack.png");

	int TPcheck = 0;

	DIR* dir = opendir(dirName);
	struct dirent* entry;

	while ((entry = readdir(dir)) != NULL)
	{
		if (FIO_SO_ISDIR(entry->d_stat.st_attr))
		{
			std::string plik = "";
			plik += entry->d_name;

			size_t found = plik.find(".");
			size_t found2 = plik.find("..");
			size_t found3 = plik.find(".svn");

			if (found == std::string::npos && found2 == std::string::npos && found3 == std::string::npos)
			{
				TP newTP;

				std::string plik2 = plik + "/";
				if (plik2 == TextureHelper::Instance()->defaultFolder)
				{
					tpCurrent = TPcheck;
				}

				newTP.name = plik;

				texturePackList.push_back(newTP);
				TPcheck++;
			}
		}
	}

	closedir(dir);

	for (int j = 0; j < texturePackList.size(); j++)
	{
		std::string packPath = "Assets/Textures/" + texturePackList[j].name + "/pack.png";

		if (fileExists(packPath) == true) // if we have pack sprite
		{
			TextureManager::Instance()->LoadConstTexture(packPath);
			texturePackList[j].packSprite = new Sprite(TextureManager::Instance()->GetConstTextureNumber(packPath), true);
		}
		else
		{
			texturePackList[j].packSprite = new Sprite(TextureManager::Instance()->GetConstTextureNumber("Assets/unknown_pack.png"), true);
		}

		std::string packDescriptionPath = "Assets/Textures/" + texturePackList[j].name + "/pack.txt";

		if (fileExists(packDescriptionPath) == true) // if we have pack description file
		{
			std::string str;
			std::string file_contents;

			std::ifstream file(packDescriptionPath.c_str());

			if (file.is_open())
			{
				while (std::getline(file, str))
				{
					file_contents += str;
					file_contents.push_back('\n');
				}
			}

			file.close();
			texturePackList[j].description = file_contents;
		}
	}
}
inline bool StateMenu::fileExists(const std::string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

unsigned int StateMenu::hash(const char* s, unsigned short seed)
{
	unsigned short hash = seed;
	while (*s)
	{
		hash = hash * 101 + *s++;
	}
	return hash;
}

int StateMenu::fileSize(const std::string& name)
{
	struct stat stat_buf;
	int rc = stat(name.c_str(), &stat_buf);
	return rc == 0 ? (int)stat_buf.st_size : -1;
}

void StateMenu::DrawText(int x, int y, unsigned int color, float size, const char* message, ...)
{
	mRender->SetFontStyle(size, color, 0, 0x00000200 | 0x00000000);
	mRender->DebugPrint(x, y, message);
}