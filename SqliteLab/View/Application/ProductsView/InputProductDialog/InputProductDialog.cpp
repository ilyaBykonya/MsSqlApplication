#include "InputProductDialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

InputProductDialog::InputProductDialog(QWidget* parent)
    :QDialog{ parent },
     m_name_input{ new QLineEdit{} },
     m_category_input{ new QLineEdit{} }
    {
        auto accept_button = new QPushButton{ "Ok" };
        auto reject_button = new QPushButton{ "Cancel" };
        connect(accept_button, &QPushButton::clicked, this, &QDialog::accept);
        connect(reject_button, &QPushButton::clicked, this, &QDialog::reject);

        m_name_input->setPlaceholderText("Name");
        m_category_input->setPlaceholderText("Category");

        m_name_input->setText("Вафли");
        m_category_input->setText("Сладости");

        auto controls_layout = new QHBoxLayout;
            controls_layout->addWidget(reject_button);
            controls_layout->addWidget(accept_button);

        auto dialog_layout = new QVBoxLayout;
        this->setLayout(dialog_layout);
            dialog_layout->addWidget(m_name_input);
            dialog_layout->addWidget(m_category_input);
            dialog_layout->addLayout(controls_layout);
    }

QString InputProductDialog::category() const {
    return m_category_input->text();
}
QString InputProductDialog::name() const {
    return m_name_input->text();
}


