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
		void SetParameters(const QVector<ShaderParamInfo>& params);
		void SetTextures(const QVector<TextureInfo>& textures);
		void AddTexture(const TextureInfo& texture);
		void AddParameter(const ShaderParamInfo& param);
		const QVector<TextureInfo>& Textures()const;
		const QVector<ShaderParamInfo>& Parameters()const;
		const QVector<ShaderSettingsElement*>& AllSettings()const;
		~ShaderSettings();

	private:
		QVector<TextureInfo> m_textures;
		QVector<ShaderParamInfo> m_params;

		QPushButton* m_btnAddParameter, *m_btnAddTexture;

		QVector<ShaderSettingsElement*> m_Settings;
	};
}
#endif // SHADERPARAMETERS_H
