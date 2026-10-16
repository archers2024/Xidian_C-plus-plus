
#include "OrderList.hpp"
#include "PromotionManager.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>

// 构造函数定义
OrderList::OrderList() {
    // 构造函数体可以为空，或者初始化一些数据
}

// 创建订单 - 使用带促销管理器的构造函数（主要方法）
Order* OrderList::createOrder(const std::vector<std::pair<Product, int>>& itemList,
    const std::string& address,
    const PromotionManager* pm) {

    if (itemList.empty()) {
        std::cout << "订单商品列表为空！" << std::endl;
        return nullptr;
    }

    if (address.empty()) {
        std::cout << "收货地址不能为空！" << std::endl;
        return nullptr;
    }

    // ========== 第一步：计算订单原价 ==========
    double totalAmount = 0.0;
    for (const auto& item : itemList) {
        const Product& product = item.first;
        int quantity = item.second;
        totalAmount += product.getPrice() * quantity;
    }

    // ========== 第二步：计算商品折扣 ==========
    double totalProductDiscount = 0.0;
    if (pm) {
        for (const auto& item : itemList) {
            const Product& product = item.first;
            int quantity = item.second;
            double originalPrice = product.getPrice() * quantity;

            // 计算该商品的折扣金额
            DiscountPromotion* discount = pm->getDiscountForProduct(product.getName());
            if (discount) {
                double discountAmount = discount->calculateDiscountAmount(originalPrice);
                totalProductDiscount += discountAmount;
            }
        }
    }

    // ========== 第三步：计算商品折扣后总价 ==========
    double productDiscountedTotal = totalAmount - totalProductDiscount;

    // ========== 第四步：计算满减优惠 ==========
    double fullReductionAmount = 0.0;
    if (pm) {
        auto activeReductions = pm->getActiveFullReductions();

        // 找出符合条件的最大满减优惠
        double bestReduction = 0.0;
        FullReductionPromotion* bestPromotion = nullptr;
        for (auto fr : activeReductions) {
            if (fr && fr->isQualified(productDiscountedTotal)) {
                double reduction = fr->getReduction();
                if (reduction > bestReduction) {
                    bestReduction = reduction;
                    bestPromotion = fr;
                }
            }
        }
        fullReductionAmount = bestReduction;
    }

    // ========== 第五步：计算最终实付金额 ==========
    double finalAmount = productDiscountedTotal - fullReductionAmount;

    // 确保金额不会为负数
    if (finalAmount < 0.0) {
        finalAmount = 0.0;
    }

    // ========== 第六步：显示促销信息 ==========
    std::cout << "\n== 订单促销信息 ==" << std::endl;
    std::cout << "购物车总额：￥" << std::fixed << std::setprecision(2) << totalAmount << std::endl;

    // 显示商品折扣明细
    if (pm) {
        for (const auto& item : itemList) {
            const Product& product = item.first;
            int quantity = item.second;

            DiscountPromotion* discount = pm->getDiscountForProduct(product.getName());
            if (discount) {
                double originalPrice = product.getPrice() * quantity;
                double discountAmount = discount->calculateDiscountAmount(originalPrice);
                std::cout << "   " << product.getName() << " x" << quantity
                    << "：商品折扣￥" << std::fixed << std::setprecision(2)
                    << discountAmount << std::endl;
            }
        }
    }

    // 显示满减优惠
    if (fullReductionAmount > 0.0) {
        std::cout << "满减优惠：满减：满5000.00减500.00（-￥"
            << std::fixed << std::setprecision(2) << fullReductionAmount << "）" << std::endl;
    }

    // 计算总节省金额
    double totalSavings = totalAmount - finalAmount;
    std::cout << "总计节省：￥" << std::fixed << std::setprecision(2) << totalSavings << std::endl;

    // ========== 第七步：确认生成订单 ==========
    std::cout << "\n确认生成订单吗？（y/n）：";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "订单创建已取消！" << std::endl;
        return nullptr;
    }

    // ========== 第八步：获取收货地址 ==========
    std::cout << "请输入收货地址：";
    std::string finalAddress;
    std::getline(std::cin, finalAddress);

    if (finalAddress.empty()) {
        finalAddress = address; // 使用默认地址
    }

    // ========== 第九步：创建最终订单 ==========
    // 使用带促销管理器的构造函数，订单会自动应用促销
    Order newOrder(itemList, finalAddress, pm);

    // ========== 第十步：添加到订单列表 ==========
    orders.push_back(newOrder);

    // ========== 第十一步：显示成功信息 ==========
    std::cout << "\n订单创建成功！订单号：" << newOrder.getOrderId() << std::endl;
    std::cout << "购物车已清空！" << std::endl;

    // ========== 第十二步：显示订单详情 ==========
    std::cout << "\n订单生成成功！" << std::endl;
    std::cout << "\n== 订单详情 ==" << std::endl;
    newOrder.displayOrderInfo();

    return &orders.back();
}

