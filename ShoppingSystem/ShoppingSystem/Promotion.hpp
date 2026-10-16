
#ifndef PROMOTION_HPP
#define PROMOTION_HPP

#include <string>
#include <ctime>

class Promotion {
protected:
    std::string promotionId;
    std::string promotionName;
    std::time_t startTime;
    std::time_t endTime;
    bool isActive;

public:
    // 构造函数
    Promotion(std::string id, std::string name,
        std::time_t start, std::time_t end);

    // 虚析构函数
    virtual ~Promotion() = default;

    // 促销有效性检查
    bool isValid() const;

    // 检查促销是否正在进行中
    bool isActiveNow() const;

    // 获取促销信息
    std::string getPromotionId() const;
    std::string getPromotionName() const;
    std::time_t getStartTime() const;
    std::time_t getEndTime() const;

    // 获取促销标签（纯虚函数）
    virtual std::string getPromotionTag() const = 0;

    // 设置激活状态
    void setActive(bool active);

    // 静态方法：检查时间有效性
    static bool isTimeValid(std::time_t start, std::time_t end);
};

#endif // PROMOTION_HPP
