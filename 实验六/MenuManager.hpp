#ifndef MENUMANAGER_HPP
#define MENUMANAGER_HPP

#include "UserManager.hpp"
#include "ProductList.hpp"
#include "ShoppingCart.hpp"
#include "OrderList.hpp"
#include <string>

class MenuManager {
private:
    UserManager& userManager;
    ProductList& productList;
    OrderList& orderList;

    // 输入辅助函数
    std::string getInput(const std::string& prompt);
    double getDoubleInput(const std::string& prompt);
    int getIntInput(const std::string& prompt);

    // 主菜单相关
    void displayMainMenu();
    void handleAdminLogin();
    void handleCustomerRegister();
    void handleCustomerLogin();

    // 管理员菜单
    void displayAdminMenu();
    void handleAdminPasswordChange();
    void handleProductManagement();
    void handleAdminOrderManagement();

    // 商品管理菜单
    void displayProductManagementMenu();
    void handleAddProduct();
    void handleRemoveProduct();
    void handleUpdateProduct();
    void handleViewAllProducts();
    void handleViewProductsByCategory();

    // 顾客菜单
    void displayCustomerMenu();
    void handleCustomerPasswordChange();
    void handleCustomerViewProducts();
    void handleCustomerProfileUpdate();
    void handleCartManagement();
    void handleOrderManagement();

    // 购物车菜单
    void displayCartMenu();
    void handleAddToCart();
    void handleRemoveFromCart();
    void handleUpdateCartQuantity();
    void handleViewCart();
    void handleClearCart();

    // 订单菜单
    void displayOrderMenu();
    void handleCreateOrder();
    void handleViewOrders();
    void handleCancelOrder();
    void handleUpdateOrderAddress();
    void handleDeleteOrder();

    // 管理员订单管理
    void displayAdminOrderMenu();
    void handleAdminViewOrders();
    void handleAdminShipOrder();
    void handleAdminDeliverOrder();
    void handleAdminCancelOrder();

public:
    MenuManager(UserManager& um, ProductList& pl, OrderList& ol);
    void runMainMenu();
};

#endif