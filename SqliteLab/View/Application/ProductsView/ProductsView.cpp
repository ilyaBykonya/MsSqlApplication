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

ProductsView::ProductsView(const QSqlDatabase& connection, QWidget *parent)
    :QWidget{ parent },
     m_table_model{ },
     m_current_item_view{ new CurrentItemView },
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
void ProductsView::showContextMenuForTableView(const QPoint &position) {
    qDebug() << "Request context menu for => " << position;
    if(m_table_model == nullptr)
        return;

    auto index = m_table_view->indexAt(position);
    if(not index.isValid())
        return;

    auto table_menu = new QMenu;
    auto update_action = table_menu->addAction("Update");
    auto remove_action = table_menu->addAction("Remove");
    connect(table_menu, &QMenu::aboutToHide, table_menu, &QObject::deleteLater);
    connect(update_action, &QAction::triggered, [this, row = index.row()] {
        InputProductDialog input_product;
        if(input_product.exec() == QDialog::DialogCode::Accepted)
            this->updateProduct(row, {}, input_product.name(), input_product.category());
    });
    connect(remove_action, &QAction::triggered, [this, row = index.row()] {
        this->deleteProduct(row, {});
    });
    table_menu->popup(m_table_view->viewport()->mapToGlobal(position));
}

void ProductsView::updateProduct(uint64_t line, uint64_t/* id*/, const QString &name, const QString &category) {
    m_table_model->setData(m_table_model->index(line, m_table_model->fieldIndex(s_field_name)), name);
    m_table_model->setData(m_table_model->index(line, m_table_model->fieldIndex(s_field_category)), category);
    m_table_model->submitAll();
    m_table_model->select();
}
void ProductsView::deleteProduct(uint64_t line, uint64_t/* id*/) {
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

    TablePrinter{ &painter, &printer }.printTable(m_table_model, { 1, 3, 3 }, header_names);
    painter.end();
}




