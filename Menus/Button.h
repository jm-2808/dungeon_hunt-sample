#ifndef BUTTON_H
#define BUTTON_H

#include "FX.h"
#include "D3D.h"

#include <string>

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class Button
{
public:
	Button(string buttonText, int textureIndex, float imageScale, float textScale);
	~Button();

	void Render(Vector2 position);

	bool isHovered();
	bool isHovered(Vector2 cursorPos);

	Vector2 GetDimensions() { return interactBox; }

private:
	string text;

	int textureIdx = 0;

	float txtScale = 0;
	float btnScale = 0;

	Vector2 imageCenter;
	Vector2 textCenter;

	Vector2 interactBox;
	Vector2 pos;

};

#endif