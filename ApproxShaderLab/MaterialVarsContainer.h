#ifndef APPROXSCENEVARS_H
#define APPROXSCENEVARS_H

#include <QFrame>
#include <vector>
#include "MaterialVarInfo.h"
using namespace std;
namespace ASL
{
	class MaterialVar;
	class MaterialVarPoint;

	class MaterialVarsContainer : public QFrame
	{
		Q_OBJECT

	public:
		MaterialVarsContainer(QWidget *parent, std::vector<MaterialVarInfo>const&);
		~MaterialVarsContainer();
	signals:
		void linkAttempt(MaterialVar* sender, const QPoint& mouseGlobalPosEnd);
	private:
		vector < MaterialVar* >  m_vars;
	};
}
#endif // APPROXSCENEVARS_H
