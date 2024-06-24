#include "PauseMenu.h"

PauseMenu::PauseMenu()
	: Menu("PAUSED", 2, "ui/b60.dds", 3)
{ 
	InitialiseButtons();

	playing = true;
}

PauseMenu::~PauseMenu()
{ }

void PauseMenu::Update(float dTime)
{
	interactDelay -= dTime;
	if (interactDelay < 0)
		interactDelay = 0;

	if (!playing)
	{
		if (GetGamepad()->IsConnected())
		{
			if (mouseActive)
				ToggleMouse(false);
		}
		else
		{
			if (!mouseActive)
				ToggleMouse(true);
		}
	}
	else
	{
		if (mouseActive)
			ToggleMouse(false);
	}
}

void PauseMenu::RenderBackground(int w, int h)
{
	Vector2 windowCenter(w / 2, h / 2);
	FX::GetMyFX()->mpSpriteB->Draw(FX::GetMyFX()->mCache.Get(backgroundTexIdx).pTex, (windowCenter - backgroundCenter), nullptr, Colours::White, 0, Vector2(0, 0), Vector2(bGScale));
}
void PauseMenu::RenderTitle(int w, int h)
{
	// Box
	Vector2 boxPos;
	boxPos.x = (w / 2);
	boxPos.y = (h / 2 - titleCenter.y) - 82;
	RenderBox(boxPos, 7, 0.1);

	// Title
	Vector2 textPos = Vector2((w / 2 - titleCenter.x), (h / 2 - titleCenter.y) - 120);
	FX::GetMyFX()->mpFont->DrawString(FX::GetMyFX()->mpSpriteB, title.str().c_str(), textPos, Colors::White, 0, Vector2(0, 0), ttlScale);
}
void PauseMenu::RenderButtons(int w, int h)
{
	for (int i = 0; i < buttons.size(); i++)
	{
		Vector2 pos;
		pos.x = w / 2;
		pos.y = (h * 0.7) + ((buttons[i].GetDimensions().y + 15) * i);
		buttons[i].Render(pos);
	}
}

void PauseMenu::InitialiseButtons()
{
	// Continue button
	Button btnContinue("Continue", buttonTexIdx, 0.1f, 1.f);
	buttons.push_back(btnContinue); // buttons[0] = Continue button
	
	// Exit button
	Button btnExit("Exit", buttonTexIdx, 0.1f, 1.f);
	buttons.push_back(btnExit); // buttons[1] = Exit to menu
}