#pragma once
#include <QtCore/QString>
#include "Enums.h"

namespace ASL
{
	struct MaterialVarInfo
	{
		QString name;
		QString type;
		QString description;
		int ID;
		VarGroup group;
	};
}