#ifndef MENUMANAGER_HPP
#define MENUMANAGER_HPP

#include "UserManager.hpp"
#include "ProductList.hpp"
#include <string>

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

public:
    MenuManager(UserManager& um, ProductList& pl);
    void runMainMenu();
};

#endif
