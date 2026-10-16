#ifndef DISCOUNT_PROMOTION_HPP
#define DISCOUNT_PROMOTION_HPP

#include "Promotion.hpp"
#include <string>

class DiscountPromotion : public Promotion {
private:
    std::string productId;
    double discountRate;  // 折扣率，如0.85表示85折

public:
    // 构造函数
    DiscountPromotion(std::string id, std::string name,
        std::string productId, double discountRate,
        std::time_t start, std::time_t end);

    // 计算折扣后价格
    double calculateDiscount(double originalPrice) const;

    // 计算折扣金额
    double calculateDiscountAmount(double originalPrice) const;

    // 获取商品ID
    std::string getProductId() const;

    // 获取折扣率
    double getDiscountRate() const;

    // 实现基类纯虚函数
    std::string getPromotionTag() const override;

    // 折扣描述
    std::string getDiscountDescription() const;

    // 获取促销详情
    std::string getPromotionDetails() const;
};

#endif // DISCOUNT_PROMOTION_HPP
