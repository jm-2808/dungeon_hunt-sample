#ifndef WEAPON_PABHC_H
#define WEAPON_PABHC_H

#include "Weapon.h"
#include "BouncyProjectile.h"

using namespace std;

class Bouncer : public Weapon
{
public:
	Bouncer(string name);
	virtual ~Bouncer();

	virtual void Update(float dTime);
	virtual void Attack(Vector3 position, Vector3 rotation, Matrix pitchYawRoll, Level* Level);
	virtual void Release();

private:
	unsigned int popSound;
	Vector3 relativeBarrelPos;

};

#endif
