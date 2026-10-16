
#include "PromotionManager.hpp"
#include "Product.hpp"
#include "Order.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <ctime>

PromotionManager::PromotionManager() {}

PromotionManager::~PromotionManager() {
    // unique_ptr会自动释放内存
}

bool PromotionManager::addDiscountPromotion(std::string id, std::string name,
    std::string productId, double discountRate,
    std::time_t start, std::time_t end) {

    // 检查时间有效性
    if (!Promotion::isTimeValid(start, end)) {
        return false;
    }

    // 创建折扣活动
    auto discount = std::make_unique<DiscountPromotion>(id, name, productId, discountRate, start, end);

    // 添加到商品折扣映射
    productDiscounts[productId] = static_cast<DiscountPromotion*>(discount.get());

    // 添加到所有促销列表
    allPromotions.push_back(std::move(discount));

    std::cout << "折扣活动添加成功: " << name << std::endl;
    return true;
}

bool PromotionManager::addFullReductionPromotion(std::string id, std::string name,
    double threshold, double reduction,
    std::time_t start, std::time_t end) {

    // 检查时间有效性
    if (!Promotion::isTimeValid(start, end)) {
        return false;
    }

    // 创建满减活动
    auto fullReduction = std::make_unique<FullReductionPromotion>(id, name, threshold, reduction, start, end);

    // 保存指针到满减列表
    FullReductionPromotion* frPtr = static_cast<FullReductionPromotion*>(fullReduction.get());
    fullReductions.push_back(frPtr);

    // 添加到所有促销列表
    allPromotions.push_back(std::move(fullReduction));

    std::cout << "满减活动添加成功: " << name << std::endl;
    return true;
}

DiscountPromotion* PromotionManager::getDiscountForProduct(const std::string& productId) const {
    auto it = productDiscounts.find(productId);
    if (it != productDiscounts.end() && it->second->isActiveNow()) {
        return it->second;
    }
    return nullptr;
}

std::vector<FullReductionPromotion*> PromotionManager::getActiveFullReductions() const {
    std::vector<FullReductionPromotion*> activeReductions;
    for (auto fr : fullReductions) {
        if (fr && fr->isActiveNow()) {
            activeReductions.push_back(fr);
        }
    }
    return activeReductions;
}

double PromotionManager::calculateProductPrice(const Product& product, int quantity) const {
    double originalPrice = product.getPrice() * quantity;

    // 查找商品折扣
    DiscountPromotion* discount = getDiscountForProduct(product.getName());
    if (discount) {
        return discount->calculateDiscount(originalPrice);
    }

    return originalPrice;
}

double PromotionManager::calculateOrderTotal(const Order& order) const {
    // 获取订单中的商品列表
    const auto& items = order.getItems();

    // ========== 第一步：计算商品折扣后总价 ==========
    double productDiscountedTotal = 0.0;

    for (const auto& item : items) {
        const Product& product = item.first;
        int quantity = item.second;

        // 计算每个商品的折扣后价格
        productDiscountedTotal += calculateProductPrice(product, quantity);
    }

    // ========== 第二步：应用满减优惠 ==========
    // 获取当前有效的满减活动
    auto activeReductions = getActiveFullReductions();
    if (activeReductions.empty()) {
        return productDiscountedTotal;  // 没有满减活动，返回商品折扣后总价
    }

    // 应用所有符合条件的满减活动中优惠最大的一个
    double bestReduction = 0.0;
    for (auto fr : activeReductions) {
        // 检查是否满足满减条件（基于商品折扣后的金额）
        if (fr->isQualified(productDiscountedTotal)) {
            double reduction = fr->getReduction();
            if (reduction > bestReduction) {
                bestReduction = reduction;
            }
        }
    }

    // 返回最终金额（商品折扣后总价 - 最优满减）
    return productDiscountedTotal - bestReduction;
}

bool PromotionManager::removePromotion(const std::string& promotionId) {
    for (auto it = allPromotions.begin(); it != allPromotions.end(); ++it) {
        if ((*it)->getPromotionId() == promotionId) {
            // 如果是折扣活动，从映射中移除
            DiscountPromotion* discount = dynamic_cast<DiscountPromotion*>(it->get());
            if (discount) {
                productDiscounts.erase(discount->getProductId());
            }

            // 如果是满减活动，从列表中移除
            FullReductionPromotion* fr = dynamic_cast<FullReductionPromotion*>(it->get());
            if (fr) {
                auto frIt = std::find(fullReductions.begin(), fullReductions.end(), fr);
                if (frIt != fullReductions.end()) {
                    fullReductions.erase(frIt);
                }
            }

            allPromotions.erase(it);
            std::cout << "促销活动删除成功: " << promotionId << std::endl;
            return true;
        }
    }

    std::cout << "未找到促销活动: " << promotionId << std::endl;
    return false;
}

void PromotionManager::updatePromotionStatus() {
    cleanupExpiredPromotions();
}

void PromotionManager::displayAllPromotions() const {
    if (allPromotions.empty()) {
        std::cout << "当前没有促销活动" << std::endl;
        return;
    }

    std::cout << "\n=== 所有促销活动 ===" << std::endl;
    for (const auto& promotion : allPromotions) {
        std::cout << "活动ID: " << promotion->getPromotionId() << std::endl;
        std::cout << "活动名称: " << promotion->getPromotionName() << std::endl;
        std::cout << "活动标签: " << promotion->getPromotionTag() << std::endl;
        std::cout << "活动状态: " << (promotion->isActiveNow() ? "进行中" : "已结束") << std::endl;
        std::cout << "------------------------" << std::endl;
    }
}

std::string PromotionManager::getProductPromotionTag(const std::string& productId) const {
    DiscountPromotion* discount = getDiscountForProduct(productId);
    if (discount) {
        return discount->getPromotionTag();
    }
    return "";
}

bool PromotionManager::hasPromotion(const std::string& productId) const {
    return getDiscountForProduct(productId) != nullptr;
}

void PromotionManager::cleanupExpiredPromotions() {
    std::time_t currentTime = std::time(nullptr);

    for (auto& promotion : allPromotions) {
        if (currentTime > promotion->getEndTime()) {
            promotion->setActive(false);
        }
    }
}
