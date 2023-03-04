#pragma once
#include "Utils/CategoriesListRepository/CategoriesListRepository.h"
#include "InputProductDialog/InputProductDialog.h"
#include "CurrentItemView/CurrentItemView.h"
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
    static constexpr auto s_field_id = "ID";
    static constexpr auto s_field_name = "Name";
    static constexpr auto s_field_category = "catID";
    static constexpr auto s_field_image_path = "ImagePath";
    static constexpr auto s_field_some_date = "SomeDate";
private:
    QSqlDatabase m_connection;
    QPointer<CategoriesListRepository> m_categories;
    QPointer<QSqlTableModel> m_table_model;
    QPointer<CurrentItemView> m_current_item_view;
    QPointer<QTableView> m_table_view;
public:
    explicit ProductsView(QPointer<CategoriesListRepository> categories, const QSqlDatabase& connection, QWidget *parent = nullptr);
public slots:
    void setConnection(const QSqlDatabase& connection);
    void clearConnection();
private:
    std::optional<ProductRecord> recordAt(const QModelIndex& index);
private slots:
    void addProduct();
    void setCurrentProduct(const QModelIndex& index);
    void showContextMenuForTableView(const QPoint& position);
private slots:
    void updateProduct(std::uint64_t line, const ProductRecord& info);
    void deleteProduct(std::uint64_t line);
    void saveReport();
};

