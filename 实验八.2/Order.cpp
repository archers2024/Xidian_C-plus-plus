#include "Order.hpp"
#include "PromotionManager.hpp"
#include "TimeUtils.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

// 静态成员初始化
int Order::orderCounter = 1000;

// ========== 构造函数 ==========

Order::Order(const std::vector<std::pair<Product, int>>& itemList,
    const std::string& address)
    : items(itemList), shippingAddress(address),
    status(OrderStatus::PENDING), shippedTime(0), deliveredTime(0),
    discountedAmount(0.0) {

    orderId = generateOrderId();
    orderTime = TimeUtils::getCurrentTime();
    totalAmount = calculateTotalAmount();
    discountedAmount = totalAmount;  // 初始时等于原价
}

Order::Order(const std::vector<std::pair<Product, int>>& itemList,
    const std::string& address, const PromotionManager* pm)
    : items(itemList), shippingAddress(address),
    status(OrderStatus::PENDING), shippedTime(0), deliveredTime(0),
    discountedAmount(0.0) {

    orderId = generateOrderId();
    orderTime = TimeUtils::getCurrentTime();
    totalAmount = calculateTotalAmount();

    if (pm) {
        applyPromotions(pm);
    }
    else {
        discountedAmount = totalAmount;
    }
}

// ========== 私有辅助方法 ==========

std::string Order::generateOrderId() {
    std::stringstream ss;
    ss << "ORD" << std::setw(8) << std::setfill('0') << orderCounter++;
    return ss.str();
}

double Order::calculateTotalAmount() {
    double total = 0.0;

    for (size_t i = 0; i < items.size(); i++) {
        const Product& product = items[i].first;
        int quantity = items[i].second;
        total += product.getPrice() * quantity;
    }

    return total;
}

// 修改：重命名为setStatusInternal，避免冲突
bool Order::setStatusInternal(OrderStatus newStatus) {
    switch (status) {
    case OrderStatus::PENDING:
        if (newStatus == OrderStatus::SHIPPED) {
            status = newStatus;
            shippedTime = TimeUtils::getCurrentTime();
            return true;
        }
        break;
    case OrderStatus::SHIPPED:
        if (newStatus == OrderStatus::DELIVERED) {
            status = newStatus;
            deliveredTime = TimeUtils::getCurrentTime();
            return true;
        }
        break;
    case OrderStatus::DELIVERED:
        return false;  // 已签收的订单不能再改变状态
    }
    return false;
}

// ========== Getter方法 ==========

std::string Order::getOrderId() const { return orderId; }
std::time_t Order::getOrderTime() const { return orderTime; }
double Order::getTotalAmount() const { return totalAmount; }
double Order::getDiscountedAmount() const { return discountedAmount; }
std::string Order::getShippingAddress() const { return shippingAddress; }
OrderStatus Order::getStatus() const { return status; }
const std::vector<std::pair<Product, int>>& Order::getItems() const { return items; }

// ========== Setter方法 ==========

void Order::setOrderId(const std::string& id) { orderId = id; }
void Order::setOrderTime(std::time_t time) { orderTime = time; }
void Order::setTotalAmount(double amount) { totalAmount = amount; }
void Order::setDiscountedAmount(double amount) { discountedAmount = amount; }
void Order::setShippingAddress(const std::string& address) { shippingAddress = address; }

// 公共的setStatus方法（用于持久化）
void Order::setStatus(OrderStatus newStatus) {
    status = newStatus;
}

void Order::setShippedTime(std::time_t time) { shippedTime = time; }
void Order::setDeliveredTime(std::time_t time) { deliveredTime = time; }
void Order::setItems(const std::vector<std::pair<Product, int>>& newItems) {
    items = newItems;
}

// ========== 订单操作 ==========

bool Order::shipOrder() {
    return setStatusInternal(OrderStatus::SHIPPED);
}

bool Order::deliverOrder() {
    return setStatusInternal(OrderStatus::DELIVERED);
}

bool Order::updateShippingAddress(const std::string& newAddress) {
    if (status == OrderStatus::PENDING) {
        shippingAddress = newAddress;
        return true;
    }
    return false;
}

// ========== 状态检查方法 ==========

bool Order::isPending() const { return status == OrderStatus::PENDING; }
bool Order::isShipped() const { return status == OrderStatus::SHIPPED; }
bool Order::isDelivered() const { return status == OrderStatus::DELIVERED; }

// ========== 显示功能 ==========

