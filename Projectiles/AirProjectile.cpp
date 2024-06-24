#include "AirProjectile.h"

#include "Enemy.h"
#include "BouncyProjectile.h"

AirProjectile::AirProjectile(Vector3 pos, Vector3 dir, int damage)
	: Projectile
	(
		"PABHC_Payload",					// Model name
		pos,								// Projectile start position
		Vector3(0.001f),					// Model scale
		Vector3(0, 0, 0),					// Model rotation
		dir,								// Projectile movement direction
		damage,								// Projectile damage
		0,									// Projectile speed
		0.2,								// Projectile lifetime
		false								// Is the projectile visible?
	)
{
	currentLife = lifeTime;
}

AirProjectile::~AirProjectile()
{ }

void AirProjectile::Update(float dTime)
{
	currentLife -= dTime;
	if (currentLife <= 0)
		DestroySelf();
}

DirectX::BoundingBox AirProjectile::GetBoundingBox()
{
	return DirectX::BoundingBox(position, Vector3(0.7f, 0.5f, 0.7f));
}

void AirProjectile::OnLevelCollision(bool inXAxis, bool inZAxis)
{

}

void AirProjectile::Collision(Entity* entityCollidedWith, bool inXAxis, bool inZAxis)
{
	Enemy* enemy = dynamic_cast<Enemy*>(entityCollidedWith);
	BouncyProjectile* bncyprjcl = dynamic_cast<BouncyProjectile*>(entityCollidedWith);
	if (enemy)
	{
		enemy->TakeDamage(GetDamage());
		DestroySelf();
	}
	else
	{
		if (bncyprjcl)
		{
			bncyprjcl->UpdateDirection(direction);
			DestroySelf();
		}
	}
}