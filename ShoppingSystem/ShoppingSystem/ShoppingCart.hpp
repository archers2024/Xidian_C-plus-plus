#ifndef SHOPPINGCART_HPP
#define SHOPPINGCART_HPP

#include <unordered_map>
#include <string>
#include "Product.hpp"

class PromotionManager;

class ShoppingCart {
private:
    std::unordered_map<std::string, std::pair<Product, int>> items;
    double totalPrice;
    int totalItems;

    void recalculateTotals();

public:
    ShoppingCart();

    // 购物车操作
    bool addItem(const Product& product, int quantity);
    bool removeItem(const std::string& productName, int quantity = 0);
    bool removeItem(const std::string& productName); // 重载
    bool updateItemQuantity(const std::string& productName, int newQuantity);
    void clearCart();

    // 显示功能
    void displayCart() const;

    // 检查功能
    bool containsItem(const std::string& productName) const;

    // 获取信息
    double getTotalPrice() const { return totalPrice; }
    int getTotalItems() const { return totalItems; }
    bool isEmpty() const { return items.empty(); }

    // ==== 新增：持久化支持 ====
    const std::unordered_map<std::string, std::pair<Product, int>>& getItems() const {
        return items;
    }

    void setItems(const std::unordered_map<std::string, std::pair<Product, int>>& newItems) {
        items = newItems;
        recalculateTotals();
    }

    // 产品信息更新
    void updateProductInfo(Product* updatedProduct);

    // 促销计算
    double calculatePromotionTotal(const PromotionManager& promoManager) const;
};

#endif // SHOPPINGCART_HPP