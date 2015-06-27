#pragma once
#include <SpriteFont.h>
#include <mutex>
#include <atomic>
#define VERDANA_FONT L"../../Core/Engine/Resources/Fonts/verdana.spritefont"
using namespace DirectX;
using namespace std;

class Text2D
{
    SpriteFont *m_spriteFont;
    SpriteBatch *m_spriteBatch;
    wchar_t *m_Text, *m_newText;
    mutex m_Mutex;
    atomic<float> m_PosX, m_PosY;

    void UpdateText();
public:
    bool Initialize(ID3D11Device* , ID3D11DeviceContext*, wchar_t* font);
    void SetText(wchar_t*);
    void SetCoord(float x, float y);
    void Shutdown();
    void Render();
    Text2D();
    ~Text2D();
};

