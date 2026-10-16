#ifndef PRODUCTLIST_HPP
#define PRODUCTLIST_HPP

#include "Product.hpp"
#include <vector>

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
    void displayProductsByCategory(const std::string& category) const;
    const std::vector<Product>& getAllProducts() const;
    bool updateProductStock(const std::string& name, int quantity);
};

#endif