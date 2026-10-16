#include "DiscountPromotion.hpp"
#include "TimeUtils.hpp"  // 新增
#include <sstream>
#include <iomanip>
#include <ctime>

// 构造函数
DiscountPromotion::DiscountPromotion(std::string id, std::string name,
    std::string productId, double discountRate,
    std::time_t start, std::time_t end)
    : Promotion(id, name, start, end),
    productId(productId), discountRate(discountRate) {
}

// 计算折扣后价格
double DiscountPromotion::calculateDiscount(double originalPrice) const {
    if (discountRate <= 0.0 || discountRate > 1.0) {
        return originalPrice;
    }
    return originalPrice * discountRate;
}

// 计算折扣金额
double DiscountPromotion::calculateDiscountAmount(double originalPrice) const {
    if (discountRate <= 0.0 || discountRate > 1.0) {
        return 0.0;
    }
    return originalPrice * (1.0 - discountRate);
}

// 获取商品ID
std::string DiscountPromotion::getProductId() const {
    return productId;
}

// 获取折扣率
double DiscountPromotion::getDiscountRate() const {
    return discountRate;
}

// 实现基类虚函数
std::string DiscountPromotion::getPromotionTag() const {
    return "商品折扣";
}

// 折扣描述
std::string DiscountPromotion::getDiscountDescription() const {
    if (discountRate <= 0.0 || discountRate > 1.0) {
        return "无效折扣";
    }

    int discountPercent = static_cast<int>((1.0 - discountRate) * 100);
    return "立减" + std::to_string(discountPercent) + "%";
}

// 获取促销详情
std::string DiscountPromotion::getPromotionDetails() const {
    std::stringstream ss;

    // 使用安全的时间转换函数
    std::string startTimeStr = TimeUtils::timeToString(startTime);  // 修改
    std::string endTimeStr = TimeUtils::timeToString(endTime);      // 修改

    ss << "促销ID: " << promotionId << "\n"
        << "促销名称: " << promotionName << "\n"
        << "商品ID: " << productId << "\n"
        << "折扣率: " << std::fixed << std::setprecision(2) << (discountRate * 100) << "%\n"
        << "促销标签: " << getPromotionTag() << "\n"
        << "促销描述: " << getDiscountDescription() << "\n"
        << "开始时间: " << startTimeStr << "\n"
        << "结束时间: " << endTimeStr << "\n"
        << "活动状态: " << (isActiveNow() ? "进行中" : "已结束");

    return ss.str();
}