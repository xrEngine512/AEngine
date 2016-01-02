#pragma once
#include "Button.h"
#include "qpropertyanimation.h"
#include <QMouseEvent>
#include <qwidget.h>
#include <qapplication.h>
#include <qlabel.h>
#include "ApproxGuiResManager.h"
#include <QGraphicsDropShadowEffect>
#include <qsizegrip.h>
#include <qdesktopwidget.h>
#include "MouseTracker.h"

namespace ApproxGUI
{
#define client getClientWidget()
	template<class Widget, int btnSize = 25, int border = 5>
	class ApproxWindow : public QWidget
	{		
		Button *m_btnClose, *m_btnMinimize, *m_btnMaximize;
		QLabel *m_title,*m_separator;
		QRect m_geometry, m_screenBounds, m_displayBounds;
		QSizeGrip* m_sizeGrip;
		Widget *m_clientWidget;
		bool m_minimized;
		QPointF m_lPrev;
		bool m_dragging;
		MouseTracker* mouseTracker;
	public:
		template<typename... Ts>
		ApproxWindow(Ts... args) :QWidget(nullptr), m_minimized(false), m_dragging(false)
		{
			setObjectName("ApproxWindow");
			setWindowFlags(Qt::FramelessWindowHint);
			m_clientWidget = new Widget(this, args...);
			m_sizeGrip = new QSizeGrip(this);			
			m_clientWidget->setWindowFlags(Qt::Widget);
			m_screenBounds = QApplication::desktop()->geometry();
			m_displayBounds = QApplication::desktop()->screenGeometry();
			m_clientWidget->move(border, btnSize + border);

			mouseTracker = MouseTracker::Instance();

			connect(mouseTracker, &MouseTracker::mouseMove, this, &ApproxWindow::On_mouseMoveEvent);

			QFile styleF;
			
			styleF.setFileName(QApplication::applicationDirPath() + "/qss/style.css");
			styleF.open(QFile::ReadOnly);
			QString qssStr = styleF.readAll();

			setStyleSheet(qssStr);
		}
		~ApproxWindow()
		{}
	private slots:
	void s_Minimize()
	{
		QRect End_geom(m_displayBounds.center().x(), m_displayBounds.height(), 100, 100);
		m_geometry = geometry();
		
		auto geom_anim = new QPropertyAnimation(this, "geometry");
		auto opac_anim = new QPropertyAnimation(this, "windowOpacity");
	
		geom_anim->setDuration(400);
		opac_anim->setDuration(400);

		geom_anim->setStartValue(geometry());
		opac_anim->setStartValue(windowOpacity());

		geom_anim->setEndValue(End_geom);
		opac_anim->setEndValue(0);

		connect(geom_anim, SIGNAL(finished()), SLOT(showMinimized()));

		geom_anim->start(QAbstractAnimation::DeleteWhenStopped);
		opac_anim->start(QAbstractAnimation::DeleteWhenStopped);

		m_minimized = true;
	}

	void s_MaximizeNormalize()
	{
		Blink(!isMaximized());
	}

