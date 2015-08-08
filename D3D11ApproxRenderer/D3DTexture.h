#pragma once

struct ID3D11ShaderResourceView;
struct ID3D11Device;

class D3DTexture
{
	ID3D11ShaderResourceView* m_texture;
public:
	D3DTexture(void);
	~D3DTexture(void);
	bool Initialize(ID3D11Device*, const wchar_t*);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture();
};

