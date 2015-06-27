#pragma once

#include "Text2D.h"

class StatisticsDisplay
{
    Text2D *m_Text2D;
    ID3D11Device* m_Device;
    ID3D11DeviceContext* m_DeviceContext;
public:
    bool Initialize(ID3D11Device*);
    void Render();
    StatisticsDisplay();
    ~StatisticsDisplay();
};

