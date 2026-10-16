#ifndef SHOPPINGCART_HPP
#define SHOPPINGCART_HPP

#include <map>
#include <string>
#include "Product.hpp"

// 前向声明
class PromotionManager;

class ShoppingCart {
private:
    std::map<std::string, std::pair<Product, int>> items;
    double totalPrice;
    int totalItems;

public:
    ShoppingCart();

    // 购物车操作
    bool addItem(const Product& product, int quantity);
    bool removeItem(const std::string& productName, int quantity = 0);
    bool updateItemQuantity(const std::string& productName, int newQuantity);
    void clearCart();

    // 更新购物车中特定产品的信息
    void updateProductInfo(Product* updatedProduct);

    // 显示购物车
    void displayCart() const;

    // Getter方法
    double getTotalPrice() const { return totalPrice; }
    int getTotalItems() const { return totalItems; }
    bool isEmpty() const { return items.empty(); }
    const std::map<std::string, std::pair<Product, int>>& getItems() const { return items; }
    bool containsItem(const std::string& productName) const;

    // 促销功能：计算促销后的总价
    double calculatePromotionTotal(const PromotionManager& promoManager) const;
};

#endif