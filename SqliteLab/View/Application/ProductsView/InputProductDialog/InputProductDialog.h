#pragma once
#include <QLineEdit>
#include <QDialog>

class InputProductDialog : public QDialog {
    Q_OBJECT
private:
    QLineEdit* m_name_input;
    QLineEdit* m_category_input;
public:
    InputProductDialog(QWidget* parent = nullptr);
    QString category() const;
    QString name() const;
};


