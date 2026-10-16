#ifndef ADMIN_HPP
#define ADMIN_HPP

#include "User.hpp"
#include <string>

class Admin : public User {
public:
    Admin();
    Admin(const std::string& uname, const std::string& pwd);

    void displayInfo() const override;

    void manageProducts();
    void viewStatistics();

    // ==== 新增：序列化方法 ====
    std::string serialize() const;
    static Admin deserialize(const std::string& data);

    // ==== 新增：密码访问方法（用于持久化）====
    std::string getPassword() const { return password; }
    void setPassword(const std::string& newPwd) { password = newPwd; }
};

#endif // ADMIN_HPP