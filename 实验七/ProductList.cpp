#include "ProductList.hpp"
#include "PromotionManager.hpp"
#include <iostream>
#include <algorithm>

void ProductList::addProduct(const Product& product) {
    for (const auto& p : products) {
        if (p.getName() == product.getName()) {
            std::cout << "商品 '" << product.getName() << "' 已存在！" << std::endl;
            return;
        }
    }
    products.push_back(product);
    std::cout << "商品 '" << product.getName() << "' 添加成功！" << std::endl;
}

bool ProductList::removeProduct(const std::string& name) {
    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->getName() == name) {
            products.erase(it);
            std::cout << "商品 '" << name << "' 删除成功！" << std::endl;
            return true;
        }
    }
    std::cout << "商品 '" << name << "' 不存在！" << std::endl;
    return false;
}

bool ProductList::updateProduct(const std::string& name, double newPrice, int newStock,
    const std::string& newCategory, const std::string& newDesc) {
    Product* product = findProduct(name);
    if (product) {
        product->setPrice(newPrice);
        product->setStock(newStock);
        if (!newCategory.empty()) product->setCategory(newCategory);
        if (!newDesc.empty()) product->setDescription(newDesc);
        std::cout << "商品 '" << name << "' 更新成功！" << std::endl;
        return true;
    }
    std::cout << "商品 '" << name << "' 不存在！" << std::endl;
    return false;
}

Product* ProductList::findProduct(const std::string& name) {
    for (auto& product : products) {
        if (product.getName() == name) return &product;
    }
    return nullptr;
}

std::vector<Product*> ProductList::findProductsByCategory(const std::string& category) {
    std::vector<Product*> result;
    for (auto& product : products) {
        if (product.getCategory() == category) result.push_back(&product);
    }
    return result;
}

std::vector<Product*> ProductList::findProductsByName(const std::string& keyword) {
    std::vector<Product*> result;
    for (auto& product : products) {
        if (product.getName().find(keyword) != std::string::npos) result.push_back(&product);
    }
    return result;
}

void ProductList::displayAllProducts() const {
    if (products.empty()) {
        std::cout << "暂无商品！" << std::endl;
        return;
    }
    std::cout << "\n=== 所有商品列表 ===" << std::endl;
    for (const auto& product : products) product.displaySimpleInfo();
}

void ProductList::displayAllProductsWithPromotion(const PromotionManager* pm) const {
    if (products.empty()) {
        std::cout << "暂无商品！" << std::endl;
        return;
    }
    std::cout << "\n=== 所有商品列表（含促销信息） ===" << std::endl;
    for (const auto& product : products) product.displayWithPromotion(pm);
}

void ProductList::displayProductsByCategory(const std::string& category) const {
    std::cout << "\n=== 类别: " << category << " ===" << std::endl;
    bool found = false;
    for (const auto& product : products) {
        if (product.getCategory() == category) {
            product.displaySimpleInfo();
            found = true;
        }
    }
    if (!found) std::cout << "该类别下暂无商品！" << std::endl;
}

void ProductList::displayProductsByCategoryWithPromotion(const std::string& category,
    const PromotionManager* pm) const {
    std::cout << "\n=== 类别: " << category << "（含促销信息） ===" << std::endl;
    bool found = false;
    for (const auto& product : products) {
        if (product.getCategory() == category) {
            product.displayWithPromotion(pm);
            found = true;
        }
    }
    if (!found) std::cout << "该类别下暂无商品！" << std::endl;
}

const std::vector<Product>& ProductList::getAllProducts() const { return products; }

bool ProductList::updateProductStock(const std::string& name, int quantity) {
    Product* product = findProduct(name);
    if (product) return product->updateStock(quantity);
    return false;
}

std::vector<Product*> ProductList::getPromotionalProducts(const PromotionManager* pm) const {
    std::vector<Product*> result;
    if (!pm) return result;

    for (auto& product : products) {
        if (product.hasPromotion(pm)) {
            result.push_back(const_cast<Product*>(&product));
        }
    }
    return result;
}