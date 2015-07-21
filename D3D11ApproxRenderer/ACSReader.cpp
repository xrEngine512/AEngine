#include "ACSReader.h"


ACSReader::ACSReader(const wchar_t* filename)
{
    auto err = _wfopen_s(&m_file,filename, L"rb");
}

ShaderElement ACSReader::NextElement()
{    
    if (feof(m_file) != 0)
        return ShaderElement();
    return ShaderElement(m_file);
}

ACSReader::~ACSReader()
{
	fclose(m_file);
}
