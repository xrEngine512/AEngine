#include "CodeEditor.h"
#include "HLSLHighlighter.h"
#include <qpropertyanimation.h>
#include <qstatusbar.h>
#include <QtWidgets/QLabel>
#include <QCheckBox.h>
#include <QGraphicsProxyWidget>
#include <qgraphicsscene.h>
#include <qsizegrip.h>
#include "ShaderLabGUIElement.h"

CodeEditor::CodeEditor(QWidget *parent, ShaderLabGUIElement* controlItem, int Width, int Height)
	: BaseClass(parent), m_contMenuOpen(false), m_chkHoldSize(nullptr), m_controlItem(controlItem)
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


	QFontMetrics metrics(font);
	setTabStopWidth(4 * metrics.width(' '));

	setFont(font);
    m_lblStatus = new QLabel(QStringLiteral("Тестовое сообщение"));
    m_statusBar = new QStatusBar(this);
    
    QPalette palette;
    palette.setColor(QPalette::Base, QColor(50,50,50));
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

ISyntaxHilighter* CodeEditor::SyntaxHilighter() const
{
	return dynamic_cast<ISyntaxHilighter*>(m_highlighter);
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
}

void CodeEditor::setZValue(int z)
{
	for (auto item : graphicsProxyWidget()->scene()->items())
	{
		if (item->data(0) == 5)
		{
			item->setZValue(7);
			item->setData(0, 5);
		}
		else
		{
			item->setZValue(8);
		}
	}
	graphicsProxyWidget()->setZValue(z);
	m_chkHoldSize->graphicsProxyWidget()->setZValue(z);
	m_controlItem->setZValue(z);

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

void CodeEditor::focusInEvent(QFocusEvent* e)
{
	Grow();
	setZValue(10);
	auto fnt = font();
	fnt.setPointSize(10);
	setFont(fnt);
	BaseClass::focusInEvent(e);
}

void CodeEditor::focusOutEvent(QFocusEvent* e)
{
	setZValue(8);

	if (!m_chkHoldSize->underMouse() && !m_chkHoldSize->isChecked() && !m_contMenuOpen)
	{
		Shrink();
		auto fnt = font();
		fnt.setPointSize(3);
		setFont(fnt);
	}
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