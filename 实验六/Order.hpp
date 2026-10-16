#ifndef ORDER_HPP
#define ORDER_HPP

#include "Product.hpp"
#include <string>
#include <vector>
#include <ctime>

// 订单状态枚举
enum class OrderStatus {
    PENDING,    // 待发货
    SHIPPED,    // 已发货
    DELIVERED   // 已签收
};

class Order {
private:
    static int orderCounter;

    std::string orderId;
    std::vector<std::pair<Product, int>> items;
    std::time_t orderTime;
    double totalAmount;
    std::string shippingAddress;
    OrderStatus status;
    std::time_t shippedTime;
    std::time_t deliveredTime;

public:
    Order(const std::vector<std::pair<Product, int>>& itemList,
        const std::string& address);

    // 获取订单信息
    std::string getOrderId() const;
    std::time_t getOrderTime() const;
    double getTotalAmount() const;
    std::string getShippingAddress() const;
    OrderStatus getStatus() const;
    const std::vector<std::pair<Product, int>>& getItems() const;

    // 订单状态管理
    bool setStatus(OrderStatus newStatus);
    bool shipOrder();
    bool deliverOrder();

    // 订单修改
    bool updateShippingAddress(const std::string& newAddress);

    // 显示订单信息
    void displayOrderInfo() const;

    // 状态检查
    bool isPending() const;
    bool isShipped() const;
    bool isDelivered() const;

    // 静态方法
    static std::string timeToString(std::time_t time);
    static std::string statusToString(OrderStatus status);

private:
    static std::string generateOrderId();
    double calculateTotalAmount() const;
};

#endif