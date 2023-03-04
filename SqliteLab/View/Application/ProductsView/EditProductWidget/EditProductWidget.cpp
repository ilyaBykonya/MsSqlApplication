#include "EditProductWidget.h"
#include "Utils/ImageSelectionDialog/ImageSelectionDialog.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QDebug>

EditProductWidget::EditProductWidget(QWidget* parent)
    :QWidget{ parent },
     m_image_view{ new QLineEdit },
     m_name_input{ new QLineEdit },
     m_category_input{ new QLineEdit },
     m_date_input{ new QDateTimeEdit{ QDate::currentDate() } },
     m_select_image_button{ new QPushButton{ "Set image" } } {
        m_image_view->setReadOnly(true);
        m_image_view->setEnabled(false);
        m_name_input->setPlaceholderText("Name");
        m_category_input->setPlaceholderText("Category");

        m_name_input->setText("Вафли");
        m_category_input->setText("Сладости");
        connect(m_select_image_button, &QPushButton::clicked, this, &EditProductWidget::selectImage);

        auto window_layout = new QVBoxLayout;
        this->setLayout(window_layout);
            window_layout->addWidget(m_name_input);
            window_layout->addWidget(m_category_input);
            window_layout->addWidget(m_date_input);
            window_layout->addWidget(m_select_image_button);

        setRecord({});
}

std::optional<ProductInfo> EditProductWidget::record() const {
    if(not this->isEnabled())
        return {};

    return ProductInfo {
        m_name_input->text(), m_category_input->text(),
        m_date_input->date(), m_image_view->text()
    };
}
void EditProductWidget::selectImage() {
    ImageSelectionDialog image{ m_image_view->text(), this };
    if(image.exec() == QDialog::DialogCode::Accepted)
        m_image_view->setText(image.path());
}
void EditProductWidget::setRecord(const std::optional<ProductInfo>& record) {
    auto has_value = record.has_value();
    this->setEnabled(has_value);

    m_name_input->setText(has_value ? record.value().name() : "");
    m_category_input->setText(has_value ? record.value().category() : "");
    m_date_input->setDate(has_value ? record.value().date() : QDate::currentDate());
    m_image_view->setText(has_value ? record.value().image_path() : "");
}

