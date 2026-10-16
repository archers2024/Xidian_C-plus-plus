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

    virtual void displayInfo() const = 0;

    bool login(const std::string& uname, const std::string& pwd) const;
    bool changePassword(const std::string& oldPwd, const std::string& newPwd);

    // Getter方法 - 只声明
    std::string getUsername() const;

    // 密码访问方法
    std::string getPassword() const;
    void setPassword(const std::string& newPwd);
};

#endif // USER_HPP