#pragma once
#include <vector>
using namespace std;

typedef unsigned char byte;

class RGS
{
    vector<void*> m_ptrs, m_ptrsToCopy;
    vector<int> m_sizes, m_shifts;
    byte* m_Data;
    int m_size;
public:
    RGS(vector<string> types,vector<void*> ptrs);
	void operator =(void*);
    void Update();
    int Size();
    byte* Data();
    ~RGS();
};

