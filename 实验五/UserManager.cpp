// UserManager.cpp
#include "UserManager.hpp"
#include <iostream>
#include <algorithm>

UserManager::UserManager() : currentUser(nullptr) {
    // 初始化默认管理员
    admin = Admin();
}

bool UserManager::isUsernameExist(const std::string& username) const {
    if (admin.getUsername() == username) return true;
    
    for (const auto& customer : customers) {
        if (customer.getUsername() == username) {
            return true;
        }
    }
    return false;
}

Customer* UserManager::findCustomerByUsername(const std::string& username) {
    for (auto& customer : customers) {
        if (customer.getUsername() == username) {
            return &customer;
        }
    }
    return nullptr;
}

bool UserManager::adminLogin(const std::string& username, const std::string& password) {
    if (admin.login(username, password)) {
        currentUser = &admin;
        return true;
    }
    return false;
}

bool UserManager::customerRegister(const std::string& username, const std::string& password,
                                 const std::string& name, const std::string& email,
                                 const std::string& phone, const std::string& address) {
    // 检查用户名是否已存在
    if (isUsernameExist(username)) {
        std::cout << "用户名已存在！" << std::endl;
        return false;
    }
    
    // 创建新顾客
    Customer newCustomer(username, password, name, email, phone, address);
    customers.push_back(newCustomer);
    
    std::cout << "注册成功！" << std::endl;
    return true;
}

bool UserManager::customerLogin(const std::string& username, const std::string& password) {
    Customer* customer = findCustomerByUsername(username);
    if (customer && customer->login(username, password)) {
        currentUser = customer;
        return true;
    }
    return false;
}

void UserManager::logout() {
    currentUser = nullptr;
}

bool UserManager::changeCurrentUserPassword(const std::string& oldPwd, const std::string& newPwd) {
    if (currentUser) {
        return currentUser->changePassword(oldPwd, newPwd);
    }
    return false;
}

User* UserManager::getCurrentUser() const {
    return currentUser;
}

bool UserManager::isLoggedIn() const {
    return currentUser != nullptr;
}

bool UserManager::isAdminLoggedIn() const {
    return dynamic_cast<Admin*>(currentUser) != nullptr;
}

bool UserManager::isCustomerLoggedIn() const {
    return dynamic_cast<Customer*>(currentUser) != nullptr;
}

bool UserManager::updateCustomerProfile(const std::string& username, 
                                      const std::string& name, const std::string& email,
                                      const std::string& phone, const std::string& address) {
    Customer* customer = findCustomerByUsername(username);
    if (customer) {
        customer->updateProfile(name, email, phone, address);
        return true;
    }
    return false;
}
