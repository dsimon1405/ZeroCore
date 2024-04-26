#pragma once

#include "ZC_UBO.h"
#include <ZC/Tools/Function/ZC_Function.h>
#include <ZC/Video/OpenGL/Renderer/ZC_RenderLevel.h>

#include <forward_list>

#define ZC_AddToRenderer ZC_INT_MIN

class ZC_UBOs
{
public:
	virtual ~ZC_UBOs() = default;

	/*
	Adds function for update existing ZC_UBO to one of the ZC_Render (renderLevel) or to ZC_Renderer.

	Params:
	- pUbo - pointer on ubo.
	- fUpdate - function for update.
	- renderLevel - one of the ZC_Render or set ZC_AddToRenderer to add in ZC_Renderer.
	*/
	static void AddUpdateFunction(ZC_UBO* pUbo, ZC_Function<void()> fUpdate, ZC_RenderLevel renderLevel);
	void AddUBO(ZC_UBO* pUbo, ZC_Function<void()>&& fUpdate);
	void EraseUBO(ZC_UBO* pUbo);

protected:
	void UpdateUBO();

private:
	struct Pair
	{
		ZC_UBO* ubo;
		ZC_Function<void()> fUpdate;

		bool operator == (ZC_UBO* pUbo) const noexcept;
	};

    std::forward_list<Pair> ubos;
};