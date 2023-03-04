#include "InputProductDialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

InputProductDialog::InputProductDialog(QPointer<CategoriesListRepository> categories, const std::optional<ProductInfo>& origin, QWidget* parent)
    :QDialog{ parent }, m_input{ new EditProductWidget{ categories } } {
        m_input->setRecord(origin);
        auto accept_button = new QPushButton{ "Ok" };
        auto reject_button = new QPushButton{ "Cancel" };
        connect(accept_button, &QPushButton::clicked, this, &QDialog::accept);
        connect(reject_button, &QPushButton::clicked, this, &QDialog::reject);

        auto controls_layout = new QHBoxLayout;
            controls_layout->addWidget(reject_button);
            controls_layout->addWidget(accept_button);

        auto dialog_layout = new QVBoxLayout;
        this->setLayout(dialog_layout);
            dialog_layout->addWidget(m_input);
            dialog_layout->addLayout(controls_layout);
    }

std::optional<ProductInfo> InputProductDialog::info() const {
    return m_input->record();
}




