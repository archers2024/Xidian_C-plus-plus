#ifndef MENUMANAGER_HPP
#define MENUMANAGER_HPP

#include "UserManager.hpp"
#include "ProductList.hpp"
#include "ShoppingCart.hpp"
#include <string>
#include <iostream> 
#include <limits>

class MenuManager {
private:
    UserManager& userManager;
    ProductList& productList;

    std::string getInput(const std::string& prompt);
    double getDoubleInput(const std::string& prompt);
    int getIntInput(const std::string& prompt);

    void displayMainMenu();
    void handleAdminLogin();
    void handleCustomerRegister();
    void handleCustomerLogin();

    void displayAdminMenu();
    void handleAdminPasswordChange();
    void handleProductManagement();

    void displayProductManagementMenu();
    void handleAddProduct();
    void handleRemoveProduct();
    void handleUpdateProduct();
    void handleViewAllProducts();
    void handleViewProductsByCategory();

    void displayCustomerMenu();
    void handleCustomerPasswordChange();
    void handleCustomerViewProducts();
    void handleCustomerProfileUpdate();

    // 新增：购物车菜单
    void displayCartMenu();
    void handleAddToCart();
    void handleRemoveFromCart();
    void handleUpdateCartQuantity();
    void handleViewCart();
    void handleClearCart();

public:
    MenuManager(UserManager& um, ProductList& pl);
    void runMainMenu();
};

#endif