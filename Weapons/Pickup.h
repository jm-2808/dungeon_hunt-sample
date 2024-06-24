#ifndef WEAPONPICKUP_H
#define WEAPONPICKUP_H

#include <string>

#include "Mesh.h"
#include "Model.h"
#include "FX.h"
#include "D3D.h"

#include "Entity.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class Pickup : public Entity
{
public:
	Pickup(string weaponName, string meshName, Vector3 position, Vector3 scale, Vector3 rotation, float speed);
	~Pickup();
	virtual void Update(float dTime);
	virtual void Render(float dTime);
	virtual DirectX::BoundingBox GetBoundingBox();
	virtual void Collision(Entity* entityCollidedWith, bool inXAxis, bool inZAxis);

protected:
	Vector3 speed;

	float baseYPos = 0.4f;
	float movementBoundary = 0.15f;

private:
	void InitialiseModel(string meshName, string filePath, Vector3 scale, Vector3 position, Vector3 rotation);
	Model model;

	string name;
};

#endif