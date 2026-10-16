#include "FullReductionPromotion.hpp"
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

// 检查是否满足满减条件
bool FullReductionPromotion::isQualified(double amount) const {
    return amount >= threshold;
}

// 获取门槛
double FullReductionPromotion::getThreshold() const {
    return threshold;
}

// 获取减额
double FullReductionPromotion::getReduction() const {
    return reduction;
}

// 实现基类纯虚函数
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

    // 转换时间
    char startTimeStr[80];
    char endTimeStr[80];
    struct tm timeinfo;

    // 使用安全的时间转换函数
#ifdef _WIN32
    // Windows 使用 localtime_s
    localtime_s(&timeinfo, &startTime);
    strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);

    localtime_s(&timeinfo, &endTime);
    strftime(endTimeStr, sizeof(endTimeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
#else
    // Linux 使用 localtime_r
    localtime_r(&startTime, &timeinfo);
    strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);

    localtime_r(&endTime, &timeinfo);
    strftime(endTimeStr, sizeof(endTimeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
#endif

    ss << "促销ID: " << promotionId << "\n"
        << "促销名称: " << promotionName << "\n"
        << "满减门槛: ￥" << std::fixed << std::setprecision(2) << threshold << "\n"
        << "减额: ￥" << std::fixed << std::setprecision(2) << reduction << "\n"
        << "促销标签: " << getPromotionTag() << "\n"
        << "促销描述: " << getFullReductionDescription() << "\n"
        << "开始时间: " << startTimeStr << "\n"
        << "结束时间: " << endTimeStr << "\n"
        << "活动状态: " << (isActiveNow() ? "进行中" : "已结束");

    return ss.str();
}
