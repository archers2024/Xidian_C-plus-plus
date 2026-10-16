// Admin.cpp
#include "Admin.hpp"
#include <iostream>

Admin::Admin() : User("admin", "admin123") {}

Admin::Admin(const std::string& uname, const std::string& pwd) 
    : User(uname, pwd) {}

void Admin::displayInfo() const {
    std::cout << "管理员账号: " << username << std::endl;
}

void Admin::manageProducts() {
    std::cout << "管理商品功能..." << std::endl;
}

void Admin::viewStatistics() {
    std::cout << "查看统计功能..." << std::endl;
}
