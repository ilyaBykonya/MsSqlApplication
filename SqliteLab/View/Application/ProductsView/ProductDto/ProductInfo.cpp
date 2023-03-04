#include "ProductInfo.h"

ProductInfo::ProductInfo(const QString& name, const QString& category, const QDate& date, const QString& image_path)
    :m_image_path{ image_path }, m_category{ category }, m_name{ name }, m_date{ date } {}
const QString &ProductInfo::image_path() const {
    return m_image_path;
}
const QString &ProductInfo::category() const {
    return m_category;
}
const QString &ProductInfo::name() const {
    return m_name;
}
const QDate &ProductInfo::date() const {
    return m_date;
}
