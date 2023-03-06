#include "ProductsView.h"
#include "Utils/TablePrinter/TablePrinter.h"
#include <QInputDialog>
#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPdfWriter>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlField>
#include <QPageSize>
#include <QPrinter>
#include <QPainter>
#include <QPixmap>
#include <QAction>
#include <QMenu>
#include <QDebug>
#include <QSqlDriver>

ProductsView::ProductsView(QPointer<CategoriesListRepository> categories, const QSqlDatabase& connection, QWidget *parent)
    :QWidget{ parent },
     m_categories{ categories },
     m_table_model{ },
     m_current_item_view{ new CurrentItemView{ categories } },
     m_table_view{ new QTableView } {
        auto add_product_button = new QPushButton{ "Add" };
        auto make_pdf_button = new QPushButton{ "Make pdf" };

        setConnection(connection);
        m_table_view->setEditTriggers(QTableView::EditTrigger::NoEditTriggers);
        m_table_view->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

        connect(make_pdf_button, &QPushButton::clicked, this, &ProductsView::saveReport);
        connect(add_product_button, &QPushButton::clicked, this, &ProductsView::addProduct);
        connect(m_table_view, &QAbstractItemView::clicked, this, &ProductsView::setCurrentProduct);
        connect(m_table_view, &QAbstractItemView::customContextMenuRequested, this, &ProductsView::showContextMenuForTableView);
        connect(m_current_item_view, &CurrentItemView::updateRecord, this, &ProductsView::updateProduct);
        connect(m_current_item_view, &CurrentItemView::deleteRecord, this, &ProductsView::deleteProduct);


        auto service_layout = new QVBoxLayout;
            service_layout->addWidget(m_current_item_view);
            service_layout->addWidget(add_product_button);
            service_layout->addWidget(make_pdf_button);

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

std::optional<ProductRecord> ProductsView::recordAt(const QModelIndex &index) {
    if(not index.isValid())
        return {};

    auto record = m_table_model->record(index.row());
    return ProductRecord {
        record.value(s_field_id).toULongLong(),
        ProductInfo {
            record.value(s_field_category).toULongLong(),
            record.value(s_field_name).toString(),
            record.value(s_field_some_date).toDateTime(),
            record.value(s_field_image_path).toString()
        }
    };
}

void ProductsView::addProduct() {
    InputProductDialog input_product{ m_categories, ProductInfo{ m_categories->id_for_name(m_categories->all_categories().first()), {}, QDateTime::currentDateTime(), {} }, this };
    if(input_product.exec() != QDialog::DialogCode::Accepted)
        return;

    auto product = input_product.info();
    if(not product.has_value())
        return;

    QSqlRecord record{};
        QSqlField field_name{ s_field_name, QVariant::Type::String };
        QSqlField field_category{ s_field_category, QVariant::Type::ULongLong };
        QSqlField field_some_date{ s_field_some_date, QVariant::Type::DateTime };
        QSqlField field_image_path{ s_field_image_path, QVariant::Type::String };
        field_name.setValue(product->name());
        field_category.setValue(product->category_id());
        field_some_date.setValue(product->date());
        field_image_path.setValue(product->image_path());
        field_name.setGenerated(true);
        field_category.setGenerated(true);
        field_some_date.setGenerated(true);
        field_image_path.setGenerated(true);
    record.append(field_name);
    record.append(field_category);
    record.append(field_some_date);
    record.append(field_image_path);

    m_table_model->insertRecord(-1, record);
    m_table_model->submitAll();
    m_table_model->select();
}
void ProductsView::setCurrentProduct(const QModelIndex &index) {
    auto record = recordAt(index);
    if(not record.has_value())
        return;

    m_current_item_view->setCurrentRecord(index.row(), record.value());
    m_table_model->submitAll();
    m_table_model->select();
}
void ProductsView::showContextMenuForTableView(const QPoint &position) {
    if(m_table_model == nullptr)
        return;

    auto index = m_table_view->indexAt(position);
    if(not index.isValid())
        return;

    auto table_menu = new QMenu;
    auto update_action = table_menu->addAction("Update");
    auto remove_action = table_menu->addAction("Remove");
    connect(table_menu, &QMenu::aboutToHide, table_menu, &QObject::deleteLater);
    connect(update_action, &QAction::triggered, [this, index = index] {
        auto product = recordAt(index);
        if(not product.has_value())
            return;

        InputProductDialog input_product{ m_categories, product.value().info(), this };
        if(input_product.exec() == QDialog::DialogCode::Accepted)
            this->updateProduct(index.row(), { product->id(), input_product.info().value() });
    });
    connect(remove_action, &QAction::triggered, [this, row = index.row()] {
        this->deleteProduct(row);
    });
    table_menu->popup(m_table_view->viewport()->mapToGlobal(position));
}

void ProductsView::updateProduct(uint64_t line, const ProductRecord& record) {
    m_table_model->setData(m_table_model->index(line, m_table_model->fieldIndex(s_field_name)), record.info().name());
    m_table_model->setData(m_table_model->index(line, m_table_model->fieldIndex(s_field_category)), record.info().category_id());
    m_table_model->setData(m_table_model->index(line, m_table_model->fieldIndex(s_field_some_date)), record.info().date());
    m_table_model->setData(m_table_model->index(line, m_table_model->fieldIndex(s_field_image_path)), record.info().image_path());
    m_table_model->submitAll();
    m_table_model->select();
}
void ProductsView::deleteProduct(uint64_t line) {
    m_table_model->removeRow(line);
    m_table_model->submitAll();
    m_table_model->select();
}
void ProductsView::saveReport() {
    auto save_path = QFileDialog::getSaveFileName(this);
    if(save_path.isEmpty())
        return;

    QPrinter printer;
    printer.setOutputFormat(QPrinter::OutputFormat::PdfFormat);
    printer.setOutputFileName(save_path);

    QPainter painter;
    painter.begin(&printer);

    QVector<QString> header_names;
    for(auto index = 0; index < m_table_model->columnCount(); ++index)
        header_names << m_table_model->headerData(index, Qt::Orientation::Horizontal).toString();

    TablePrinter{ &painter, &printer }.printTable(m_table_model, { 1, 3, 3, 3, 3 }, header_names);
    painter.end();
}




