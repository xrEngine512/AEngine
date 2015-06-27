#pragma once

__interface IMaterialVariable
{
    void* Ptr();
    int Size()const;
    int ID()const;
    const char* Description()const;
    const char* Name()const;
    const char* Type()const;
};