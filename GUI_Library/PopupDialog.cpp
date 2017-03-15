//
// Created by islam on 17.02.17.
//
#include "PopupDialog.h"

namespace ApproxGUI {

    PopupDialog::PopupDialog(QWidget* parent): PopupDialog::BaseClass(parent)
    {
        new QVBoxLayout(this);
        SetAnimationVariation(GO_LEFT_AND_EXPAND);
        setWindowFlags(Qt::Popup);
        setAttribute(Qt::WA_DeleteOnClose);
    }

    QVector<QString> PopupDialog::Data()
    {
        QVector<QString> res;
        for (auto elem : m_Elements)
        {
            res.push_back(elem->Data());
        }
        return res;
    }

    void PopupDialog::Process(const QPair<QString, QVector<QString>>& arg)
    {
        auto elem = new Element<QComboBox>(this, arg.first);
        m_Elements.push_back(elem);
        layout()->addWidget(elem);
        for (auto choice : arg.second)
            elem->m_txt->addItem(choice);
    }

    PopupDialog::~PopupDialog()
    {
        qDebug("destroyed");
    }
};