	void On_mouseMoveEvent(const QPoint& pos)
	{
		if (m_dragging)
		{
			int px, py;
			if (m_screenBounds.contains(pos))
			{
				move(pos - m_lPrev.toPoint());
			}
		}
	}
	private:
		void Blink(bool maximize)
		{
			auto opac_anim = new QPropertyAnimation(this, "windowOpacity");
			auto opac_anim2 = new QPropertyAnimation(this, "windowOpacity");
			opac_anim->setDuration(200);
			opac_anim->setStartValue(windowOpacity());
			opac_anim->setEndValue(0);

			opac_anim2->setDuration(200);
			opac_anim2->setStartValue(0);
			opac_anim2->setEndValue(1);

			if (maximize)
			{
				//m_geometry = geometry();
				connect(opac_anim, &QPropertyAnimation::finished, this, &ApproxWindow::showMaximized);
			}
			else
			{
				connect(opac_anim, &QPropertyAnimation::finished, [=]()
				{
					showNormal();
					//setGeometry(m_geometry);
				});
			}

			connect(opac_anim, &QPropertyAnimation::finished, [=]()
			{
				opac_anim2->start(QAbstractAnimation::DeleteWhenStopped);
			});
			opac_anim->start(QAbstractAnimation::DeleteWhenStopped);
		}
	protected:
		void InitUI()
		{
			m_btnClose = new Button("closeButton", this);
			m_btnMinimize = new Button("minimizeButton", this);
			m_btnMaximize = new Button("maximizeButton", this);
			m_separator = new QLabel(this);
			if (windowTitle().isEmpty())
				setWindowTitle(m_clientWidget->windowTitle());

			m_title = new QLabel(windowTitle(), this);
			
			connect(m_btnClose, SIGNAL(clicked()), SLOT(close()));
			connect(m_btnMaximize, &Button::clicked, this, &ApproxWindow::s_MaximizeNormalize);
			connect(m_btnMinimize, &Button::clicked, this, &ApproxWindow::s_Minimize);

			m_btnClose->setAttribute(Qt::WA_TranslucentBackground);
			m_btnMaximize->setAttribute(Qt::WA_TranslucentBackground);
			m_btnMinimize->setAttribute(Qt::WA_TranslucentBackground);

			m_title->move(3, 3);

			setWindowIcon(*g_ResManager->GetPic(":/Editor/title.png"));

			QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
			effect->setBlurRadius(5);
			effect->setOffset(2);
			effect->setColor(Qt::black);
			m_title->setGraphicsEffect(effect);

			QFont font("Franklin Gothic Medium", 15);
			font.setStyleStrategy(QFont::PreferAntialias);
			m_title->setFont(font);
			m_title->setAttribute(Qt::WA_TranslucentBackground);

			m_title->adjustSize();
			resize(m_clientWidget->size().width(), m_clientWidget->size().height() + btnSize);

			m_separator->move(0, btnSize -19);
			m_separator->lower();
			m_separator->setAttribute(Qt::WA_TranslucentBackground);
			
			m_btnClose->setStyleSheet("background-color : rgba(0,0,0,0);");
			m_btnMaximize->setStyleSheet("background-color : rgba(0,0,0,0);");
			m_btnMinimize->setStyleSheet("background-color : rgba(0,0,0,0);");
		}
		Widget* getClientWidget()
		{
			return m_clientWidget;
		}
		int ButtonSize()const
		{
			return btnSize;
		}

		void mousePressEvent(QMouseEvent* event)override
		{
			if (event->buttons().testFlag(Qt::LeftButton))
			{
				if (!m_clientWidget->geometry().contains(event->pos()))
				{
					if (!m_dragging)
					{
						m_lPrev = event->localPos();

						setCursor(Qt::CursorShape::ClosedHandCursor);
					}
					m_dragging = true;
					if (isMaximized())
						showNormal();
					event->accept();
				}
			}
			else
			{
				QWidget::mousePressEvent(event);
			}
		}

		void mouseReleaseEvent(QMouseEvent* event)override
		{
			if (event->button() == Qt::LeftButton)
			{
				m_dragging = false;
				unsetCursor();
			}
			else
			{
				QWidget::mouseReleaseEvent(event);
			}
		}

		void showEvent(QShowEvent* e)override
		{
			if (m_minimized)
			{
				auto geom_anim = new QPropertyAnimation(this, "geometry");
				auto opac_anim = new QPropertyAnimation(this, "windowOpacity");

				geom_anim->setDuration(400);
				opac_anim->setDuration(400);

				geom_anim->setStartValue(geometry());
				opac_anim->setStartValue(windowOpacity());

				geom_anim->setEndValue(m_geometry);
				opac_anim->setEndValue(1);

				geom_anim->start(QAbstractAnimation::DeleteWhenStopped);
				opac_anim->start(QAbstractAnimation::DeleteWhenStopped);
				m_minimized = false;
			}
			QWidget::showEvent(e);
		}

		void resizeEvent(QResizeEvent* e)override
		{
			m_btnClose->setGeometry(width() - btnSize, 0, btnSize, btnSize);
			m_btnMaximize->setGeometry(width() - btnSize * 2, 0, btnSize, btnSize);
			m_btnMinimize->setGeometry(width() - btnSize * 3, 0, btnSize, btnSize);
			m_separator->resize(size().width() - btnSize * 3, 19);
			m_separator->setPixmap(g_ResManager->GetPic(":/Editor/separator.png")->scaled(m_separator->size()));
			m_clientWidget->resize(width() - border*2, height() - btnSize - border * 2);
			m_sizeGrip->move(m_clientWidget->geometry().bottomRight());
			QWidget::resizeEvent(e);
		}
	};
}