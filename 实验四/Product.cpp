#include "Product.hpp"
#include <iostream>
#include <iomanip>

Product::Product(const std::string& n, double p, int s,
    const std::string& c, const std::string& d)
    : name(n), price(p), stock(s), category(c), description(d) {
}

std::string Product::getName() const { return name; }
double Product::getPrice() const { return price; }
int Product::getStock() const { return stock; }
std::string Product::getCategory() const { return category; }
std::string Product::getDescription() const { return description; }

void Product::setPrice(double newPrice) { price = newPrice; }
void Product::setStock(int newStock) { stock = newStock; }
void Product::setDescription(const std::string& desc) { description = desc; }
void Product::setCategory(const std::string& cat) { category = cat; }

void Product::displayInfo() const {
    std::cout << "=== 商品详细信息 ===" << std::endl;
    std::cout << "名称: " << name << std::endl;
    std::cout << "价格: ￥" << std::fixed << std::setprecision(2) << price << std::endl;
    std::cout << "库存: " << stock << std::endl;
    std::cout << "类别: " << (category.empty() ? "未分类" : category) << std::endl;
    if (!description.empty()) std::cout << "描述: " << description << std::endl;
}

void Product::displaySimpleInfo() const {
    std::cout << name << " | ￥" << std::fixed << std::setprecision(2) << price
        << " | 库存:" << stock << " | "
        << (category.empty() ? "未分类" : category) << std::endl;
}

bool Product::updateStock(int quantity) {
    if (stock + quantity >= 0) {
        stock += quantity;
        return true;
    }
    return false;
}