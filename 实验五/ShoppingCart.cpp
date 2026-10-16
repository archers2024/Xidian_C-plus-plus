#include "ShoppingCart.hpp"
#include <iostream>
#include <iomanip>

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

    // 修复：直接使用pair构造函数
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
            return removeItem(productName);
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

// 修复：添加const限定符
double ShoppingCart::getTotalPrice() const {
    return totalPrice;
}

// 修复：添加const限定符
int ShoppingCart::getTotalItems() const {
    return totalItems;
}

bool ShoppingCart::isEmpty() const {
    return items.empty();
}

// 修复：返回类型一致
const std::map<std::string, std::pair<Product, int>>& ShoppingCart::getItems() const {
    return items;
}

bool ShoppingCart::containsItem(const std::string& productName) const {
    return items.find(productName) != items.end();
}