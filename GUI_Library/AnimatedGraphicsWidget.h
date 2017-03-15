#pragma once
#include "AnimatedWidget.h"
#include <qgraphicsproxywidget.h>

namespace ApproxGUI
{
	template <class Widget>
	class AnimatedGraphicsWidget : public AnimatedWidget<Widget>
	{
	public:
		template<class T>
		AnimatedGraphicsWidget(T* parent = nullptr) :
			AnimatedWidget<Widget>::AnimatedWidget(parent),
			m_proxy(nullptr)
		{
		}
		virtual void SetProxyWidget(QGraphicsProxyWidget* proxy)
		{
			m_proxy = proxy;
			if (m_proxy)
				m_proxy->setZValue(8);
		}
	protected:
		typedef AnimatedGraphicsWidget<Widget> BaseClass;

		QGraphicsProxyWidget* m_proxy;
	};
}
