#ifndef SHADERBUFFER_H
#define SHADERBUFFER_H

#include <qobject.h>
#include "Enums.h"
#include "RuntimeBufferInfo.h"
namespace ASL
{
	struct MaterialVarInfo;
	struct ShaderParamInfo;

	class ShaderBuffer : public QObject
	{
		Q_OBJECT

	public:
		ShaderBuffer(QObject *parent, VarGroup group);
		ShaderBuffer(QObject *parent, const QString& name);
		void AddVariable(int ID, const QString& type, const QString& name);
		void AddVariable(const MaterialVarInfo& var);
		void AddVariable(const ShaderParamInfo& var);
		void DeleteVariable(const MaterialVarInfo& var);
		QString ToCode()const;
		int CodeSize()const;
		const RuntimeBufferInfo& Info();
		const std::vector<const ShaderParamInfo*>& Params()const;
		static inline QString NormalizeType(const QString& type);
		~ShaderBuffer();
	private:
		inline QString VarToCode(const MaterialVarInfo& var)const;
		inline QString VarToCode(const ShaderParamInfo& var)const;
		static int SizeOf(const QString& type);

		QString m_bufferName;
		RuntimeBufferInfo m_info;

		std::vector<const MaterialVarInfo*> m_vars;
		std::vector<const ShaderParamInfo*> m_params;

		int m_bufSize;
		
	};
}
#endif // SHADERBUFFER_H
