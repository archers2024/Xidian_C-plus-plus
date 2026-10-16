#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>
#include <vector>
#include <ctime>
#include "Product.hpp"

// 前向声明
class PromotionManager;

enum class OrderStatus {
    PENDING,
    SHIPPED,
    DELIVERED
};

struct OrderStats {
    int totalOrders;
    int pendingCount;
    int shippedCount;
    int deliveredCount;
    double totalRevenue;
};

class Order {
private:
    std::string orderId;
    std::time_t orderTime;
    std::vector<std::pair<Product, int>> items;
    std::string shippingAddress;
    OrderStatus status;
    std::time_t shippedTime;
    std::time_t deliveredTime;
    double totalAmount;
    double discountedAmount;

    static int orderCounter;

    // 私有方法
    std::string generateOrderId();
    double calculateTotalAmount();
    bool setStatusInternal(OrderStatus newStatus);  // 修改：重命名私有方法

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

    // Setter方法
    void setOrderId(const std::string& id);
    void setOrderTime(std::time_t time);
    void setTotalAmount(double amount);
    void setDiscountedAmount(double amount);
    void setShippingAddress(const std::string& address);
    void setStatus(OrderStatus newStatus);  // 公共的setter，用于持久化
    void setShippedTime(std::time_t time);
    void setDeliveredTime(std::time_t time);
    void setItems(const std::vector<std::pair<Product, int>>& newItems);

    // 订单操作
    bool shipOrder();
    bool deliverOrder();
    bool updateShippingAddress(const std::string& newAddress);

    // 状态检查
    bool isPending() const;
    bool isShipped() const;
    bool isDelivered() const;

    // 显示功能
    void displayOrderInfo() const;
    void displayOrderInfoWithPromotion(const PromotionManager* pm) const;
    void displayPromotionSummary(const PromotionManager* pm) const;

    // 促销相关
    void applyPromotions(const PromotionManager* pm);

    // 静态工具函数
    static std::string timeToString(std::time_t time);
    static std::string statusToString(OrderStatus status);

    // 序列化方法
    std::string serialize() const;
    static Order deserialize(const std::string& data);
};

#endif // ORDER_HPP