void Order::displayOrderInfo() const {
    std::cout << "\n== 订单详情 ==" << std::endl;
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

    if (discountedAmount < totalAmount) {
        std::cout << "折后金额: ￥" << std::fixed << std::setprecision(2) << discountedAmount
            << "（节省: ￥" << std::fixed << std::setprecision(2) << (totalAmount - discountedAmount) << "）" << std::endl;
    }
    else {
        std::cout << "折后金额: ￥" << std::fixed << std::setprecision(2) << discountedAmount << std::endl;
    }

    std::cout << "\n=== 商品列表 ===" << std::endl;
    std::cout << std::left << std::setw(20) << "商品名称"
        << std::setw(10) << "单价"
        << std::setw(8) << "数量"
        << std::setw(12) << "小计" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    for (size_t i = 0; i < items.size(); i++) {
        const Product& product = items[i].first;
        int quantity = items[i].second;
        double subtotal = product.getPrice() * quantity;

        std::cout << std::left << std::setw(20) << product.getName()
            << "￥" << std::setw(8) << std::fixed << std::setprecision(2) << product.getPrice()
            << std::setw(8) << quantity
            << "￥" << std::setw(10) << std::fixed << std::setprecision(2) << subtotal << std::endl;
    }
    std::cout << "--------------------------------------------------" << std::endl;
}

void Order::displayOrderInfoWithPromotion(const PromotionManager* pm) const {
    std::cout << "\n=== 订单详情（含促销信息） ===" << std::endl;
    std::cout << "订单编号: " << orderId << std::endl;
    std::cout << "下单时间: " << timeToString(orderTime) << std::endl;
    std::cout << "订单状态: " << statusToString(status) << std::endl;

    std::cout << "收货地址: " << shippingAddress << std::endl;
    std::cout << "订单原价: ￥" << std::fixed << std::setprecision(2) << totalAmount << std::endl;
    std::cout << "折后金额: ￥" << std::fixed << std::setprecision(2) << discountedAmount
        << " (节省: ￥" << std::fixed << std::setprecision(2) << (totalAmount - discountedAmount) << ")" << std::endl;

    std::cout << "\n=== 商品列表（含促销） ===" << std::endl;
    std::cout << std::left << std::setw(20) << "商品名称"
        << std::setw(15) << "单价/折后价"
        << std::setw(8) << "数量"
        << std::setw(12) << "小计"
        << std::setw(15) << "促销标签" << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;

    for (size_t i = 0; i < items.size(); i++) {
        const Product& product = items[i].first;
        int quantity = items[i].second;
        double originalPrice = product.getPrice();
        double subtotal = originalPrice * quantity;

        std::string promotionTag = "";
        if (pm) {
            promotionTag = product.getPromotionTag(pm);
        }

        std::cout << std::left << std::setw(20) << product.getName();

        if (!promotionTag.empty()) {
            std::cout << "原价:￥" << std::setw(8) << std::fixed << std::setprecision(2) << originalPrice;
        }
        else {
            std::cout << "单价:￥" << std::setw(8) << std::fixed << std::setprecision(2) << originalPrice;
        }

        std::cout << std::setw(8) << quantity
            << "￥" << std::setw(10) << std::fixed << std::setprecision(2) << subtotal
            << std::setw(15) << (promotionTag.empty() ? "无" : promotionTag) << std::endl;
    }
    std::cout << "----------------------------------------------------------------------" << std::endl;
}

// ========== 新增：显示促销摘要方法 ==========
void Order::displayPromotionSummary(const PromotionManager* pm) const {
    std::cout << "\n== 订单促销信息 ==" << std::endl;
    std::cout << "购物车总额：￥" << std::fixed << std::setprecision(2) << totalAmount << std::endl;

    double totalProductDiscount = 0.0;
    for (size_t i = 0; i < items.size(); i++) {
        const Product& product = items[i].first;
        int quantity = items[i].second;
        double originalPrice = product.getPrice() * quantity;

        if (pm) {
            DiscountPromotion* discount = pm->getDiscountForProduct(product.getName());
            if (discount) {
                double discountAmount = discount->calculateDiscountAmount(originalPrice);
                totalProductDiscount += discountAmount;
                std::cout << "   " << product.getName() << " x" << quantity
                    << ": 商品折扣￥" << std::fixed << std::setprecision(2)
                    << discountAmount << std::endl;
            }
        }
    }

    double fullReductionAmount = 0.0;
    std::string fullReductionDesc = "";
    if (pm) {
        auto activeReductions = pm->getActiveFullReductions();
        double productDiscountedTotal = totalAmount - totalProductDiscount;

        double bestReduction = 0.0;
        FullReductionPromotion* bestPromotion = nullptr;
        for (auto fr : activeReductions) {
            if (fr->isQualified(productDiscountedTotal)) {
                double reduction = fr->getReduction();
                if (reduction > bestReduction) {
                    bestReduction = reduction;
                    bestPromotion = fr;
                }
            }
        }
        fullReductionAmount = bestReduction;

        if (fullReductionAmount > 0.0 && bestPromotion) {
            fullReductionDesc = "满" + std::to_string(static_cast<int>(bestPromotion->getThreshold())) +
                ".00减" + std::to_string(static_cast<int>(bestPromotion->getReduction())) + ".00";
            std::cout << "满减优惠：满减：" << fullReductionDesc
                << "（-￥" << std::fixed << std::setprecision(2)
                << fullReductionAmount << "）" << std::endl;
        }
    }

    double finalAmount = totalAmount - totalProductDiscount - fullReductionAmount;
    double totalSavings = totalAmount - finalAmount;
    std::cout << "总计节省：￥" << std::fixed << std::setprecision(2) << totalSavings << std::endl;
}

