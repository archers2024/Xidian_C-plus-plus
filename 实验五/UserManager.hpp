// UserManager.hpp - 用户管理类
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
    User* currentUser;  // 当前登录用户
    
    // 辅助函数
    bool isUsernameExist(const std::string& username) const;
    Customer* findCustomerByUsername(const std::string& username);

public:
    UserManager();
    
    // 管理员登录
    bool adminLogin(const std::string& username, const std::string& password);
    
    // 顾客注册和登录
    bool customerRegister(const std::string& username, const std::string& password,
                         const std::string& name, const std::string& email,
                         const std::string& phone = "", const std::string& address = "");
    bool customerLogin(const std::string& username, const std::string& password);
    
    // 登出
    void logout();
    
    // 密码修改
    bool changeCurrentUserPassword(const std::string& oldPwd, const std::string& newPwd);
    
    // 获取当前用户信息
    User* getCurrentUser() const;
    bool isLoggedIn() const;
    bool isAdminLoggedIn() const;
    bool isCustomerLoggedIn() const;
    
    // 顾客信息管理
    bool updateCustomerProfile(const std::string& username, 
                              const std::string& name, const std::string& email,
                              const std::string& phone, const std::string& address);
};

#endif
