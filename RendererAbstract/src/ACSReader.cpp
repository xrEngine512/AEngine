#include "ACSReader.h"


ACSReader::ACSReader(const std::string& filename)
{
    m_file = OS::Files::open_file(filename, "rb");
}

ShaderElement ACSReader::NextElement()
{    
    if (!m_file || feof(m_file) != 0)
        return ShaderElement();
    return ShaderElement(m_file);
}

ACSReader::~ACSReader()
{
    if (m_file)
	    fclose(m_file);
}
