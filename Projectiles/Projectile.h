#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <string>

#include "Mesh.h"
#include "Model.h"
#include "FX.h"
#include "D3D.h"

#include "Entity.h"
#include "AudioMgr.h"
#include "AudioMgrFMOD.h"
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class Projectile : public Entity
{
public:
	Projectile(string meshName, Vector3 position, Vector3 scale, Vector3 rotation, Vector3 dir, int damage, float speed, float lifetime, bool visible);
	~Projectile();
	virtual void Update(float dTime) = 0;
	virtual void Render(float dTime);
	virtual DirectX::BoundingBox GetBoundingBox() = 0;

	Vector3 GetModelPosition()				{ return model.GetPosition(); }
	void SetModelPosition(Vector3 position) { model.GetPosition() = position; }

	Vector3 GetModelScale()				{ return model.GetScale(); }
	void SetModelScale(Vector3 scale)	{ model.GetScale() = scale; }

	Vector3 GetModelRotation()				{ return model.GetRotation(); }
	void SetModelRotation(Vector3 rotation) { model.GetRotation() = rotation; }

	int GetDamage()	{ return damage; }

protected:
	float speed = 5.f;
	Vector3 direction;

	const float lifeTime;
	float currentLife = 0.f;

	const bool visible;

private:
	int damage;
	void InitialiseModel(string meshName, string filePath, Vector3 scale, Vector3 position, Vector3 rotation);
	Model model;
};

#endif