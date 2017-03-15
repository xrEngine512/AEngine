#pragma once
class QWidget;

namespace ASL
{
	interface IApproxShaderLabControl
	{
		virtual void Run() = 0;
        virtual void Shutdown() = 0;
        virtual void SetParentWidget(QWidget*) = 0;
        virtual ~IApproxShaderLabControl(){};
    };

	APPROX_API IApproxShaderLabControl* GetASLControl();
}