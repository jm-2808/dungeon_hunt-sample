#include "Menu.h"

Menu::Menu(string menuTitle, float titleScale, string backgroundFilePath, float backgroundScale)
	: ttlScale(titleScale), bGScale(backgroundScale)
{ 
	InitialiseTitle(menuTitle, titleScale);
	InitialiseBackground(backgroundFilePath, backgroundScale);
	InitialiseButtonTextures();
	InitialiseBoxTextures();
}

Menu::~Menu()
{ }

void Menu::InitialiseTitle(string titleText, float titleScale)
{
	title << L"" << titleText.c_str();

	titleCenter = (FX::GetMyFX()->mpFont->MeasureString(title.str().c_str()) / 2) * titleScale;
}
void Menu::InitialiseBackground(string backgroundFilePath, float backgroundScale)
{
	if (!(backgroundFilePath == ""))
		FX::GetMyFX()->mCache.LoadTexture(backgroundFilePath, true, gd3dDevice);
	backgroundTexIdx = FX::GetMyFX()->mCache.GetIdx(backgroundFilePath);

	backgroundCenter = Vector2(FX::GetMyFX()->mCache.Get(backgroundTexIdx).dim.x / 2, FX::GetMyFX()->mCache.Get(backgroundTexIdx).dim.y / 2) * backgroundScale;
}
void Menu::InitialiseButtonTextures()
{
	FX::GetMyFX()->mCache.LoadTexture("ui/button.dds", true, gd3dDevice);
	buttonTexIdx = FX::GetMyFX()->mCache.GetIdx("ui/button.dds");
}
void Menu::InitialiseBoxTextures()
{
	FX::GetMyFX()->mCache.LoadTexture("ui/box/boxCenter.dds", true, gd3dDevice);	// [0] Box Center
	boxTexIdxs.push_back(FX::GetMyFX()->mCache.GetIdx("ui/box/boxCenter.dds"));

	FX::GetMyFX()->mCache.LoadTexture("ui/box/boxEnd002.dds", true, gd3dDevice);	// [1] Box Top/Left
	boxTexIdxs.push_back(FX::GetMyFX()->mCache.GetIdx("ui/box/boxEnd002.dds"));

	FX::GetMyFX()->mCache.LoadTexture("ui/box/boxEnd001.dds", true, gd3dDevice);	// [2] Box Top/Right
	boxTexIdxs.push_back(FX::GetMyFX()->mCache.GetIdx("ui/box/boxEnd001.dds"));

	FX::GetMyFX()->mCache.LoadTexture("ui/box/boxEnd003.dds", true, gd3dDevice);	// [3] Box Bottom/Left
	boxTexIdxs.push_back(FX::GetMyFX()->mCache.GetIdx("ui/box/boxEnd003.dds"));

	FX::GetMyFX()->mCache.LoadTexture("ui/box/boxEnd004.dds", true, gd3dDevice);	// [4] Box Bottom/Right
	boxTexIdxs.push_back(FX::GetMyFX()->mCache.GetIdx("ui/box/boxEnd004.dds"));

	boxPieceCenter = Vector2(FX::GetMyFX()->mCache.Get(boxTexIdxs[0]).dim.x / 2, FX::GetMyFX()->mCache.Get(boxTexIdxs[0]).dim.y / 2);

	boxPieceWidth = FX::GetMyFX()->mCache.Get(boxTexIdxs[0]).dim.x;
}

void Menu::Update(float dTime)
{
	interactDelay -= dTime;
	if (interactDelay < 0)
		interactDelay = 0;

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

void Menu::Render(float dTime)
{
	FX::MyFX& fx = *FX::GetMyFX();
	CommonStates state(gd3dDevice);
	fx.mpSpriteB->Begin(SpriteSortMode_Deferred, state.NonPremultiplied());

	// Window info
	int w, h;
	GetClientExtents(w, h);

	RenderBackground(w, h);
	RenderTitle(w, h);
	RenderButtons(w, h);

	fx.mpSpriteB->End();

	if (GetGamepad()->IsConnected())
		UpdateCursor(dTime);
}

void Menu::RenderBox(Vector2 startPos, int segments, float scale)
{
	for (int i = 0; i < segments; i++)
	{
		Vector2 pos;
		pos.x = (startPos.x - ((boxPieceWidth * scale) * (segments / 2))) + (i * (boxPieceWidth * scale));
		pos.y = startPos.y;
		if (i == 0)
			FX::GetMyFX()->mpSpriteB->Draw(FX::GetMyFX()->mCache.Get(boxTexIdxs[2]).pTex, pos - (boxPieceCenter * scale), nullptr, Colours::White, 0, Vector2(0, 0), scale);
		else if (i == segments - 1)
			FX::GetMyFX()->mpSpriteB->Draw(FX::GetMyFX()->mCache.Get(boxTexIdxs[1]).pTex, pos - (boxPieceCenter * scale), nullptr, Colours::White, 0, Vector2(0, 0), scale);
		else
			FX::GetMyFX()->mpSpriteB->Draw(FX::GetMyFX()->mCache.Get(boxTexIdxs[0]).pTex, pos - (boxPieceCenter * scale), nullptr, Colours::White, 0, Vector2(0, 0), scale);
	}
}

bool Menu::isButtonPressed(int buttonNo)
{
	if (interactDelay == 0)
	{
		if(GetGamepad()->IsConnected())
			return (buttons[buttonNo].isHovered(cursor.GetPosition()) && GetGamepad()->IsPressed(ABUTTON));
		else
			return (buttons[buttonNo].isHovered() && GetMouseAndKeys()->GetMouseButton(LCLICK));
	}
	else
		return false;
}

void Menu::UpdateCursor(float dTime)
{
	//Vector2 p = ((Vector2(GetGamepad()->GetState().leftStickX, GetGamepad()->GetState().leftStickY * -1) * GetGamepad()->sensitivity) * dTime) * 500;
	Vector2 p(0, 0);

	if (GetGamepad()->GetState().leftStickX > 0)
		p.x = GetGamepad()->sensitivity * dTime;
	else if (GetGamepad()->GetState().leftStickX < 0)
		p.x = -(GetGamepad()->sensitivity * dTime);

	if (GetGamepad()->GetState().leftStickY < 0)
		p.y = GetGamepad()->sensitivity * dTime;
	else if (GetGamepad()->GetState().leftStickY > 0)
		p.y = -(GetGamepad()->sensitivity * dTime);

	p *= 400;

	cursor.UpdatePosition(p.x, p.y);
	cursor.Render(dTime);
}

void Menu::ToggleMouse(bool state)
{
	if (state)
	{
		while (ShowCursor(true) <= 0) {}
		mouseActive = true;
	}
	else
	{
		while (ShowCursor(false) >= 0) {}
		mouseActive = false;
	}
}