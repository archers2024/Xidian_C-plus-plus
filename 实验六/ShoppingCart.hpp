#ifndef SHOPPINGCART_HPP
#define SHOPPINGCART_HPP

#include "Product.hpp"
#include <map>
#include <string>
#include <utility>

class ShoppingCart {
private:
    std::map<std::string, std::pair<Product, int>> items;
    double totalPrice;
    int totalItems;

public:
    ShoppingCart();

    // 修复：添加正确的参数类型
    bool addItem(const Product& product, int quantity = 1);

    // 修复：添加正确的参数类型
    bool removeItem(const std::string& productName, int quantity = 1);

    // 修复：添加正确的参数类型
    bool updateItemQuantity(const std::string& productName, int newQuantity);

    void clearCart();
    void displayCart() const;

    // 修复：添加const限定符
    double getTotalPrice() const;
    int getTotalItems() const;
    bool isEmpty() const;

    // 修复：返回类型一致
    const std::map<std::string, std::pair<Product, int>>& getItems() const;

    bool containsItem(const std::string& productName) const;
};

#endif