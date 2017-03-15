#include "HUD2D.h"

#include <OS.h>

using namespace std;

HUD2D::HUD2D(): m_Time(0)
{
}

bool HUD2D::Initialize()
{
    return true;
}

HUD2D::TextElementRef HUD2D::ShowMessage(const string& msg, float x, float y, unsigned int duration_ms, const string& font)
{
    auto newText = make_shared<TextElement>();
    if(font.empty())
        newText->object.initialize();
    else
        newText->object.initialize(font);
    newText->object.set_coord(x, y);
    newText->object.set_text(msg);
    newText->lifetime = duration_ms;
    {
        auto lock = m_newTexts.lock();
        m_newTexts.push(newText);
    }
    return newText;
}

void HUD2D::UpdateTextElements()
{
    if(auto lock = m_newTexts.try_lock()) {
        while (!m_newTexts.empty()) {
            m_Texts.push_back(m_newTexts.front());
            m_newTexts.pop();
        }
    }

    for (auto element = m_Texts.begin(); element != m_Texts.end(); )
    {
        if ((*element)->destroy)
            element = m_Texts.erase(element);
        else
            element++;
    }
}

void HUD2D::Render()
{
    if (!m_Time)
        m_Time = OS::Time::count_from();

    for (auto element : m_Texts)
    {
        if (!element->destroy)
        {
            if (element->visible)
            {
                if (element->lifetime > 0)
                {
                    element->object.render();
                    element->lifetime -= OS::Time::count_to(m_Time);

                    if (element->lifetime <= 0)
                    {
                        element->lifetime = 0;
                        element->destroy = true;
                    }
                }
                else if (element->lifetime == -1)
                    element->object.render();
            }
        }
    }
    UpdateTextElements();
    m_Time = OS::Time::count_from();
}

HUD2D::~HUD2D()
{
}
