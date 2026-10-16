#include "Customer.hpp"
#include <iostream>

Customer::Customer(const std::string& uname, const std::string& pwd,
    const std::string& cname, const std::string& cemail)
    : User(uname, pwd), name(cname), email(cemail) {
}

Customer::Customer(const std::string& uname, const std::string& pwd,
    const std::string& cname, const std::string& cemail,
    const std::string& cphone, const std::string& caddr)
    : User(uname, pwd), name(cname), email(cemail), phone(cphone), address(caddr) {
}

void Customer::displayInfo() const {
    std::cout << "=== 顾客信息 ===" << std::endl;
    std::cout << "账号: " << username << std::endl;
    std::cout << "姓名: " << name << std::endl;
    std::cout << "邮箱: " << email << std::endl;
    if (!phone.empty()) std::cout << "电话: " << phone << std::endl;
    if (!address.empty()) std::cout << "地址: " << address << std::endl;
    std::cout << "购物车商品数: " << cart.getTotalItems() << std::endl;
}

void Customer::updateProfile(const std::string& newName, const std::string& newEmail,
    const std::string& newPhone, const std::string& newAddr) {
    name = newName;
    email = newEmail;
    phone = newPhone;
    address = newAddr;
}

ShoppingCart& Customer::getCart() {
    return cart;
}

const ShoppingCart& Customer::getCart() const {
    return cart;
}

std::string Customer::getName() const { return name; }
std::string Customer::getEmail() const { return email; }
std::string Customer::getPhone() const { return phone; }
std::string Customer::getAddress() const { return address; }

void Customer::setAddress(const std::string& newAddress) {
    address = newAddress;
}