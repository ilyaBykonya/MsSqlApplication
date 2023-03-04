#include "ProductsView.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlField>
#include <QDebug>

ProductsView::ProductsView(const QSqlDatabase& connection, QWidget *parent)
    :QWidget{ parent },
     m_table_model{ },
     m_current_item_view{ new CurrentItemView },
     m_table_view{ new QTableView } {
        setConnection(connection);
        auto add_product_button = new QPushButton{ "Add" };
        connect(add_product_button, &QPushButton::clicked, this, &ProductsView::addProduct);
        connect(m_table_view, &QAbstractItemView::clicked, this, &ProductsView::setCurrentProduct);
        connect(m_current_item_view, &CurrentItemView::updateRecord, this, &ProductsView::updateProduct);
        connect(m_current_item_view, &CurrentItemView::deleteRecord, this, &ProductsView::deleteProduct);
        m_table_view->setEditTriggers(QTableView::EditTrigger::NoEditTriggers);

        auto service_layout = new QVBoxLayout;
            service_layout->addWidget(m_current_item_view);
            service_layout->addWidget(add_product_button);

        auto window_layout = new QHBoxLayout;
        this->setLayout(window_layout);
            window_layout->addLayout(service_layout);
            window_layout->addWidget(m_table_view, 1);
}

void ProductsView::setConnection(const QSqlDatabase &connection) {
    clearConnection();
    m_table_model = new QSqlTableModel{ this, connection };
    m_table_view->setModel(m_table_model);
    m_table_model->setTable(s_table_name);
    m_table_model->setEditStrategy(QSqlTableModel::OnRowChange);
    m_table_model->select();
}
void ProductsView::clearConnection() {
    if(m_table_model)
        m_table_model->deleteLater();
}

void ProductsView::addProduct() {
    InputProductDialog input_product;
    if(input_product.exec() != QDialog::DialogCode::Accepted)
        return;

    QSqlRecord record{};
        QSqlField field_name{ s_field_name, QVariant::Type::String };
        QSqlField field_category{ s_field_category, QVariant::Type::String };
        field_name.setValue(input_product.name());
        field_category.setValue(input_product.category());
        field_name.setGenerated(true);
        field_category.setGenerated(true);
    record.append(field_name);
    record.append(field_category);

    m_table_model->insertRecord(-1, record);
    m_table_model->submitAll();
    m_table_model->select();
}
void ProductsView::setCurrentProduct(const QModelIndex &index) {
    auto record = m_table_model->record(index.row());
    m_current_item_view->setCurrentRecord(index.row(), record.value(s_field_id).toULongLong(), record.value(s_field_name).toString(), record.value(s_field_category).toString());

    m_table_model->submitAll();
    m_table_model->select();
}

void ProductsView::updateProduct(uint64_t line, uint64_t id, const QString &name, const QString &category) {
    qDebug() << "Reset product: " << id << name << category;
    m_table_model->setData(m_table_model->index(line, m_table_model->fieldIndex(s_field_name)), name);
    m_table_model->setData(m_table_model->index(line, m_table_model->fieldIndex(s_field_category)), category);
    m_table_model->submitAll();
    m_table_model->select();
}
void ProductsView::deleteProduct(uint64_t line, uint64_t id) {
    qDebug() << "Delete product: " << id;
    m_table_model->removeRow(line);
    m_table_model->submitAll();
    m_table_model->select();
}




