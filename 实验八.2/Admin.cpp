#include "Admin.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>

Admin::Admin() : User("admin", "admin123") {}

Admin::Admin(const std::string& uname, const std::string& pwd)
    : User(uname, pwd) {
}

void Admin::displayInfo() const {
    std::cout << "管理员: " << username << std::endl;
}

void Admin::manageProducts() {
    std::cout << "管理商品..." << std::endl;
}

void Admin::viewStatistics() {
    std::cout << "查看统计..." << std::endl;
}

// ==== 新增：序列化方法 ====
std::string Admin::serialize() const {
    return username + "|" + password;
}

Admin Admin::deserialize(const std::string& data) {
    size_t pos = data.find('|');
    if (pos == std::string::npos) {
        throw std::runtime_error("无效的管理员数据");
    }

    std::string username = data.substr(0, pos);
    std::string password = data.substr(pos + 1);

    return Admin(username, password);
}