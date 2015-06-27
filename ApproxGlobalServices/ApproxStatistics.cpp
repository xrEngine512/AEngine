#include "ApproxStatistics.h"


Statistics::Statistics() :m_DrawCallsCount(0), m_NumberOfShaders(0)
{
}

void Statistics::IncDrawCalls()
{
    m_DrawCallsCount++;
}

Statistics* const g_pInstance = Statistics::Instance();

void Statistics::IncNumOfShaders()
{
    m_NumberOfShaders++;
}

void Statistics::ResDrawCalls()
{
    m_DrawCallsCount = 0;
}

unsigned int Statistics::GetDrawCalls()const
{
    return m_DrawCallsCount;
}

unsigned int Statistics::GetNumOfShaders()const
{
    return m_NumberOfShaders;
}

Statistics* Statistics::Instance()
{ 
	static Statistics instance;
	return &instance;
}

Statistics::~Statistics()
{
}

Statistics* getStatistics()
{
	return g_pInstance;
}