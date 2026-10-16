// User.cpp
#include "User.hpp"
#include <iostream>

User::User(const std::string& uname, const std::string& pwd) 
    : username(uname), password(pwd) {}

bool User::login(const std::string& uname, const std::string& pwd) const {
    return (username == uname && password == pwd);
}

bool User::changePassword(const std::string& oldPwd, const std::string& newPwd) {
    if (password == oldPwd) {
        password = newPwd;
        return true;
    }
    return false;
}

std::string User::getUsername() const {
    return username;
}
