#pragma once
#include "../ProductDto/ProductInfo.h"
#include <QDateTimeEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QPointer>
#include <optional>
#include <QLabel>


class EditProductWidget : public QWidget {
private:
    QPointer<QLineEdit> m_image_view;
    QPointer<QLineEdit> m_name_input;
    QPointer<QLineEdit> m_category_input;
    QPointer<QDateTimeEdit> m_date_input;
    QPointer<QPushButton> m_select_image_button;
public:
    EditProductWidget(QWidget* parent = nullptr);
    std::optional<ProductInfo> record() const;
public slots:
    void setRecord(const std::optional<ProductInfo>& record);
private slots:
    void selectImage();
};

