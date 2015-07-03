#ifndef HLSLHIGHLIGHTER_H
#define HLSLHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include "ISyntaxHilighter.h"
#include <qregexp.h>

namespace ASL
{
	class HLSLHighlighter : public QSyntaxHighlighter, public ISyntaxHilighter
	{
		Q_OBJECT

	public:
		struct HighlightColorScheme
		{
			QColor keywords;
			QColor comments;
			QColor numLiterals;
			QColor defChars;
			QColor approxVar;
		};
		HLSLHighlighter(QObject *parent);
		~HLSLHighlighter();
		void highlightBlock(const QString& text)override;
		void SetApproxVars(const std::vector<QString>& vars)override final;
		void AddApproxVar(const QString& var)override final;
		void Update()override final;
	private:
		QRegExp m_regExpKeywords,
			m_regExpNumLiterals, m_regExpComments, m_regExpSkipSymbols, *m_regExpApproxVars;
		HighlightColorScheme m_colorScheme;

		void Highlight(const QRegExp& expr, const QString& code, const QColor& color);
	};
}
#endif // HLSLHIGHLIGHTER_H
