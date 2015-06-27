#include "Text2D.h"


Text2D::Text2D() : m_spriteFont(nullptr), m_spriteBatch(nullptr), m_Text(nullptr), m_newText(nullptr), m_PosX(0), m_PosY(0)
{
}

bool Text2D::Initialize(ID3D11Device* device, ID3D11DeviceContext* devContext, wchar_t* font )
{
    m_spriteBatch = new SpriteBatch(devContext);
    m_spriteFont = new SpriteFont(device, font);
    return true;
}

void Text2D::SetText(wchar_t* text)
{
    int len = wcslen(text);
    m_Mutex.lock();
    m_newText = new wchar_t[++len];
    wcscpy_s(m_newText,len, text);
    m_Mutex.unlock();
}

void Text2D::UpdateText()
{
    if (m_Mutex.try_lock())
    {
        if (m_newText)
        {
            delete m_Text;
            m_Text = m_newText;
            m_newText = nullptr;
        }
        m_Mutex.unlock();
    }
}

void Text2D::Shutdown()
{
    if (m_spriteBatch)
    {
        delete m_spriteBatch;
        m_spriteBatch = nullptr;
    }

    if (m_spriteFont)
    {
        delete m_spriteFont;
        m_spriteFont = nullptr;
    }
}

void Text2D::SetCoord(float x, float y)
{
    m_PosX.store(x);
    m_PosY.store(y);
}

void Text2D::Render()
{
    if (m_Text)
    {
        m_spriteBatch->Begin();
        m_spriteFont->DrawString(m_spriteBatch, m_Text,XMFLOAT2(m_PosX.load(), m_PosY.load()));
        m_spriteBatch->End();
    }
    UpdateText();
}

Text2D::~Text2D()
{
}
