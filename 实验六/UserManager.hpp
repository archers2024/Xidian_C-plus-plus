#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include "Admin.hpp"
#include "Customer.hpp"
#include <vector>
#include <memory>

class UserManager {
private:
    Admin admin;
    std::vector<Customer> customers;
    User* currentUser;

    bool isUsernameExist(const std::string& username) const;
    Customer* findCustomerByUsername(const std::string& username);

public:
    UserManager();

    // 登录相关
    bool adminLogin(const std::string& username, const std::string& password);
    bool customerRegister(const std::string& username, const std::string& password,
        const std::string& name, const std::string& email,
        const std::string& phone = "", const std::string& address = "");
    bool customerLogin(const std::string& username, const std::string& password);
    void logout();

    // 密码管理
    bool changeCurrentUserPassword(const std::string& oldPwd, const std::string& newPwd);

    // 状态查询
    User* getCurrentUser() const;
    bool isLoggedIn() const;
    bool isAdminLoggedIn() const;
    bool isCustomerLoggedIn() const;

    // 顾客信息管理
    bool updateCustomerProfile(const std::string& username,
        const std::string& name, const std::string& email,
        const std::string& phone, const std::string& address);

    // 辅助方法（可选）
    Customer* getCustomer(const std::string& username);
    const std::vector<Customer>& getAllCustomers() const;
};

#endif