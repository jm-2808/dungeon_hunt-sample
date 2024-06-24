#include "Pickup.h"

#include "Player.h"

Pickup::Pickup(string weaponName, string meshName, Vector3 pos, Vector3 scale, Vector3 rotation, float speed)
	: speed(0, speed, 0), name(weaponName)
{
	position = pos;

	position.y = baseYPos;

	string filePath = "data/weapons/" + meshName + ".fbx";
	InitialiseModel(meshName, filePath, scale, pos, rotation);
}

Pickup::~Pickup()
{ }

void Pickup::Update(float dTime)
{
	position += speed * dTime;

	if (position.y < baseYPos - movementBoundary)
	{
		position.y = baseYPos - movementBoundary;
		speed *= -1;
	}
	else if (position.y > baseYPos + movementBoundary)
	{
		position.y = baseYPos + movementBoundary;
		speed *= -1;
	}

	model.GetRotation() += Vector3(0, PI / 180, 0);

	model.GetPosition() = position;
}

void Pickup::Render(float dTime)
{
	FX::MyFX& fx = *FX::GetMyFX();
	fx.Render(model, gd3dImmediateContext);
}

DirectX::BoundingBox Pickup::GetBoundingBox()
{
	return DirectX::BoundingBox(position, Vector3(0.4f, 0.4f, 0.4f));
}

void Pickup::Collision(Entity* entityCollidedWith, bool inXAxis, bool inZAxis)
{
	Player* player = dynamic_cast<Player*>(entityCollidedWith);
	if (player)
	{
		player->AddWeapon(name);
		DestroySelf();
	}
}

void Pickup::InitialiseModel(string meshName, string filePath, Vector3 scale, Vector3 position, Vector3 rotation)
{
	if (!GetMeshManager()->GetMesh(meshName))
	{
		Mesh& m = GetMeshManager()->CreateMesh(meshName);
		m.CreateFrom(filePath, gd3dDevice, FX::GetMyFX()->mCache);
		model.Initialise(m);
	}
	else
	{
		Mesh* m = GetMeshManager()->GetMesh(meshName);
		model.Initialise(*m);
	}

	model.GetScale() = scale;
	model.GetPosition() = position;
	model.GetRotation() = rotation;
}