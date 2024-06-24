#ifndef MENU_H
#define MENU_H

#include "FX.h"
#include "D3D.h"
#include "Input.h"

#include "Button.h"
#include "Cursor.h"

// Includes for UI
#include "WindowUtils.h"
#include "CommonStates.h"

#include <string>
#include <vector>

// Keyboard
#define LCLICK MouseAndKeys::LBUTTON

// Gamepad
#define ABUTTON XINPUT_GAMEPAD_A

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class Menu
{
public:
	Menu(string menuTitle, float titleScale, string backgroundFilePath, float backgroundScale);
	virtual ~Menu();

	virtual void Update(float dTime);
	virtual void Render(float dTime);

	virtual void RenderBackground(int w, int h) = 0;
	virtual void RenderTitle(int w, int h) = 0;
	virtual void RenderButtons(int w, int h) = 0;

	void RenderBox(Vector2 startPos, int segments, float scale);

	bool isButtonPressed(int buttonNo);

	void UpdateCursor(float dTime);

	void ToggleMouse(bool state);

protected:
	// Title
	wstringstream title;
	float ttlScale = 0;
	Vector2 titleCenter;

	// Background
	int backgroundTexIdx = 0;
	float bGScale = 0;
	Vector2 backgroundCenter;

	// Buttons
	int buttonTexIdx = 0;
	vector<Button> buttons;
	vector<bool> pressedButtons;

	// Box
	vector<int> boxTexIdxs;
	Vector2 boxPieceCenter;
	float boxPieceWidth = 0;

	// Cursor
	Cursor cursor;
	bool mouseActive = true; // Mouse currently on screen

	float interactDelay = 0.2f;

private:
	virtual void InitialiseButtons() = 0;

	void InitialiseTitle(string titleText, float titleScale);
	void InitialiseBackground(string backgroundFilePath, float backgroundScale);
	void InitialiseButtonTextures();
	void InitialiseBoxTextures();

};

#endif