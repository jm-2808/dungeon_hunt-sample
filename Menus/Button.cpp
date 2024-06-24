#include "Button.h"

#include "Input.h"

Button::Button(string buttonText, int textureIndex, float imageScale, float textScale)
	: text(buttonText), textureIdx(textureIndex), btnScale(imageScale), txtScale(textScale)
{ 
	wstringstream ss;
	ss << buttonText.c_str();
	
	imageCenter = Vector2(FX::GetMyFX()->mCache.Get(textureIdx).dim.x / 2, FX::GetMyFX()->mCache.Get(textureIdx).dim.y / 2);
	textCenter = (FX::GetMyFX()->mpFont->MeasureString(ss.str().c_str()) / 2) * textScale;
	
	interactBox = Vector2(FX::GetMyFX()->mCache.Get(textureIdx).dim.x * imageScale, FX::GetMyFX()->mCache.Get(textureIdx).dim.y * imageScale);
}
Button::~Button()
{ }

void Button::Render(Vector2 position)
{
	if(pos != position)
		pos = position;

	//Button image
	FX::GetMyFX()->mpSpriteB->Draw(FX::GetMyFX()->mCache.Get(textureIdx).pTex, (pos - (imageCenter * btnScale)), nullptr, Colours::White, 0, Vector2(0, 0), btnScale);

	// Text
	wstringstream ss;
	ss << text.c_str();
	FX::GetMyFX()->mpFont->DrawString(FX::GetMyFX()->mpSpriteB, ss.str().c_str(), (pos - textCenter), Colors::White, 0, Vector2(0, 0), txtScale);
}

bool Button::isHovered()
{
	Vector2 mousePos = GetMouseAndKeys()->GetMousePos(true);

	if ((mousePos.x > (pos.x - interactBox.x / 2)) && (mousePos.x < (pos.x + interactBox.x / 2)))
	{
		if ((mousePos.y > (pos.y - interactBox.y / 2)) && (mousePos.y < (pos.y + interactBox.y / 2)))
			return true;
		else
			return false;
	}
	else
		return false;
}
bool Button::isHovered(Vector2 cursorPos)
{
	if ((cursorPos.x >(pos.x - interactBox.x / 2)) && (cursorPos.x < (pos.x + interactBox.x / 2)))
	{
		if ((cursorPos.y >(pos.y - interactBox.y / 2)) && (cursorPos.y < (pos.y + interactBox.y / 2)))
			return true;
		else
			return false;
	}
	else
		return false;
}
