#include "MainMenu.h"

MainMenu::MainMenu()
	: Menu("Dungeon Hunt", 2, "ui/background.dds", 1.f)
{
	InitialiseButtons();
}

MainMenu::~MainMenu()
{ }

void MainMenu::RenderBackground(int w, int h)
{
	Vector2 windowCenter(w / 2, h / 2);
	FX::GetMyFX()->mpSpriteB->Draw(FX::GetMyFX()->mCache.Get(backgroundTexIdx).pTex, (windowCenter - backgroundCenter), nullptr, Colours::White, 0, Vector2(0, 0), Vector2(bGScale));

	wstringstream t;
	t << "CS4G Year 2 [2017 - 2018]" << "\nMade by:";
	wstringstream c;
	c << "Callum Coaster" << "\nJack Magnay" << "\nTom Dunn" << "\nLiam Warner";
	Vector2 tScale(0.7);
	Vector2 cScale(0.5);
	Vector2 tPos(10 * tScale.x, h - (195 * tScale.y));
	Vector2 cPos(20 * cScale.x, h - (170 * cScale.y));
	FX::GetMyFX()->mpFont->DrawString(FX::GetMyFX()->mpSpriteB, t.str().c_str(), tPos, Colors::White, 0, Vector2(0, 0), tScale);

	FX::GetMyFX()->mpFont->DrawString(FX::GetMyFX()->mpSpriteB, c.str().c_str(), cPos, Colors::White, 0, Vector2(0, 0), cScale);
}
void MainMenu::RenderTitle(int w, int h)
{
	// Box
	Vector2 boxPos;
	boxPos.x = (w / 2);
	boxPos.y = 50 + titleCenter.y;
	RenderBox(boxPos, 7, 0.1);

	// Title
	Vector2 textPos = Vector2((w / 2 - titleCenter.x), 50);
	FX::GetMyFX()->mpFont->DrawString(FX::GetMyFX()->mpSpriteB, title.str().c_str(), textPos, Colors::White, 0, Vector2(0, 0), ttlScale);
}
void MainMenu::RenderButtons(int w, int h)
{
	for (int i = 0; i < buttons.size(); i++)
	{
		Vector2 pos;
		pos.x = w / 2;
		pos.y = 300 + (buttons[i].GetDimensions().y + 10) * i;
		buttons[i].Render(pos);
	}
}

void MainMenu::InitialiseButtons()
{
	// Continue button
	Button btnPlay("Play", buttonTexIdx, 0.1f, 1.f);
	buttons.push_back(btnPlay); // buttons[0] = Play button

	// Exit button
	Button btnLeaderboard("Leaderboard", buttonTexIdx, 0.1f, 1.f);
	buttons.push_back(btnLeaderboard); // buttons[1] = Go to leaderboard
	
	Button btnControls("Controls", buttonTexIdx, 0.1f, 1.f);
	buttons.push_back(btnControls); // buttons[3] = Exit
									   // Exit button
	Button btnExit("Exit", buttonTexIdx, 0.1f, 1.f);
	buttons.push_back(btnExit); // buttons[2] = Exit



}