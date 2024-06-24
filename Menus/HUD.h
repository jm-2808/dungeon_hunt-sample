#ifndef HUD_H
#define HUD_H

#include "FX.h"
#include "D3D.h"

#include <vector>

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class HUD
{
public:
	HUD();
	~HUD();

	void Render(float dTime, int playerHealth, int playerScore);

private:
	// Box
	vector<int> boxTexIdxs;
	Vector2 boxPieceCenter;
	float boxPieceWidth = 0;

	// Health bar
	const float healthbarScale = 0.04f;
	vector<int> hpBarTexIdxs;
	Vector2 incCenter;
	Vector2 endCenter;
	float incWidth = 0;
	float endWidth = 0;

	// Score
	const float scoreScale = 1.2f;

	void LoadBoxTextures();
	void LoadHPBarTextures();

	void RenderHpBar(int screenWidth, int screenHeight, int playerHealth);
	void RenderScore(int screenWidth, int screenHeight, int playerScore);

};

#endif