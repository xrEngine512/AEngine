#pragma once
#include <stdio.h>
#include <ShaderPackFile.h>

typedef PackElement<ApproxCommon::Memory::CopyDataOwnershipPolicy,ShaderPackElementID> ShaderElement;

class ACSReader
{
    FILE* m_file;
public:
    explicit ACSReader(const std::string& filename);
    ShaderElement NextElement();
    ~ACSReader();
};

