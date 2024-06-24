#ifndef WEAPON_GAUNTLETS_H
#define WEAPON_GAUNTLETS_H

#include <vector>

#include "Weapon.h"
#include "AirProjectile.h"

using namespace std;

class Gauntlets : public Weapon
{
public:
	Gauntlets(string name);
	virtual ~Gauntlets();

	virtual void Update(float dTime);
	virtual void Attack(Vector3 position, Vector3 rotation, Matrix pitchYawRoll, Level* Level);
	virtual void Release();

private:
	unsigned int punchSound;
	enum { RIGHTGAUNTLET, LEFTGAUNTLET };
	int currentGauntlet;

	void Initialise();
	void AnimatePunch(float dTime, int gauntlet);

	// Animation variables
	bool punch;
	bool retract;

	float animCounter;
	const float punchDuration = 0.45f;
	const float rotationTimeMax = 0.05f;
	const float hOffsetTimeMax = 0.1;

	const float forwardForce = 3.f;
	float horizontalOffset = 1.f;

	float xRotationIncrement = -5.f;
	float yRotationIncrement = 12.f;
	float zRotationIncrement = 15.f;

	vector<Vector3> baseOffsets;
	vector<Vector3> currentOffset;
	vector<Vector3> baseRotations;
	vector<Vector3> currentRotations;

};

#endif