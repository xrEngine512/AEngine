#include "ACSReader.h"


ACSReader::ACSReader(const std::string& filename)
{
    m_file = OS::Files::open_file(filename, "rb");
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
