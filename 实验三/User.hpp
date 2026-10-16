// User.hpp - 用户基类
#ifndef USER_HPP
#define USER_HPP

#include <string>

class User {
protected:
    std::string username;
    std::string password;

public:
    User(const std::string& uname, const std::string& pwd);
    virtual ~User() = default;
    
    // 登录验证
    virtual bool login(const std::string& uname, const std::string& pwd) const;
    
    // 修改密码
    virtual bool changePassword(const std::string& oldPwd, const std::string& newPwd);
    
    // 获取用户名
    std::string getUsername() const;
    
    // 纯虚函数 - 使类成为抽象类
    virtual void displayInfo() const = 0;
};

#endif
