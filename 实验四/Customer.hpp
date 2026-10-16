// Customer.hpp - 顾客类
#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include "User.hpp"
#include <string>

class Customer : public User {
private:
    std::string name;
    std::string email;
    std::string phone;
    std::string address;

public:
    Customer(const std::string& uname, const std::string& pwd, 
             const std::string& cname, const std::string& cemail);
    
    // 注册时的完整构造函数
    Customer(const std::string& uname, const std::string& pwd, 
             const std::string& cname, const std::string& cemail,
             const std::string& cphone, const std::string& caddr);
    
    void displayInfo() const override;
    
    // 顾客信息管理
    void updateProfile(const std::string& newName, const std::string& newEmail,
                      const std::string& newPhone, const std::string& newAddr);
    
    // getter方法
    std::string getName() const;
    std::string getEmail() const;
    std::string getPhone() const;
    std::string getAddress() const;
};

#endif
