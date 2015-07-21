#pragma once
#include <vector>
using namespace std;

typedef unsigned char byte;

const int struct_alignment = 16;
const int memory_alignment = 16;
class RGS
{
    vector<void*> m_ptrsToCopy;
	vector<int> m_sizes, m_shifts;
    byte* m_Data;
    int m_size;
public:
	explicit RGS(const pair<vector<void*> ,vector<string>>& data);
    RGS(const vector<void*>& ptrs, const vector<string>& types);
	void operator =(void*);
    void Update()const;
    int Size()const;
    byte* Data()const;
    ~RGS();
};

