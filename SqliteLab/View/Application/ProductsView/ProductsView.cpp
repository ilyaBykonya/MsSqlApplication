#include "ProductsView.h"
#include "InputProductDialog/InputProductDialog.h"
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
     m_table_view{ new QTableView } {
        setConnection(connection);
        auto add_product_button = new QPushButton{ "Add" };
        connect(add_product_button, &QPushButton::clicked, this, &ProductsView::addProduct);
        m_table_view->setEditTriggers(QTableView::EditTrigger::NoEditTriggers);

        auto window_layout = new QVBoxLayout;
        this->setLayout(window_layout);
            window_layout->addWidget(m_table_view, 1);
            window_layout->addWidget(add_product_button);
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
        QSqlField field_name{ "Name", QVariant::Type::String };
        QSqlField field_category{ "Category", QVariant::Type::String };
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




