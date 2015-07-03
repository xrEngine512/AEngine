#ifndef SHADERBUFFER_H
#define SHADERBUFFER_H

#include <QObject>
#include "Enums.h"

namespace ASL
{
	struct MaterialVarInfo;

	class ShaderBuffer : public QObject
	{
		Q_OBJECT

	public:
		ShaderBuffer(QObject *parent, VarGroup group);
		ShaderBuffer(QObject *parent, const QString& name);
		void AddVariable(int ID, const QString& type, const QString& name);
		void AddVariable(const MaterialVarInfo& var);
		QString ToCode();
		int CodeSize()const;
		static inline QString NormalizeType(const QString& type);
		~ShaderBuffer();

	private:
		QString m_code;
		VarGroup m_group;
		std::vector<int> m_IDs;
	};
}
#endif // SHADERBUFFER_H
