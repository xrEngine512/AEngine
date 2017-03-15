#pragma once
#include <vector>

class QString;

namespace ASL
{
	interface ISyntaxHighlighter
	{
		virtual void AddApproxVar(const QString& var) = 0;
		virtual void AddShaderParam(const QString& var) = 0;
		virtual void SetApproxVars(const std::vector<QString> &vars) = 0;
		virtual void Update() = 0;
		virtual ~ISyntaxHighlighter(){};
	};
}