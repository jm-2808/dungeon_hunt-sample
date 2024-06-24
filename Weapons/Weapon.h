#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <vector>

#include "Mesh.h"
#include "Model.h"
#include "FX.h"
#include "D3D.h"

#include "FPSCamera.h"
#include "Projectile.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

const float radian = PI / 180;

class Weapon
{
public:
	Weapon(string name, vector<string> modelNames, vector<Vector3> offsets, vector<Vector3> scales, vector<Vector3> rotations, string crosshairName, int dmg, float cooldown);
	virtual ~Weapon();

	virtual void Update(float dTime) = 0;
	virtual void Attack(Vector3 position, Vector3 rotation, Matrix pitchYawRoll, Level* level) = 0;
	virtual void Render(float dTime, FPSCamera cam);
	virtual void Release() = 0;

	string GetWeaponName()		{ return weaponName; }

	int GetDamage()				{ return damage; }

	bool GetEquipped()			{ return equipped; }
	void SetEquipped(bool eqpt) { equipped = eqpt; }

	float GetCooldown()			{ return weaponFireCooldown; }

protected:
	float fireDelay = 0;

	vector<Vector3> GetWeaponScales()	{ return weaponScales; }
	void SetWeaponScale(int modelNo, Vector3 scale);

	vector<Vector3> GetWeaponOffsets()	{ return weaponOffsets; }
	void SetWeaponOffset(int modelNo, Vector3 offset);

	vector<Vector3> GetWeaponRotations() { return weaponRotations; }
	void SetWeaponRotation(int modelNo, Vector3 rotation);
	
private:
	string weaponName;

	int damage;
	bool equipped;
	float weaponFireCooldown = 0;

	void InitialiseModels(vector<string> meshNames, vector<string> filePaths, vector<Vector3> scales, vector<Vector3> positions, vector<Vector3> rotations);
	vector<Model> models;
	vector<Vector3> weaponScales;
	vector<Vector3> weaponOffsets;
	vector<Vector3> weaponRotations;

	void LoadCrosshair(string filename);
	int crosshairTexIndex;

	class Level* level;

};

#endif