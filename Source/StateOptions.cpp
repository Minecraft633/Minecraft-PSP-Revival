#include "StateOptions.h"
#include "InputHelper.h"
#include "TextureHelper.h"

#define normal_size 0.44
#define small_size 0.35

StateOptions::StateOptions()
{
}

StateOptions::~StateOptions()
{
}

void StateOptions::Init()
{
	//set render manager instance
	mRender = RenderManager::InstancePtr();
	mSystemMgr = SystemManager::Instance();
	mSoundMgr = SoundManager::Instance();

	dark_backgroundSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::dark_background));
	dark_backgroundSprite->SetPosition(240, 136);

	menu1Sprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::menu1));
	menu1Sprite->SetPosition(240, 136);

	backgroundSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::background));
	backgroundSprite->Scale(4.25f, 4.25f);
	backgroundSprite->SetPosition(240, 136);
	background = true;
	bx = 900;

	buttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons), 0, 0, 128, 16); // stand
	buttonSprite->SetPosition(240, 150);
	buttonSprite->Scale(1.45f, 1.45f);

	sbuttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons), 0, 16, 128, 16); // stand selected
	sbuttonSprite->SetPosition(240, 150);
	sbuttonSprite->Scale(1.45f, 1.45f);

	nbuttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons), 0, 32, 128, 16); // dark
	nbuttonSprite->SetPosition(240, 150);
	nbuttonSprite->Scale(1.35f, 1.0f);
	nbuttonSprite->Scale(1.45f, 1.45f);

	buttonSmallSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons), 0, 0, 128, 16); // stand
	buttonSmallSprite->SetPosition(240, 150);
	buttonSmallSprite->Scale(0.7f, 1.0f);
	buttonSmallSprite->Scale(1.45f, 1.45f);

	sbuttonSmallSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons), 0, 16, 128, 16); // stand selected
	sbuttonSmallSprite->SetPosition(240, 150);
	sbuttonSmallSprite->Scale(0.7f, 1.0f);
	sbuttonSmallSprite->Scale(1.45f, 1.45f);

	buttonLargeSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons), 0, 0, 128, 16); // stand
	buttonLargeSprite->SetPosition(240, 150);
	buttonLargeSprite->Scale(1.35f, 1.0f);
	buttonLargeSprite->Scale(1.45f, 1.45f);

	sbuttonLargeSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons), 0, 16, 128, 16); // stand selected
	sbuttonLargeSprite->SetPosition(240, 150);
	sbuttonLargeSprite->Scale(1.35f, 1.0f);
	sbuttonLargeSprite->Scale(1.45f, 1.45f);

	smbuttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons), 0, 65, 128, 16); // gray selected
	smbuttonSprite->SetPosition(240, 150);
	smbuttonSprite->Scale(1.35f, 1.0f);
	smbuttonSprite->Scale(1.45f, 1.45f);

	moverSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons), 0, 81, 8, 16);
	moverSprite->Scale(1.45f, 1.45f);

	selectPos = 0;
	menuState = 0;

	//controls
	controlPos = 0;
	controlStart = 0;
	controlEnd = 0;

	//key states
	chooseKeyState = false;
	configChanged = false;
	currentKey = -1;
	newKey = -1;

	currentAnalogPos = 0;
}

void StateOptions::Enter()
{
	mRender->SetOrtho(0, 0, 0, 0, 0, 0);
	selectPos = 0;
}

void StateOptions::CleanUp()
{
	delete dark_backgroundSprite;
	delete backgroundSprite;
	delete buttonSprite;
	delete nbuttonSprite;
	delete sbuttonSprite;
	delete buttonSmallSprite;
	delete sbuttonSmallSprite;
	delete buttonLargeSprite;
	delete sbuttonLargeSprite;
	delete smbuttonSprite;
	delete moverSprite;
}

void StateOptions::Pause()
{
}

void StateOptions::Resume()
{
	menuState = 0;
	mRender->SetOrtho(0, 0, 0, 0, 0, 0);
}

