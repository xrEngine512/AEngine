#ifndef APPROXSCENEVARS_H
#define APPROXSCENEVARS_H

#include <QFrame>
#include <vector>
#include "MaterialVarInfo.h"
using namespace std;

class QLabel;

namespace ASL
{
	class MaterialVar;
	class LinkingPoint;

	class MaterialVarsContainer : public QFrame
	{
		Q_OBJECT

	public:
		MaterialVarsContainer(QWidget *parent, std::vector<MaterialVarInfo>const&, const QString& version);
		const vector < MaterialVar* >& Vars()const;
		~MaterialVarsContainer();
	signals:
		void linkAttempt(MaterialVar* sender, const QPoint& mouseGlobalPosEnd);
	private:
		QLabel *m_title;
		vector < MaterialVar* >  m_vars;
	};
}
#endif // APPROXSCENEVARS_H
