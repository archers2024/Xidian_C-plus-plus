
#ifndef FULL_REDUCTION_PROMOTION_HPP
#define FULL_REDUCTION_PROMOTION_HPP

#include "Promotion.hpp"
#include <string>

class FullReductionPromotion : public Promotion {
private:
    double threshold;  // 满减门槛，如5000.0
    double reduction;  // 减额，如500.0

public:
    // 构造函数
    FullReductionPromotion(std::string id, std::string name,
        double threshold, double reduction,
        std::time_t start, std::time_t end);

    // 检查是否满足满减条件
    bool isQualified(double amount) const;

    // 获取门槛和减额
    double getThreshold() const;
    double getReduction() const;

    // 实现基类纯虚函数
    std::string getPromotionTag() const override;

    // 满减描述
    std::string getFullReductionDescription() const;

    // 获取促销详情
    std::string getPromotionDetails() const;
};

#endif // FULL_REDUCTION_PROMOTION_HPP
