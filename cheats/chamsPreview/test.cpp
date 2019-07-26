#include "../../valve_sdk/csgostructs.hpp"
#include "../../valve_sdk/sdk.hpp"
#include "../../helpers/math.hpp"
#include "../../singleton.hpp"
#include "../../settings.hpp"
#include "../../render.hpp"
#include "../../hooks.hpp"
#include "../../valve_sdk/misc/vfunc.hpp"
#include "../../menu.hpp"

inline void RenderCustomView(CViewSetup& view, ITexture* texture)
{
	IMatRenderContext* renderCtx = g_MatSystem->GetRenderContext();

	renderCtx->PushRenderTargetAndViewport();
	renderCtx->SetRenderTarget(texture);

	static auto oRenderView = Hooks::renderview_hook.get_original<decltype(&Hooks::hkRenderView)>(index::RenderView);

	oRenderView(g_ViewRender,
		nullptr, view, view,
		(ClearFlags_t)((int)ClearFlags_t::VIEW_CLEAR_COLOR | (int)ClearFlags_t::VIEW_CLEAR_DEPTH | (int)ClearFlags_t::VIEW_CLEAR_STENCIL),
		(RenderViewInfo_t)0);

	renderCtx->PopRenderTargetAndViewport();
	renderCtx->Release();
}

