#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <string>

// 前向声明
class PromotionManager;

class Product {
private:
    std::string name;
    double price;
    int stock;
    std::string category;
    std::string description;

public:
    Product();
    Product(const std::string& n, double p, int s,
        const std::string& c = "", const std::string& d = "");

    // Getter方法 - 只声明
    std::string getName() const;
    double getPrice() const;
    int getStock() const;
    std::string getCategory() const;
    std::string getDescription() const;

    // Setter方法 - 只声明
    void setName(const std::string& newName);
    void setPrice(double newPrice);
    void setStock(int newStock);
    void setDescription(const std::string& desc);
    void setCategory(const std::string& cat);

    // 显示功能
    void displayInfo() const;
    void displaySimpleInfo() const;
    void displayWithPromotion(const PromotionManager* pm = nullptr) const;

    // 库存管理
    bool updateStock(int quantity);

    // 促销相关
    std::string getPromotionTag(const PromotionManager* pm) const;
    double getDiscountedPrice(const PromotionManager* pm) const;
    bool hasPromotion(const PromotionManager* pm) const;

    // 序列化方法
    std::string serialize() const;
    static Product deserialize(const std::string& data);
};

#endif // PRODUCT_HPP