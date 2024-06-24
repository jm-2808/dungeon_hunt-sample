#include "Player.h"

#include "Level.h"
#include "Block.h"
#include "SpecialTileBlock.h"
#include "LiquidBlock.h"
#include "GameModeManager.h"
#include "Leaderboard.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player()
	: Character(100)
{
	mCamera.Initialise(&position, Vector3(0, 0, 1), FX::GetViewMatrix());
	mCamera.LockMovementAxis(FPSCamera::UNLOCK, FPSCamera::UNLOCK, FPSCamera::UNLOCK);
	mCamera.SetCamRotateSpeed(CAMROTATESPEED);

	controllerConnected = GetGamepad()->IsConnected();

	inAir = false;

	TogglePause();
}

void Player::TogglePause()
{
	if (pauseDelay == 0)
	{
		paused = !paused;
		
		GameModeManager::GetInstance();

		pauseDelay = 0.2;
		inputDelay = 0.2;

		if(!paused)
			GetMouseAndKeys()->GetMouseMoveAndCentre();
	}
}

void Player::ToggleDebug()
{
	if (debugActivateDelay <= 0)
	{
		debugActive = !debugActive;
		debugActivateDelay = 0.5;
	}
}

void Player::Update(float dTime)
{
	controllerConnected = GetGamepad()->IsConnected();

	if (!paused)
	{
		MovePlayer(dTime);

		if(inputDelay == 0)
			UpdateWeapons(dTime);
	}

	UpdateCamera(dTime);

	UpdatePause(dTime);

	UpdateDebug(dTime);

	// Input delay to prevent the weapons from responding to residual mouse button inputs from menus
	if (inputDelay > 0)
		inputDelay -= dTime;
	else if (inputDelay < 0)
		inputDelay = 0;
}
void Player::UpdatePause(float dTime)
{
	if (controllerConnected)
	{
		if (GetGamepad()->IsPressed(GP_PAUSE))
			TogglePause();
	}
	else
	{
		if (GetMouseAndKeys()->IsPressed(MK_PAUSE))
			TogglePause();
	}

	// Delays
	if (pauseDelay > 0)
		pauseDelay -= dTime;
	else if (pauseDelay < 0)
		pauseDelay = 0;
}
void Player::UpdateDebug(float dTime)
{
	if (DEBUG)
	{
		if (GetMouseAndKeys()->IsPressed(DEBUG_TOGGLE))
		{
			ToggleDebug();
		}

		if (debugActive)
		{
			if (GetMouseAndKeys()->IsPressed(DEBUG_CAMROTATESPEEDUP))
			{
				if (mCamera.GetCamRotateSpeed() < 10.0f)
					mCamera.SetCamRotateSpeed(mCamera.GetCamRotateSpeed() + DEBUG_CAMROTATEINCREMENT);
			}
			else if (GetMouseAndKeys()->IsPressed(DEBUG_CAMROTATESPEEDDOWN))
			{
				if (mCamera.GetCamRotateSpeed() > 0.5f)
					mCamera.SetCamRotateSpeed(mCamera.GetCamRotateSpeed() - DEBUG_CAMROTATEINCREMENT);
			}
		}
	}

	// Delay
	if (debugActivateDelay != 0)
		debugActivateDelay -= dTime;
	else if (debugActivateDelay < 0)
		debugActivateDelay = 0;
}
void Player::MovePlayer(float dTime)
{
	// Forwards
	bool forward_btn; 
	if (controllerConnected)
		forward_btn = (GetGamepad()->GetState().leftStickY > 0);
	else
		forward_btn = GetMouseAndKeys()->IsPressed(MK_FORWARDS);

	// Backwards
	bool back_btn;
	if (controllerConnected)
		back_btn = (GetGamepad()->GetState().leftStickY < 0);
	else
		back_btn = GetMouseAndKeys()->IsPressed(MK_BACKWARDS);

	// Left
	bool left_btn;
	if (controllerConnected)
		left_btn = (GetGamepad()->GetState().leftStickX < 0);
	else
		left_btn = GetMouseAndKeys()->IsPressed(MK_LEFT);

	// Right
	bool right_btn;
	if (controllerConnected)
		right_btn = (GetGamepad()->GetState().leftStickX > 0);
	else
		right_btn = GetMouseAndKeys()->IsPressed(MK_RIGHT);

	// Jump
	bool up_btn;
	if (controllerConnected)
		up_btn = false; // GetGamepad()->IsPressed(GP_UPWARDS);
	else
		up_btn = false; // GetMouseAndKeys()->IsPressed(MK_UPWARDS);

	// Crouch
	bool down_btn;
	if (controllerConnected)
		down_btn = false; // GetGamepad()->IsPressed(GP_DOWNWARDS);
	else
		down_btn = false; // GetMouseAndKeys()->IsPressed(MK_DOWNWARDS);

	// Move player
	// Get a matrix to represent the camera rotation
	Matrix ori = Matrix::CreateFromYawPitchRoll(mCamera.GetYaw(), mCamera.GetPitch(), mCamera.GetRoll());

	// Movement directions (dir = f/b, strafe = l/r, up = u/d)
	Vector3 dir(0, 0, 1), up(0, 1, 0);
	dir = Vector3::TransformNormal(dir, ori);		// Transform dir so it faces camera direction
	up = Vector3::TransformNormal(up, ori);
	Vector3 strafe = dir.Cross(up);

	Vector3 moveDir = Vector3(dir.x, 0, dir.z);		// Remove y from dir so the player only move in the x and z relative to the camera
	strafe.y = 0;									// Same as above but for strafe

	// Return if no buttons pressed
	if (!forward_btn && !back_btn && !left_btn && !right_btn && !up_btn && !down_btn)
	{
		// Gravity
		position += acceleration * dTime;
		if (acceleration.y <= 0 && inAir)
		{
			acceleration = Vector3(0, 0, 0);
			ApplyGravity(dTime, debugActive);
		}
		else if (acceleration.y != 0)
			acceleration.y -= accelerationDecrease;
	}
	// Otherwise, move the player
	else
	{
		Vector3 playerPos(position);

		// Horizontal Movement
		if (forward_btn)
			playerPos += moveDir * dTime * moveSpeed;
		else if (back_btn)
			playerPos -= moveDir * dTime * moveSpeed;

		if (left_btn)
			playerPos += strafe * dTime * (moveSpeed / 2);
		else if (right_btn)
			playerPos -= strafe * dTime * (moveSpeed / 2);

		// Vertical movement
		if (up_btn && !inAir)
		{
			acceleration.y += jumpPower;
			inAir = true;
		}
		else if (down_btn)
			playerPos -= up * dTime * moveSpeed;

		playerPos += acceleration * dTime;

		// Gravity
		position = playerPos;
		if (acceleration.y <= 0 && inAir)
		{
			acceleration = Vector3(0, 0, 0);
			ApplyGravity(dTime, debugActive);
		}
		else if (acceleration.y != 0)
			acceleration.y -= accelerationDecrease;
	}

	// Vertical position in liquids
	if (IsOnLiquid()) {
		float dis = 0.1f - position.y;
		position.y += dis * dTime * 10;
	}
	else {
		float dis = 0.5f - position.y;
		position.y += dis * dTime * 10;
	}

	// Update camera
	mCamera.UpdateViewMatrix(dTime);
}
void Player::UpdateWeapons(float dTime)
{
	// Fire
	bool fire_btn;
	if (controllerConnected)
		fire_btn = (GetGamepad()->GetState().rightTrigger > 0);
	else
		fire_btn = GetMouseAndKeys()->GetMouseButton(MK_FIRE);

	// Next weapon
	bool nextWeapon_btn;
	if (controllerConnected)
		nextWeapon_btn = (GetGamepad()->IsPressed(GP_NEXTWEAPON));
	else
		nextWeapon_btn = GetMouseAndKeys()->IsPressed(MK_NEXTWEAPON);

	// Previous weapon
	bool previousWeapon_btn;
	if (controllerConnected)
		previousWeapon_btn = (GetGamepad()->IsPressed(GP_PREVIOUSWEAPON));
	else
		previousWeapon_btn = GetMouseAndKeys()->IsPressed(MK_PREVIOUSWEAPON);

	// Weapon Stuff
	if(fire_btn)
	{
		inv.GetCurrentWeapon()->Attack(position, Vector3(0, mCamera.GetYaw(), 0), Matrix::CreateFromYawPitchRoll(mCamera.GetYaw(), mCamera.GetPitch(), mCamera.GetRoll()), level);
	}
	
	if(nextWeapon_btn)
	{
		inv.NextWeapon();
	}
	else if(previousWeapon_btn)
	{
		inv.PreviousWeapon();
	}

	inv.Update(dTime);
	inv.GetCurrentWeapon()->Update(dTime);
}
void Player::UpdateCamera(float dTime)
{
	// Update camera
	FX::GetMyFX()->cameraPos = mCamera.GetPos();
	if (!paused)
	{
		Vector2 m;
		if (controllerConnected)
			m = Vector2(GetGamepad()->GetState().rightStickX, GetGamepad()->GetState().rightStickY * -1) * GetGamepad()->sensitivity;
		else
			m = GetMouseAndKeys()->GetMouseMoveAndCentre();

		if (m.x != 0 || m.y != 0)
		{
			if (m.y < 100)
				mCamera.Rotate(dTime, m.x, m.y, 0, controllerConnected);
		}
	}

	FX::SetPerFrameConsts(gd3dImmediateContext, mCamera.GetPos());
}

