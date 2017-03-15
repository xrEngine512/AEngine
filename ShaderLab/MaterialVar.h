#ifndef APPROXSCENEVAR_H
#define APPROXSCENEVAR_H

#include <QWidget>
class QLabel;
namespace ASL
{
	class LinkingPoint;
	struct MaterialVarInfo;

	class MaterialVar : public QWidget
	{
		Q_OBJECT

		const MaterialVarInfo& m_info;
	signals:
		void linkAttempt(MaterialVar*, const QPoint& mouseGlobalPos);
	public:
		MaterialVar(QWidget *parent, const MaterialVarInfo& info);
		~MaterialVar();
		LinkingPoint& Point()const;
		const MaterialVarInfo& VarInfo()const;
	private:
		QLabel* m_flag, *m_name;
		LinkingPoint *m_point;

		int m_refCount;
	};
}
#endif // APPROXSCENEVAR_H
