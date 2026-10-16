#include "MenuManager.hpp"
#include <iostream>
#include <limits>
#include <iomanip>

MenuManager::MenuManager(UserManager& um, ProductList& pl)
    : userManager(um), productList(pl) {
}

std::string MenuManager::getInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

double MenuManager::getDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入无效，请输入数字！" << std::endl;
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    return value;
}

int MenuManager::getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入无效，请输入整数！" << std::endl;
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    return value;
}

void MenuManager::displayMainMenu() {
    std::cout << "\n=== 网上购物系统 ===" << std::endl;
    std::cout << "1. 管理员登录" << std::endl;
    std::cout << "2. 顾客注册" << std::endl;
    std::cout << "3. 顾客登录" << std::endl;
    std::cout << "4. 浏览商品" << std::endl;
    std::cout << "5. 退出系统" << std::endl;
    std::cout << "请选择: ";
}

void MenuManager::handleAdminLogin() {
    std::string username = getInput("用户名: ");
    std::string password = getInput("密码: ");
    userManager.adminLogin(username, password);
}

void MenuManager::handleCustomerRegister() {
    std::string username = getInput("用户名: ");
    std::string password = getInput("密码: ");
    std::string name = getInput("姓名: ");
    std::string email = getInput("邮箱: ");
    std::string phone = getInput("电话(可选): ");
    std::string address = getInput("地址(可选): ");

    userManager.customerRegister(username, password, name, email, phone, address);
}

void MenuManager::handleCustomerLogin() {
    std::string username = getInput("用户名: ");
    std::string password = getInput("密码: ");
    userManager.customerLogin(username, password);
}

void MenuManager::displayAdminMenu() {
    std::cout << "\n=== 管理员菜单 ===" << std::endl;
    std::cout << "1. 修改密码" << std::endl;
    std::cout << "2. 查看个人信息" << std::endl;
    std::cout << "3. 商品管理" << std::endl;
    std::cout << "4. 退出登录" << std::endl;
    std::cout << "请选择: ";
}

void MenuManager::handleAdminPasswordChange() {
    std::string oldPwd = getInput("旧密码: ");
    std::string newPwd = getInput("新密码: ");
    userManager.changeCurrentUserPassword(oldPwd, newPwd);
}

void MenuManager::handleProductManagement() {
    int choice;
    do {
        displayProductManagementMenu();
        choice = getIntInput("");

        switch (choice) {
        case 1:
            handleAddProduct();
            break;
        case 2:
            handleRemoveProduct();
            break;
        case 3:
            handleUpdateProduct();
            break;
        case 4:
            handleViewAllProducts();
            break;
        case 5:
            handleViewProductsByCategory();
            break;
        case 6:
            std::cout << "返回上级菜单..." << std::endl;
            break;
        default:
            std::cout << "无效选择！" << std::endl;
        }
    } while (choice != 6);
}

void MenuManager::displayProductManagementMenu() {
    std::cout << "\n=== 商品管理 ===" << std::endl;
    std::cout << "1. 添加商品" << std::endl;
    std::cout << "2. 删除商品" << std::endl;
    std::cout << "3. 修改商品" << std::endl;
    std::cout << "4. 查看所有商品" << std::endl;
    std::cout << "5. 按类别查看商品" << std::endl;
    std::cout << "6. 返回上级菜单" << std::endl;
    std::cout << "请选择: ";
}

void MenuManager::handleAddProduct() {
    std::string name = getInput("商品名称: ");
    double price = getDoubleInput("价格: ");
    int stock = getIntInput("库存: ");
    std::string category = getInput("类别: ");
    std::string description = getInput("描述: ");

    Product newProduct(name, price, stock, category, description);
    productList.addProduct(newProduct);
}

void MenuManager::handleRemoveProduct() {
    std::string name = getInput("要删除的商品名称: ");
    productList.removeProduct(name);
}

void MenuManager::handleUpdateProduct() {
    std::string name = getInput("要修改的商品名称: ");

    Product* product = productList.findProduct(name);
    if (product) {
        double newPrice = getDoubleInput("新价格: ");
        int newStock = getIntInput("新库存: ");
        std::string newCategory = getInput("新类别: ");
        std::string newDesc = getInput("新描述: ");

        productList.updateProduct(name, newPrice, newStock, newCategory, newDesc);
    }
    else {
        std::cout << "商品不存在！" << std::endl;
    }
}

void MenuManager::handleViewAllProducts() {
    productList.displayAllProducts();
}

void MenuManager::handleViewProductsByCategory() {
    std::string category = getInput("请输入类别: ");
    productList.displayProductsByCategory(category);
}

void MenuManager::displayCustomerMenu() {
    std::cout << "\n=== 顾客菜单 ===" << std::endl;
    std::cout << "1. 修改密码" << std::endl;
    std::cout << "2. 查看个人信息" << std::endl;
    std::cout << "3. 更新个人信息" << std::endl;
    std::cout << "4. 浏览商品" << std::endl;
    std::cout << "5. 购物车管理" << std::endl;
    std::cout << "6. 退出登录" << std::endl;
    std::cout << "请选择: ";
}

void MenuManager::handleCustomerPasswordChange() {
    std::string oldPwd = getInput("旧密码: ");
    std::string newPwd = getInput("新密码: ");
    userManager.changeCurrentUserPassword(oldPwd, newPwd);
}

void MenuManager::handleCustomerViewProducts() {
    productList.displayAllProducts();
}

void MenuManager::handleCustomerProfileUpdate() {
    if (userManager.isCustomerLoggedIn()) {
        Customer* customer = dynamic_cast<Customer*>(userManager.getCurrentUser());
        if (customer) {
            std::string name = getInput("新姓名: ");
            std::string email = getInput("新邮箱: ");
            std::string phone = getInput("新电话: ");
            std::string address = getInput("新地址: ");

            userManager.updateCustomerProfile(customer->getUsername(), name, email, phone, address);
        }
    }
}