void Player::Render(float dTime)
{
	// Refresh depth buffer
	gd3dImmediateContext->ClearDepthStencilView(gDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Render current weapon
	inv.GetCurrentWeapon()->Render(dTime, mCamera);

	FX::MyFX& fx = *FX::GetMyFX();
	if (!paused)
	{
		if (DEBUG) // Debug menu
		{
			CommonStates state(gd3dDevice);
			fx.mpSpriteB->Begin(SpriteSortMode_Deferred, state.NonPremultiplied());

			// Window info
			int w, h;
			GetClientExtents(w, h);
			Vector2 windowCenter(w / 2, h / 2);

			Vector2 pos;

			std::wstringstream ss;

			ss << L"Debug: ";
			if (debugActive)
				ss << "ENABLED [Press HOME to deactivate]";
			else
				ss << "DISABLED [Press HOME to activate]";
			fx.mpFont->DrawString(fx.mpSpriteB, ss.str().c_str(), Vector2(w - 400, h - 30), Colors::White, 0, Vector2(0, 0), Vector2(0.6, 0.6));

			if (debugActive)
			{
				std::wstringstream ss2eb; // StringStream 2: electric boogaloo

				std::wstring output;
				ss2eb << L"==={ General }===";
				if (controllerConnected)
				{
					GetGamepad()->GetAnalogueNames(output);
					ss2eb << "\nAnalogue: " << output;
					GetGamepad()->GetDigitalNames(output);
					ss2eb << "\nDigital: " << output;
				}
				else
				{
					GetMouseAndKeys()->GetPressedKeyNames(output);
					ss2eb << "\nKeyboard: " << output;
					GetMouseAndKeys()->GetMouseNames(output);
					ss2eb << "\nMouse:    " << output;
				}

				ss2eb << "\ndTime:      " << dTime;
				if (dTime > 0)
					ss2eb << "\nFPS:      " << (int)(1.f / dTime);
				else
					ss2eb << "\nFPS:      0";

				ss2eb << "\n\n==={ Player }===";
				ss2eb << L"\nHealth: " << GetCurrentHealth() << "/" << GetMaxHealth();
				ss2eb << L"\nScore: " << Leaderboard::GetInstance()->GetCurrentScore()->GetScore();
				ss2eb << L"\nPosition: X = " << (float)position.x << " Y =  " << (float)position.y << " Z = " << (float)position.z;

				ss2eb << "\n\n==={ Camera }===";
				ss2eb << L"\nCam rotation: Yaw = " << (float)mCamera.GetYaw() << " Pitch = " << (float)mCamera.GetPitch() << " Roll = " << (float)mCamera.GetRoll();
				ss2eb << L"\nCam rotation speed = " << (float)mCamera.GetCamRotateSpeed() << " [Edit with Numpad +/-]";

				ss2eb << "\n\n==={ Inventory }===";
				ss2eb << L"\nWeapons: ";
				for (int i = 0; i < inv.GetAllWeapons().size(); i++)
				{
					ss2eb << L"\n     " << inv.GetAllWeapons()[i]->GetWeaponName().c_str();
				}
				ss2eb << L"\nCurrent Weapon: " << inv.GetCurrentWeapon()->GetWeaponName().c_str();
				ss2eb << L"\n     Damage: " << (int)inv.GetCurrentWeapon()->GetDamage();

				fx.mpFont->DrawString(fx.mpSpriteB, ss2eb.str().c_str(), Vector2(20, 20), Colors::White, 0, Vector2(0, 0), Vector2(0.6, 0.6));
			}
			fx.mpSpriteB->End();
		}
		hud.Render(dTime, GetCurrentHealth(), Leaderboard::GetInstance()->GetCurrentScore()->GetScore()); // HUD
	}
}

bool Player::IsOnLiquid()
{
	return dynamic_cast<LiquidBlock*>(level->GetBlockAt(position.x, position.z)) != nullptr;
}

void Player::ApplyGravity(float dTime, bool debug)
{
	if (!debug)
	{
		if (position.y > 0.5f)
			position += gravity * gravityMultiplier * dTime;
		else if (position.y <= 0.5f && inAir)
			inAir = false;
	}
}

void Player::AddWeapon(string weaponName)
{
	inv.AddWeapon(weaponName);
}

void Player::OnEnable()
{
	// Making sure the player is looking the right way
	Block* currentBlock = level->GetBlockAt(static_cast<int>(position.x), static_cast<int>(position.z));
	Block* leftBlock = level->GetBlockAt(static_cast<int>(position.x - 1.0f), static_cast<int>(position.z));
	Block* rightBlock = level->GetBlockAt(static_cast<int>(position.x + 1.0f), static_cast<int>(position.z));
	Block* frontBlock = level->GetBlockAt(static_cast<int>(position.x), static_cast<int>(position.z - 1.0f));
	Block* backBlock = level->GetBlockAt(static_cast<int>(position.x), static_cast<int>(position.z + 1.0f));

	if (dynamic_cast<SoildBlock*>(leftBlock) && !dynamic_cast<SoildBlock*>(rightBlock)) {
		// We need to look right
		mCamera.SetYaw(DirectX::XMConvertToRadians(90));
	}

	if (dynamic_cast<SoildBlock*>(rightBlock) && !dynamic_cast<SoildBlock*>(leftBlock)) {
		// We need to look left
		mCamera.SetYaw(DirectX::XMConvertToRadians(-90));
	}

	if (dynamic_cast<SoildBlock*>(frontBlock) && !dynamic_cast<SoildBlock*>(backBlock)) {
		// We need to look back
		mCamera.SetYaw(DirectX::XMConvertToRadians(0));
	}

	if (dynamic_cast<SoildBlock*>(backBlock) && !dynamic_cast<SoildBlock*>(frontBlock)) {
		// We need to look front
		mCamera.SetYaw(DirectX::XMConvertToRadians(180));
	}
}

DirectX::BoundingBox Player::GetBoundingBox()
{
	return DirectX::BoundingBox(position, Vector3(0.3f, 1.0f, 0.3f));
}

void Player::Die()
{
	//DO player related death stuff. Play a sound, put a delay in here, other visual queues

	//Load the game over screen
	if(!GameModeManager::GetInstance()->AwaitingSwap())
		GameModeManager::GetInstance()->SwapMode(3);
}