#include "HLSLHighlighter.h"
#include <QTextCharFormat>

namespace ASL
{
	HLSLHighlighter::HLSLHighlighter(QObject *parent)
		: QSyntaxHighlighter(parent),
		m_regExpKeywords("\\b(float|float2|float3|float4|matrix|min16float|min10float|min16int|min12int|min16uint|line|point|\
					    |RWTexture1D|RWTexture1DArray|RWTexture2D|RWTexture2DArray|RWTexture3D|bool|int|\
					    |if|else|AppendStructuredBuffer|asm|asm_fragment|BlendState|break|Buffer|ByteAddressBuffer|\
						|case|cbuffer|struct|return|centroid|class|column_major|compile|compile_fragment|CompileShader|\
						|const|continue|ComputeShader|ConsumeStructuredBuffer|default|DepthStencilState|DepthStencilView|\
						|discard|do|double|DomainShader|dword|export|extern|false|for|fxgroup|GeometryShader|groupshared|\
						|half|Hullshader|in|inline|inout|InputPatch|interface|lineadj|linear|LineStream|namespace|\
						|nointerpolation|noperspective|NULL|out|OutputPatch|packoffset|pass|pixelfragment|PixelShader|\
						|PointStream|precise|RasterizerState|RenderTargetView|register|row_major|RWBuffer|RWByteAddressBuffer|\
						|RWStructuredBuffer|sample|sampler|SamplerState|SamplerComparisonState|shared|snorm|\
						|stateblock|stateblock_state|static|string|switch|StructuredBuffer|tbuffer|technique|\
						|technique10|technique11|texture|Texture1D|Texture1DArray|ITexture2D|Texture2DArray|\
						|Texture2DMS|Texture2DMSArray|Texture3D|TextureCube|TextureCubeArray|true|typedef|\
						|triangle|triangleadj|TriangleStream|uint|uniform|unorm|unsigned|vector|vertexfragment|\
						|VertexShader|void|volatile|while)\\b"),
		m_regExpComments("\s*//[^\n]*(?=\n|$)"),
		m_regExpNumLiterals("(^|\\s+|=|\\+|\\-|\\*|/)([0-9]+)(\\s+|$|;)|(^|\\s+|=|\\+|\\-|\\*|/)(([0-9]+\\.[0-9]+)|([0-9]+\\.[0-9]+f))(\\s+|$|;)"),
		m_regExpSkipSymbols("=|\\+|\\-|\\+|\\*|/|;"),
		m_regExpApproxVars(nullptr),
		m_regExpShaderParams(nullptr)
	{
		m_colorScheme.comments		=	QColor(20, 200, 20);
		m_colorScheme.keywords		=	QColor(60, 150, 255);
		m_colorScheme.numLiterals	=	QColor(160, 100, 20);
		m_colorScheme.defChars		=	QColor(180, 180, 180);
		m_colorScheme.approxVar		=	QColor(0, 213, 226);
		m_colorScheme.shaderParams	=	QColor(225,132,0);
	}

	HLSLHighlighter::~HLSLHighlighter()
	{
		if (m_regExpApproxVars)
		{
			delete m_regExpApproxVars;
		}
	}

	void HLSLHighlighter::SetApproxVars(const std::vector<QString>& vars)
	{
		if (!m_regExpApproxVars)
		{
			QString pattern("\\b(");
			for (auto var : vars)
			{
				pattern += var + '|';
			}
			pattern.chop(1);
			pattern += ")\\b";
			m_regExpApproxVars = new QRegExp(pattern);
		}
	}

	void HLSLHighlighter::AddApproxVar(const QString& var)
	{
		if (m_regExpApproxVars)
		{
			auto pattern = m_regExpApproxVars->pattern();
			pattern.chop(3);
			pattern += '|' + var;
			pattern += ")\\b";
			m_regExpApproxVars->setPattern(pattern);
		}
		else
		{
			QString pattern("\\b(");
			pattern += var;
			pattern += ")\\b";
			m_regExpApproxVars = new QRegExp(pattern);
		}
	}

	void HLSLHighlighter::AddShaderParam(const QString& var)
	{
		if (m_regExpShaderParams)
		{
			auto pattern = m_regExpShaderParams->pattern();
			pattern.chop(3);
			pattern += '|' + var;
			pattern += ")\\b";
			m_regExpShaderParams->setPattern(pattern);
		}
		else
		{
			QString pattern("\\b(");
			pattern += var;
			pattern += ")\\b";
			m_regExpShaderParams = new QRegExp(pattern);
		}
	}

	void HLSLHighlighter::Update()
	{
		rehighlight();
	}

	void HLSLHighlighter::highlightBlock(const QString& text)
	{
		Highlight(m_regExpKeywords, text, m_colorScheme.keywords);

		Highlight(m_regExpNumLiterals, text, m_colorScheme.numLiterals);
		Highlight(m_regExpSkipSymbols, text, m_colorScheme.defChars);

		Highlight(m_regExpComments, text, m_colorScheme.comments);

		if (m_regExpApproxVars)
		{
			Highlight(*m_regExpApproxVars, text, m_colorScheme.approxVar);
		}
		if (m_regExpShaderParams)
		{
			Highlight(*m_regExpShaderParams, text, m_colorScheme.shaderParams);
		}
	}

	void HLSLHighlighter::Highlight(const QRegExp& expr, const QString& code, const QColor& color)
	{
		QTextCharFormat format;

		if (!expr.isValid() || expr.isEmpty() || expr.exactMatch(""))
		{
			setFormat(0, code.length(), format);
		}
		format.setForeground(color);
		int index = expr.indexIn(code);
		while (index >= 0)
		{
			int len = expr.matchedLength();
			setFormat(index, len, format);
			index = expr.indexIn(code, index + len);
		}
	}
}