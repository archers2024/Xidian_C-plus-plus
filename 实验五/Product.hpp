#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <string>

class Product {
private:
    std::string name;
    double price;
    int stock;
    std::string category;
    std::string description;

public:
    // 确保有默认构造函数
    Product();

    Product(const std::string& n, double p, int s,
        const std::string& c = "", const std::string& d = "");

    std::string getName() const;
    double getPrice() const;
    int getStock() const;
    std::string getCategory() const;
    std::string getDescription() const;

    void setPrice(double newPrice);
    void setStock(int newStock);
    void setDescription(const std::string& desc);
    void setCategory(const std::string& cat);

    void displayInfo() const;
    void displaySimpleInfo() const;
    bool updateStock(int quantity);
};

#endif