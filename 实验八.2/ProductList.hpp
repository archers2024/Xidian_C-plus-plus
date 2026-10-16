#ifndef PRODUCTLIST_HPP
#define PRODUCTLIST_HPP

#include <vector>
#include <string>
#include "Product.hpp"

// 前向声明
class PromotionManager;

class ProductList {
private:
    std::vector<Product> products;

public:
    void addProduct(const Product& product);
    bool removeProduct(const std::string& name);
    bool updateProduct(const std::string& name, double newPrice, int newStock,
        const std::string& newCategory = "", const std::string& newDesc = "");

    Product* findProduct(const std::string& name);
    std::vector<Product*> findProductsByCategory(const std::string& category);
    std::vector<Product*> findProductsByName(const std::string& keyword);

    void displayAllProducts() const;
    void displayAllProductsWithPromotion(const PromotionManager* pm) const;
    void displayProductsByCategory(const std::string& category) const;
    void displayProductsByCategoryWithPromotion(const std::string& category,
        const PromotionManager* pm) const;

    const std::vector<Product>& getAllProducts() const { return products; }

    bool updateProductStock(const std::string& name, int quantity);

    std::vector<Product*> getPromotionalProducts(const PromotionManager* pm) const;

    // ==== 新增：持久化支持 ====
    void clearProducts() { products.clear(); }
    void addProductDirectly(const Product& product) {
        products.push_back(product);
    }

    void setProducts(const std::vector<Product>& newProducts) {
        products = newProducts;
    }

    // 批量操作
    bool removeProductsByCategory(const std::string& category);
    int getTotalStock() const;
    double getTotalValue() const;
};

#endif // PRODUCTLIST_HPP