#include "Projectile.h"

Projectile::Projectile(string meshName, Vector3 pos, Vector3 scale, Vector3 rotation, Vector3 dir, int damage, float speed, float lifetime, bool visible)
	: direction(dir), damage(damage), speed(speed), lifeTime(lifetime), visible(visible)
{
	position = pos;

	string filePath = "data/projectiles/" + meshName + ".fbx";
	InitialiseModel(meshName, filePath, scale, pos, rotation);
}

Projectile::~Projectile()
{ }

void Projectile::Render(float dTime)
{
	if (visible)
	{
		FX::MyFX& fx = *FX::GetMyFX();
		fx.Render(model, gd3dImmediateContext);
	}
}

void Projectile::InitialiseModel(string meshName, string filePath, Vector3 scale, Vector3 position, Vector3 rotation)
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