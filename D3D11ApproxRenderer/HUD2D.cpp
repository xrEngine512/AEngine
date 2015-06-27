#include "HUD2D.h"
//#define WIN32_LEAN_AND_MEAN


HUD2D::HUD2D() :m_Time(0)
{
}

bool HUD2D::Initialize(ID3D11Device* device)
{
    if (!device)
        return false;
    m_Device = device;
    device->GetImmediateContext(&m_DeviceContext);
    return true;
}

void HUD2D::ShowMessage(wchar_t* msg, float x, float y, unsigned int duration_ms, wchar_t* font)
{
    TextElement* newText = new TextElement;
    newText->object.Initialize(m_Device, m_DeviceContext, font);
    newText->object.SetCoord(x, y);
    newText->object.SetText(msg);
    newText->lifetime = duration_ms;
    m_MutexTexts.lock();
    m_newTexts.push(newText);
    m_MutexTexts.unlock();
}

void HUD2D::UpdateTextElements()
{
    if (!m_newTexts.empty())
    {
        if (m_MutexTexts.try_lock())
        {
            while (!m_newTexts.empty())
            {
                m_Texts.push_back(m_newTexts.front());
                m_newTexts.pop();
            }
            m_MutexTexts.unlock();
        }
    }
    for (auto element = m_Texts.begin(); element != m_Texts.end(); )
    {
        if ((*element)->destroy)
        {
            delete *element;
            element = m_Texts.erase(element);
        }
        else
        {
            element++;
        }
    }
}

void HUD2D::Render()
{
    if (m_FirstFrame)
    {
        m_FirstFrame = false;
        m_Time = timeGetTime();
    }


    for each (TextElement* element in m_Texts)
    {
        if (!element->destroy)
        {
            if (element->visible)
            {
                if (element->lifetime > 0)
                {
                    element->object.Render();
                    if (element->lifetime <= timeGetTime() - m_Time)
                    {
                        element->lifetime = 0;
                        element->destroy = true;
                    }
                    else
                    {
                        element->lifetime -= timeGetTime() - m_Time;
                    }
                }
                else if (element->lifetime == -1)
                {
                    element->object.Render();
                }
            }
        }

    }
    UpdateTextElements();
    m_Time = timeGetTime();
}

HUD2D::~HUD2D()
{
}
