#include "ShoppingCart.hpp"
#include "PromotionManager.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

ShoppingCart::ShoppingCart() : totalPrice(0.0), totalItems(0) {}

bool ShoppingCart::addItem(const Product& product, int quantity) {
    if (quantity <= 0) {
        std::cout << "数量必须大于0！" << std::endl;
        return false;
    }

    std::string productName = product.getName();

    if (items.find(productName) != items.end()) {
        std::cout << "商品 '" << productName << "' 已在购物车中！" << std::endl;
        std::cout << "是否修改数量？(y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 'y' || choice == 'Y') {
            return updateItemQuantity(productName, items[productName].second + quantity);
        }
        else {
            return false;
        }
    }

    items[productName] = std::pair<Product, int>(product, quantity);
    totalPrice += product.getPrice() * quantity;
    totalItems += quantity;

    std::cout << "成功添加 " << quantity << " 件 '" << productName << "' 到购物车！" << std::endl;
    return true;
}

bool ShoppingCart::removeItem(const std::string& productName, int quantity) {
    auto it = items.find(productName);
    if (it == items.end()) {
        std::cout << "商品 '" << productName << "' 不在购物车中！" << std::endl;
        return false;
    }

    if (quantity <= 0 || quantity >= it->second.second) {
        totalPrice -= it->second.first.getPrice() * it->second.second;
        totalItems -= it->second.second;
        items.erase(it);
        std::cout << "成功从购物车移除商品 '" << productName << "'！" << std::endl;
    }
    else {
        it->second.second -= quantity;
        totalPrice -= it->second.first.getPrice() * quantity;
        totalItems -= quantity;
        std::cout << "成功从购物车移除 " << quantity << " 件 '" << productName << "'！" << std::endl;
    }

    return true;
}

// 重载版本
bool ShoppingCart::removeItem(const std::string& productName) {
    return removeItem(productName, 0);
}

bool ShoppingCart::updateItemQuantity(const std::string& productName, int newQuantity) {
    auto it = items.find(productName);
    if (it == items.end()) {
        std::cout << "商品 '" << productName << "' 不在购物车中！" << std::endl;
        return false;
    }

    if (newQuantity <= 0) {
        std::cout << "数量为0，是否删除该商品？(y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 'y' || choice == 'Y') {
            return removeItem(productName,0);
        }
        else {
            std::cout << "保持原有数量: " << it->second.second << std::endl;
            return false;
        }
    }

    int oldQuantity = it->second.second;
    double price = it->second.first.getPrice();

    totalPrice -= price * oldQuantity;
    totalItems -= oldQuantity;

    it->second.second = newQuantity;
    totalPrice += price * newQuantity;
    totalItems += newQuantity;

    std::cout << "成功修改 '" << productName << "' 数量为: " << newQuantity << std::endl;
    return true;
}

void ShoppingCart::clearCart() {
    items.clear();
    totalPrice = 0.0;
    totalItems = 0;
    std::cout << "购物车已清空！" << std::endl;
}

void ShoppingCart::displayCart() const {
    if (items.empty()) {
        std::cout << "购物车为空！" << std::endl;
        return;
    }

    std::cout << "\n=== 购物车内容 ===" << std::endl;
    std::cout << std::left << std::setw(20) << "商品名称"
        << std::setw(10) << "单价"
        << std::setw(8) << "数量"
        << std::setw(12) << "小计" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    for (const auto& item : items) {
        const Product& product = item.second.first;
        int quantity = item.second.second;
        double subtotal = product.getPrice() * quantity;

        std::cout << std::left << std::setw(20) << product.getName()
            << "￥" << std::setw(8) << std::fixed << std::setprecision(2) << product.getPrice()
            << std::setw(8) << quantity
            << "￥" << std::setw(10) << std::fixed << std::setprecision(2) << subtotal << std::endl;
    }

    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "商品总数: " << totalItems << std::endl;
    std::cout << "总价格: ￥" << std::fixed << std::setprecision(2) << totalPrice << std::endl;
}

bool ShoppingCart::containsItem(const std::string& productName) const {
    return items.find(productName) != items.end();
}

void ShoppingCart::updateProductInfo(Product* updatedProduct) {
    if (!updatedProduct) {
        return; // 空指针检查
    }

    std::string productName = updatedProduct->getName();
    auto it = items.find(productName);

    if (it != items.end()) {
        // 保存原来的数量
        int oldQuantity = it->second.second;

        // 更新产品信息（用新的 Product 对象替换）
        items[productName] = std::make_pair(*updatedProduct, oldQuantity);

        // 重新计算总价
        recalculateTotals();

        std::cout << "购物车中的商品 '" << productName << "' 信息已更新" << std::endl;
    }
}

// ==== 新增：重新计算总价方法 ====
void ShoppingCart::recalculateTotals() {
    totalPrice = 0.0;
    totalItems = 0;
    for (const auto& item : items) {
        const Product& product = item.second.first;
        int quantity = item.second.second;
        totalPrice += product.getPrice() * quantity;
        totalItems += quantity;
    }
}

double ShoppingCart::calculatePromotionTotal(const PromotionManager& promoManager) const {
    // 第一步：计算所有商品的折扣后价格
    double productDiscountedTotal = 0.0;

    // 遍历购物车中的每个商品
    for (const auto& item : items) {
        const Product& product = item.second.first;  // 商品对象
        int quantity = item.second.second;           // 商品数量
        double discountedPrice = promoManager.calculateProductPrice(product, quantity);
        productDiscountedTotal += discountedPrice;
    }

    // 第二步：应用满减优惠
    // 根据你的输出显示，规则是：满5000减500
    double finalTotal = productDiscountedTotal;

    // 检查是否满足满减条件
    if (productDiscountedTotal >= 5000.0) {
        finalTotal = productDiscountedTotal - 500.0;
    }

    return finalTotal;
}