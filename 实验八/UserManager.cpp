#include "UserManager.hpp"
#include <iostream>
#include <algorithm>

// 构造函数
UserManager::UserManager() : currentUser(nullptr) {
    // 初始化默认管理员
    admin = Admin();
}

// 检查用户名是否存在
bool UserManager::isUsernameExist(const std::string& username) const {
    if (admin.getUsername() == username) return true;

    for (const auto& customer : customers) {
        if (customer.getUsername() == username) {
            return true;
        }
    }
    return false;
}

// 通过用户名查找顾客
Customer* UserManager::findCustomerByUsername(const std::string& username) {
    for (auto& customer : customers) {
        if (customer.getUsername() == username) {
            return &customer;
        }
    }
    return nullptr;
}

// 管理员登录
bool UserManager::adminLogin(const std::string& username, const std::string& password) {
    if (admin.login(username, password)) {
        currentUser = &admin;
        std::cout << "管理员登录成功！" << std::endl;
        return true;
    }
    std::cout << "管理员登录失败！用户名或密码错误" << std::endl;
    return false;
}

// 顾客注册
bool UserManager::customerRegister(const std::string& username, const std::string& password,
    const std::string& name, const std::string& email,
    const std::string& phone, const std::string& address) {
    // 检查用户名是否已存在
    if (isUsernameExist(username)) {
        std::cout << "用户名已存在！" << std::endl;
        return false;
    }

    // 检查必填字段
    if (username.empty() || password.empty() || name.empty() || email.empty()) {
        std::cout << "用户名、密码、姓名和邮箱为必填项！" << std::endl;
        return false;
    }

    // 创建新顾客
    Customer newCustomer(username, password, name, email, phone, address);
    customers.push_back(newCustomer);

    std::cout << "顾客注册成功！" << std::endl;
    return true;
}

// 顾客登录
bool UserManager::customerLogin(const std::string& username, const std::string& password) {
    Customer* customer = findCustomerByUsername(username);
    if (customer && customer->login(username, password)) {
        currentUser = customer;
        std::cout << "顾客登录成功！" << std::endl;
        return true;
    }
    std::cout << "顾客登录失败！用户名或密码错误" << std::endl;
    return false;
}

// 登出
void UserManager::logout() {
    if (currentUser) {
        std::cout << currentUser->getUsername() << " 已退出登录" << std::endl;
    }
    currentUser = nullptr;
}

// 修改当前用户密码
bool UserManager::changeCurrentUserPassword(const std::string& oldPwd, const std::string& newPwd) {
    if (currentUser) {
        if (newPwd.empty()) {
            std::cout << "新密码不能为空！" << std::endl;
            return false;
        }

        bool success = currentUser->changePassword(oldPwd, newPwd);
        if (success) {
            std::cout << "密码修改成功！" << std::endl;
        }
        else {
            std::cout << "旧密码错误，密码修改失败！" << std::endl;
        }
        return success;
    }
    std::cout << "没有用户登录！" << std::endl;
    return false;
}

// 获取当前用户
User* UserManager::getCurrentUser() const {
    return currentUser;
}

// 检查是否已登录
bool UserManager::isLoggedIn() const {
    return currentUser != nullptr;
}

// 检查是否是管理员登录
bool UserManager::isAdminLoggedIn() const {
    if (currentUser == &admin) {
        return true;
    }
    return false;
}

// 检查是否是顾客登录
bool UserManager::isCustomerLoggedIn() const {
    if (!currentUser) return false;

    // 如果不是管理员，就是顾客
    if (currentUser != &admin) {
        // 进一步验证是顾客对象
        for (const auto& customer : customers) {
            if (customer.getUsername() == currentUser->getUsername()) {
                return true;
            }
        }
    }
    return false;
}

// 更新顾客信息
bool UserManager::updateCustomerProfile(const std::string& username,
    const std::string& name, const std::string& email,
    const std::string& phone, const std::string& address) {
    Customer* customer = findCustomerByUsername(username);
    if (customer) {
        // 检查必填字段
        if (name.empty() || email.empty()) {
            std::cout << "姓名和邮箱为必填项！" << std::endl;
            return false;
        }

        customer->updateProfile(name, email, phone, address);
        std::cout << "个人信息更新成功！" << std::endl;
        return true;
    }
    std::cout << "用户不存在！" << std::endl;
    return false;
}

// 通过用户名获取顾客
Customer* UserManager::getCustomer(const std::string& username) {
    return findCustomerByUsername(username);
}

// 更新所有顾客购物车中的产品信息
void UserManager::updateAllCustomerCarts(const std::string& productName, Product* updatedProduct) {
    for (auto& customer : customers) {
        if (customer.getCart().containsItem(productName)) {
            customer.getCart().updateProductInfo(updatedProduct);
        }
    }
}