
#ifndef PROMOTION_MANAGER_HPP
#define PROMOTION_MANAGER_HPP

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

    // 折扣促销管理
    bool addDiscountPromotion(std::string id, std::string name,
        std::string productId, double discountRate,
        std::time_t start, std::time_t end);

    // 满减促销管理
    bool addFullReductionPromotion(std::string id, std::string name,
        double threshold, double reduction,
        std::time_t start, std::time_t end);

    // 促销查询
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

    // 查询功能
    bool hasPromotion(const std::string& productId) const;
};

#endif // PROMOTION_MANAGER_HPP
