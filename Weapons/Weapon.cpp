#include "Weapon.h"

// Includes for UI
#include "WindowUtils.h"
#include "CommonStates.h"

Weapon::Weapon(string name, vector<string> modelNames,  vector<Vector3> offsets, vector<Vector3> scales, vector<Vector3> rotations, string crosshairName, int dmg, float cooldown)
	: weaponName(name), weaponOffsets(offsets), weaponScales(scales), weaponRotations(rotations), damage(dmg), weaponFireCooldown(cooldown), fireDelay(0), equipped(false)
{
	vector<string> weaponFilePaths;
	for (int i = 0; i < modelNames.size(); i++)	// For each model name, generate a file path
	{
		string weaponFile = "data/weapons/" + modelNames[i] + ".fbx";
		weaponFilePaths.push_back(weaponFile);
	}
	InitialiseModels(modelNames, weaponFilePaths, scales, offsets, rotations);

	string crosshairFile = "weapons/crosshairs/" + crosshairName + ".dds";
	LoadCrosshair(crosshairFile);
}

Weapon::~Weapon()
{ }

void Weapon::Render(float dTime, FPSCamera cam)
{
	if (equipped)
	{
		FX::MyFX& fx = *FX::GetMyFX();
		
		// Render each model
		Matrix camLocalToWorld = cam.GetMatrix().Invert();	
		for (int i = 0; i < models.size(); i++) 
		{		
			Matrix scale = Matrix::CreateScale(weaponScales[i]);

			Matrix rotationX = Matrix::CreateRotationX(weaponRotations[i].x);
			Matrix rotationY = Matrix::CreateRotationY(weaponRotations[i].y);
			Matrix rotationZ = Matrix::CreateRotationZ(weaponRotations[i].z);
			Matrix rotation = rotationX * rotationY * rotationZ;

			Matrix offset = Matrix::CreateTranslation(weaponOffsets[i]);
	
			Matrix camLock = scale * rotation * offset * camLocalToWorld;

			fx.Render(models[i], gd3dImmediateContext, nullptr, &camLock);
		}

		// Render crosshair
		CommonStates state(gd3dDevice);
		fx.mpSpriteB->Begin(SpriteSortMode_Deferred, state.NonPremultiplied());
		Vector2 imageOrigin(fx.mCache.Get(crosshairTexIndex).dim.x / 2, fx.mCache.Get(crosshairTexIndex).dim.y / 2);
		int w, h;
		GetClientExtents(w, h);
		Vector2 windowCenter(w / 2, h / 2);
		Vector2 scale(0.2);
		fx.mpSpriteB->Draw(fx.mCache.Get(crosshairTexIndex).pTex, (windowCenter - (imageOrigin * scale)), nullptr, Colours::White, 0, Vector2(0, 0), scale);
		fx.mpSpriteB->End();
	}
}

void Weapon::InitialiseModels(vector<string> meshNames, vector<string> filePaths, vector<Vector3> scales, vector<Vector3> positions, vector<Vector3> rotations)
{
	for (int i = 0; i < meshNames.size(); i++) // For each model, load it and store it in the models vector with the specified default values
	{
		Model model;
		models.push_back(model);

		if (!GetMeshManager()->GetMesh(meshNames[i]))
		{
			Mesh& mesh = GetMeshManager()->CreateMesh(meshNames[i]);
			mesh.CreateFrom(filePaths[i], gd3dDevice, FX::GetMyFX()->mCache);
			models[i].Initialise(mesh);
		}
		else
		{
			Mesh* mesh = GetMeshManager()->GetMesh(meshNames[i]);
			models[i].Initialise(*mesh);
		}

		models[i].GetScale()	= scales[i];
		models[i].GetPosition()	= positions[i];
		models[i].GetRotation() = rotations[i];
	}
}

void Weapon::SetWeaponScale(int modelNo, Vector3 scale)
{
	if (modelNo < weaponScales.size() && modelNo >= 0)
		weaponScales[modelNo] = scale;
}

void Weapon::SetWeaponOffset(int modelNo, Vector3 offset)
{
	if (modelNo < weaponOffsets.size() && modelNo >= 0)
		weaponOffsets[modelNo] = offset;
}

void Weapon::SetWeaponRotation(int modelNo, Vector3 rotation)
{
	if (modelNo < weaponRotations.size() && modelNo >= 0)
		weaponRotations[modelNo] = rotation;
}

void Weapon::LoadCrosshair(string filename)
{
	FX::GetMyFX()->mCache.LoadTexture(filename, true, gd3dDevice);
	crosshairTexIndex = FX::GetMyFX()->mCache.GetIdx(filename);
}