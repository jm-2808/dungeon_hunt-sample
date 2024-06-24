#include "Gauntlets.h"

#include "Level.h"

Gauntlets::Gauntlets(string name)
	: Weapon
	(
		name,								// Weapon Name
		{									// Model names
			"RightGauntlet", 
			"LeftGauntlet" 
		},		
		{									// Weapon Position Offsets
			Vector3(0.5, -0.2, 1.1),
			Vector3(-0.5, -0.2, 1.1)
		},			
		{									// Weapon Scales
			Vector3(0.0035),
			Vector3(0.0035)
		},	
		{									// Weapon Rotations
			Vector3((radian * 20), PI + -(radian * 40), -(radian * 30)),
			Vector3((radian * 20), PI + (radian * 40),   (radian * 30))
		},	
		"gauntlets_crosshair",
		100,								// Weapon Damage			
		0.001									// Weapon Fire Cooldown
	)
{
	// Animation
	currentGauntlet = RIGHTGAUNTLET;
	punch = false;
	retract = false;
	animCounter = 0;
	Initialise();
}

void Gauntlets::Initialise()
{
	for (int i = 0; i < GetWeaponOffsets().size(); i++)
	{
		baseOffsets.push_back(GetWeaponOffsets()[i]);
		currentOffset.push_back(Vector3(0, 0, 0));
	}

	for (int i = 0; i < GetWeaponRotations().size(); i++)
	{
		baseRotations.push_back(GetWeaponRotations()[i]);
		currentRotations.push_back(Vector3(0, 0, 0));
	}
}

Gauntlets::~Gauntlets()
{ }

void Gauntlets::Update(float dTime)
{
	if (fireDelay > 0)
		fireDelay -= dTime;
	else if (fireDelay != 0)
		fireDelay = 0;

	if (punch)
		AnimatePunch(dTime, currentGauntlet);
}

void Gauntlets::Attack(Vector3 position, Vector3 rotation, Matrix pitchYawRoll, Level* level)
{
	if (fireDelay <= 0 && !punch)
	{
		if (GetIAudioMgr()->GetSfxMgr()->IsPlaying(punchSound) == false)
			GetIAudioMgr()->GetSfxMgr()->Play("174764__yottasounds__swoop-007", false, false, &punchSound, 0.1);
		else
			GetIAudioMgr()->GetSfxMgr()->Stop(punchSound);

		Vector3 dir(0, 0, 1);
		dir = Vector3::TransformNormal(dir, pitchYawRoll);		// Transform dir so it faces camera direction
		dir.y = 0;

		AirProjectile* airBurst = new AirProjectile(position + (dir*1.2), dir, GetDamage());

		level->GetBlockAt(position.x, position.z)->sprites.push_back(airBurst);
		airBurst->LinkEntityToMap(level);

		punch = true;
	}
}

void Gauntlets::AnimatePunch(float dTime, int gauntlet)
{
	// Idle rotation
	// Vector3(	(radian * 20), -(radian * 40), -(PI / 2) + (radian * 10)),
	// Vector3(	(radian * 20),  (radian * 40),	(PI / 2) - (radian * 10))

	// Punch rotation
	// Vector3(				0,				0,			  -(radian * 10)),
	// Vector3(				0,				0,			   (radian * 10))

	if (currentOffset[gauntlet].z > punchDuration)
		retract = true;

	if (!retract)
	{
		if (animCounter < hOffsetTimeMax)
			currentOffset[gauntlet].x -= horizontalOffset * dTime;
		currentOffset[gauntlet].z += forwardForce * dTime;

		if (animCounter < rotationTimeMax)
		{
			currentRotations[gauntlet].x += xRotationIncrement * dTime;
			currentRotations[gauntlet].y += yRotationIncrement * dTime;
			currentRotations[gauntlet].z += zRotationIncrement * dTime;
		}
	}
	else
	{
		if (animCounter < hOffsetTimeMax)
			currentOffset[gauntlet].x += horizontalOffset * dTime;
		currentOffset[gauntlet].z -= forwardForce * dTime;

		if (animCounter < rotationTimeMax)
		{
			currentRotations[gauntlet].x -= xRotationIncrement * dTime;
			currentRotations[gauntlet].y -= yRotationIncrement * dTime;
			currentRotations[gauntlet].z -= zRotationIncrement * dTime;
		}
	}

	// Set weapon position and rotation using modified values
	SetWeaponOffset(gauntlet, baseOffsets[gauntlet] + currentOffset[gauntlet]);
	SetWeaponRotation(gauntlet, baseRotations[gauntlet] + currentRotations[gauntlet]);
	
	if(!retract)
		animCounter += dTime;
	else
		animCounter -= dTime;

	// If the gauntlet is back to it's default position, reset and swap gauntlets
	float total = currentOffset[gauntlet].x + currentOffset[gauntlet].y + currentOffset[gauntlet].z;
	if (total <= 0)
	{
		punch = false;
		retract = false;
		animCounter = 0;

		// Offset
		currentOffset[gauntlet] = Vector3(0, 0, 0);
		SetWeaponOffset(gauntlet, baseOffsets[gauntlet] + currentOffset[gauntlet]);

		// Rotation
		currentRotations[gauntlet] = Vector3(0, 0, 0);
		SetWeaponRotation(gauntlet, baseRotations[gauntlet] + currentRotations[gauntlet]);
		
		// Swap hand
		if (currentGauntlet == RIGHTGAUNTLET)
			currentGauntlet = LEFTGAUNTLET;
		else
			currentGauntlet = RIGHTGAUNTLET;
		// Invert values for opposite hand
		horizontalOffset *= -1;
		yRotationIncrement *= -1;
		zRotationIncrement *= -1;
		
		fireDelay = GetCooldown();
	}
}

void Gauntlets::Release()
{
	baseOffsets.clear();
	currentOffset.clear();

	baseRotations.clear();
	currentRotations.clear();
}