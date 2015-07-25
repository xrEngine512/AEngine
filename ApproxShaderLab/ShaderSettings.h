#ifndef SHADERPARAMETERS_H
#define SHADERPARAMETERS_H

#include <QWidget>
#include "TextureInfo.h"
#include "ShaderParamInfo.h"

class QPushButton;

namespace ASL
{
	class ShaderSettingsElement;

	class ShaderSettings : public QWidget
	{
		Q_OBJECT
	private slots:
		void AddTexture();
		void AddParameter();
	signals:
		void linkAttempt(ShaderSettingsElement* sender, const QPoint& mouseGlobalPosEnd);
	public:
		ShaderSettings(QWidget *parent);
		const QVector<TextureInfo>& Textures()const;
		const QVector<ShaderParamInfo>& Parameters()const;
		~ShaderSettings();

	private:
		QVector<TextureInfo> m_textures;
		QVector<ShaderParamInfo> m_params;

		QPushButton* m_btnAddParameter, *m_btnAddTexture;

		QVector<ShaderSettingsElement*> m_Settings;
	};
}
#endif // SHADERPARAMETERS_H
