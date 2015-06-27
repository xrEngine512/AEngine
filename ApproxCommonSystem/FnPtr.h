#pragma once

#include "IFnPtr.h"

template<class FuncClass>

class FnPtr : public IFnPtr
{
    ExitCode(FuncClass::*m_FnPTR)();
    FuncClass* m_Object;
public:
    FnPtr(ExitCode(FuncClass::*Fn)(), FuncClass* Object)
    {
        m_FnPTR = Fn;
        m_Object = Object;
    }
    ExitCode Call()override
    {
        return (m_Object->*m_FnPTR)();
    }
    virtual ~FnPtr(){};
};

