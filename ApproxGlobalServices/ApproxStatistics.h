#pragma once
#define g_Statistics getStatistics()
class __declspec(dllexport) Statistics
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

__declspec(dllexport) Statistics* getStatistics();