void StateOptions::HandleEvents(StateManager* sManager)
{
	//update input
	mSystemMgr->InputUpdate();
	if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
	{
		mSoundMgr->PlayMenuSound();
	}

	switch (menuState)
	{
	case 0:
	{
		//up, down
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

		if (mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
		{
			InputHelper::Instance()->Save();
			sManager->PopState();
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
		{
			if (selectPos == 0)
			{
				InputHelper::Instance()->Limit_RAM == true ? InputHelper::Instance()->Limit_RAM = false : InputHelper::Instance()->Limit_RAM = true;
			}
			if (selectPos == 1)
			{
				menuState = 1;

				controlPos = 0;
				controlStart = 0;
				controlEnd = 5;

				//reset key state settings
				chooseKeyState = false;
				configChanged = false;
				currentKey = -1;
				newKey = -1;
			}
			if (selectPos == 2)
			{
				menuState = 2;
				currentAnalogPos = 0;
			}
			if (selectPos == 3)
			{
				menuState = 3;
			}
			if (selectPos == 4)
			{
				InputHelper::Instance()->Save();
				sManager->PopState();
			}
		}
	}
	break;
	case 1:
	{
		if (chooseKeyState)//here we set new key for action
		{
			//check each button
			if (mSystemMgr->KeyPressed(PSP_CTRL_UP))
				currentKey = 0;
			if (mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
				currentKey = 1;
			if (mSystemMgr->KeyPressed(PSP_CTRL_LEFT))
				currentKey = 2;
			if (mSystemMgr->KeyPressed(PSP_CTRL_RIGHT))
				currentKey = 3;

			if (mSystemMgr->KeyPressed(PSP_CTRL_TRIANGLE))
				currentKey = 4;
			if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
				currentKey = 5;
			if (mSystemMgr->KeyPressed(PSP_CTRL_SQUARE))
				currentKey = 6;
			if (mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
				currentKey = 7;

			if (mSystemMgr->KeyPressed(PSP_CTRL_LTRIGGER))
				currentKey = 8;
			if (mSystemMgr->KeyPressed(PSP_CTRL_RTRIGGER))
				currentKey = 9;
			if (mSystemMgr->KeyPressed(PSP_CTRL_SELECT))
				currentKey = 10;
			if (mSystemMgr->KeyPressed(PSP_CTRL_START))
				currentKey = 15;

			if (mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)//analog up
				currentKey = 12;
			if (mSystemMgr->GetAnalogY() < InputHelper::Instance()->analogYdown)//analog down
				currentKey = 11;
			if (mSystemMgr->GetAnalogX() < InputHelper::Instance()->analogXleft)//analog left
				currentKey = 13;
			if (mSystemMgr->GetAnalogX() > InputHelper::Instance()->analogXright)//analog right
				currentKey = 14;

			if (currentKey != -1)
			{
				//chek if it the same key
				if (InputHelper::Instance()->getButtonToAction(controlPos) != currentKey)
					configChanged = true;

				//set key
				InputHelper::Instance()->setButtonToAction(controlPos, currentKey);

				//quit
				chooseKeyState = false;
			}
		}
		else
		{
			if (mSystemMgr->KeyPressed(PSP_CTRL_UP))
			{
				controlPos--;

				if (controlPos < controlStart)
				{
					controlStart--;
					controlEnd--;

					if (controlEnd < 5)
					{
						controlStart = 0;
						controlEnd = 5;
					}
				}

				if (controlPos < 0)
				{
					controlPos = 15;
					controlEnd = 15;
					controlStart = 10;
					if (controlStart < 0)
						controlStart = 0;
				}
			}

			if (mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
			{
				controlPos++;

				if (controlPos == controlEnd)
				{
					controlStart++;
					controlEnd++;
					if (controlEnd > 15)
					{
						controlStart = 10;
						controlEnd = 15;
					}
				}
				if (controlPos > 15)
				{
					controlPos = 0;
					controlStart = 0;
					controlEnd = 5;
				}
			}

			if (mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
			{
				menuState = 0;
			}

			if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
			{
				if (controlPos == 15)//cancel button - exit from controls menu
				{
					menuState = 0;
				}
				else//start key choosing
				{
					currentKey = -1;
					chooseKeyState = true;
				}
			}
		}
	}
	break;
	case 2:
	{
		if (mSystemMgr->KeyPressed(PSP_CTRL_UP))
		{
			currentAnalogPos--;

			if (currentAnalogPos < 0)
				currentAnalogPos = 4;
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
		{
			currentAnalogPos++;
			if (currentAnalogPos > 4)
				currentAnalogPos = 0;
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_LEFT))
		{
			if (currentAnalogPos == 0 || currentAnalogPos == 3)
			{
				if (currentAnalogPos == 0)
				{
					InputHelper::Instance()->analogYup -= 0.1f;
					if (InputHelper::Instance()->analogYup < 0.1f)
						InputHelper::Instance()->analogYup = 0.1f;
				}
				else
				{
					InputHelper::Instance()->analogXright -= 0.1f;
					if (InputHelper::Instance()->analogXright < 0.1f)
						InputHelper::Instance()->analogXright = 0.1f;
				}
			}
			if (currentAnalogPos == 1 || currentAnalogPos == 2)
			{
				if (currentAnalogPos == 1)
				{
					InputHelper::Instance()->analogYdown += 0.1f;
					if (InputHelper::Instance()->analogYdown > -0.1f)
						InputHelper::Instance()->analogYdown = -0.1f;
				}
				else
				{
					InputHelper::Instance()->analogXleft += 0.1f;
					if (InputHelper::Instance()->analogXleft > -0.1f)
						InputHelper::Instance()->analogXleft = -0.1f;
				}
			}
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_RIGHT))
		{
			if (currentAnalogPos == 0 || currentAnalogPos == 3)
			{
				if (currentAnalogPos == 0)
				{
					InputHelper::Instance()->analogYup += 0.1f;
					if (InputHelper::Instance()->analogYup > 0.9f)
						InputHelper::Instance()->analogYup = 0.9f;
				}
				else
				{
					InputHelper::Instance()->analogXright += 0.1f;
					if (InputHelper::Instance()->analogXright > 0.9f)
						InputHelper::Instance()->analogXright = 0.9f;
				}
			}
			if (currentAnalogPos == 1 || currentAnalogPos == 2)
			{
				if (currentAnalogPos == 1)
				{
					InputHelper::Instance()->analogYdown -= 0.1f;
					if (InputHelper::Instance()->analogYdown < -0.9f)
						InputHelper::Instance()->analogYdown = -0.9f;
				}
				else
				{
					InputHelper::Instance()->analogXleft -= 0.1f;
					if (InputHelper::Instance()->analogXleft < -0.9f)
						InputHelper::Instance()->analogXleft = -0.9f;
				}
			}
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
		{
			menuState = 0;
		}

		if (mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
		{
			if (currentAnalogPos == 4)
				menuState = 0;
		}
	}
	break;
	case 3:
	{
		if (mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE) || mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
		{
			menuState = 0;
		}
	}
	break;
	}
}

void StateOptions::Update(StateManager* sManager)
{
}

void StateOptions::Draw(StateManager* sManager)
{
	mRender->StartFrame(1, 1, 1);

	if (background == true)
	{
		if (bx <= -515)
		{
			bx = 1020;
		}
		bx -= 1.0f / 6.0f;
	}

	switch (menuState)
	{
	case 0:
	{
		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));

		backgroundSprite->SetPosition(bx, 136);
		backgroundSprite->DrawLinear();

		menu1Sprite->Draw();

		mRender->SetFontStyle(0.35, GU_COLOR(0.28, 0.28, 0.28, 1), 0, 0x00000000);
		mRender->DebugPrint(117, 87, "Selecting 64MB on a 32MB PSP will");
		mRender->DebugPrint(117, 100, "result in a crash when loading or");
		mRender->DebugPrint(117, 113, "creating new worlds.");

		// Available Ram
		buttonLargeSprite->SetPosition(240, 120);
		buttonLargeSprite->Draw();

		// Analog Sensitivity
		buttonLargeSprite->SetPosition(240, 145);
		buttonLargeSprite->Draw();

		// Controls
		buttonLargeSprite->SetPosition(240, 170);
		buttonLargeSprite->Draw();

		// Credits
		buttonLargeSprite->SetPosition(240, 195);
		buttonLargeSprite->Draw();

		// Cancel
		buttonLargeSprite->SetPosition(240, 220);
		buttonLargeSprite->Draw();

		// Selected button
		sbuttonLargeSprite->SetPosition(240, (selectPos * 25) + 120);
		sbuttonLargeSprite->Draw();

		sceGuDisable(GU_BLEND);
		sceGuEnable(GU_DEPTH_TEST);

		if (InputHelper::Instance()->Limit_RAM == true)
		{
			DrawText(241, 137, GU_COLOR(0.28f, 0.28f, 0.28f, 1), normal_size, "Available Ram: 32MB");
			selectPos == 0 ? DrawText(240, 136, GU_COLOR(1, 1, 0.25, 1), normal_size, "Available Ram: 32MB") : DrawText(240, 136, GU_COLOR(1, 1, 1, 1), normal_size, "Available Ram: 32MB");
		}
		if (InputHelper::Instance()->Limit_RAM == false)
		{
			DrawText(241, 137, GU_COLOR(0.28f, 0.28f, 0.28f, 1), normal_size, "Available Ram: 64MB");
			selectPos == 0 ? DrawText(240, 136, GU_COLOR(1, 1, 0.25, 1), normal_size, "Available Ram: 64MB") : DrawText(240, 136, GU_COLOR(1, 1, 1, 1), normal_size, "Available Ram: 64MB");
		}

		DrawText(241, 161, GU_COLOR(0.28f, 0.28f, 0.28f, 1), normal_size, "Controls");
		selectPos == 1 ? DrawText(240, 160, GU_COLOR(1, 1, 0, 1), normal_size, "Controls") : DrawText(240, 160, GU_COLOR(1, 1, 1, 1), normal_size, "Controls");

		DrawText(241, 186, GU_COLOR(0.28f, 0.28f, 0.28f, 1), normal_size, "Analog Sensitivity");
		selectPos == 2 ? DrawText(240, 185, GU_COLOR(1, 1, 0, 1), normal_size, "Analog Sensitivity") : DrawText(240, 185, GU_COLOR(1, 1, 1, 1), normal_size, "Analog Sensitivity");

		DrawText(241, 211, GU_COLOR(0.28f, 0.28f, 0.28f, 1), normal_size, "Credits");
		selectPos == 3 ? DrawText(240, 210, GU_COLOR(1, 1, 0, 1), normal_size, "Credits") : DrawText(240, 210, GU_COLOR(1, 1, 1, 1), normal_size, "Credits");

		DrawText(241, 236, GU_COLOR(0.28f, 0.28f, 0.28f, 1), normal_size, "Cancel");
		selectPos == 4 ? DrawText(240, 235, GU_COLOR(1, 1, 0, 1), normal_size, "Cancel") : DrawText(240, 235, GU_COLOR(1, 1, 1, 1), normal_size, "Cancel");

		DrawText(240, 73, GU_COLOR(0.28, 0.28, 0.28, 1), normal_size, "Help & Options");

	}
	break;
	case 1:// Controls
	{
		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));

		backgroundSprite->SetPosition(bx, 136);
		backgroundSprite->DrawLinear();
		menu1Sprite->Draw();
		int starty = 90;
		for (int i = controlStart; i < controlEnd; i++)
		{
			if (controlPos == i)
			{
				sbuttonSmallSprite->SetPosition(180, starty + (i * 25) - (controlStart * 25));
				sbuttonSmallSprite->Draw();
			}
			else
			{
				buttonSmallSprite->SetPosition(180, starty + (i * 25) - (controlStart * 25));
				buttonSmallSprite->Draw();
			}
		}
		// Cancel
		if (controlPos == 15)
		{
			sbuttonLargeSprite->SetPosition(240, 220);
			sbuttonLargeSprite->Draw();
		}
		else
		{
			buttonLargeSprite->SetPosition(240, 220);
			buttonLargeSprite->Draw();
		}
		sceGuDisable(GU_BLEND);
		sceGuEnable(GU_DEPTH_TEST);

		//write action names
		starty = 100;
		mRender->SetFontStyle(normal_size, 0xFFFFFFFF, 0, 0x00000000);
		for (int i = controlStart; i < controlEnd; i++)
		{
			//action
			controlPos == i ? mRender->SetFontStyle(small_size, GU_COLOR(1, 1, 0.25f, 1), 0, 0x00000000) : mRender->SetFontStyle(small_size, GU_COLOR(0.25f, 0.25f, 0.25f, 1), 0, 0x00000000);
			mRender->DebugPrint(256, starty + (i * 25) - (controlStart * 25) + 4, InputHelper::Instance()->getActionName(i).c_str());
		}

		mRender->SetFontStyle(normal_size, 0xFFFFFFFF, 0, 0x00000200);
		for (int i = controlStart; i < controlEnd; i++)
		{
			//button assigned to this action
			controlPos == i ? mRender->SetFontStyle(normal_size, GU_COLOR(1, 1, 0.25f, 1), 0, 0x00000200) : mRender->SetFontStyle(normal_size, GU_COLOR(1, 1, 1, 1), 0, 0x00000200);
			if (controlPos == i && chooseKeyState == true)
				mRender->DebugPrint(180, starty + (i * 25) - (controlStart * 25) + 4, "...");
			else
				mRender->DebugPrint(180, starty + (i * 25) - (controlStart * 25) + 4, InputHelper::Instance()->getButtonName(InputHelper::Instance()->getConnection(i).button).c_str());
		}
		mRender->SetDefaultFont();

		DrawText(241, 236, GU_COLOR(0.28f, 0.28f, 0.28f, 1), normal_size, "Cancel");
		DrawText(240, 73, GU_COLOR(0.28, 0.28, 0.28, 1), normal_size, "Controls");
		controlPos == 15 ? DrawText(240, 235, GU_COLOR(1, 1, 0, 1), normal_size, "Cancel") : DrawText(240, 235, GU_COLOR(1, 1, 1, 1), normal_size, "Cancel");
	}
	break;
	case 2:// Analog Sensitivity
	{
		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));

		backgroundSprite->SetPosition(bx, 136);
		backgroundSprite->DrawLinear();

		menu1Sprite->Draw();
		nbuttonSprite->SetPosition(240, 120);
		nbuttonSprite->Draw();

		nbuttonSprite->SetPosition(240, 145);
		nbuttonSprite->Draw();

		nbuttonSprite->SetPosition(240, 170);
		nbuttonSprite->Draw();

		nbuttonSprite->SetPosition(240, 195);
		nbuttonSprite->Draw();

		// Cancel
		buttonLargeSprite->SetPosition(240, 220);
		buttonLargeSprite->Draw();

		if (currentAnalogPos < 4)
		{
			smbuttonSprite->SetPosition(240, 120 + (currentAnalogPos * 25));
			smbuttonSprite->Draw();
		}
		else
		{
			sbuttonLargeSprite->SetPosition(240, 220);
			sbuttonLargeSprite->Draw();
		}

		moverSprite->SetPosition(90 + ((fabs(InputHelper::Instance()->analogYup) * 15) * 20), 120);
		moverSprite->Draw();

		moverSprite->SetPosition(90 + ((fabs(InputHelper::Instance()->analogYdown) * 15) * 20), 145);
		moverSprite->Draw();

		moverSprite->SetPosition(90 + ((fabs(InputHelper::Instance()->analogXleft) * 15) * 20), 170);
		moverSprite->Draw();

		moverSprite->SetPosition(90 + ((fabs(InputHelper::Instance()->analogXright) * 15) * 20), 195);
		moverSprite->Draw();

		sceGuDisable(GU_BLEND);
		sceGuEnable(GU_DEPTH_TEST);

		DrawText(240, 73, GU_COLOR(0.28, 0.28, 0.28, 1), normal_size, "Analog Sensitivity");

		mRender->SetFontStyle(0.35, GU_COLOR(0.28, 0.28, 0.28, 1), 0, 0x00000000);
		mRender->DebugPrint(117, 87, "Lower Percentages will make the");
		mRender->DebugPrint(117, 100, "Analog more sensitive.");

		mRender->SetFontStyle(normal_size, 0xFFFFFFFF, 0, 0x00000200);
		currentAnalogPos == 0 ? mRender->SetFontStyle(normal_size, GU_COLOR(1, 1, 0.25f, 1), 0, 0x00000200) : mRender->SetFontStyle(normal_size, GU_COLOR(1, 1, 1, 1), 0, 0x00000200);
		mRender->DebugPrint(240, 135, "Analog up : %d%%", (int)(fabs(InputHelper::Instance()->analogYup) * 112.0f));

		currentAnalogPos == 1 ? mRender->SetFontStyle(normal_size, GU_COLOR(1, 1, 0.25f, 1), 0, 0x00000200) : mRender->SetFontStyle(normal_size, GU_COLOR(1, 1, 1, 1), 0, 0x00000200);
		mRender->DebugPrint(240, 160, "Analog down : %d%%", (int)(fabs(InputHelper::Instance()->analogYdown) * 112.0f));

		currentAnalogPos == 2 ? mRender->SetFontStyle(normal_size, GU_COLOR(1, 1, 0.25f, 1), 0, 0x00000200) : mRender->SetFontStyle(normal_size, GU_COLOR(1, 1, 1, 1), 0, 0x00000200);
		mRender->DebugPrint(240, 185, "Analog left : %d%%", (int)(fabs(InputHelper::Instance()->analogXleft) * 112.0f));

		currentAnalogPos == 3 ? mRender->SetFontStyle(normal_size, GU_COLOR(1, 1, 0.25f, 1), 0, 0x00000200) : mRender->SetFontStyle(normal_size, GU_COLOR(1, 1, 1, 1), 0, 0x00000200);
		mRender->DebugPrint(240, 210, "Analog right : %d%%", (int)(fabs(InputHelper::Instance()->analogXright) * 112.0f));

		DrawText(241, 236, GU_COLOR(0.28f, 0.28f, 0.28f, 1), normal_size, "Cancel");
		currentAnalogPos >= 4 ? mRender->SetFontStyle(normal_size, GU_COLOR(1, 1, 0.25f, 1), 0, 0x00000200) : mRender->SetFontStyle(normal_size, GU_COLOR(1, 1, 1, 1), 0, 0x00000200);
		mRender->DebugPrint(240, 235, "Cancel");

	}
	break;
	case 3: // Credits
	{
		sceGuDisable(GU_DEPTH_TEST);
		sceGuEnable(GU_BLEND);
		sceGuColor(GU_COLOR(1, 1, 1, 1.0f));

		dark_backgroundSprite->Draw();

		DrawText(136, 115, GU_COLOR(1, 1, 1, 1), normal_size, "Marcin Ploska (Drakon)");
		DrawText(86, 128, GU_COLOR(0.5f, 0.5f, 0.5f, 1), normal_size, "2011-2013");
		DrawText(122, 152, GU_COLOR(1, 1, 1, 1), normal_size, "Kirill Skibin (Woolio)");
		DrawText(86, 167, GU_COLOR(0.5f, 0.5f, 0.5f, 1), normal_size, "2014-2017");
		DrawText(140, 193, GU_COLOR(1, 1, 1, 1), normal_size, "Dillon Venus (InvisiBro)");
		DrawText(86, 206, GU_COLOR(0.5f, 0.5f, 0.5f, 1), normal_size, "2018-2021");

		DrawText(340, 138, GU_COLOR(1, 1, 1, 1), normal_size, "============");
		DrawText(340, 149, GU_COLOR(1, 1, 0, 1), normal_size, "Version");
		DrawText(340, 166, GU_COLOR(1, 1, 0, 1), normal_size, "Beta 0.8");
		DrawText(340, 176, GU_COLOR(1, 1, 1, 1), normal_size, "============");

		//back
		sbuttonSprite->SetPosition(240, 220);
		sbuttonSprite->Draw();
		DrawText(241, 236, GU_COLOR(0.28f, 0.28f, 0.28f, 1), normal_size, "Back");
		DrawText(240, 235, GU_COLOR(1, 1, 0.25, 1), normal_size, "Back");
	}
	break;
	}
	mRender->EndFrame();
}

void StateOptions::DrawText(int x, int y, unsigned int color, float size, const char* message, ...)
{
	mRender->SetFontStyle(size, color, 0, 0x00000200 | 0x00000000);
	mRender->DebugPrint(x, y, message);
}
