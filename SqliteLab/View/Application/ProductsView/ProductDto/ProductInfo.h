#pragma once
#include <QString>
#include <QDate>

class ProductInfo {
private:
    std::uint64_t m_category_id;
    QString m_image_path;
    QString m_name;
    QDateTime m_date;
public:
    ProductInfo(std::uint64_t category_id, const QString& name, const QDateTime& date, const QString& image_path);
    const QString& image_path() const;
    uint64_t category_id() const;
    const QString& name() const;
    const QDateTime& date() const;
};

