#pragma once
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/Core/Image.hpp"
#include "Engine/RHI/RHIOutput.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/Core/Image.hpp"
#include "Engine/DX11/DX11.hpp"
#include <cstdint>
#include "stdint.h"
#include <d3d11.h>
#include <d3d10.h>
#include <DXGI.h>

class RHIDevice;
class RHIOutput;

class DX11Texture2D
{
public:
	DX11Texture2D(RHIDevice *device);
	DX11Texture2D(RHIDevice *device, ID3D11Texture2D *tex);
	DX11Texture2D(RHIDevice *device, RHIOutput *output);
	DX11Texture2D(RHIDevice *device, char const *filename);
	DX11Texture2D(RHIDevice *owner, char const *filename, bool isUAVTexture);
	DX11Texture2D(RHIDevice *device, Image const &image);
	DX11Texture2D(RHIDevice *owner, Image const &img, bool isUAVTexture);
	DX11Texture2D(RHIDevice *device, unsigned int width, unsigned int height, eImageFormat format);
	~DX11Texture2D();

	unsigned int getWidth();
	unsigned int getHeight();

	void setWidth(unsigned int targetWidth);
	void setHeight(unsigned int targetHeight);

	inline bool isValid() const { return (m_dxResource != nullptr); }
	inline bool isRenderTarget() const { return (m_dxRtv != nullptr); }

private:
	void createViews();
	bool LoadFromImage(Image const &image, bool isUAVTexture);

public:
	ID3D11Texture2D *m_dxResource;
	ID3D11RenderTargetView *m_dxRtv;
	ID3D11ShaderResourceView *m_dxSrv;
	ID3D11DepthStencilView *m_dxDsv;
	ID3D11UnorderedAccessView* m_dxUav;

private:
	RHIDevice *m_device;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_dxBindFlags;
};
