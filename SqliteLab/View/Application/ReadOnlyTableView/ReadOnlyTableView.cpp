#include "ReadOnlyTableView.h"
#include <QSqlTableModel>
#include <QVBoxLayout>
#include <QTableView>
#include <QPointer>
#include <QWidget>

ReadOnlyTableView::ReadOnlyTableView(const QSqlDatabase& connection, const QString& table, QWidget *parent)
    :QDialog{ parent } {
        auto table_model = new QSqlTableModel{ this, connection };
        auto table_view = new QTableView{};
        table_model->setTable(table);
        table_view->setEditTriggers(QTableView::EditTrigger::NoEditTriggers);
        table_view->setModel(table_model);
        table_model->select();

        auto window_layout = new QVBoxLayout;
        window_layout->addWidget(table_view);
        this->setLayout(window_layout);
    }
