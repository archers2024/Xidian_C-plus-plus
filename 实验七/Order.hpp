
#ifndef ORDER_HPP
#define ORDER_HPP

#include <vector>
#include <string>
#include <ctime>
#include "Product.hpp"

// 前向声明
class PromotionManager;

enum class OrderStatus {
    PENDING,    // 待发货
    SHIPPED,    // 已发货
    DELIVERED   // 已签收
};

class Order {
private:
    static int orderCounter;

    std::string orderId;
    std::time_t orderTime;
    double totalAmount;
    double discountedAmount;
    std::string shippingAddress;
    OrderStatus status;
    std::time_t shippedTime;
    std::time_t deliveredTime;
    std::vector<std::pair<Product, int>> items;

    // 私有辅助方法
    std::string generateOrderId();
    double calculateTotalAmount();
    bool setStatus(OrderStatus newStatus);

public:
    // 构造函数
    Order(const std::vector<std::pair<Product, int>>& itemList,
        const std::string& address);

    Order(const std::vector<std::pair<Product, int>>& itemList,
        const std::string& address, const PromotionManager* pm);

    // Getter方法
    std::string getOrderId() const;
    std::time_t getOrderTime() const;
    double getTotalAmount() const;
    double getDiscountedAmount() const;
    std::string getShippingAddress() const;
    OrderStatus getStatus() const;
    const std::vector<std::pair<Product, int>>& getItems() const;

    // 状态操作方法
    bool shipOrder();
    bool deliverOrder();
    bool updateShippingAddress(const std::string& newAddress);

    // 状态检查方法
    bool isPending() const;
    bool isShipped() const;
    bool isDelivered() const;

    // 显示功能
    void displayOrderInfo() const;
    void displayOrderInfoWithPromotion(const PromotionManager* pm) const;

    // 新增：显示促销摘要（修复的关键函数）
    void displayPromotionSummary(const PromotionManager* pm) const;

    // 促销相关方法
    void applyPromotions(const PromotionManager* pm);

    // 静态工具函数
    static std::string timeToString(std::time_t time);
    static std::string statusToString(OrderStatus status);
};

#endif // ORDER_HPP
