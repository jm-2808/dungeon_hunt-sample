#include "Bouncer.h"

#include "Level.h"

Bouncer::Bouncer(string name)
	: Weapon
	(
		name,								// Weapon Name
		{									// Model names
			"PABHC" 
		},						
		{									// Weapon Position Offsets
			Vector3(0.6, 0, 0.7) 
		},			
		{									// Weapon Scales
			Vector3(0.005) 
		},	
		{									// Weapon Rotations
			Vector3(0, PI, 0) 
		},
		"pabhc_crosshair",
		100,								// Weapon Damage				
		0.8									// Weapon Fire Cooldown
	)				
{ }

Bouncer::~Bouncer()
{ }

void Bouncer::Update(float dTime)
{
	if (fireDelay > 0)
		fireDelay -= dTime;
	else if (fireDelay != 0)
		fireDelay = 0;
}

void Bouncer::Attack(Vector3 position, Vector3 rotation, Matrix pitchYawRoll, Level* level)
{
	if (fireDelay <= 0)
	{
		if (GetIAudioMgr()->GetSfxMgr()->IsPlaying(popSound) == false)
			GetIAudioMgr()->GetSfxMgr()->Play("104398__kantouth__flak-exp", false, false, &popSound, 0.25);
		else
			GetIAudioMgr()->GetSfxMgr()->Stop(popSound);

		Vector3 dir(0, 0, 1);
		dir = Vector3::TransformNormal(dir, pitchYawRoll);		// Transform dir so it faces camera direction
		dir.y = 0;

		rotation.y += PI;

		BouncyProjectile* bouncyHopper = new BouncyProjectile(position, rotation, dir, GetDamage());

		level->GetBlockAt(position.x, position.z)->sprites.push_back(bouncyHopper);
		bouncyHopper->LinkEntityToMap(level);

		fireDelay = GetCooldown();
	}
}

void Bouncer::Release()
{

}