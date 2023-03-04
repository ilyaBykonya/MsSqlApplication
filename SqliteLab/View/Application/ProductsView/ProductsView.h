#pragma once
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QPushButton>
#include <QTableView>
#include <QPointer>
#include <QWidget>

class ProductsView : public QWidget {
    Q_OBJECT
private:
    static constexpr auto s_table_name = "product";
    QPointer<QSqlTableModel> m_table_model;
    QPointer<QTableView> m_table_view;
    QSqlDatabase m_connection;
public:
    explicit ProductsView(const QSqlDatabase& connection, QWidget *parent = nullptr);
public slots:
    void setConnection(const QSqlDatabase& connection);
    void clearConnection();
private slots:
    void addProduct();
};

