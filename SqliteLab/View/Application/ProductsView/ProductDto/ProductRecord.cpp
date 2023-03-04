#include "ProductRecord.h"


ProductRecord::ProductRecord(uint64_t id, const ProductInfo &info)
    :m_id{ id }, m_info{ info } {}

const ProductInfo &ProductRecord::info() const {
    return m_info;
}

uint64_t ProductRecord::id() const {
    return m_id;
}
