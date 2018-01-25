#pragma once
#include "Engine/DX11/DX11.hpp"
#include "Engine/Core/StructsAndDefines.hpp"
#include "Engine/DX11/DX11.hpp"
#include "Engine/Core/StructsAndDefines.hpp"
#include "Engine/RHI/RHIDevice.hpp"

enum eBlendFactor
{
	BLEND_ONE,
	BLEND_ZERO,

	BLEND_SRC_ALPHA,
	BLEND_INV_SRC_ALPHA,

	BLEND_SRC_COLOR,
	BLEND_DEST_COLOR,
};

class RHIDevice;

class DX11BlendState
{
public:
		DX11BlendState(RHIDevice *owner, bool enabled, eBlendFactor const src_factor = BLEND_ONE, eBlendFactor const dest_factor = BLEND_ONE);
		~DX11BlendState();

		inline bool is_valid() const { return (nullptr != dx_state); }

public:
	RHIDevice* device;
	ID3D11BlendState *dx_state;
};