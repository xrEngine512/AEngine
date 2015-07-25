#pragma once
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <qpushbutton.h>
#include <QVBoxLayout>
#include "AnimatedWidget.h"
#include <qpropertyanimation.h>
#include <qcombobox.h>

namespace ApproxGUI
{
	class PopupDialog :
		public AnimatedWidget<QWidget>
	{
		Q_OBJECT
		struct IElement
		{
			virtual QString Data()const = 0;
			virtual ~IElement(){}
		};

		template <class Control>
		struct AbstractElement : QWidget, IElement
		{
			Control* m_txt;
			QLabel* m_lblTitle;
			QComboBox j;
			
			AbstractElement(QWidget* parent, const QString& name) :QWidget(parent)
			{
				m_lblTitle = new QLabel(this);
				m_txt = new Control(this);
				m_lblTitle->setText(name);
				resize(100, 40);
				m_lblTitle->setGeometry(0, 0, 100, 20);
				m_txt->setGeometry(0, 20, 100, 20);				
			}			
		};
		template <class Control>
		struct Element : AbstractElement < Control >
		{
			Element(QWidget* parent, const QString& name) :AbstractElement(parent, name){}
		};
		template <>
		struct Element<QLineEdit> : AbstractElement<QLineEdit>
		{
			Element(QWidget* parent, const QString& name) :AbstractElement(parent, name){}
			QString Data()const override
			{
				return m_txt->text();
			}
		};
		template <>
		struct Element<QComboBox> : AbstractElement<QComboBox>
		{
			Element(QWidget* parent, const QString& name) :AbstractElement(parent, name){}
			QString Data()const override
			{
				return m_txt->currentText();
			}
		};

		QPushButton* m_btnOK;
		QVector<IElement*> m_Elements;
		void showEvent(QShowEvent* e) override
		{
			/*auto anim = new QPropertyAnimation(this,"size");
			anim->setStartValue(QSize(10, 10));
			anim->setEndValue(size());
			anim->setDuration(300);
			BaseClass::showEvent(e);
			anim->start(QAbstractAnimation::DeleteWhenStopped);*/	
			
		}
	signals:
		void OK();
	public:
		PopupDialog(QWidget* parent) :BaseClass(parent)
		{
			new QVBoxLayout(this);
			SetAnimationVariation(GO_LEFT_AND_EXPAND);
			setWindowFlags(Qt::Popup);
			setAttribute(Qt::WA_DeleteOnClose);		
		}

		template<class T>
		void Exec(const T& arg)
		{
			Process(arg);
			m_memoredSize = QSize(100, m_Elements.size() * 60 + 30);
			m_btnOK = new QPushButton;
			layout()->addWidget(m_btnOK);
			
			auto g = geometry();
			g.setSize(QSize(10, 10));
			ShowAnimated(g);
			connect(m_btnOK, &QPushButton::clicked, [=]()
			{
				emit OK();
				close();
			});
		}
		QVector<QString> Data()
		{
			QVector<QString> res;
			for (auto elem : m_Elements)
			{
				res.push_back(elem->Data());
			}
			return res;
		}
		template<class T, class ...Ts>
		void Exec(const T& arg, const Ts&... args)
		{
			Process(arg);
			return Exec<Ts...>(args...);
		}
		template<class T>
		void Process(const T& arg)
		{
			auto elem = new Element<QLineEdit>(this, arg);
			m_Elements.push_back(elem);
			layout()->addWidget(elem);
		}
		template<>
		void Process<QPair<QString, QVector<QString>>>(const QPair<QString, QVector<QString>>& arg)
		{
			auto elem = new Element<QComboBox>(this, arg.first);
			m_Elements.push_back(elem);
			layout()->addWidget(elem);
			for (auto choice : arg.second)
				elem->m_txt->addItem(choice);
		}
		~PopupDialog()
		{
			qDebug("destroyed");
		}
	};
}
