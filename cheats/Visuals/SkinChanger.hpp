#pragma once
#include "../common_includes.hpp"

#include <unordered_map>

namespace SkinChanger
{
	void nSequence(const CRecvProxyData* pDta, void* pStruct, void* pOut);
	bool ApplyKnifeModel(C_BaseAttributableItem* weapoon, const char* model);
	bool ApplyKnifeSkin(C_BaseAttributableItem* weapon, int ItemDefinitionIndex, int paint_kit, int model_int, int entity_quality, float wear);
	void Run();
}