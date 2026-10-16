#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include <vector>
#include <string>
#include "User.hpp"
#include "Admin.hpp"
#include "Customer.hpp"

class UserManager {
private:
    std::vector<Customer> customers;
    Admin admin;
    User* currentUser;

    bool isUsernameExist(const std::string& username) const;
    Customer* findCustomerByUsername(const std::string& username);

public:
    UserManager();

    // 用户认证
    bool adminLogin(const std::string& username, const std::string& password);
    bool customerRegister(const std::string& username, const std::string& password,
        const std::string& name, const std::string& email,
        const std::string& phone = "", const std::string& address = "");
    bool customerLogin(const std::string& username, const std::string& password);
    void logout();

    // 密码管理
    bool changeCurrentUserPassword(const std::string& oldPwd, const std::string& newPwd);

    // 获取用户信息
    User* getCurrentUser() const;
    const Admin& getAdmin() const { return admin; }
    Admin& getAdmin() { return admin; }
    const std::vector<Customer>& getAllCustomers() const { return customers; }

    // 状态检查
    bool isLoggedIn() const;
    bool isAdminLoggedIn() const;
    bool isCustomerLoggedIn() const;

    // 顾客管理
    bool updateCustomerProfile(const std::string& username,
        const std::string& name, const std::string& email,
        const std::string& phone, const std::string& address);
    Customer* getCustomer(const std::string& username);

    // ==== 新增：持久化支持 ====
    void clearCustomers() { customers.clear(); }
    void addCustomerDirectly(const Customer& customer) {
        customers.push_back(customer);
    }

    // 购物车更新
    void updateAllCustomerCarts(const std::string& productName, Product* updatedProduct);
};

#endif // USERMANAGER_HPP