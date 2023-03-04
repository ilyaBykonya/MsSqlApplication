#pragma once
#include "../EditProductWidget/EditProductWidget.h"
#include <QPointer>
#include <QDialog>

class InputProductDialog : public QDialog {
    Q_OBJECT
private:
    QPointer<EditProductWidget> m_input;
public:
    InputProductDialog(QPointer<CategoriesListRepository> categories, const std::optional<ProductInfo>& origin = {}, QWidget* parent = nullptr);
    std::optional<ProductInfo> info() const;
};


