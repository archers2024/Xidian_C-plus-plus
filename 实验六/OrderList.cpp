#include "OrderList.hpp"
#include <iostream>
#include <iomanip>

// 构造函数定义（只能有一个）
OrderList::OrderList() {
    // 构造函数体可以为空，或者初始化一些数据
}

// 创建订单
Order* OrderList::createOrder(const std::vector<std::pair<Product, int>>& itemList,
    const std::string& address) {
    if (itemList.empty()) {
        std::cout << "订单商品列表为空！" << std::endl;
        return nullptr;
    }

    if (address.empty()) {
        std::cout << "收货地址不能为空！" << std::endl;
        return nullptr;
    }

    Order newOrder(itemList, address);
    orders.push_back(newOrder);

    std::cout << "订单创建成功！订单号: " << newOrder.getOrderId() << std::endl;
    return &orders.back();
}

// ... 其他函数实现 ...

bool OrderList::removeOrder(const std::string& orderId) {
    for (auto it = orders.begin(); it != orders.end(); ++it) {
        if (it->getOrderId() == orderId) {
            if (it->isDelivered()) {
                orders.erase(it);
                std::cout << "订单删除成功！" << std::endl;
                return true;
            }
            else {
                std::cout << "只有已签收的订单可以删除！" << std::endl;
                return false;
            }
        }
    }

    std::cout << "订单不存在！" << std::endl;
    return false;
}

Order* OrderList::findOrder(const std::string& orderId) {
    for (auto& order : orders) {
        if (order.getOrderId() == orderId) {
            return &order;
        }
    }
    return nullptr;
}

std::vector<Order*> OrderList::getAllOrders() {
    std::vector<Order*> result;
    for (auto& order : orders) {
        result.push_back(&order);
    }
    return result;
}

std::vector<Order*> OrderList::getPendingOrders() {
    std::vector<Order*> result;
    for (auto& order : orders) {
        if (order.isPending()) {
            result.push_back(&order);
        }
    }
    return result;
}

std::vector<Order*> OrderList::getShippedOrders() {
    std::vector<Order*> result;
    for (auto& order : orders) {
        if (order.isShipped()) {
            result.push_back(&order);
        }
    }
    return result;
}

std::vector<Order*> OrderList::getDeliveredOrders() {
    std::vector<Order*> result;
    for (auto& order : orders) {
        if (order.isDelivered()) {
            result.push_back(&order);
        }
    }
    return result;
}

bool OrderList::shipOrder(const std::string& orderId) {
    Order* order = findOrder(orderId);
    if (!order) {
        std::cout << "订单不存在！" << std::endl;
        return false;
    }

    if (order->shipOrder()) {
        std::cout << "订单发货成功！" << std::endl;
        return true;
    }
    else {
        std::cout << "订单发货失败！" << std::endl;
        return false;
    }
}

bool OrderList::deliverOrder(const std::string& orderId) {
    Order* order = findOrder(orderId);
    if (!order) {
        std::cout << "订单不存在！" << std::endl;
        return false;
    }

    if (order->deliverOrder()) {
        std::cout << "订单签收成功！" << std::endl;
        return true;
    }
    else {
        std::cout << "订单签收失败！" << std::endl;
        return false;
    }
}

bool OrderList::cancelOrder(const std::string& orderId, ProductList& productList) {
    Order* order = findOrder(orderId);
    if (!order) {
        std::cout << "订单不存在！" << std::endl;
        return false;
    }

    if (!order->isPending()) {
        std::cout << "只有待发货的订单可以取消！" << std::endl;
        return false;
    }

    // 恢复库存
    const auto& items = order->getItems();
    for (const auto& item : items) {
        const Product& product = item.first;
        int quantity = item.second;
        productList.updateProductStock(product.getName(), quantity);
    }

    // 从订单列表中移除
    for (auto it = orders.begin(); it != orders.end(); ++it) {
        if (it->getOrderId() == orderId) {
            orders.erase(it);
            std::cout << "订单取消成功，库存已恢复！" << std::endl;
            return true;
        }
    }

    return false;
}

bool OrderList::updateOrderAddress(const std::string& orderId, const std::string& newAddress) {
    Order* order = findOrder(orderId);
    if (!order) {
        std::cout << "订单不存在！" << std::endl;
        return false;
    }

    if (order->updateShippingAddress(newAddress)) {
        std::cout << "收货地址修改成功！" << std::endl;
        return true;
    }
    else {
        std::cout << "只有待发货的订单可以修改地址！" << std::endl;
        return false;
    }
}

void OrderList::displayAllOrders() const {
    if (orders.empty()) {
        std::cout << "暂无订单！" << std::endl;
        return;
    }

    std::cout << "\n=== 所有订单列表 ===" << std::endl;
    std::cout << std::left << std::setw(12) << "订单号"
        << std::setw(20) << "下单时间"
        << std::setw(12) << "总额"
        << std::setw(10) << "状态" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    for (const auto& order : orders) {
        std::cout << std::left << std::setw(12) << order.getOrderId()
            << std::setw(20) << Order::timeToString(order.getOrderTime())
            << "￥" << std::setw(10) << std::fixed << std::setprecision(2) << order.getTotalAmount()
            << std::setw(10) << Order::statusToString(order.getStatus()) << std::endl;
    }
}

void OrderList::displayOrdersByStatus(OrderStatus status) const {
    std::string statusStr = Order::statusToString(status);
    std::cout << "\n=== " << statusStr << "订单 ===" << std::endl;

    bool found = false;
    for (const auto& order : orders) {
        if (order.getStatus() == status) {
            std::cout << order.getOrderId() << " | "
                << Order::timeToString(order.getOrderTime()) << " | "
                << "￥" << std::fixed << std::setprecision(2) << order.getTotalAmount()
                << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "暂无" << statusStr << "的订单！" << std::endl;
    }
}