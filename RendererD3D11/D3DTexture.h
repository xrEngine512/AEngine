#pragma once

#include <ITexture2D.h>

struct ID3D11ShaderResourceView;
struct ID3D11Device;

class D3DTexture: public Texture2D
{
	ID3D11ShaderResourceView* m_texture;
public:
	D3DTexture(void);
	~D3DTexture(void);
	bool Initialize(const std::string&);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture();
};

