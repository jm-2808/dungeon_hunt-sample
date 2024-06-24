#ifndef PROJECTILE_BOUNCY_H
#define PROJECTILE_BOUNCY_H

#include "Projectile.h"

class BouncyProjectile : public Projectile
{
public:
	BouncyProjectile(Vector3 pos, Vector3 rotation, Vector3 dir, int damage);
	~BouncyProjectile();
	virtual void Update(float dTime);
	virtual DirectX::BoundingBox GetBoundingBox();
	virtual void OnLevelCollision(bool inXAxis, bool inZAxis);
	virtual void Collision(Entity* entityCollidedWith, bool inXAxis, bool inZAxis);

	void Bounce(float dTime);
	void UpdateDirection(Vector3 dir);

private:
	Vector3 velocity = Vector3(0, 0, 0);
	Vector3 gravity = Vector3(0, -9.81f, 0);

	float COR = 0.6f;
	const float bounceRangeMid = 3.5f;
	const float bounceRange = 0.5f;
	void BounceReflect(bool inXAxis, bool inZAxis);

	const float scale = 0.004f;
	const float compressMultiplier = 0.8f;
	const float radius = 50.f;
	unsigned int bounceSound;
	void MakeSound();
};

#endif