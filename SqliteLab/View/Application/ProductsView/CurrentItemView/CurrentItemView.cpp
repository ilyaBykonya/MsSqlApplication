#include "CurrentItemView.h"
#include <QPushButton>
#include <QVBoxLayout>

CurrentItemView::CurrentItemView(QWidget* parent)
    :QWidget{ parent }, m_record{},
     m_current_id_view{ new QLineEdit },
     m_current_name_view{ new QLineEdit },
     m_current_category_view{ new QLineEdit },
     m_update_button{ new QPushButton{ "Update" } },
     m_delete_button{ new QPushButton{ "Delete" } },
     m_refresh_button{ new QPushButton{ "Refresh" } } {
        m_current_id_view->setReadOnly(true);
        m_current_id_view->setEnabled(false);
        m_current_id_view->setPlaceholderText("Id");
        m_current_name_view->setPlaceholderText("Name");
        m_current_category_view->setPlaceholderText("Category");

        connect(m_update_button, &QPushButton::clicked, this, &CurrentItemView::onUpdate);
        connect(m_delete_button, &QPushButton::clicked, this, &CurrentItemView::onDelete);
        connect(m_refresh_button, &QPushButton::clicked, this, &CurrentItemView::onRefresh);

        auto window_layout = new QVBoxLayout;
        this->setLayout(window_layout);
            window_layout->addWidget(m_current_id_view);
            window_layout->addWidget(m_current_name_view);
            window_layout->addWidget(m_current_category_view);
            window_layout->addSpacing(10);
            window_layout->addWidget(m_update_button);
            window_layout->addWidget(m_delete_button);
            window_layout->addWidget(m_refresh_button);

        resetRecord({});
}

void CurrentItemView::setCurrentRecord(std::uint64_t line, uint64_t id, const QString &name, const QString &category) {
    resetRecord(std::pair{ line, ProductRecord{ id, name, category } });
}
void CurrentItemView::resetRecord(const std::optional<std::pair<std::uint64_t, ProductRecord>>& record) {
    m_record = record;
    auto has_value = record.has_value();
    m_current_name_view->setEnabled(has_value);
    m_current_category_view->setEnabled(has_value);
    m_update_button->setEnabled(has_value);
    m_delete_button->setEnabled(has_value);

    m_current_id_view->setText(has_value ? QString::number(record.value().second.id()) : "");
    m_current_category_view->setText(has_value ? record.value().second.category() : "");
    m_current_name_view->setText(has_value ? record.value().second.name() : "");
}

void CurrentItemView::onUpdate() {
    if(m_record.has_value())
        emit this->updateRecord(m_record.value().first, m_record.value().second.id(), m_current_name_view->text(), m_current_category_view->text());
}
void CurrentItemView::onDelete() {
    if(m_record.has_value())
        emit this->deleteRecord(m_record.value().first, m_record.value().second.id());
}
void CurrentItemView::onRefresh() {
    resetRecord(m_record);
}

CurrentItemView::ProductRecord::ProductRecord(uint64_t id, const QString &name, const QString &category)
    :m_id{ id }, m_category{ category }, m_name{ name } {}
const QString &CurrentItemView::ProductRecord::category() const {
    return m_category;
}
const QString &CurrentItemView::ProductRecord::name() const {
    return m_name;
}
uint64_t CurrentItemView::ProductRecord::id() const {
    return m_id;
}
