#pragma once
#include <vector>

class QString;

__interface ISyntaxHilighter
{
	void SetApproxVars(const std::vector<std::string> &vars);
	void SetApproxVars(const std::vector<QString> &vars);
};