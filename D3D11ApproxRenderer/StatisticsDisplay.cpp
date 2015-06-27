#include "StatisticsDisplay.h"
#include <ApproxSystemTools.h>
#include <ApproxStatistics.h>

StatisticsDisplay::StatisticsDisplay()
{
    m_Text2D = new Text2D;
}


bool StatisticsDisplay::Initialize(ID3D11Device* device)
{
    if (!device)
        return false;
    m_Device = device;
    device->GetImmediateContext(&m_DeviceContext);
    if (!m_Text2D->Initialize(m_Device, m_DeviceContext, VERDANA_FONT))
        return false;
    return true;
}

void StatisticsDisplay::Render()
{
    if ( 1!=0 )
    {
        wchar_t outBuf[8];
        _itow_s(g_Statistics->GetDrawCalls(), outBuf,10);
        wstring out = wstring(L"Draw Calls: ") + wstring(outBuf);

        m_Text2D->SetText(const_cast<wchar_t*>( out.c_str()));
        m_Text2D->SetCoord(0, 540);
        m_Text2D->Render();

        _itow_s(g_Statistics->GetNumOfShaders(), outBuf, 10);
        out = wstring(L"Shaders Created: " + wstring(outBuf));

        m_Text2D->SetText(const_cast<wchar_t*>(out.c_str()));
        m_Text2D->SetCoord(0, 650);
        m_Text2D->Render();
    }
}

StatisticsDisplay::~StatisticsDisplay()
{
    DELETE_SYS_OBJECT(m_Text2D);
}
