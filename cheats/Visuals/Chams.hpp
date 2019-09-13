#pragma once

#include "../common_includes.hpp"

namespace Chams
{
	void SceneEnd();
	void OnDME(
		IMatRenderContext* ctx,
		const DrawModelState_t& state,
		const ModelRenderInfo_t& info,
		matrix3x4_t* matrix);

	void Initialize();
	void Shutdown();
};