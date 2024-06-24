#include "BouncyProjectile.h"

#include "Enemy.h"
#include "AirProjectile.h"

BouncyProjectile::BouncyProjectile(Vector3 pos, Vector3 rotation, Vector3 dir, int damage)
	: Projectile
	(
		"PABHC_Payload",					// Model name
		pos,								// Projectile start position
		Vector3(0.004f),					// Model scale
		rotation,							// Model rotation
		dir,								// Projectile movement direction
		damage,								// Projectile damage
		5,									// Projectile speed
		5,									// Projectile lifetime
		true								// Is the projectile visible?
	)
{
	currentLife = lifeTime;
}

BouncyProjectile::~BouncyProjectile()
{ }

void BouncyProjectile::Update(float dTime)
{
	Bounce(dTime);

	if (GetModelScale().y < scale)
		SetModelScale(Vector3(scale, GetModelScale().y + ((scale / 2) * dTime), scale));
	else if (GetModelScale().y > scale)
		SetModelScale(Vector3(scale, scale, scale));

	currentLife -= dTime;
	if (currentLife <= 0)
		DestroySelf();
}

void BouncyProjectile::Bounce(float dTime)
{
	// Bounce
	position += ((direction * speed) + velocity) * dTime;

	//if (dynamic_cast<LiquidBlock*>(level->GetBlockAt(position.x, position.z)) != nullptr) // On liquid
	//{
	//	if (position.y < (radius * scale) - 0.5)
	//	{
	//		//bounce up
	//		velocity.y *= -1;
	//		if (velocity.y >(bounceRangeMid + bounceRange))
	//			velocity.y = (bounceRangeMid + bounceRange);
	//		else if (velocity.y < (bounceRangeMid - bounceRange))
	//			velocity.y = (bounceRangeMid - bounceRange);
	//
	//		SetModelScale(Vector3(scale, scale * compressMultiplier, scale));
	//
	//		MakeSound();
	//
	//		//velocity *= COR;
	//
	//		//ensure we aren't left under the floor
	//		position.y = (radius * scale);
	//	}
	//}
	//else
	//{
		if (position.y < (radius * scale))
		{
			//bounce up
			velocity.y *= -1;
			if (velocity.y > (bounceRangeMid + bounceRange))
				velocity.y = (bounceRangeMid + bounceRange);
			else if (velocity.y < (bounceRangeMid - bounceRange))
				velocity.y = (bounceRangeMid - bounceRange);

			SetModelScale(Vector3(scale, scale * compressMultiplier, scale));

			MakeSound();

			//velocity *= COR;

			//ensure we aren't left under the floor
			position.y = (radius * scale);
		}
	//}

	if (velocity.y > gravity.y)
		velocity += gravity * dTime;
	SetModelPosition(position);
}

void BouncyProjectile::UpdateDirection(Vector3 dir)
{
	currentLife = lifeTime;
	direction = dir;
	MakeSound();
}

DirectX::BoundingBox BouncyProjectile::GetBoundingBox()
{
	return DirectX::BoundingBox(position, Vector3(0.3f, 0.3f, 0.3f));
}

void BouncyProjectile::OnLevelCollision(bool inXAxis, bool inZAxis)
{
	BounceReflect(inXAxis, inZAxis);
}

void BouncyProjectile::Collision(Entity* entityCollidedWith, bool inXAxis, bool inZAxis)
{
	Enemy* enemy = dynamic_cast<Enemy*>(entityCollidedWith);
	if (enemy)
	{
		enemy->TakeDamage(GetDamage());
		DestroySelf();
	}
	else
	{
		if (dynamic_cast<BouncyProjectile*>(entityCollidedWith))
		{
			BounceReflect(inXAxis, inZAxis);
		}
	}
}

void BouncyProjectile::BounceReflect(bool inXAxis, bool inZAxis)
{
	Vector3 dir = direction;
	Vector3 rot = GetModelRotation();
	Vector3 wallNormal(0, 0, 0);
	float x(0);
	
	if (inXAxis)
	{
		if (dir.z > 0)				// Positive z
			wallNormal = Vector3(0, 0, -1);
		else						// Negative z
			wallNormal = Vector3(0, 0, 1);

		x = dir.Dot(wallNormal);
		x /= dir.Length() * wallNormal.Length();
		x = acos(x);
		x *= 2;

		if (dir.x > 0)
		{
			if (dir.z > 0)				// Positive z
				rot.y += x;
			else						// Negative z
				rot.y -= x;
		}
		else
		{
			if (dir.z > 0)				// Positive z
				rot.y -= x;
			else						// Negative z
				rot.y += x;
		}

		dir.x *= -1;
		MakeSound();
	}
	if (inZAxis)
	{
		if (dir.x > 0)				// Positive x
			wallNormal = Vector3(-1, 0, 0);
		else						// Negative x
			wallNormal = Vector3(1, 0, 0);

		x = dir.Dot(wallNormal);
		x /= dir.Length() * wallNormal.Length();
		x = acos(x);
		x *= 2;

		if (dir.z > 0)
		{
			if (dir.x > 0)				// Positive x
				rot.y -= x;
			else						// Negative x
				rot.y += x;
		}
		else
		{
			if (dir.x > 0)				// Positive x
				rot.y += x;
			else						// Negative x
				rot.y -= x;
		}

		dir.z *= -1;
		MakeSound();
	}

	SetModelRotation(rot);
	direction = dir;
}

void BouncyProjectile::MakeSound()
{
	//make a sound
	if (GetIAudioMgr()->GetSfxMgr()->IsPlaying(bounceSound) == false)
		GetIAudioMgr()->GetSfxMgr()->Play("161122__reelworldstudio__cartoon-boing", false, false, &bounceSound, 1 / (level->GetPlayer()->GetPosition() - position).Length());
	else
	{
		GetIAudioMgr()->GetSfxMgr()->Stop(bounceSound);
		GetIAudioMgr()->GetSfxMgr()->Play("161122__reelworldstudio__cartoon-boing", false, false, &bounceSound, 1 / (level->GetPlayer()->GetPosition() - position).Length());
	}
}