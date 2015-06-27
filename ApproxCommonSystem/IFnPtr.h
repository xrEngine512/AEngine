#pragma once

enum ExitCode { DONE, RUNNING, SUSPENDED };

__interface IFnPtr
{
    ExitCode Call();
};