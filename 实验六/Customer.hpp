#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include "User.hpp"
#include "ShoppingCart.hpp"
#include <string>
#include <vector>

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

    // 购物车相关方法
    ShoppingCart& getCart();
    const ShoppingCart& getCart() const;

    // Getter方法
    std::string getName() const;
    std::string getEmail() const;
    std::string getPhone() const;
    std::string getAddress() const;

    // Setter方法
    void setAddress(const std::string& newAddress);
};

#endif