void MenuManager::displayCartMenu() {
    std::cout << "\n=== 购物车管理 ===" << std::endl;
    std::cout << "1. 添加商品到购物车" << std::endl;
    std::cout << "2. 从购物车移除商品" << std::endl;
    std::cout << "3. 修改商品数量" << std::endl;
    std::cout << "4. 查看购物车" << std::endl;
    std::cout << "5. 清空购物车" << std::endl;
    std::cout << "6. 返回上级菜单" << std::endl;
    std::cout << "请选择: ";
}

void MenuManager::handleAddToCart() {
    if (!userManager.isCustomerLoggedIn()) {
        std::cout << "请先登录顾客账号！" << std::endl;
        return;
    }

    productList.displayAllProducts();
    std::string productName = getInput("请输入要添加的商品名称: ");

    Product* product = productList.findProduct(productName);
    if (!product) {
        std::cout << "商品不存在！" << std::endl;
        return;
    }

    int quantity = getIntInput("请输入数量: ");
    if (quantity <= 0) {
        std::cout << "数量必须大于0！" << std::endl;
        return;
    }

    Customer* customer = dynamic_cast<Customer*>(userManager.getCurrentUser());
    if (customer) {
        customer->getCart().addItem(*product, quantity);
    }
}

void MenuManager::handleRemoveFromCart() {
    if (!userManager.isCustomerLoggedIn()) {
        std::cout << "请先登录顾客账号！" << std::endl;
        return;
    }

    Customer* customer = dynamic_cast<Customer*>(userManager.getCurrentUser());
    if (customer && !customer->getCart().isEmpty()) {
        customer->getCart().displayCart();
        std::string productName = getInput("请输入要移除的商品名称: ");
        int quantity = getIntInput("请输入要移除的数量(0表示移除全部): ");
        customer->getCart().removeItem(productName, quantity);
    }
    else {
        std::cout << "购物车为空！" << std::endl;
    }
}

void MenuManager::handleUpdateCartQuantity() {
    if (!userManager.isCustomerLoggedIn()) {
        std::cout << "请先登录顾客账号！" << std::endl;
        return;
    }

    Customer* customer = dynamic_cast<Customer*>(userManager.getCurrentUser());
    if (customer && !customer->getCart().isEmpty()) {
        customer->getCart().displayCart();
        std::string productName = getInput("请输入要修改数量的商品名称: ");
        int newQuantity = getIntInput("请输入新的数量: ");
        customer->getCart().updateItemQuantity(productName, newQuantity);
    }
    else {
        std::cout << "购物车为空！" << std::endl;
    }
}

void MenuManager::handleViewCart() {
    if (!userManager.isCustomerLoggedIn()) {
        std::cout << "请先登录顾客账号！" << std::endl;
        return;
    }

    Customer* customer = dynamic_cast<Customer*>(userManager.getCurrentUser());
    if (customer) {
        customer->getCart().displayCart();
    }
}

void MenuManager::handleClearCart() {
    if (!userManager.isCustomerLoggedIn()) {
        std::cout << "请先登录顾客账号！" << std::endl;
        return;
    }

    Customer* customer = dynamic_cast<Customer*>(userManager.getCurrentUser());
    if (customer) {
        std::cout << "确定要清空购物车吗？(y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 'y' || choice == 'Y') {
            customer->getCart().clearCart();
        }
    }
}

void MenuManager::runMainMenu() {
    int choice;

    while (true) {
        if (!userManager.isLoggedIn()) {
            displayMainMenu();
            choice = getIntInput("");

            switch (choice) {
            case 1:
                handleAdminLogin();
                break;
            case 2:
                handleCustomerRegister();
                break;
            case 3:
                handleCustomerLogin();
                break;
            case 4:
                productList.displayAllProducts();
                break;
            case 5:
                std::cout << "感谢使用网上购物系统！" << std::endl;
                return;
            default:
                std::cout << "无效选择！" << std::endl;
            }
        }
        else {
            if (userManager.isAdminLoggedIn()) {
                displayAdminMenu();
                choice = getIntInput("");

                switch (choice) {
                case 1:
                    handleAdminPasswordChange();
                    break;
                case 2:
                    userManager.getCurrentUser()->displayInfo();
                    break;
                case 3:
                    handleProductManagement();
                    break;
                case 4:
                    userManager.logout();
                    break;
                default:
                    std::cout << "无效选择！" << std::endl;
                }
            }
            else if (userManager.isCustomerLoggedIn()) {
                displayCustomerMenu();
                choice = getIntInput("");

                switch (choice) {
                case 1:
                    handleCustomerPasswordChange();
                    break;
                case 2:
                    userManager.getCurrentUser()->displayInfo();
                    break;
                case 3:
                    handleCustomerProfileUpdate();
                    break;
                case 4:
                    handleCustomerViewProducts();
                    break;
                case 5:
                {
                    int cartChoice;
                    do {
                        displayCartMenu();
                        cartChoice = getIntInput("");
                        switch (cartChoice) {
                        case 1: handleAddToCart(); break;
                        case 2: handleRemoveFromCart(); break;
                        case 3: handleUpdateCartQuantity(); break;
                        case 4: handleViewCart(); break;
                        case 5: handleClearCart(); break;
                        case 6: std::cout << "返回上级菜单..." << std::endl; break;
                        default: std::cout << "无效选择！" << std::endl;
                        }
                    } while (cartChoice != 6);
                }
                break;
                case 6: userManager.logout(); break;
                default: std::cout << "无效选择！" << std::endl;
                }
            }
        }
    }
}