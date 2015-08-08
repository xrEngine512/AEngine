#include <DDSTextureLoader.h>
#include "D3DTexture.h"
#include <ApproxSystemTools.h>

using namespace DirectX;

D3DTexture::D3DTexture(void):m_texture(nullptr)
{
}

bool D3DTexture::Initialize(ID3D11Device *device, const wchar_t* filename)
{
	HRESULT result;

	// Load the texture in.
	result = CreateDDSTextureFromFile(device,filename,NULL,&m_texture);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void D3DTexture::Shutdown()
{
	// Release the texture resource.
	DELETE_D3D11_OBJECT(m_texture)

	return;
}

ID3D11ShaderResourceView* D3DTexture::GetTexture()
{
	return m_texture;
}


D3DTexture::~D3DTexture(void)
{
}
