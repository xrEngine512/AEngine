#include "CodeEditor.h"
#include "HLSLHighlighter.h"
#include <qpropertyanimation.h>
#include <qstatusbar.h>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QGraphicsProxyWidget>
#include <qgraphicsscene.h>
#include "Literals.h"
#include "ShaderLabGUIElement.h"

namespace ASL
{
	CodeEditor::CodeEditor(QWidget *parent, ShaderLabGUIElement* controlItem, int Width, int Height)
		: BaseClass(parent), m_chkHoldSize(nullptr), m_controlItem(controlItem), m_readOnlyStart(0), 
		m_readOnlyEnd(0), m_cursorPrevPos(0), m_contMenuOpen(false)
	{
		SetAnimationVariation(GO_RIGHT_AND_EXPAND);
		m_memoredSize = QSize(Width, Height);
		setObjectName("codeEditor");
		resize(150, 150);
		m_highlighter = new HLSLHighlighter(this);
		m_highlighter->setDocument(document());

		QFont font;
		font.setFamily("Courier");
		font.setStyleHint(QFont::Monospace);
		font.setFixedPitch(true);
		font.setPointSize(10);
		
		connect(this,SIGNAL(cursorPositionChanged()), SLOT(On_cursorPositionChanged()));

		QFontMetrics metrics(font);
		setTabStopWidth(4 * metrics.width(' '));

		setFont(font);
		m_lblStatus = new QLabel(QStringLiteral("Тестовое сообщение"));
		m_statusBar = new QStatusBar(this);

		QPalette palette;
		palette.setColor(QPalette::Base, QColor(50, 50, 50));
		palette.setColor(QPalette::Background, QColor(50, 50, 50));
		palette.setColor(QPalette::Text, QColor(200, 200, 200));
		setPalette(palette);

		//m_chkHoldSize->resize(25, 25);

		m_statusBar->setGeometry(0, height() - 23, width(), 23);
		m_statusBar->addWidget(m_lblStatus);
	}

	CodeEditor::~CodeEditor()
	{
		delete m_chkHoldSize;
	}

	void CodeEditor::SetReadOnlyArea(int begin, int len)
	{
		m_readOnlyStart = begin;
		m_readOnlyEnd = m_readOnlyStart + len;
	}

	ISyntaxHighlighter* CodeEditor::SyntaxHilighter() const
	{
		return dynamic_cast<ISyntaxHighlighter*>(m_highlighter);
	}

	void CodeEditor::SetTabWidth(int characters)
	{
		QFontMetrics metrics(font());
		setTabStopWidth(characters * metrics.width(' '));
	}

	void CodeEditor::SetProxyWidget(QGraphicsProxyWidget* wgt)
	{
		BaseClass::SetProxyWidget(wgt);
		m_chkHoldSize = new QCheckBox;
		m_chkHoldSize->hide();
		wgt->scene()->addWidget(m_chkHoldSize);
		m_chkHoldSize->setToolTip(TIP_HOLD_SIZE_QT);
	}

	void CodeEditor::setZValue(int z)
	{
		graphicsProxyWidget()->setZValue(z);
		m_chkHoldSize->graphicsProxyWidget()->setZValue(z);
	}

	inline bool intersects(int begin, int pos, int end)
	{
		return pos > begin && pos < end;
	}

	inline bool intersects(int min1, int max1, int min2, int max2)
	{
		if (min1 == max1) return false;
		if (min2 == max2) return false;
		if ((min1 < min2 && max1 > max2) || (min2 < min1 && max2 > max1)) //means that one contains another
			return true;
		if (min1 > max2) return false;
		if (min2 > max1) return false;
		return true;
	}

	void CodeEditor::On_cursorPositionChanged()
	{
		auto curPos = textCursor().position();
		if (intersects(m_readOnlyStart,curPos,m_readOnlyEnd))
		{
			auto cursor = textCursor();
			cursor.setPosition(m_cursorPrevPos);
			setTextCursor(cursor);
		}
		else
			m_cursorPrevPos = textCursor().position();
	}

	QString CodeEditor::getCode() const
	{
		return toPlainText();
	}

	void CodeEditor::setCode(QString& code)
	{
		setPlainText(code);
		m_controlItem->UpdateGeneratedCode();
	}

	int CodeEditor::GetTabWidth() const
	{
		QFontMetrics metrics(font());
		return tabStopWidth() / metrics.width(' ');
	}

	void CodeEditor::SetHighlighter(QSyntaxHighlighter* h)
	{
		if (m_highlighter)
			delete m_highlighter;

		m_highlighter = h;
		m_highlighter->setDocument(document());
	}

	void CodeEditor::resizeEvent(QResizeEvent* e)
	{
		m_statusBar->setGeometry(0, height() - 23, width(), 23);

		if (!m_animRunning1 && !m_animRunning2 && m_rememberSize)
		{
			m_memoredSize = size();
		}
		BaseClass::resizeEvent(e);
	}

	void CodeEditor::moveEvent(QMoveEvent* e)
	{
		if (m_chkHoldSize)
		{
			auto tL = geometry().topLeft();
			tL.setY(tL.y() - m_chkHoldSize->height());
			m_chkHoldSize->move(tL);
		}
		BaseClass::moveEvent(e);
	}

	void CodeEditor::mousePressEvent(QMouseEvent* e)
	{
		if (e->buttons().testFlag(Qt::LeftButton))
		{

		}
		BaseClass::mousePressEvent(e);
	}

	void CodeEditor::mouseReleaseEvent(QMouseEvent* e)
	{
		if (e->button() == Qt::LeftButton)
		{

		}
		BaseClass::mouseReleaseEvent(e);
	}

	void CodeEditor::keyPressEvent(QKeyEvent* e)
	{
		if (e->key() == Qt::Key_ScrollLock)
		{
			m_chkHoldSize->setChecked(!m_chkHoldSize->isChecked());
		}
		bool intersects_with_selection = intersects(m_readOnlyStart, m_readOnlyEnd, textCursor().selectionStart(), textCursor().selectionEnd());
		if ((e->key() == Qt::Key_Backspace && m_cursorPrevPos == m_readOnlyEnd) || intersects_with_selection || (m_cursorPrevPos == m_readOnlyStart && m_readOnlyEnd != m_readOnlyStart) )
		{
			e->accept();
		}
		else
		{
			BaseClass::keyPressEvent(e);
		}
	}

	void CodeEditor::focusInEvent(QFocusEvent* e)
	{
		Grow();
		auto fnt = font();
		fnt.setPointSize(10);
		setFont(fnt);
		emit focused();
		BaseClass::focusInEvent(e);
	}

	void CodeEditor::focusOutEvent(QFocusEvent* e)
	{
		if (!m_chkHoldSize->underMouse() && !m_chkHoldSize->isChecked() && !m_contMenuOpen)
		{
			Shrink();
			auto fnt = font();
			fnt.setPointSize(3);
			setFont(fnt);
		}
		emit unFocused();
		BaseClass::focusOutEvent(e);
	}

	void CodeEditor::contextMenuEvent(QContextMenuEvent* e)
	{
		m_contMenuOpen = true;
		BaseClass::contextMenuEvent(e);
		m_contMenuOpen = false;
	}

	void CodeEditor::showEvent(QShowEvent* e)
	{
		m_chkHoldSize->show();
		BaseClass::showEvent(e);
	}

	void CodeEditor::hideEvent(QHideEvent* e)
	{
		m_chkHoldSize->hide();
		BaseClass::hideEvent(e);
	}
}