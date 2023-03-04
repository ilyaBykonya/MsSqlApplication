#pragma once
#include "../ProductDto/ProductInfo.h"

class ProductRecord {
private:
    std::uint64_t m_id;
    ProductInfo m_info;
public:
    ProductRecord(std::uint64_t id, const ProductInfo& info);
    const ProductInfo& info() const;
    std::uint64_t id() const;
};

