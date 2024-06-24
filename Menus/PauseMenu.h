#ifndef MENU_PAUSE_H
#define MENU_PAUSE_H

#include "Menu.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class PauseMenu : public Menu
{
public:
	PauseMenu();
	virtual ~PauseMenu();

	virtual void Update(float dTime);

	virtual void RenderBackground(int w, int h);
	virtual void RenderTitle(int w, int h);
	virtual void RenderButtons(int w, int h);

	void SetPlaying(bool state) { playing = state; }
	bool isPlaying()			{ return playing; }

private:
	virtual void InitialiseButtons();

	bool playing;
};

#endif