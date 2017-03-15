#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <AnimatedGraphicsWidget.h>

class QCheckBox;
class QStatusBar;
class QLabel;
class QSyntaxHighlighter;

namespace ASL
{
	class ShaderLabGUIElement;

	interface ISyntaxHighlighter;

	interface ICodeEditor
	{
		virtual void SetHighlighter(QSyntaxHighlighter*) = 0;
		virtual void SetTabWidth(int characters) = 0;
		virtual int GetTabWidth()const = 0;
		virtual QString getCode()const = 0;
		virtual void SetReadOnlyArea(int begin, int len) = 0;
		virtual void setCode(QString&) = 0;
		virtual ISyntaxHighlighter* SyntaxHilighter() const = 0;
		virtual ~ICodeEditor(){};
	};

	class CodeEditor : public ApproxGUI::AnimatedGraphicsWidget<QPlainTextEdit>, public ICodeEditor
	{
		Q_OBJECT
	signals :
		void focused();
		void unFocused();
	private slots:
		void On_cursorPositionChanged();
	public:
		CodeEditor(QWidget *parent, ShaderLabGUIElement* controlItem, int Width = 300, int Height = 500);
		~CodeEditor();
		void SetReadOnlyArea(int begin, int len)override final;
		void SetHighlighter(QSyntaxHighlighter*)override final;
		void SetTabWidth(int characters)override final;
		int GetTabWidth()const override final;
		ISyntaxHighlighter* SyntaxHilighter() const override final;
		void SetProxyWidget(QGraphicsProxyWidget*)override;
		void setZValue(int);
		QString getCode()const override final;
		void setCode(QString&)override final;
	protected:
		void resizeEvent(QResizeEvent*) override final;
		void moveEvent(QMoveEvent*) override final;
		void mousePressEvent(QMouseEvent*)override final;
		void mouseReleaseEvent(QMouseEvent *)override final;
		void keyPressEvent(QKeyEvent*)override final;
		void focusInEvent(QFocusEvent*)override final;
		void focusOutEvent(QFocusEvent*)override final;
		void contextMenuEvent(QContextMenuEvent*)override final;
		void showEvent(QShowEvent*)override final;
		void hideEvent(QHideEvent*)override final;

		QSyntaxHighlighter* m_highlighter;
		QStatusBar* m_statusBar;
		QLabel* m_lblStatus;
		QCheckBox* m_chkHoldSize;
		ShaderLabGUIElement* m_controlItem;

		int m_readOnlyStart, m_readOnlyEnd, m_cursorPrevPos;

		bool m_contMenuOpen;
	};
}
#endif // CODEEDITOR_H
