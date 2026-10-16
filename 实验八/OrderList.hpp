#ifndef ORDERLIST_HPP
#define ORDERLIST_HPP

#include <vector>
#include <string>
#include "Order.hpp"
#include "ProductList.hpp"

// 前向声明
class PromotionManager;

class OrderList {
private:
    std::vector<Order> orders;

public:
    OrderList();

    // 订单管理
    Order* createOrder(const std::vector<std::pair<Product, int>>& itemList,
        const std::string& address, const PromotionManager* pm);
    Order* createOrder(const std::vector<std::pair<Product, int>>& itemList,
        const std::string& address, double promoTotal); // 兼容性重载

    bool removeOrder(const std::string& orderId);
    Order* findOrder(const std::string& orderId);
    const Order* findOrder(const std::string& orderId) const;

    // 获取订单列表
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

    // 促销应用
    void applyPromotionsToAllOrders(const PromotionManager* pm);

    // 统计功能
    OrderStats getOrderStats() const;
    void displayStats() const;

    // 批量操作
    bool batchShipOrders(const std::vector<std::string>& orderIds);

    // 搜索功能
    std::vector<Order*> searchOrders(const std::string& keyword);
    std::vector<Order*> getOrdersByTimeRange(time_t startTime, time_t endTime);

    // 导入导出
    bool exportToFile(const std::string& filename) const;
    bool importFromFile(const std::string& filename);

    // ==== 新增：持久化支持 ====
    const std::vector<Order>& getOrders() const { return orders; }
    std::vector<Order>& getOrders() { return orders; }

    void clearOrders() { orders.clear(); }
    void addOrderDirectly(const Order& order) {
        orders.push_back(order);
    }

    // 直接设置订单（用于加载数据时）
    void setOrders(const std::vector<Order>& newOrders) {
        orders = newOrders;
    }
};

#endif // ORDERLIST_HPP