#ifndef SHADERPARAMETERS_H
#define SHADERPARAMETERS_H

#include <QWidget>
#include "serialization/TextureInfo.h"
#include "serialization/ShaderParameterInfo.h"

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
		void SetParameters(const QVector<ShaderParameterInfo>& params);
		void SetTextures(const QVector<TextureInfo>& textures);
		void AddTexture(const TextureInfo& texture);
		void AddParameter(const ShaderParameterInfo& param);
		const QVector<TextureInfo>& Textures()const;
		const QVector<ShaderParameterInfo>& Parameters()const;
		const QVector<ShaderSettingsElement*>& AllSettings()const;
		void ClearElements();
		~ShaderSettings();

	private:
		QVector<TextureInfo> m_textures;
		QVector<ShaderParameterInfo> m_params;

		QPushButton* m_btnAddParameter, *m_btnAddTexture;

		QVector<ShaderSettingsElement*> m_Settings;
	};
}
#endif // SHADERPARAMETERS_H
