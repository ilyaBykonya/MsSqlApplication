#include "CurrentItemView.h"
#include <QPushButton>
#include <QVBoxLayout>

CurrentItemView::CurrentItemView(QWidget* parent)
    :QWidget{ parent }, m_record{},
     m_current_id_view{ new QLineEdit },
     m_current_product{ new EditProductWidget },
     m_update_button{ new QPushButton{ "Update" } },
     m_delete_button{ new QPushButton{ "Delete" } },
     m_refresh_button{ new QPushButton{ "Refresh" } } {
        m_current_id_view->setReadOnly(true);
        m_current_id_view->setEnabled(false);
        m_current_id_view->setPlaceholderText("Id");

        connect(m_update_button, &QPushButton::clicked, this, &CurrentItemView::onUpdate);
        connect(m_delete_button, &QPushButton::clicked, this, &CurrentItemView::onDelete);
        connect(m_refresh_button, &QPushButton::clicked, this, &CurrentItemView::onRefresh);

        auto window_layout = new QVBoxLayout;
        this->setLayout(window_layout);
            window_layout->addWidget(m_current_id_view);
            window_layout->addWidget(m_current_product);
            window_layout->addSpacing(10);
            window_layout->addWidget(m_update_button);
            window_layout->addWidget(m_delete_button);
            window_layout->addWidget(m_refresh_button);

        resetRecord({});
}

void CurrentItemView::setCurrentRecord(std::uint64_t line, const ProductRecord& record) {
    resetRecord(std::pair{ line, record });
}
void CurrentItemView::resetRecord(const std::optional<std::pair<std::uint64_t, ProductRecord>>& record) {
    m_record = record;
    auto has_value = record.has_value();
    m_current_product->setRecord(has_value ? record->second.info() : std::optional<ProductInfo>{});
    m_current_id_view->setText(has_value ? QString::number(record->second.id()) : "");
    m_update_button->setEnabled(has_value);
    m_delete_button->setEnabled(has_value);
}

void CurrentItemView::onUpdate() {
    if(auto result = m_current_product->record(); result.has_value() and m_record.has_value())
        emit this->updateRecord(m_record->first, ProductRecord{ m_record->second.id(), result.value() });
}
void CurrentItemView::onDelete() {
    if(m_record.has_value())
        emit this->deleteRecord(m_record->first);
}
void CurrentItemView::onRefresh() {
    resetRecord(m_record);
}

