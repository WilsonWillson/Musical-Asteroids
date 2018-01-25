#include "Engine/DX11/DX11Texture2D.hpp"



//-------------------------Constructors/Decountructors------------------------
DX11Texture2D::DX11Texture2D(RHIDevice *owner)
	: m_device(owner),
	 m_dxResource(nullptr),
	 m_dxRtv(nullptr),
	m_dxSrv(nullptr),
	m_dxDsv(nullptr),
	 m_width(0),
	 m_height(0)
{}

DX11Texture2D::DX11Texture2D(RHIDevice *rhiDevice, ID3D11Texture2D *texture)
{
	m_device = rhiDevice;
    m_dxResource = texture;
	m_dxRtv = nullptr;
	m_dxDsv = nullptr;
	m_dxSrv = nullptr;
	if (texture != nullptr) 
	{
		texture->AddRef();
		m_dxResource = texture;

		D3D11_TEXTURE2D_DESC textureDescription;
		texture->GetDesc(&textureDescription);

		m_width = textureDescription.Width;
		m_height = textureDescription.Height;

		ID3D11Device *dd = rhiDevice->m_dx11Device;
		dd->CreateRenderTargetView(m_dxResource, nullptr, &m_dxRtv);
	}
}

// specialized for the output's texture
DX11Texture2D::DX11Texture2D(RHIDevice *owner, RHIOutput *output)
	: DX11Texture2D(owner)
{
	m_dxDsv = nullptr;
	if (output != nullptr) 
	{
		ID3D11Texture2D *back_buffer = nullptr;
		output->m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);

		if (nullptr != back_buffer) 
		{
			m_dxResource = back_buffer;

			D3D11_TEXTURE2D_DESC textureDescription;
			m_dxResource->GetDesc(&textureDescription);

			// Set flags for backbuffer texture
			m_width = textureDescription.Width;
			m_height = textureDescription.Height;
			m_dxBindFlags = textureDescription.BindFlags;

			createViews();
		}
	}
}


DX11Texture2D::DX11Texture2D(RHIDevice *owner, char const *filename)
	: DX11Texture2D(owner)
{
	m_dxDsv = nullptr;
	Image image;
	if (!image.LoadFromFile(filename))
	{
		return;
	}

	LoadFromImage(image, false);
}



DX11Texture2D::DX11Texture2D(RHIDevice *owner, char const *filename, bool isUAVTexture)
	: DX11Texture2D(owner)
{
	m_dxDsv = nullptr;
	Image image;
	if (!image.LoadFromFile(filename))
	{
		return;
	}

	 LoadFromImage( image, isUAVTexture );
}

DX11Texture2D::DX11Texture2D(RHIDevice *owner, Image const &img)
	: DX11Texture2D(owner)
{
	m_dxDsv = nullptr;
	LoadFromImage(img, false);
}

DX11Texture2D::DX11Texture2D(RHIDevice *owner, Image const &img, bool isUAVTexture)
	: DX11Texture2D(owner)
{
	m_dxDsv = nullptr;
	LoadFromImage(img, isUAVTexture);
}
//------------------------------------------------------------------------
DX11Texture2D::DX11Texture2D(RHIDevice *owner, unsigned int w, unsigned int h, eImageFormat format)
	: DX11Texture2D(owner)
{
	// default usage - this is going to be written to by the GPU
	D3D11_USAGE usage = D3D11_USAGE_DEFAULT;
	m_width = w;
	m_height = h;

	DXGI_FORMAT dx_format;
	unsigned int dx_bindings = 0U;
	switch (format)
	{
	case IMAGEFORMAT_RGBA8:
		dx_format = DXGI_FORMAT_R8G8B8A8_UNORM;
		dx_bindings = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		break;
	case IMAGEFORMAT_D24S8:
		// depth textures are 24-bit depth, 8-bit stencil
		dx_format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dx_bindings = D3D11_BIND_DEPTH_STENCIL; // binding this to as a shader resource requires a couple extra steps - saved for later
		break;
	default:
		// ASSERTORDIE
		return;
	};

	D3D11_TEXTURE2D_DESC tex_desc;
	memset(&tex_desc, 0, sizeof(tex_desc));

	tex_desc.Width = m_width;
	tex_desc.Height = m_height;
	tex_desc.MipLevels = 1;                // no mip mapping
	tex_desc.ArraySize = 1;                // NO ARRAYS!  
	tex_desc.Usage = usage;
	tex_desc.Format = dx_format;
	tex_desc.BindFlags = dx_bindings;
	tex_desc.CPUAccessFlags = 0U;
	tex_desc.MiscFlags = 0U; // there is one for generating mip maps.

							 // multisampling options
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;

	// no initial data - we're creating renderable targets

	ID3D11Device *dx_device = m_device->m_dx11Device;
	HRESULT hr = dx_device->CreateTexture2D(&tex_desc, nullptr, &m_dxResource);

	if (SUCCEEDED(hr)) 
	{
		m_dxBindFlags = dx_bindings;
		createViews();
	}
}