// 保持兼容性的重载版本
Order* OrderList::createOrder(const std::vector<std::pair<Product, int>>& itemList,
    const std::string& address,
    double promoTotal) {

    if (itemList.empty()) {
        std::cout << "订单商品列表为空！" << std::endl;
        return nullptr;
    }

    if (address.empty()) {
        std::cout << "收货地址不能为空！" << std::endl;
        return nullptr;
    }

    // 使用原始构造函数（不应用促销）
    Order newOrder(itemList, address);

    orders.push_back(newOrder);

    std::cout << "订单创建成功！订单号: " << newOrder.getOrderId() << std::endl;
    return &orders.back();
}

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

const Order* OrderList::findOrder(const std::string& orderId) const {
    for (const auto& order : orders) {
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
        << std::setw(12) << "实付金额"
        << std::setw(10) << "状态" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    for (const auto& order : orders) {
        double finalAmount = order.getDiscountedAmount();

        std::cout << std::left << std::setw(12) << order.getOrderId()
            << std::setw(20) << Order::timeToString(order.getOrderTime())
            << "￥" << std::setw(10) << std::fixed << std::setprecision(2) << finalAmount
            << std::setw(10) << Order::statusToString(order.getStatus()) << std::endl;
    }
}

void OrderList::displayOrdersByStatus(OrderStatus status) const {
    std::string statusStr = Order::statusToString(status);
    std::cout << "\n=== " << statusStr << "订单 ===" << std::endl;

    bool found = false;
    for (const auto& order : orders) {
        if (order.getStatus() == status) {
            double finalAmount = order.getDiscountedAmount();

            std::cout << order.getOrderId() << " | "
                << Order::timeToString(order.getOrderTime()) << " | "
                << "￥" << std::fixed << std::setprecision(2) << finalAmount
                << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "暂无" << statusStr << "的订单！" << std::endl;
    }
}

// 新增函数：应用促销到所有订单
void OrderList::applyPromotionsToAllOrders(const PromotionManager* pm) {
    int appliedCount = 0;
    for (auto& order : orders) {
        // 只有待发货的订单可以应用促销
        if (order.isPending()) {
            order.applyPromotions(pm);
            appliedCount++;
        }
    }

    if (appliedCount > 0) {
        std::cout << "已对 " << appliedCount << " 个待发货订单应用促销" << std::endl;
    }
}

// 新增函数：显示订单详情
void OrderList::displayOrderDetails(const std::string& orderId) const {
    const Order* order = findOrder(orderId);
    if (!order) {
        std::cout << "订单不存在！" << std::endl;
        return;
    }

    order->displayOrderInfo();
}

// 新增函数：显示订单详情（含促销信息）
void OrderList::displayOrderDetailsWithPromotion(const std::string& orderId, const PromotionManager* pm) const {
    const Order* order = findOrder(orderId);
    if (!order) {
        std::cout << "订单不存在！" << std::endl;
        return;
    }

    order->displayOrderInfoWithPromotion(pm);
}

// 新增函数：获取订单统计信息
OrderStats OrderList::getOrderStats() const {
    OrderStats stats;
    stats.totalOrders = static_cast<int>(orders.size());

    for (const auto& order : orders) {
        // 使用折扣后金额作为销售额
        stats.totalRevenue += order.getDiscountedAmount();

        switch (order.getStatus()) {
        case OrderStatus::PENDING:
            stats.pendingCount++;
            break;
        case OrderStatus::SHIPPED:
            stats.shippedCount++;
            break;
        case OrderStatus::DELIVERED:
            stats.deliveredCount++;
            break;
        }
    }

    return stats;
}

// 新增函数：显示统计信息
void OrderList::displayStats() const {
    OrderStats stats = getOrderStats();

    std::cout << "\n=== 订单统计信息 ===" << std::endl;
    std::cout << "总订单数: " << stats.totalOrders << std::endl;
    std::cout << "待发货订单: " << stats.pendingCount << std::endl;
    std::cout << "已发货订单: " << stats.shippedCount << std::endl;
    std::cout << "已签收订单: " << stats.deliveredCount << std::endl;
    std::cout << "总销售额: ￥" << std::fixed << std::setprecision(2) << stats.totalRevenue << std::endl;

    if (stats.totalOrders > 0) {
        double avgOrderValue = stats.totalRevenue / stats.totalOrders;
        std::cout << "平均订单金额: ￥" << std::fixed << std::setprecision(2) << avgOrderValue << std::endl;
    }
}

// 新增函数：批量发货
bool OrderList::batchShipOrders(const std::vector<std::string>& orderIds) {
    int successCount = 0;
    int failCount = 0;

    for (const auto& orderId : orderIds) {
        Order* order = findOrder(orderId);
        if (!order) {
            std::cout << "订单 " << orderId << " 不存在" << std::endl;
            failCount++;
            continue;
        }

        if (!order->isPending()) {
            std::cout << "订单 " << orderId << " 状态不是待发货" << std::endl;
            failCount++;
            continue;
        }

        if (order->shipOrder()) {
            successCount++;
            std::cout << "订单 " << orderId << " 发货成功" << std::endl;
        }
        else {
            failCount++;
            std::cout << "订单 " << orderId << " 发货失败" << std::endl;
        }
    }

    std::cout << "\n批量发货完成：" << std::endl;
    std::cout << "成功: " << successCount << " 个订单" << std::endl;
    std::cout << "失败: " << failCount << " 个订单" << std::endl;

    return successCount > 0;
}

// 新增函数：搜索订单
std::vector<Order*> OrderList::searchOrders(const std::string& keyword) {
    std::vector<Order*> result;

    for (auto& order : orders) {
        // 搜索订单号
        if (order.getOrderId().find(keyword) != std::string::npos) {
            result.push_back(&order);
            continue;
        }

        // 搜索收货地址
        if (order.getShippingAddress().find(keyword) != std::string::npos) {
            result.push_back(&order);
            continue;
        }

        // 搜索商品名称
        const auto& items = order.getItems();
        for (const auto& item : items) {
            const Product& product = item.first;
            if (product.getName().find(keyword) != std::string::npos) {
                result.push_back(&order);
                break;
            }
        }
    }

    return result;
}

// 新增函数：获取指定时间范围内的订单
std::vector<Order*> OrderList::getOrdersByTimeRange(time_t startTime, time_t endTime) {
    std::vector<Order*> result;

    for (auto& order : orders) {
        time_t orderTime = order.getOrderTime();
        if (orderTime >= startTime && orderTime <= endTime) {
            result.push_back(&order);
        }
    }

    return result;
}

// 新增函数：导出订单数据到文件
bool OrderList::exportToFile(const std::string& filename) const {
    // 这里可以添加文件导出逻辑
    std::cout << "导出订单数据到文件: " << filename << " (功能待实现)" << std::endl;
    return true;
}

// 新增函数：从文件导入订单数据
bool OrderList::importFromFile(const std::string& filename) {
    // 这里可以添加文件导入逻辑
    std::cout << "从文件导入订单数据: " << filename << " (功能待实现)" << std::endl;
    return true;
}
