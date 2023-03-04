#pragma once
#include "Utils/CategoriesListRepository/CategoriesListRepository.h"
#include "../EditProductWidget/EditProductWidget.h"
#include "../ProductDto/ProductRecord.h"
#include <QSqlQueryModel>
#include <QPushButton>
#include <QLineEdit>
#include <QPointer>
#include <optional>

class CurrentItemView : public QWidget {
    Q_OBJECT
private:
    std::optional<std::pair<std::uint64_t, ProductRecord>> m_record;
    QPointer<QLineEdit> m_current_id_view;
    QPointer<EditProductWidget> m_current_product;
    QPointer<QPushButton> m_update_button;
    QPointer<QPushButton> m_delete_button;
    QPointer<QPushButton> m_refresh_button;
public:
    CurrentItemView(QPointer<CategoriesListRepository> categories, QWidget* parent = nullptr);
public slots:
    void setCurrentRecord(std::uint64_t line, const ProductRecord& record);
private slots:
    void resetRecord(const std::optional<std::pair<std::uint64_t, ProductRecord>>& record);
private slots:
    void onUpdate();
    void onDelete();
    void onRefresh();
signals:
    void deleteRecord(std::uint64_t line);
    void updateRecord(std::uint64_t line, const ProductRecord& record);
};

