#include "FullReductionPromotion.hpp"
#include "TimeUtils.hpp"  // 新增
#include <sstream>
#include <iomanip>
#include <ctime>

// 构造函数
FullReductionPromotion::FullReductionPromotion(std::string id, std::string name,
    double threshold, double reduction,
    std::time_t start, std::time_t end)
    : Promotion(id, name, start, end),
    threshold(threshold), reduction(reduction) {
}

// 检查是否满足条件
bool FullReductionPromotion::isQualified(double amount) const {
    return amount >= threshold;
}

// 获取门槛
double FullReductionPromotion::getThreshold() const {
    return threshold;
}

// 获取减免金额
double FullReductionPromotion::getReduction() const {
    return reduction;
}

// 实现基类虚函数
std::string FullReductionPromotion::getPromotionTag() const {
    return "满减优惠";
}

// 满减描述
std::string FullReductionPromotion::getFullReductionDescription() const {
    std::stringstream ss;
    ss << "满" << std::fixed << std::setprecision(2) << threshold
        << "减" << std::fixed << std::setprecision(2) << reduction;
    return ss.str();
}

// 获取促销详情
std::string FullReductionPromotion::getPromotionDetails() const {
    std::stringstream ss;

    // 使用安全的时间转换函数
    std::string startTimeStr = TimeUtils::timeToString(startTime);  // 修改
    std::string endTimeStr = TimeUtils::timeToString(endTime);      // 修改

    ss << "促销ID: " << promotionId << "\n"
        << "促销名称: " << promotionName << "\n"
        << "满减门槛: ￥" << std::fixed << std::setprecision(2) << threshold << "\n"
        << "减免金额: ￥" << std::fixed << std::setprecision(2) << reduction << "\n"
        << "促销标签: " << getPromotionTag() << "\n"
        << "促销描述: " << getFullReductionDescription() << "\n"
        << "开始时间: " << startTimeStr << "\n"
        << "结束时间: " << endTimeStr << "\n"
        << "活动状态: " << (isActiveNow() ? "进行中" : "已结束");

    return ss.str();
}