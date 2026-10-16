#include "Customer.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>

// 构造函数
Customer::Customer(const std::string& uname, const std::string& pwd,
    const std::string& cname, const std::string& cemail)
    : User(uname, pwd), name(cname), email(cemail) {
}

Customer::Customer(const std::string& uname, const std::string& pwd,
    const std::string& cname, const std::string& cemail,
    const std::string& cphone, const std::string& caddr)
    : User(uname, pwd), name(cname), email(cemail), phone(cphone), address(caddr) {
}

// 显示信息
void Customer::displayInfo() const {
    std::cout << "=== 顾客信息 ===" << std::endl;
    std::cout << "用户名: " << getUsername() << std::endl;
    std::cout << "姓名: " << name << std::endl;
    std::cout << "邮箱: " << email << std::endl;
    if (!phone.empty()) std::cout << "电话: " << phone << std::endl;
    if (!address.empty()) std::cout << "地址: " << address << std::endl;
    std::cout << "购物车商品数: " << cart.getTotalItems() << std::endl;
}

// 更新个人信息
void Customer::updateProfile(const std::string& newName, const std::string& newEmail,
    const std::string& newPhone, const std::string& newAddr) {
    name = newName;
    email = newEmail;
    phone = newPhone;
    address = newAddr;
}

// 购物车访问
ShoppingCart& Customer::getCart() {
    return cart;
}

const ShoppingCart& Customer::getCart() const {
    return cart;
}

// Getter方法实现
std::string Customer::getName() const {
    return name;
}

std::string Customer::getEmail() const {
    return email;
}

std::string Customer::getPhone() const {
    return phone;
}

std::string Customer::getAddress() const {
    return address;
}

// Setter方法
void Customer::setAddress(const std::string& newAddress) {
    address = newAddress;
}

// 购物车设置
void Customer::setCart(const ShoppingCart& newCart) {
    cart = newCart;
}

// 序列化方法
std::string Customer::serialize() const {
    std::stringstream ss;
    ss << getUsername() << "|"
        << getPassword() << "|"
        << name << "|"
        << email << "|"
        << phone << "|"
        << address;
    return ss.str();
}

Customer Customer::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string part;
    std::vector<std::string> parts;

    while (std::getline(ss, part, '|')) {
        parts.push_back(part);
    }

    if (parts.size() >= 4) {
        std::string username = parts[0];
        std::string password = parts[1];
        std::string name = parts[2];
        std::string email = parts[3];

        std::string phone = (parts.size() > 4) ? parts[4] : "";
        std::string address = (parts.size() > 5) ? parts[5] : "";

        return Customer(username, password, name, email, phone, address);
    }

    throw std::runtime_error("无效的顾客数据");
}