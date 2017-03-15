#include "StatisticsDisplay.h"
#include <ApproxSystemTools.h>
#include <ApproxStatistics.h>

using namespace std;

StatisticsDisplay::StatisticsDisplay()
{
    m_Text2D = new Text2D;
}


bool StatisticsDisplay::Initialize() {
    return m_Text2D->initialize(VERDANA_FONT);
}

void StatisticsDisplay::Render() {
    m_Text2D->set_text("Draw Calls: " + to_string(g_Statistics->GetDrawCalls()));
    m_Text2D->set_coord(0, 540);
    m_Text2D->render();

    m_Text2D->set_text("Shaders Created: " + to_string(g_Statistics->GetNumOfShaders()));
    m_Text2D->set_coord(0, 650);
    m_Text2D->render();
}

StatisticsDisplay::~StatisticsDisplay()
{
    DELETE_SYS_OBJECT(m_Text2D);
}
