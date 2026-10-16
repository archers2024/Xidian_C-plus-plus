#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include "User.hpp"
#include "ShoppingCart.hpp"
#include <string>

class Customer : public User {
private:
    std::string name;
    std::string email;
    std::string phone;
    std::string address;
    ShoppingCart cart;

public:
    Customer(const std::string& uname, const std::string& pwd,
        const std::string& cname, const std::string& cemail);

    Customer(const std::string& uname, const std::string& pwd,
        const std::string& cname, const std::string& cemail,
        const std::string& cphone, const std::string& caddr);

    void displayInfo() const override;

    void updateProfile(const std::string& newName, const std::string& newEmail,
        const std::string& newPhone, const std::string& newAddr);

    // 购物车访问 - 只声明，不在头文件中实现
    ShoppingCart& getCart();
    const ShoppingCart& getCart() const;

    // Getter方法 - 只声明
    std::string getName() const;
    std::string getEmail() const;
    std::string getPhone() const;
    std::string getAddress() const;

    void setAddress(const std::string& newAddress);

    // 购物车设置（用于持久化）
    void setCart(const ShoppingCart& newCart);

    // 序列化方法
    std::string serialize() const;
    static Customer deserialize(const std::string& data);
};

#endif // CUSTOMER_HPP