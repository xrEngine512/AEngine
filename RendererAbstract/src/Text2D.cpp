#include "Text2D.h"

#include <ITextEngine.h>
#include <IObjectFactory.h>

using namespace std;

Text2D::Text2D() : m_PosX(0), m_PosY(0)
{
}

bool Text2D::initialize(const string &font)
{
    return true;
}

void Text2D::set_text(const string &text)
{
    set_text(move(string(text)));
}

void Text2D::update_text()
{
    if (auto lock = m_newText.try_lock())
        m_Text = move(m_newText);
}

void Text2D::shutdown()
{
}

void Text2D::set_coord(float x, float y)
{
    m_PosX.store(x);
    m_PosY.store(y);
}

void Text2D::render()
{
    if (!m_Text.empty())
        getObjectFactory()->getTextEngine()->draw_string(m_Text, {m_PosX.load(), m_PosY.load()});

    update_text();
}

Text2D::~Text2D()
{
}

void Text2D::set_text(std::string &&text) {
    m_newText = move(text);
}
