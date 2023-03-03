#include "ConnectionDialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

ConnectionDialog::ConnectionDialog(QWidget* parent)
    :QDialog{ parent },
     m_address_input{ new QLineEdit{} },
     m_database_input{ new QLineEdit{} },
     m_user_input{ new QLineEdit{} },
     m_password_input{ new QLineEdit{} }
    {
        auto accept_button = new QPushButton{ "Connect" };
        auto reject_button = new QPushButton{ "Cancel" };
        connect(accept_button, &QPushButton::clicked, this, &QDialog::accept);
        connect(reject_button, &QPushButton::clicked, this, &QDialog::reject);

        m_address_input->setPlaceholderText("Address");
        m_database_input->setPlaceholderText("Database");
        m_user_input->setPlaceholderText("User");
        m_password_input->setPlaceholderText("Password");

        auto controls_layout = new QHBoxLayout;
            controls_layout->addWidget(reject_button);
            controls_layout->addWidget(accept_button);

        auto dialog_layout = new QVBoxLayout;
        this->setLayout(dialog_layout);
            dialog_layout->addWidget(m_address_input);
            dialog_layout->addWidget(m_database_input);
            dialog_layout->addWidget(m_user_input);
            dialog_layout->addWidget(m_password_input);
            dialog_layout->addLayout(controls_layout);
    }

QString ConnectionDialog::connection() const {
    return QString{ "DRIVER={SQL Server};SERVER=%1;DATABASE=%2;" }.arg(m_address_input->text()).arg(m_database_input->text());
}
QString ConnectionDialog::password() const {
    return m_password_input->text();
}
QString ConnectionDialog::user() const {
    return m_user_input->text();
}

