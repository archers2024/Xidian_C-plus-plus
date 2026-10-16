#include "Promotion.hpp"
#include <ctime>
#include <iostream>

Promotion::Promotion(std::string id, std::string name,
    std::time_t start, std::time_t end)
    : promotionId(id), promotionName(name),
    startTime(start), endTime(end), isActive(true) {
}

bool Promotion::isValid() const {
    return isTimeValid(startTime, endTime);
}

bool Promotion::isActiveNow() const {
    if (!isActive) return false;

    std::time_t currentTime = std::time(nullptr);
    return (currentTime >= startTime && currentTime <= endTime);
}

std::string Promotion::getPromotionId() const {
    return promotionId;
}

std::string Promotion::getPromotionName() const {
    return promotionName;
}

std::time_t Promotion::getStartTime() const {
    return startTime;
}

std::time_t Promotion::getEndTime() const {
    return endTime;
}

void Promotion::setActive(bool active) {
    isActive = active;
}

bool Promotion::isTimeValid(std::time_t start, std::time_t end) {
    if (start >= end) {
        std::cout << "促销开始时间必须早于结束时间！" << std::endl;
        return false;
    }

    std::time_t currentTime = std::time(nullptr);
    if (end < currentTime) {
        std::cout << "促销结束时间不能早于当前时间！" << std::endl;
        return false;
    }

    return true;
}