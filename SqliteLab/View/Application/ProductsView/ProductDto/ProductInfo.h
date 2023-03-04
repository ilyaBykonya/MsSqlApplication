#pragma once
#include <QString>
#include <QDate>

class ProductInfo {
private:
    QString m_image_path;
    QString m_category;
    QString m_name;
    QDate m_date;
public:
    ProductInfo(const QString& name, const QString& category, const QDate& date, const QString& image_path);
    const QString& image_path() const;
    const QString& category() const;
    const QString& name() const;
    const QDate& date() const;
};

