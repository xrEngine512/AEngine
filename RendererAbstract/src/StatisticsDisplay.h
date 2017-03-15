#pragma once

#include "Text2D.h"

class StatisticsDisplay
{
    Text2D *m_Text2D;
public:
    bool Initialize();
    void Render();
    StatisticsDisplay();
    ~StatisticsDisplay();
};

