#ifndef PROMOTIONMANAGER_HPP
#define PROMOTIONMANAGER_HPP

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <ctime>
#include "Promotion.hpp"
#include "DiscountPromotion.hpp"
#include "FullReductionPromotion.hpp"

// 前向声明
class Product;
class Order;

class PromotionManager {
private:
    std::vector<std::unique_ptr<Promotion>> allPromotions;
    std::unordered_map<std::string, DiscountPromotion*> productDiscounts;
    std::vector<FullReductionPromotion*> fullReductions;

    void cleanupExpiredPromotions();

public:
    PromotionManager();
    ~PromotionManager();

    // 添加促销活动
    bool addDiscountPromotion(std::string id, std::string name,
        std::string productId, double discountRate,
        std::time_t start, std::time_t end);

    bool addFullReductionPromotion(std::string id, std::string name,
        double threshold, double reduction,
        std::time_t start, std::time_t end);

    // 获取促销活动
    DiscountPromotion* getDiscountForProduct(const std::string& productId) const;
    std::vector<FullReductionPromotion*> getActiveFullReductions() const;

    // 价格计算
    double calculateProductPrice(const Product& product, int quantity) const;
    double calculateOrderTotal(const Order& order) const;

    // 促销管理
    bool removePromotion(const std::string& promotionId);
    void updatePromotionStatus();

    // 显示功能
    void displayAllPromotions() const;
    std::string getProductPromotionTag(const std::string& productId) const;
    bool hasPromotion(const std::string& productId) const;

    // ==== 新增：持久化支持 ====
    const std::vector<std::unique_ptr<Promotion>>& getAllPromotions() const {
        return allPromotions;
    }

    void clearAllPromotions() {
        allPromotions.clear();
        productDiscounts.clear();
        fullReductions.clear();
    }

    void addPromotionDirectly(std::unique_ptr<Promotion> promotion) {
        if (auto* discount = dynamic_cast<DiscountPromotion*>(promotion.get())) {
            productDiscounts[discount->getProductId()] = discount;
        }
        else if (auto* fullReduction = dynamic_cast<FullReductionPromotion*>(promotion.get())) {
            fullReductions.push_back(fullReduction);
        }
        allPromotions.push_back(std::move(promotion));
    }
};

#endif // PROMOTIONMANAGER_HPP