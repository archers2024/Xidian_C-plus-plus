#ifndef ORDERLIST_HPP
#define ORDERLIST_HPP

#include "Order.hpp"
#include "ProductList.hpp"
#include <vector>
#include <string>

class OrderList {
private:
    std::vector<Order> orders;

public:
    // 构造函数声明
    OrderList();  // 只有声明，没有默认实现

    // 订单管理
    Order* createOrder(const std::vector<std::pair<Product, int>>& itemList,
        const std::string& address);
    bool removeOrder(const std::string& orderId);

    // 订单查询
    Order* findOrder(const std::string& orderId);
    std::vector<Order*> getAllOrders();

    // 按状态查询
    std::vector<Order*> getPendingOrders();
    std::vector<Order*> getShippedOrders();
    std::vector<Order*> getDeliveredOrders();

    // 订单状态管理
    bool shipOrder(const std::string& orderId);
    bool deliverOrder(const std::string& orderId);
    bool cancelOrder(const std::string& orderId, ProductList& productList);

    // 订单修改
    bool updateOrderAddress(const std::string& orderId, const std::string& newAddress);

    // 显示功能
    void displayAllOrders() const;
    void displayOrdersByStatus(OrderStatus status) const;
};

#endif