// ========== 促销相关方法 ==========
void Order::applyPromotions(const PromotionManager* pm) {
    if (!pm) {
        discountedAmount = totalAmount;
        return;
    }

    double productDiscountedTotal = 0.0;
    for (size_t i = 0; i < items.size(); i++) {
        const Product& product = items[i].first;
        int quantity = items[i].second;
        productDiscountedTotal += pm->calculateProductPrice(product, quantity);
    }

    auto activeFullReductions = pm->getActiveFullReductions();
    discountedAmount = productDiscountedTotal;

    double bestReduction = 0.0;
    for (auto fr : activeFullReductions) {
        if (fr->isQualified(productDiscountedTotal)) {
            double reduction = fr->getReduction();
            if (reduction > bestReduction) {
                bestReduction = reduction;
            }
        }
    }

    if (bestReduction > 0.0) {
        discountedAmount = productDiscountedTotal - bestReduction;
    }

    if (discountedAmount > totalAmount) {
        discountedAmount = totalAmount;
    }

    if (discountedAmount < 0.0) {
        discountedAmount = 0.0;
    }
}

// ========== 静态工具函数 ==========

std::string Order::timeToString(std::time_t time) {
    return TimeUtils::timeToString(time);
}

std::string Order::statusToString(OrderStatus status) {
    switch (status) {
    case OrderStatus::PENDING: return "待发货";
    case OrderStatus::SHIPPED: return "已发货";
    case OrderStatus::DELIVERED: return "已签收";
    default: return "未知状态";
    }
}

// ========== 序列化方法 ==========

std::string Order::serialize() const {
    std::stringstream ss;
    ss << orderId << "|"
        << orderTime << "|"
        << totalAmount << "|"
        << discountedAmount << "|"
        << shippingAddress << "|"
        << static_cast<int>(status) << "|"
        << shippedTime << "|"
        << deliveredTime << "|"
        << items.size();

    for (const auto& item : items) {
        ss << "|" << item.first.getName() << "|"
            << item.first.getPrice() << "|"
            << item.first.getStock() << "|"
            << item.first.getCategory() << "|"
            << item.first.getDescription() << "|"
            << item.second;
    }

    return ss.str();
}

Order Order::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 9) {
        throw std::runtime_error("无效的订单数据");
    }

    std::string orderId = tokens[0];
    std::time_t orderTime = std::stoll(tokens[1]);
    double totalAmount = std::stod(tokens[2]);
    double discountedAmount = std::stod(tokens[3]);
    std::string shippingAddress = tokens[4];
    OrderStatus status = static_cast<OrderStatus>(std::stoi(tokens[5]));
    std::time_t shippedTime = std::stoll(tokens[6]);
    std::time_t deliveredTime = std::stoll(tokens[7]);
    size_t itemCount = std::stoi(tokens[8]);

    std::vector<std::pair<Product, int>> items;
    size_t index = 9;

    for (size_t i = 0; i < itemCount; ++i) {
        if (index + 5 >= tokens.size()) {
            break;
        }

        std::string name = tokens[index++];
        double price = std::stod(tokens[index++]);
        int stock = std::stoi(tokens[index++]);
        std::string category = tokens[index++];
        std::string description = tokens[index++];
        int quantity = std::stoi(tokens[index++]);

        Product product(name, price, stock, category, description);
        items.push_back(std::make_pair(product, quantity));
    }

    Order order(items, shippingAddress);
    order.setOrderId(orderId);
    order.setOrderTime(orderTime);
    order.setTotalAmount(totalAmount);
    order.setDiscountedAmount(discountedAmount);
    order.setStatus(status);
    order.setShippedTime(shippedTime);
    order.setDeliveredTime(deliveredTime);

    return order;
}