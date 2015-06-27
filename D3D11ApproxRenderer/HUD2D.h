#pragma once
#include "Text2D.h"
#include <vector>
#include <queue>

class HUD2D
{
    template <class ElementType>
    struct Element
    {
        ElementType object;
        bool visible = true, destroy = false;
        int lifetime=-1;
    };
    typedef Element<Text2D> TextElement;
    typedef Element<SpriteBatch> SpriteElement;

    mutex m_MutexTexts, m_MutexSprites;
    vector<TextElement*> m_Texts;
    queue<TextElement*> m_newTexts;
    vector<SpriteElement*> m_Sprites;
    queue<SpriteElement*> m_newSprites;
    ID3D11Device* m_Device;
    ID3D11DeviceContext* m_DeviceContext;
    unsigned int m_Time;
    bool m_FirstFrame = true;

    void UpdateTextElements();
public:
    bool Initialize(ID3D11Device*);
    void ShowMessage(wchar_t* msg, float x, float y, unsigned int duration_ms = 2000, wchar_t* font = VERDANA_FONT);
    void Render();
    HUD2D();
    ~HUD2D();
};

