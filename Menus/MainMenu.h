#ifndef MENU_MAIN_H
#define MENU_MAIN_H

#include "Menu.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class MainMenu : public Menu
{
public:
	MainMenu();
	virtual ~MainMenu();

	virtual void RenderBackground(int w, int h);
	virtual void RenderTitle(int w, int h);
	virtual void RenderButtons(int w, int h);

private:
	virtual void InitialiseButtons();

};

#endif