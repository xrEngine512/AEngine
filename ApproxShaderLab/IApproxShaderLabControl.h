#pragma once
class QWidget;

namespace ASL
{
	__interface IApproxShaderLabControl
	{
		void Run();
		void Shutdown();
		void SetParentWidget(QWidget*);
	};

	extern __declspec(dllexport) IApproxShaderLabControl* GetASLControl();
}