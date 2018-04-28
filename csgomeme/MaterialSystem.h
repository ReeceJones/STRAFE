#pragma once
#include "IMaterial.h"
#include "Util.h"
#include <string>
#include <sstream>
#include "KeyValues.h"

typedef unsigned short MaterialHandle_t;
class IMaterialSystem
{
public:
	IMaterial* CreateMaterial(std::string type, std::string texture, bool ignorez, bool nofog, bool model, bool nocull, bool halflambert, bool wireframe, bool _bubble)
	{
		static std::string env = "env_cubemap";
		//std::stringstream materialData;
		std::string materialData;
		if (!_bubble)
		materialData = ("\"" + type + "\"\n"
			"{\n"
			"\t\"$basetexture\" \"" + texture + "\"\n"
			"\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n"
			"\t\"$nofog\" \"" + std::to_string(nofog) + "\"\n"
			"\t\"$model\" \"" + std::to_string(model) + "\"\n"
			"\t\"$nocull\" \"" + std::to_string(nocull) + "\"\n"
			"\t\"$halflambert\" \"" + std::to_string(halflambert) + "\"\n"
			"\t\"$wireframe\" \"" + std::to_string(wireframe) + "\"\n"
		//	"\t\"$envmap\" \"" + "env_cubemap" + "\"\n" //terminator effect
			"}\n");
		else
		{
			materialData = ("\"" + type + "\"\n"
				"{\n"
				"\t\"$basetexture\" \"" + texture + "\"\n"
				"\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n"
				"\t\"$nofog\" \"" + std::to_string(nofog) + "\"\n"
				"\t\"$model\" \"" + std::to_string(model) + "\"\n"
				"\t\"$nocull\" \"" + std::to_string(nocull) + "\"\n"
				"\t\"$halflambert\" \"" + std::to_string(halflambert) + "\"\n"
				"\t\"$wireframe\" \"" + std::to_string(wireframe) + "\"\n"
				"\t\"$envmap\" \"" + "env_cubemap" + "\"\n" //terminator effect
				"}\n");
		}

		std::string materialName = "custom_" + std::to_string(std::rand() % 10 + 999991);
		KeyValues* keyValues = new KeyValues(materialName.c_str());

		g_pUtil->InitKeyValues(keyValues, type.c_str());
		//g_pUtil->LoadFromBuffer(keyValues, materialName.c_str(), materialData.str().c_str(), nullptr, NULL, nullptr);
		g_pUtil->LoadFromBuffer(keyValues, materialName.c_str(), materialData.c_str(), nullptr, NULL, nullptr);
		typedef IMaterial*(__thiscall* OriginalFn)(void*, const char *pMaterialName, KeyValues *pVMTKeyValues);
		IMaterial* createdMaterial = g_pUtil->CallVFunc<OriginalFn>(this, 83)(this, materialName.c_str(), keyValues);

		createdMaterial->IncrementReferenceCount();
		return createdMaterial;
	}

	IMaterial* FindMaterial(char const* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = nullptr)
	{
		typedef IMaterial* (*oFindMaterial)(void*, char const*, const char*, bool, const char*);
		return g_pUtil->CallVFunc<oFindMaterial>(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}

	MaterialHandle_t FirstMaterial()
	{
		typedef MaterialHandle_t(__thiscall* FirstMaterialFn)(void*);
		return g_pUtil->CallVFunc<FirstMaterialFn>(this, 86)(this);
	}

	MaterialHandle_t NextMaterial(MaterialHandle_t h)
	{
		typedef MaterialHandle_t(__thiscall* NextMaterialFn)(void*, MaterialHandle_t);
		return g_pUtil->CallVFunc<NextMaterialFn>(this, 87)(this, h);
	}

	MaterialHandle_t InvalidMaterial()
	{
		typedef MaterialHandle_t(__thiscall* InvalidMaterialFn)(void*);
		return g_pUtil->CallVFunc<InvalidMaterialFn>(this, 88)(this);
	}

	IMaterial* GetMaterial(MaterialHandle_t h)
	{
		typedef IMaterial*(__thiscall* GetMaterialFn)(void*, MaterialHandle_t);
		return g_pUtil->CallVFunc<GetMaterialFn>(this, 89)(this, h);
	}
};