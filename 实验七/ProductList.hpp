#ifndef PRODUCTLIST_HPP
#define PRODUCTLIST_HPP

#include "Product.hpp"
#include <vector>
#include <string>

// 前向声明
class PromotionManager;

class ProductList {
private:
    std::vector<Product> products;

public:
    // 商品管理
    void addProduct(const Product& product);
    bool removeProduct(const std::string& name);
    bool updateProduct(const std::string& name, double newPrice, int newStock,
        const std::string& newCategory, const std::string& newDesc);

    // 商品查找
    Product* findProduct(const std::string& name);
    std::vector<Product*> findProductsByCategory(const std::string& category);
    std::vector<Product*> findProductsByName(const std::string& keyword);

    // 显示商品
    void displayAllProducts() const;
    void displayAllProductsWithPromotion(const PromotionManager* pm) const;
    void displayProductsByCategory(const std::string& category) const;
    void displayProductsByCategoryWithPromotion(const std::string& category,
        const PromotionManager* pm) const;

    // 获取所有商品
    const std::vector<Product>& getAllProducts() const;

    // 库存管理
    bool updateProductStock(const std::string& name, int quantity);

    // 获取有促销的商品
    std::vector<Product*> getPromotionalProducts(const PromotionManager* pm) const;
};

#endif