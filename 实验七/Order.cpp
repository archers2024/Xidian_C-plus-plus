
#include "Order.hpp"
#include "PromotionManager.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

// 静态成员初始化
int Order::orderCounter = 1000;

// ========== 构造函数 ==========

// 构造函数1：原始构造函数（保持兼容）
Order::Order(const std::vector<std::pair<Product, int>>& itemList,
    const std::string& address)
    : items(itemList), shippingAddress(address),
    status(OrderStatus::PENDING), shippedTime(0), deliveredTime(0),
    discountedAmount(0.0) {

    orderId = generateOrderId();
    orderTime = std::time(nullptr);
    totalAmount = calculateTotalAmount();
    discountedAmount = totalAmount;  // 初始时等于原价
}

// 构造函数2：带促销管理器的构造函数
Order::Order(const std::vector<std::pair<Product, int>>& itemList,
    const std::string& address, const PromotionManager* pm)
    : items(itemList), shippingAddress(address),
    status(OrderStatus::PENDING), shippedTime(0), deliveredTime(0),
    discountedAmount(0.0) {

    orderId = generateOrderId();
    orderTime = std::time(nullptr);
    totalAmount = calculateTotalAmount();

    // 创建订单时立即应用促销
    if (pm) {
        applyPromotions(pm);
    }
    else {
        discountedAmount = totalAmount;  // 如果没有促销管理器，使用原价
    }
}

// ========== 私有辅助方法 ==========

// 生成订单ID
std::string Order::generateOrderId() {
    std::stringstream ss;
    ss << "ORD" << std::setw(8) << std::setfill('0') << orderCounter++;
    return ss.str();
}

// 计算订单总金额 - 非静态方法
double Order::calculateTotalAmount() {
    double total = 0.0;

    // 使用下标遍历，确保兼容性
    for (size_t i = 0; i < items.size(); i++) {
        const Product& product = items[i].first;
        int quantity = items[i].second;
        total += product.getPrice() * quantity;
    }

    return total;
}

// 设置订单状态 - 非静态方法
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

// ========== Getter方法 ==========

std::string Order::getOrderId() const { return orderId; }
std::time_t Order::getOrderTime() const { return orderTime; }
double Order::getTotalAmount() const { return totalAmount; }
double Order::getDiscountedAmount() const { return discountedAmount; }
std::string Order::getShippingAddress() const { return shippingAddress; }
OrderStatus Order::getStatus() const { return status; }
const std::vector<std::pair<Product, int>>& Order::getItems() const { return items; }

// ========== 状态操作方法 ==========

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

    // 计算商品折扣总额
    double totalProductDiscount = 0.0;
    for (size_t i = 0; i < items.size(); i++) {
        const Product& product = items[i].first;
        int quantity = items[i].second;
        double originalPrice = product.getPrice() * quantity;

        // 如果有商品折扣
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

    // 计算满减优惠
    double fullReductionAmount = 0.0;
    std::string fullReductionDesc = "";
    if (pm) {
        auto activeReductions = pm->getActiveFullReductions();
        double productDiscountedTotal = totalAmount - totalProductDiscount;

        // 找出最优的满减
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

    // 计算正确的订单实付金额
    double finalAmount = totalAmount - totalProductDiscount - fullReductionAmount;

    // 计算总节省
    double totalSavings = totalAmount - finalAmount;
    std::cout << "总计节省：￥" << std::fixed << std::setprecision(2) << totalSavings << std::endl;
}

// ========== 促销相关方法 ==========
void Order::applyPromotions(const PromotionManager* pm) {
    if (!pm) {
        discountedAmount = totalAmount;
        return;
    }

    // ========== 第一步：应用商品折扣 ==========
    double productDiscountedTotal = 0.0;
    for (size_t i = 0; i < items.size(); i++) {
        const Product& product = items[i].first;
        int quantity = items[i].second;
        productDiscountedTotal += pm->calculateProductPrice(product, quantity);
    }

    // ========== 第二步：应用满减优惠 ==========
    // 获取当前有效的满减活动
    auto activeFullReductions = pm->getActiveFullReductions();

    // 初始化最终金额为商品折扣后金额
    discountedAmount = productDiscountedTotal;

    // 应用所有符合条件的满减活动中优惠最大的一个
    double bestReduction = 0.0;
    for (auto fr : activeFullReductions) {
        // 检查是否满足满减条件（基于商品折扣后的金额）
        if (fr->isQualified(productDiscountedTotal)) {
            double reduction = fr->getReduction();
            if (reduction > bestReduction) {
                bestReduction = reduction;
            }
        }
    }

    // 应用最优的满减优惠
    if (bestReduction > 0.0) {
        discountedAmount = productDiscountedTotal - bestReduction;
    }

    // 确保折扣后金额不会超过原价
    if (discountedAmount > totalAmount) {
        discountedAmount = totalAmount;
    }

    // 确保折扣后金额不会为负数
    if (discountedAmount < 0.0) {
        discountedAmount = 0.0;
    }
}

// ========== 静态工具函数 ==========

std::string Order::timeToString(std::time_t time) {
    if (time == 0) return "未设置";

    char buffer[80];
    struct tm timeinfo;

    // Windows使用localtime_s，Linux使用localtime_r
#ifdef _WIN32
    localtime_s(&timeinfo, &time);
#else
    localtime_r(&time, &timeinfo);
#endif

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
