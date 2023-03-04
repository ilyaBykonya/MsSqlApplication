#pragma once
#include "Utils/CategoriesListRepository/CategoriesListRepository.h"
#include "ProductsView/ProductsView.h"
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QPushButton>
#include <QTableView>
#include <QPointer>
#include <QWidget>

class Application : public QWidget {
    Q_OBJECT
private:
    QSqlDatabase m_connection;
    QPointer<CategoriesListRepository> m_categories;
    QPointer<ProductsView> m_products;
public:
    explicit Application(QWidget *parent = nullptr);
private slots:
    void connectToDatabase();
    void showScoresView();
    void showTableView();
};

