// main.cpp - 测试程序
#include "UserManager.hpp"
#include <iostream>
#include <string>



void displayMainMenu() {
    std::cout << "\n=== 网上购物系统 ===" << std::endl;
    std::cout << "1. 管理员登录" << std::endl;
    std::cout << "2. 顾客注册" << std::endl;
    std::cout << "3. 顾客登录" << std::endl;
    std::cout << "4. 退出" << std::endl;
    std::cout << "请选择: ";
}


void displayAdminMenu() {
    std::cout << "\n=== 管理员菜单 ===" << std::endl;
    std::cout << "1. 修改密码" << std::endl;
    std::cout << "2. 查看信息" << std::endl;
    std::cout << "3. 退出登录" << std::endl;
    std::cout << "请选择: ";
}

void displayCustomerMenu() {
    std::cout << "\n=== 顾客菜单 ===" << std::endl;
    std::cout << "1. 修改密码" << std::endl;
    std::cout << "2. 查看信息" << std::endl;
    std::cout << "3. 退出登录" << std::endl;
    std::cout << "请选择: ";
}

int main() {
    UserManager userManager;
    int choice;
    std::string username, password, name, email, phone, address;
    
    while (true) {
        if (!userManager.isLoggedIn()) {
            // 未登录状态
            displayMainMenu();
            std::cin >> choice;
            std::cin.ignore(); // 清除输入缓冲区
            
            switch (choice) {
                case 1: {
                    // 管理员登录
                    std::cout << "用户名: ";
                    std::getline(std::cin, username);
                    std::cout << "密码: ";
                    std::getline(std::cin, password);
                    
                    if (userManager.adminLogin(username, password)) {
                        std::cout << "管理员登录成功！" << std::endl;
                    } else {
                        std::cout << "登录失败！" << std::endl;
                    }
                    break;
                }
                case 2: {
                    // 顾客注册
                    std::cout << "用户名: ";
                    std::getline(std::cin, username);
                    std::cout << "密码: ";
                    std::getline(std::cin, password);
                    std::cout << "姓名: ";
                    std::getline(std::cin, name);
                    std::cout << "邮箱: ";
                    std::getline(std::cin, email);
                    std::cout << "电话(可选): ";
                    std::getline(std::cin, phone);
                    std::cout << "地址(可选): ";
                    std::getline(std::cin, address);
                    
                    userManager.customerRegister(username, password, name, email, phone, address);
                    break;
                }
                case 3: {
                    // 顾客登录
                    std::cout << "用户名: ";
                    std::getline(std::cin, username);
                    std::cout << "密码: ";
                    std::getline(std::cin, password);
                    
                    if (userManager.customerLogin(username, password)) {
                        std::cout << "顾客登录成功！" << std::endl;
                    } else {
                        std::cout << "登录失败！" << std::endl;
                    }
                    break;
                }
                case 4:
                    std::cout << "再见！" << std::endl;
                    return 0;
                default:
                    std::cout << "无效选择！" << std::endl;
            }
        } else {
            // 已登录状态
            if (userManager.isAdminLoggedIn()) {
                displayAdminMenu();
                std::cin >> choice;
                std::cin.ignore();
                
                switch (choice) {
                    case 1: {
                        std::string oldPwd, newPwd;
                        std::cout << "旧密码: ";
                        std::getline(std::cin, oldPwd);
                        std::cout << "新密码: ";
                        std::getline(std::cin, newPwd);
                        
                        if (userManager.changeCurrentUserPassword(oldPwd, newPwd)) {
                            std::cout << "密码修改成功！" << std::endl;
                        } else {
                            std::cout << "密码修改失败！" << std::endl;
                        }
                        break;
                    }
                    case 2:
                        userManager.getCurrentUser()->displayInfo();
                        break;
                    case 3:
                        userManager.logout();
                        std::cout << "已退出登录" << std::endl;
                        break;
                    default:
                        std::cout << "无效选择！" << std::endl;
                }
            } else if (userManager.isCustomerLoggedIn()) {
                displayCustomerMenu();
                std::cin >> choice;
                std::cin.ignore();
                
                switch (choice) {
                    case 1: {
                        std::string oldPwd, newPwd;
                        std::cout << "旧密码: ";
                        std::getline(std::cin, oldPwd);
                        std::cout << "新密码: ";
                        std::getline(std::cin, newPwd);
                        
                        if (userManager.changeCurrentUserPassword(oldPwd, newPwd)) {
                            std::cout << "密码修改成功！" << std::endl;
                        } else {
                            std::cout << "密码修改失败！" << std::endl;
                        }
                        break;
                    }
                    case 2:
                        userManager.getCurrentUser()->displayInfo();
                        break;
                    case 3:
                        userManager.logout();
                        std::cout << "已退出登录" << std::endl;
                        break;
                    default:
                        std::cout << "无效选择！" << std::endl;
                }
            }
        }
    }
    
    return 0;
}
