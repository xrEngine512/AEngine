#pragma once
#include <vector>

class QString;

namespace ASL
{
	__interface ISyntaxHilighter
	{
		void AddApproxVar(const QString& var);
		void AddShaderParam(const QString& var);
		void SetApproxVars(const std::vector<QString> &vars);
		void Update();
	};
}