DX11Texture2D::~DX11Texture2D()
{
	if(m_dxSrv != nullptr)
		DX_SAFE_RELEASE(m_dxSrv);
	if(m_dxResource != nullptr)
		DX_SAFE_RELEASE(m_dxResource);
	if(m_dxDsv != nullptr)
		DX_SAFE_RELEASE(m_dxDsv);
	if (m_dxRtv != nullptr)
		DX_SAFE_RELEASE(m_dxRtv);
}

//--------------------------Creating and Loading------------------------------------------------------
void DX11Texture2D::createViews()
{
	ID3D11Device *idOfDx11Device = m_device->m_dx11Device;

	if (m_dxBindFlags & D3D11_BIND_RENDER_TARGET)
	{
		idOfDx11Device->CreateRenderTargetView(m_dxResource, nullptr, &m_dxRtv);
	}

	if (m_dxBindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		idOfDx11Device->CreateShaderResourceView(m_dxResource, nullptr, &m_dxSrv);
	}

	if (m_dxBindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		idOfDx11Device->CreateDepthStencilView(m_dxResource, nullptr, &m_dxDsv);
	}
	if (m_dxBindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		//HRESULT result = dd->CreateUnorderedAccessView(m_dxResource, &desc, &m_dxUav);
	}

}


bool DX11Texture2D::LoadFromImage(Image const &image, bool isUAVTexture)
{
	D3D11_USAGE usage = D3D11_USAGE_DEFAULT;

	D3D11_TEXTURE2D_DESC textureDescription;
	memset(&textureDescription, 0, sizeof(textureDescription));
	
	textureDescription.Width = image.GetWidth();
	textureDescription.Height = image.GetHeight();
	textureDescription.MipLevels = 1;                // no mip mapping
	textureDescription.ArraySize = 1;                // NO ARRAYS!  
	textureDescription.Usage = usage;
	textureDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	textureDescription.CPUAccessFlags = 0U;
	textureDescription.MiscFlags = 0U; // there is one for generating mip maps.
	textureDescription.SampleDesc.Count = 1;// multisampling options
	textureDescription.SampleDesc.Quality = 0;

	// Helps us figure out what views to make
	m_dxBindFlags = D3D11_BIND_SHADER_RESOURCE;

	// Setup initial data
	D3D11_SUBRESOURCE_DATA d3d11Data;
	memset(&d3d11Data, 0, sizeof(d3d11Data));
	d3d11Data.pSysMem = image.m_buffer;
	d3d11Data.SysMemPitch = image.m_bpp * textureDescription.Width;

	ID3D11Device *dx_device = m_device->m_dx11Device;
	HRESULT hr = dx_device->CreateTexture2D(&textureDescription, &d3d11Data, &m_dxResource);

	//Does UAVTexture
	if (isUAVTexture == true)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
		MemZero(&desc);

		desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;;
		desc.Texture2D.MipSlice = 0;

		// NOTE:  Depth Stencil Buffers need to have a different view
		// then their normal 
		DXGI_FORMAT dx_format;
		dx_format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//desc.Format = DXFormatFromImageFormat(dx_format);
		dx_device->CreateUnorderedAccessView(m_dxResource, &desc, &m_dxUav);
	}

	if (SUCCEEDED(hr))
	{
		createViews();
		return true;
	}
	else 
		return false;
}

//----------------------------Getters---------------------------------

unsigned int DX11Texture2D::getWidth()
{
	return m_width;
}

unsigned int DX11Texture2D::getHeight()
{
	return m_height;
}


//------------------------Setters--------------------------------------

void DX11Texture2D::setWidth(unsigned int width)
{
	m_width = width;
}

void DX11Texture2D::setHeight(unsigned int height)
{
	m_height = height;
}