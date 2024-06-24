#include "HUD.h"

// Includes for UI
#include "WindowUtils.h"
#include "CommonStates.h"

HUD::HUD()
{
	LoadBoxTextures();
	LoadHPBarTextures();
}
HUD::~HUD()
{

}

void HUD::Render(float dTime, int playerHealth, int playerScore)
{
	FX::MyFX& fx = *FX::GetMyFX();
	CommonStates state(gd3dDevice);
	fx.mpSpriteB->Begin(SpriteSortMode_Deferred, state.NonPremultiplied());

	// Window info
	int w, h;
	GetClientExtents(w, h);

	RenderHpBar(w, h, playerHealth);

	RenderScore(w, h, playerScore);

	fx.mpSpriteB->End();
}

void HUD::LoadBoxTextures()
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

void HUD::LoadHPBarTextures()
{
	FX::GetMyFX()->mCache.LoadTexture("ui/healthbar/hpBar000-Red.dds", true, gd3dDevice);	// [0] HP Red 000
	hpBarTexIdxs.push_back(FX::GetMyFX()->mCache.GetIdx("ui/healthbar/hpBar000-Red.dds"));

	FX::GetMyFX()->mCache.LoadTexture("ui/healthbar/hpBar-Red.dds", true, gd3dDevice);		// [1] HP Red
	hpBarTexIdxs.push_back(FX::GetMyFX()->mCache.GetIdx("ui/healthbar/hpBar-Red.dds"));

	FX::GetMyFX()->mCache.LoadTexture("ui/healthbar/hpBar100-Red.dds", true, gd3dDevice);	// [2] HP Red 100
	hpBarTexIdxs.push_back(FX::GetMyFX()->mCache.GetIdx("ui/healthbar/hpBar100-Red.dds"));

	FX::GetMyFX()->mCache.LoadTexture("ui/healthbar/hpBar000-Grey.dds", true, gd3dDevice);	// [3] HP Grey 000
	hpBarTexIdxs.push_back(FX::GetMyFX()->mCache.GetIdx("ui/healthbar/hpBar000-Grey.dds"));

	FX::GetMyFX()->mCache.LoadTexture("ui/healthbar/hpBar-Grey.dds", true, gd3dDevice);		// [4] HP Grey
	hpBarTexIdxs.push_back(FX::GetMyFX()->mCache.GetIdx("ui/healthbar/hpBar-Grey.dds"));

	FX::GetMyFX()->mCache.LoadTexture("ui/healthbar/hpBar100-Grey.dds", true, gd3dDevice);	// [5] HP Grey 100
	hpBarTexIdxs.push_back(FX::GetMyFX()->mCache.GetIdx("ui/healthbar/hpBar100-Grey.dds"));

	// Doesn't matter which image we use, both files for each part are the same dimensions
	endCenter = Vector2(FX::GetMyFX()->mCache.Get(hpBarTexIdxs[0]).dim.x / 2, FX::GetMyFX()->mCache.Get(hpBarTexIdxs[0]).dim.y / 2);
	incCenter = Vector2(FX::GetMyFX()->mCache.Get(hpBarTexIdxs[1]).dim.x / 2, FX::GetMyFX()->mCache.Get(hpBarTexIdxs[1]).dim.y / 2);

	endWidth = FX::GetMyFX()->mCache.Get(hpBarTexIdxs[0]).dim.x;
	incWidth = FX::GetMyFX()->mCache.Get(hpBarTexIdxs[1]).dim.x;
}

void HUD::RenderHpBar(int screenWidth, int screenHeight, int playerHealth)
{
	// Box
	float boxScale = 0.05f;
	int segments = 13;
	for (int i = 0; i < segments; i++)
	{
		Vector2 pos;
		pos.x = 20 + (i * (boxPieceWidth * boxScale));
		pos.y = screenHeight - 30;
		if(i == segments - 1)
			FX::GetMyFX()->mpSpriteB->Draw(FX::GetMyFX()->mCache.Get(boxTexIdxs[3]).pTex, pos - (boxPieceCenter * boxScale), nullptr, Colours::White, 0, Vector2(0, 0), boxScale);
		else
			FX::GetMyFX()->mpSpriteB->Draw(FX::GetMyFX()->mCache.Get(boxTexIdxs[0]).pTex, pos - (boxPieceCenter * boxScale), nullptr, Colours::White, 0, Vector2(0, 0), boxScale);
	}
	
	// Bar
	for (int i = 0; i < 100; i++)
	{
		int alive = 0;
		int dead = 0;
		Vector2 offset;

		if (i == 0)
		{
			alive = 0;
			dead = 3;
			offset = endCenter;
		}
		else if (i == 99)
		{
			alive = 2;
			dead = 5;
			offset = endCenter;
		}
		else
		{
			alive = 1;
			dead = 4;
			offset = incCenter;
		}

		Vector2 pos;
		pos.x = 20 + (i * (incWidth * healthbarScale));
		pos.y = screenHeight - 30;
		if (i > 0)
			pos.x += (((endWidth / 2) - incWidth) * healthbarScale);
		if (i == 99)
			pos.x += (((endWidth / 2) - incWidth) * healthbarScale);

		if (i < playerHealth)
			FX::GetMyFX()->mpSpriteB->Draw(FX::GetMyFX()->mCache.Get(hpBarTexIdxs[alive]).pTex, pos - (offset * healthbarScale), nullptr, Colours::White, 0, Vector2(0, 0), healthbarScale);
		else
			FX::GetMyFX()->mpSpriteB->Draw(FX::GetMyFX()->mCache.Get(hpBarTexIdxs[dead]).pTex, pos - (offset * healthbarScale), nullptr, Colours::White, 0, Vector2(0, 0), healthbarScale);
	}
}

void HUD::RenderScore(int screenWidth, int screenHeight, int playerScore)
{
	// Box
	float boxScale = 0.1f;
	int segments = 4;
	for (int i = 0; i < segments; i++)
	{
		Vector2 pos;
		pos.x = screenWidth - (i * (boxPieceWidth * boxScale));
		pos.y = 60;
		if (i == segments - 1)
			FX::GetMyFX()->mpSpriteB->Draw(FX::GetMyFX()->mCache.Get(boxTexIdxs[2]).pTex, pos - (boxPieceCenter * boxScale), nullptr, Colours::White, 0, Vector2(0, 0), boxScale);
		else
			FX::GetMyFX()->mpSpriteB->Draw(FX::GetMyFX()->mCache.Get(boxTexIdxs[0]).pTex, pos - (boxPieceCenter * boxScale), nullptr, Colours::White, 0, Vector2(0, 0), boxScale);
	}

	// Title
	std::wstringstream ss;
	ss << "SCORE: " << playerScore;


	Vector2 textCenter = (FX::GetMyFX()->mpFont->MeasureString(ss.str().c_str()) / 2) * scoreScale;
	Vector2 pos;
	pos.x = screenWidth - 150;
	pos.y = 60;
	FX::GetMyFX()->mpFont->DrawString(FX::GetMyFX()->mpSpriteB, ss.str().c_str(), pos - textCenter, Colors::White, 0, Vector2(0, 0), scoreScale);
}