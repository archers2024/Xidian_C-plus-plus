#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include "User.hpp"
#include "Customer.hpp"
#include "Admin.hpp"
#include <vector>
#include <string>

class UserManager {
private:
    std::vector<Customer> customers;
    Admin admin;
    User* currentUser;

public:
    // 构造函数
    UserManager();

    // 用户管理
    bool isUsernameExist(const std::string& username) const;
    Customer* findCustomerByUsername(const std::string& username);

    // 登录注册
    bool adminLogin(const std::string& username, const std::string& password);
    bool customerRegister(const std::string& username, const std::string& password,
        const std::string& name, const std::string& email,
        const std::string& phone = "", const std::string& address = "");
    bool customerLogin(const std::string& username, const std::string& password);
    void logout();

    // 用户操作
    bool changeCurrentUserPassword(const std::string& oldPwd, const std::string& newPwd);
    User* getCurrentUser() const;
    bool isLoggedIn() const;
    bool isAdminLoggedIn() const;
    bool isCustomerLoggedIn() const;

    // 顾客信息管理
    bool updateCustomerProfile(const std::string& username,
        const std::string& name, const std::string& email,
        const std::string& phone = "", const std::string& address = "");

    // 获取顾客信息
    const std::vector<Customer>& getAllCustomers() const;
    Customer* getCustomer(const std::string& username);

    // 购物车相关
    void updateAllCustomerCarts(const std::string& productName, Product* updatedProduct);
};

#endif // USERMANAGER_HPP