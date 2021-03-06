#pragma once

#include <qpropertyanimation.h>

namespace ApproxGUI
{
	template <class Widget>
	class AnimatedWidget : public Widget
	{
	protected:
		typedef AnimatedWidget<Widget> BaseClass;
		enum AnimationVariation{ GO_RIGHT_AND_EXPAND, GO_LEFT_AND_EXPAND, GO_DOWN_AND_EXPAND };

	public:
		template<class T>
		AnimatedWidget(T* parent = nullptr) :
			Widget(parent),
			anim(nullptr),
			anim1(nullptr),
			m_trigger(true),
			m_animRunning1(false),
			m_animRunning2(false),
			m_rememberSize(false),
			m_animVariation(GO_RIGHT_AND_EXPAND)
		{
			QPalette palette;
			palette.setColor(QPalette::Base, QColor(50, 50, 50));
			palette.setColor(QPalette::Background, QColor(50, 50, 50));
			palette.setColor(QPalette::Text, QColor(180, 180, 180));
			Widget::setPalette(palette);
		}
		void SetAnimationVariation(AnimationVariation variation)
		{
			m_animVariation = variation;
		}
		QSize getSize()const
		{
			return m_memoredSize;
		}
		void Grow()
		{
			if (m_trigger)
			{
				m_animRunning1 = true;
				QPropertyAnimation* anim;
				switch (m_animVariation)
				{
				case GO_RIGHT_AND_EXPAND:
				{
					anim = new QPropertyAnimation(this, "size");
					anim->setDuration(500);
					anim->setEasingCurve(QEasingCurve::OutCubic);
					anim->setStartValue(Widget::size());
					anim->setEndValue(m_memoredSize);
					break;
				}
				case GO_DOWN_AND_EXPAND:
				case GO_LEFT_AND_EXPAND:
				{
					QPoint topRight(Widget::geometry().topRight().x() - m_memoredSize.width() + 1, Widget::geometry().topRight().y());
					anim = new QPropertyAnimation(this, "geometry");
					anim->setDuration(500);
					anim->setEasingCurve(QEasingCurve::OutCubic);
					anim->setStartValue(Widget::geometry());
					anim->setEndValue(QRect(topRight, m_memoredSize));
					break;
				}
				}
                Widget::connect(anim, &QPropertyAnimation::finished, [&](){m_animRunning1 = false; });
				anim->start(QAbstractAnimation::DeleteWhenStopped);
				m_trigger = false;
			}
		}
		void Shrink()
		{
			if (!m_trigger)
			{
				m_animRunning1 = true;
				QPropertyAnimation* anim;
				switch (m_animVariation)
				{
				case GO_RIGHT_AND_EXPAND:
				{
					anim = new QPropertyAnimation(this, "size");
					anim->setDuration(500);
					anim->setEasingCurve(QEasingCurve::OutCubic);
					anim->setStartValue(Widget::size());
					anim->setEndValue(QSize(150, 150));
					break;
				}
				case GO_DOWN_AND_EXPAND:
				case GO_LEFT_AND_EXPAND:
				{
					QPoint topRight = Widget::geometry().topRight();
					anim = new QPropertyAnimation(this, "geometry");
					anim->setDuration(500);
					anim->setEasingCurve(QEasingCurve::OutCubic);
					anim->setStartValue(Widget::geometry());
					anim->setEndValue(QRect(topRight.x() - 149, topRight.y(), 150, 150));
					break;
				}
				}
                Widget::connect(anim, &QPropertyAnimation::finished, [&](){m_animRunning1 = false; });
				anim->start(QAbstractAnimation::DeleteWhenStopped);
				m_trigger = true;
			}
		}
		void ShowAnimated(QRect iG)
		{

			if (Widget::isHidden())
			{
				if (anim)
				{
					anim->stop();
					delete anim;
				}
				if (anim1)
				{
					anim1->stop();
					delete anim1;
				}

				anim = new QPropertyAnimation(this, "geometry");

				anim1 = new QPropertyAnimation(this, "windowOpacity");
				m_animRunning2 = true;

				setGeometry(iG);

				anim->setDuration(450);
				anim->setStartValue(Widget::geometry());
				anim->setEasingCurve(QEasingCurve::Linear);
				switch (m_animVariation)
				{
				case GO_RIGHT_AND_EXPAND:
				{
					anim->setKeyValueAt(0.3, QRect(iG.topRight().x(), iG.y(), iG.width(), iG.height()));
					anim->setKeyValueAt(0.6, QRect(iG.topRight().x(), iG.y(), iG.width(), iG.height()));
					anim->setKeyValueAt(1.0, QRect(iG.topRight().x(), iG.y(), m_memoredSize.width(), m_memoredSize.height()));
					break;
				}
				case GO_LEFT_AND_EXPAND:
				{
					anim->setKeyValueAt(0.3, QRect(iG.topLeft().x() - iG.width(), iG.y(), iG.width(), iG.height()));
					anim->setKeyValueAt(0.6, QRect(iG.topLeft().x() - iG.width(), iG.y(), iG.width(), iG.height()));
					anim->setKeyValueAt(1.0, QRect(iG.topLeft().x() - m_memoredSize.width(), iG.y(), m_memoredSize.width(), m_memoredSize.height()));
					break;
				}
				case GO_DOWN_AND_EXPAND:
				{
					anim->setKeyValueAt(0.3, QRect(iG.bottomLeft().x(), iG.bottomLeft().y(), iG.width(), iG.height()));
					anim->setKeyValueAt(0.6, QRect(iG.bottomLeft().x(), iG.bottomLeft().y(), iG.width(), iG.height()));
					anim->setKeyValueAt(1.0, QRect(iG.bottomLeft().x() - m_memoredSize.width() + iG.width(), iG.bottomLeft().y(), m_memoredSize.width(), m_memoredSize.height()));
					break;
				}
				}

				anim1->setDuration(450);
				anim1->setStartValue(Widget::windowOpacity());
				anim1->setEndValue(1.0);
				anim1->setEasingCurve(QEasingCurve::Linear);
				Widget::show();
                Widget::connect(anim, &QPropertyAnimation::finished, [&](){m_animRunning2 = false; });

				anim->start();

				anim1->start();
			}
		}
		void HideAnimated(QRect eG)
		{
			if (!Widget::isHidden())
			{
				if (anim)
				{
					anim->stop();
					delete anim;
				}
				if (anim1)
				{
					anim1->stop();
					delete anim1;
				}

				anim = new QPropertyAnimation(this, "geometry");

				anim1 = new QPropertyAnimation(this, "windowOpacity");

				anim->setDuration(450);
				anim->setStartValue(Widget::geometry());
				anim->setEasingCurve(QEasingCurve::Linear);



				switch (m_animVariation)
				{
				case GO_RIGHT_AND_EXPAND:
				{
					anim->setKeyValueAt(0.4, QRect(eG.topRight().x(), eG.y(), eG.width(), eG.height()));
					anim->setKeyValueAt(0.7, QRect(eG.topRight().x(), eG.y(), eG.width(), eG.height()));
					anim->setKeyValueAt(1.0, eG);
					break;
				}
				case GO_LEFT_AND_EXPAND:
				{
					anim->setKeyValueAt(0.4, QRect(eG.topLeft().x() - eG.width(), eG.y(), eG.width(), eG.height()));
					anim->setKeyValueAt(0.7, QRect(eG.topLeft().x() - eG.width(), eG.y(), eG.width(), eG.height()));
					anim->setKeyValueAt(1.0, eG);
					break;
				}
				case GO_DOWN_AND_EXPAND:
				{
					anim->setKeyValueAt(0.4, QRect(eG.bottomLeft().x(), eG.bottomLeft().y(), eG.width(), eG.height()));
					anim->setKeyValueAt(0.7, QRect(eG.bottomLeft().x(), eG.bottomLeft().y(), eG.width(), eG.height()));
					anim->setKeyValueAt(1.0, eG);
					break;
				}
				}

				anim1->setDuration(450);
				anim1->setStartValue(Widget::windowOpacity());
				anim1->setEndValue(0.0);

				Widget::connect(anim, &QPropertyAnimation::finished, [&](){m_animRunning2 = false; Widget::hide(); });
				m_animRunning2 = true;
				anim->start();

				anim1->start();
			}
		}
		void RememberSize(bool remember)
		{
			m_rememberSize = remember;
		}
		void setGeometry(QRect r)
		{
			m_rememberSize = false;
			Widget::setGeometry(r);
			m_rememberSize = true;
		}
		virtual ~AnimatedWidget()
		{
			if (anim)
			{
				delete anim;
			}
			if (anim1)
			{
				delete anim1;
			}
		}
	protected:
		void resizeEvent(QResizeEvent* e)override
		{
			if (!m_animRunning1 && !m_animRunning2 && m_rememberSize)
			{
				m_memoredSize = Widget::size();
			}
			Widget::resizeEvent(e);
		}

		QSize m_memoredSize;
		QPropertyAnimation* anim, *anim1;

		bool m_trigger, m_animRunning1, m_animRunning2, m_rememberSize;
		AnimationVariation m_animVariation;
	};
}