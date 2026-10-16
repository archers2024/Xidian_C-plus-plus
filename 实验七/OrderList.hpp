
#ifndef ORDER_LIST_HPP
#define ORDER_LIST_HPP

#include <vector>
#include <string>
#include "Order.hpp"
#include "Product.hpp"
#include "ProductList.hpp"

struct OrderStats {
    int totalOrders = 0;
    int pendingCount = 0;
    int shippedCount = 0;
    int deliveredCount = 0;
    double totalRevenue = 0.0;
};

class OrderList {
private:
    std::vector<Order> orders;

public:
    OrderList();

    // 订单创建（主要方法）
    Order* createOrder(const std::vector<std::pair<Product, int>>& itemList,
        const std::string& address,
        const PromotionManager* pm);

    // 兼容性版本
    Order* createOrder(const std::vector<std::pair<Product, int>>& itemList,
        const std::string& address,
        double promoTotal);

    // 订单管理
    bool removeOrder(const std::string& orderId);
    Order* findOrder(const std::string& orderId);
    const Order* findOrder(const std::string& orderId) const;

    // 订单查询
    std::vector<Order*> getAllOrders();
    std::vector<Order*> getPendingOrders();
    std::vector<Order*> getShippedOrders();
    std::vector<Order*> getDeliveredOrders();

    // 订单操作
    bool shipOrder(const std::string& orderId);
    bool deliverOrder(const std::string& orderId);
    bool cancelOrder(const std::string& orderId, ProductList& productList);
    bool updateOrderAddress(const std::string& orderId, const std::string& newAddress);

    // 显示功能
    void displayAllOrders() const;
    void displayOrdersByStatus(OrderStatus status) const;
    void displayOrderDetails(const std::string& orderId) const;
    void displayOrderDetailsWithPromotion(const std::string& orderId, const PromotionManager* pm) const;

    // 统计功能
    void displayStats() const;
    OrderStats getOrderStats() const;

    // 批量操作
    bool batchShipOrders(const std::vector<std::string>& orderIds);

    // 搜索功能
    std::vector<Order*> searchOrders(const std::string& keyword);
    std::vector<Order*> getOrdersByTimeRange(time_t startTime, time_t endTime);

    // 促销相关
    void applyPromotionsToAllOrders(const PromotionManager* pm);

    // 文件操作
    bool exportToFile(const std::string& filename) const;
    bool importFromFile(const std::string& filename);
};

#endif // ORDER_LIST_HPP
