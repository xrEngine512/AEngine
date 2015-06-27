#pragma once
class QWidget;

__interface IApproxShaderLabControl
{
    void Run();
    void Shutdown();
    void SetParentWidget(QWidget*);
};

extern __declspec(dllexport) IApproxShaderLabControl* GetASLControl();