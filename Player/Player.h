#ifndef PLAYER_H
#define PLAYER_H

#include "FX.h"
#include "D3D.h"
#include "Input.h"

#include "Character.h"
#include "FPSCamera.h"
#include "Inventory.h"
#include "HUD.h"

// === Mouse and Keys ===
// Movement Controls
#define MK_FORWARDS		VK_W
#define MK_BACKWARDS	VK_S
#define MK_LEFT			VK_A
#define MK_RIGHT		VK_D
#define MK_UPWARDS		VK_SPACE
#define MK_DOWNWARDS	VK_X

// Attack Controls
#define MK_FIRE			MouseAndKeys::LBUTTON
#define MK_NEXTWEAPON		VK_Q
#define MK_PREVIOUSWEAPON	VK_E

// Utility controls
#define MK_PAUSE		VK_ESCAPE

// === Gamepad ===
// Movement Controls
//#define GP_FORWARDS	LEFTSTICK_Y > 0
//#define GP_BACKWARDS	LEFTSTICK_Y < 0
//#define GP_LEFT		LEFTSTICK_X < 0
//#define GP_RIGHT		LEFTSTICK_X > 0
#define GP_UPWARDS		XINPUT_GAMEPAD_A 
#define GP_DOWNWARDS	XINPUT_GAMEPAD_LEFT_THUMB

// Attack Controls
// #define GP_FIRE			RIGHT TRIGGER	
#define GP_NEXTWEAPON		XINPUT_GAMEPAD_LEFT_SHOULDER
#define GP_PREVIOUSWEAPON	XINPUT_GAMEPAD_RIGHT_SHOULDER

// Utility controls
#define GP_PAUSE			XINPUT_GAMEPAD_START	

// Debug
#define DEBUG						false
#define DEBUG_TOGGLE				VK_HOME

#define DEBUG_CAMROTATESPEEDUP		VK_ADD
#define DEBUG_CAMROTATESPEEDDOWN	VK_SUBTRACT
#define DEBUG_CAMROTATEINCREMENT	0.1

#define CAMROTATESPEED				0.5

class Player : public Character 
{
public:
	Player();

	virtual void Update(float dTime);
	virtual void Render(float dTime);
	virtual void OnEnable();
	virtual DirectX::BoundingBox GetBoundingBox();

	bool isPaused() { return paused; }
	void TogglePause();

	void AddWeapon(string weaponName);

private:
	FPSCamera mCamera;
	Inventory inv;
	HUD hud;

	// Pause variables
	bool paused = true;
	float pauseDelay = 0;
	float inputDelay = 0;

	// Update functions
	void MovePlayer(float dTime);
	void UpdateWeapons(float dTime);
	void UpdateCamera(float dTime);
	void UpdatePause(float dTime);
	void UpdateDebug(float dTime);

	// Movement
	const float moveSpeed = 5.f;
	DirectX::SimpleMath::Vector3 acceleration = DirectX::SimpleMath::Vector3(0, 0, 0);
	const float accelerationDecrease = 0.01f;
	DirectX::SimpleMath::Vector3 gravity = DirectX::SimpleMath::Vector3(0, -9.31f, 0);
	const float gravityMultiplier = 0.35f;
	const float jumpPower = 5.f;
	bool inAir;

	void ApplyGravity(float dTime, bool debug);

	virtual bool IsOnLiquid();	

	// Gamepad support
	bool controllerConnected = false;
	
	// Debug stuff
	bool debugActive = false;
	float debugActivateDelay = 0;
	void ToggleDebug();

protected:
	virtual void Die();

};

#endif