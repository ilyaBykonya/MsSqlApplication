#pragma once
#include <QLineEdit>
#include <QDialog>

class ConnectionDialog : public QDialog {
    Q_OBJECT
private:
    QLineEdit* m_address_input;
    QLineEdit* m_database_input;
    QLineEdit* m_user_input;
    QLineEdit* m_password_input;
public:
    ConnectionDialog(QWidget* parent = nullptr);
    QString connection() const;
    QString password() const;
    QString user() const;
};

