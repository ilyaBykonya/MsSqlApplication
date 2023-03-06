#include "ProductInfo.h"

ProductInfo::ProductInfo(std::uint64_t category_id, const QString& name, const QDateTime& date, const QString& image_path)
    :m_category_id{ category_id },m_image_path{ image_path }, m_name{ name }, m_date{ date } {}
const QString &ProductInfo::image_path() const {
    return m_image_path;
}
uint64_t ProductInfo::category_id() const {
    return m_category_id;
}
const QString &ProductInfo::name() const {
    return m_name;
}
const QDateTime &ProductInfo::date() const {
    return m_date;
}

