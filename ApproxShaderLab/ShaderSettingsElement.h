#ifndef SHADERSETTINGSELEMENT_H
#define SHADERSETTINGSELEMENT_H

#include <QWidget>

class QLabel;

namespace ASL
{
	struct AbstractSaveData;
	class LinkingPoint;
	struct ShaderParamInfo;
	struct TextureInfo;

	enum Elem_Type{ PARAMETER, TEXTURE };

	class ShaderSettingsElement : public QWidget
	{
		Q_OBJECT

	public:
		ShaderSettingsElement(QWidget *parent, const ShaderParamInfo& info);
		ShaderSettingsElement(QWidget *parent, const TextureInfo& info);
		LinkingPoint& Point()const;
		~ShaderSettingsElement();
		QString ToCode()const;
		int CodeSize()const;
		AbstractSaveData* Info()const;
	signals:
		void linkAttempt(ShaderSettingsElement* sender, const QPoint& gPos);

	private:
		ShaderSettingsElement(QWidget *parent);
		QLabel* m_Name;
		QLabel* m_Icon;
		LinkingPoint* m_Point;
		AbstractSaveData* m_Data;
		Elem_Type m_Type;
	};
}
#endif // SHADERSETTINGSELEMENT_H
