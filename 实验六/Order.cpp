#include "Order.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

int Order::orderCounter = 1000;

// 构造函数
Order::Order(const std::vector<std::pair<Product, int>>& itemList,
    const std::string& address)
    : items(itemList), shippingAddress(address),
    status(OrderStatus::PENDING), shippedTime(0), deliveredTime(0) {

    orderId = generateOrderId();
    orderTime = std::time(nullptr);
    totalAmount = calculateTotalAmount();
}

std::string Order::generateOrderId() {
    std::stringstream ss;
    ss << "ORD" << std::setw(8) << std::setfill('0') << orderCounter++;
    return ss.str();
}

double Order::calculateTotalAmount() const {
    double total = 0.0;
    for (const auto& item : items) {
        total += item.first.getPrice() * item.second;
    }
    return total;
}

std::string Order::getOrderId() const { return orderId; }
std::time_t Order::getOrderTime() const { return orderTime; }
double Order::getTotalAmount() const { return totalAmount; }
std::string Order::getShippingAddress() const { return shippingAddress; }
OrderStatus Order::getStatus() const { return status; }
const std::vector<std::pair<Product, int>>& Order::getItems() const { return items; }

bool Order::setStatus(OrderStatus newStatus) {
    switch (status) {
    case OrderStatus::PENDING:
        if (newStatus == OrderStatus::SHIPPED) {
            status = newStatus;
            shippedTime = std::time(nullptr);
            return true;
        }
        break;
    case OrderStatus::SHIPPED:
        if (newStatus == OrderStatus::DELIVERED) {
            status = newStatus;
            deliveredTime = std::time(nullptr);
            return true;
        }
        break;
    case OrderStatus::DELIVERED:
        return false;
    }
    return false;
}

bool Order::shipOrder() {
    return setStatus(OrderStatus::SHIPPED);
}

bool Order::deliverOrder() {
    return setStatus(OrderStatus::DELIVERED);
}

bool Order::updateShippingAddress(const std::string& newAddress) {
    if (status == OrderStatus::PENDING) {
        shippingAddress = newAddress;
        return true;
    }
    return false;
}

void Order::displayOrderInfo() const {
    std::cout << "\n=== 订单详情 ===" << std::endl;
    std::cout << "订单编号: " << orderId << std::endl;
    std::cout << "下单时间: " << timeToString(orderTime) << std::endl;
    std::cout << "订单状态: " << statusToString(status) << std::endl;

    if (status == OrderStatus::SHIPPED) {
        std::cout << "发货时间: " << timeToString(shippedTime) << std::endl;
    }
    else if (status == OrderStatus::DELIVERED) {
        std::cout << "发货时间: " << timeToString(shippedTime) << std::endl;
        std::cout << "签收时间: " << timeToString(deliveredTime) << std::endl;
    }

    std::cout << "收货地址: " << shippingAddress << std::endl;
    std::cout << "订单总额: ￥" << std::fixed << std::setprecision(2) << totalAmount << std::endl;

    std::cout << "\n=== 商品列表 ===" << std::endl;
    std::cout << std::left << std::setw(20) << "商品名称"
        << std::setw(10) << "单价"
        << std::setw(8) << "数量"
        << std::setw(12) << "小计" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    for (const auto& item : items) {
        const Product& product = item.first;
        int quantity = item.second;
        double subtotal = product.getPrice() * quantity;

        std::cout << std::left << std::setw(20) << product.getName()
            << "￥" << std::setw(8) << std::fixed << std::setprecision(2) << product.getPrice()
            << std::setw(8) << quantity
            << "￥" << std::setw(10) << std::fixed << std::setprecision(2) << subtotal << std::endl;
    }
    std::cout << "--------------------------------------------------" << std::endl;
}

bool Order::isPending() const { return status == OrderStatus::PENDING; }
bool Order::isShipped() const { return status == OrderStatus::SHIPPED; }
bool Order::isDelivered() const { return status == OrderStatus::DELIVERED; }

std::string Order::timeToString(std::time_t time) {
    if (time == 0) return "未设置";

    char buffer[80];
    // 使用安全的 localtime_s 替代 localtime
    struct tm timeinfo;
    localtime_s(&timeinfo, &time);  // Windows 安全的版本
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return std::string(buffer);
}

std::string Order::statusToString(OrderStatus status) {
    switch (status) {
    case OrderStatus::PENDING: return "待发货";
    case OrderStatus::SHIPPED: return "已发货";
    case OrderStatus::DELIVERED: return "已签收";
    default: return "未知状态";
    }
}