#pragma once
#include <d3d11.h>

class D3DTexture
{
	ID3D11ShaderResourceView* m_texture;
public:
	D3DTexture(void);
	~D3DTexture(void);
	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture();
};

