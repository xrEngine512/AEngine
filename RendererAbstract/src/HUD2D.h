#pragma once
#include "Text2D.h"

#include <ThreadSafe.h>
#include <vector>
#include <queue>
#include <memory>

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
    typedef std::weak_ptr<TextElement> TextElementRef;
    typedef std::shared_ptr<TextElement> TextElementPtr;

    std::vector<TextElementPtr> m_Texts;
    thread_safe<std::queue<TextElementPtr>> m_newTexts;

    long long m_Time;

    void UpdateTextElements();
public:
    bool Initialize();
    TextElementRef ShowMessage(const std::string& msg, float x, float y, unsigned int duration_ms = 2000, const std::string& font = "");
    void Render();
    HUD2D();
    ~HUD2D();
};

