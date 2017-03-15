#pragma once
#define g_Statistics getStatistics()
class APPROX_API Statistics
{
    unsigned int m_DrawCallsCount, m_NumberOfShaders;
public:
    void IncDrawCalls();
    void IncNumOfShaders();
    void ResDrawCalls();
    unsigned int GetDrawCalls() const;
    unsigned int GetNumOfShaders() const;
	static  Statistics* Instance();
private:
    Statistics();
    ~Statistics();
};

APPROX_API Statistics* getStatistics();