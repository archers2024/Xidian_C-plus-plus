// Admin.hpp - 管理员类
#ifndef ADMIN_HPP
#define ADMIN_HPP

#include "User.hpp"
#include <string>

class Admin : public User {
public:
    // 默认管理员账号
    Admin();
    Admin(const std::string& uname, const std::string& pwd);
    
    void displayInfo() const override;
    
    // 管理员特有功能占位
    void manageProducts();
    void viewStatistics();
};

#endif
