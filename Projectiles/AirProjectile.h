#ifndef PROJECTILE_AIR_H
#define PROJECTILE_AIR_H

#include "Projectile.h"

class AirProjectile : public Projectile
{
public:
	AirProjectile(Vector3 pos, Vector3 dir, int damage);
	~AirProjectile();
	virtual void Update(float dTime);
	virtual DirectX::BoundingBox GetBoundingBox();
	virtual void OnLevelCollision(bool inXAxis, bool inZAxis);
	virtual void Collision(Entity* entityCollidedWith, bool inXAxis, bool inZAxis);

private:

};

#endif