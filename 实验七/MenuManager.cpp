#include "MenuManager.hpp"
#include "PromotionMenu.hpp"
#include <iostream>
#include <limits>
#include <iomanip>
#include <chrono>
#include <thread>
#include <vector>
#include <string>

// 构造函数
MenuManager::MenuManager(UserManager& um, ProductList& pl, OrderList& ol, PromotionManager& pm)
    : userManager(um), productList(pl), orderList(ol), promotionManager(pm) {
}

// ========== 基础输入方法 ==========

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

// ========== 主菜单相关 ==========

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

// ========== 管理员菜单相关 ==========

void MenuManager::displayAdminMenu() {
    std::cout << "\n=== 管理员菜单 ===" << std::endl;
    std::cout << "1. 修改密码" << std::endl;
    std::cout << "2. 查看个人信息" << std::endl;
    std::cout << "3. 商品管理" << std::endl;
    std::cout << "4. 订单管理" << std::endl;
    std::cout << "5. 促销活动管理" << std::endl;
    std::cout << "6. 退出登录" << std::endl;
    std::cout << "请选择: ";
}

void MenuManager::handleAdminPasswordChange() {
    std::string oldPwd = getInput("旧密码: ");
    std::string newPwd = getInput("新密码: ");
    userManager.changeCurrentUserPassword(oldPwd, newPwd);
}

// ========== 促销管理相关 ==========

void MenuManager::handlePromotionManagement() {
    PromotionMenu promotionMenu(promotionManager, productList);
    promotionMenu.run();
}

// ========== 商品管理相关 ==========

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
    std::cout << "\n=== 查看商品 ===" << std::endl;
    std::cout << "1. 查看所有商品（简单信息）" << std::endl;
    std::cout << "2. 查看所有商品（含促销信息）" << std::endl;
    std::cout << "请选择: ";

    int choice = getIntInput("");
    switch (choice) {
    case 1:
        productList.displayAllProducts();
        break;
    case 2:
        productList.displayAllProductsWithPromotion(&promotionManager);
        break;
    default:
        std::cout << "无效选择！" << std::endl;
    }
}

void MenuManager::handleViewProductsByCategory() {
    std::string category = getInput("请输入类别: ");
    std::cout << "\n=== 查看商品 ===" << std::endl;
    std::cout << "1. 简单信息" << std::endl;
    std::cout << "2. 含促销信息" << std::endl;
    std::cout << "请选择: ";

    int choice = getIntInput("");
    switch (choice) {
    case 1:
        productList.displayProductsByCategory(category);
        break;
    case 2:
        productList.displayProductsByCategoryWithPromotion(category, &promotionManager);
        break;
    default:
        std::cout << "无效选择！" << std::endl;
    }
}

// ========== 顾客菜单相关 ==========

void MenuManager::displayCustomerMenu() {
    std::cout << "\n=== 顾客菜单 ===" << std::endl;
    std::cout << "1. 修改密码" << std::endl;
    std::cout << "2. 查看个人信息" << std::endl;
    std::cout << "3. 更新个人信息" << std::endl;
    std::cout << "4. 浏览商品" << std::endl;
    std::cout << "5. 购物车管理" << std::endl;
    std::cout << "6. 订单管理" << std::endl;
    std::cout << "7. 查看促销活动" << std::endl;
    std::cout << "8. 退出登录" << std::endl;
    std::cout << "请选择: ";
}

void MenuManager::handleCustomerPasswordChange() {
    std::string oldPwd = getInput("旧密码: ");
    std::string newPwd = getInput("新密码: ");
    userManager.changeCurrentUserPassword(oldPwd, newPwd);
}

void MenuManager::handleCustomerViewProducts() {
    std::cout << "\n=== 浏览商品 ===" << std::endl;
    std::cout << "1. 查看所有商品" << std::endl;
    std::cout << "2. 查看促销商品" << std::endl;
    std::cout << "3. 按类别查看" << std::endl;
    std::cout << "4. 搜索商品" << std::endl;
    std::cout << "请选择: ";

    int choice = getIntInput("");
    switch (choice) {
    case 1:
        productList.displayAllProductsWithPromotion(&promotionManager);
        break;
    case 2:
    {
        std::cout << "\n=== 促销商品列表 ===" << std::endl;
        std::vector<Product*> promotionalProducts = productList.getPromotionalProducts(&promotionManager);
        if (promotionalProducts.empty()) {
            std::cout << "暂无促销商品！" << std::endl;
        }
        else {
            for (size_t i = 0; i < promotionalProducts.size(); ++i) {
                Product* product = promotionalProducts[i];
                if (product) {
                    product->displayWithPromotion(&promotionManager);
                }
            }
        }
    }
    break;
    case 3: {
        std::string category = getInput("请输入类别: ");
        productList.displayProductsByCategoryWithPromotion(category, &promotionManager);
        break;
    }
    case 4: {
        std::string keyword = getInput("请输入搜索关键词: ");
        std::vector<Product*> products = productList.findProductsByName(keyword);
        if (products.empty()) {
            std::cout << "未找到相关商品！" << std::endl;
        }
        else {
            std::cout << "\n=== 搜索结果 ===" << std::endl;
            for (size_t i = 0; i < products.size(); ++i) {
                Product* product = products[i];
                if (product) {
                    product->displayWithPromotion(&promotionManager);
                }
            }
        }
        break;
    }
    default:
        std::cout << "无效选择！" << std::endl;
    }
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

// ========== 购物车管理相关 ==========

void MenuManager::handleCartManagement() {
    int cartChoice;
    do {
        displayCartMenu();
        cartChoice = getIntInput("");
        switch (cartChoice) {
        case 1:
            handleAddToCart();
            break;
        case 2:
            handleRemoveFromCart();
            break;
        case 3:
            handleUpdateCartQuantity();
            break;
        case 4:
            handleViewCart();
            break;
        case 5:
            handleClearCart();
            break;
        case 6:
            std::cout << "返回上级菜单..." << std::endl;
            break;
        default:
            std::cout << "无效选择！" << std::endl;
        }
    } while (cartChoice != 6);
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

    std::cout << "\n=== 商品列表（含促销信息） ===" << std::endl;
    productList.displayAllProductsWithPromotion(&promotionManager);

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

    if (product->getStock() < quantity) {
        std::cout << "库存不足！当前库存: " << product->getStock() << std::endl;
        return;
    }

    Customer* customer = dynamic_cast<Customer*>(userManager.getCurrentUser());
    if (customer) {
        if (product->hasPromotion(&promotionManager)) {
            double originalPrice = product->getPrice() * quantity;
            double discountedPrice = promotionManager.calculateProductPrice(*product, quantity);
            std::cout << "促销信息: 原价￥" << std::fixed << std::setprecision(2) << originalPrice
                << "，折后价￥" << std::fixed << std::setprecision(2) << discountedPrice
                << "，节省￥" << std::fixed << std::setprecision(2) << (originalPrice - discountedPrice) << std::endl;
        }

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

        const auto& items = customer->getCart().getItems();
        auto it = items.find(productName);
        if (it != items.end()) {
            Product* product = productList.findProduct(productName);
            if (product) {
                int currentInCart = it->second.second;
                int stockChange = newQuantity - currentInCart;
                if (stockChange > 0 && product->getStock() < stockChange) {
                    std::cout << "库存不足！当前库存: " << product->getStock()
                        << "，需要增加: " << stockChange << std::endl;
                    return;
                }
            }
        }

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

        double cartTotal = customer->getCart().getTotalPrice();

        std::cout << "\n=== 可用的促销活动 ===" << std::endl;
        std::vector<FullReductionPromotion*> fullReductions = promotionManager.getActiveFullReductions();
        if (fullReductions.empty()) {
            std::cout << "暂无可用满减活动" << std::endl;
        }
        else {
            bool hasQualified = false;
            for (size_t i = 0; i < fullReductions.size(); ++i) {
                FullReductionPromotion* fr = fullReductions[i];
                if (fr) {
                    if (fr->isQualified(cartTotal)) {
                        std::cout << "[符合] " << fr->getPromotionTag()
                            << " - 可减免￥" << std::fixed << std::setprecision(2) << fr->getReduction() << std::endl;
                        hasQualified = true;
                    }
                    else {
                        double needed = fr->getThreshold() - cartTotal;
                        if (needed > 0) {
                            std::cout << "[需凑单] " << fr->getPromotionTag()
                                << " - 还需￥" << std::fixed << std::setprecision(2) << needed << std::endl;
                        }
                    }
                }
            }

            if (hasQualified) {
                double bestReduction = 0.0;
                std::string bestPromotion = "";
                for (size_t i = 0; i < fullReductions.size(); ++i) {
                    FullReductionPromotion* fr = fullReductions[i];
                    if (fr && fr->isQualified(cartTotal) && fr->getReduction() > bestReduction) {
                        bestReduction = fr->getReduction();
                        bestPromotion = fr->getPromotionTag();
                    }
                }

                double finalPrice = cartTotal - bestReduction;
                std::cout << "\n=== 预估结算 ===" << std::endl;
                std::cout << "购物车总额: ￥" << std::fixed << std::setprecision(2) << cartTotal << std::endl;
                std::cout << "最优促销: " << bestPromotion << std::endl;
                std::cout << "预估实付: ￥" << std::fixed << std::setprecision(2) << finalPrice << std::endl;
                std::cout << "预估节省: ￥" << std::fixed << std::setprecision(2) << bestReduction << std::endl;
            }
        }

        std::cout << "\n=== 商品促销信息 ===" << std::endl;
        const auto& items = customer->getCart().getItems();
        bool hasProductPromotion = false;
        for (const auto& item : items) {
            const Product& product = item.second.first;
            int quantity = item.second.second;

            if (product.hasPromotion(&promotionManager)) {
                double originalPrice = product.getPrice() * quantity;
                double discountedPrice = promotionManager.calculateProductPrice(product, quantity);

                if (discountedPrice < originalPrice) {
                    std::cout << "特价商品 " << product.getName()
                        << " x" << quantity
                        << ": 原价￥" << std::fixed << std::setprecision(2) << originalPrice
                        << "，折后￥" << std::fixed << std::setprecision(2) << discountedPrice
                        << "，节省￥" << std::fixed << std::setprecision(2) << (originalPrice - discountedPrice) << std::endl;
                    hasProductPromotion = true;
                }
            }
        }

        if (!hasProductPromotion) {
            std::cout << "购物车中暂无商品享受折扣促销" << std::endl;
        }
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

// ========== 订单管理相关 ==========

void MenuManager::handleOrderManagement() {
    int orderChoice;
    do {
        displayOrderMenu();
        orderChoice = getIntInput("");
        switch (orderChoice) {
        case 1:
            handleCreateOrder();
            break;
        case 2:
            handleViewOrders();
            break;
        case 3:
            handleCancelOrder();
            break;
        case 4:
            handleUpdateOrderAddress();
            break;
        case 5:
            handleDeleteOrder();
            break;
        case 6:
            std::cout << "返回上级菜单..." << std::endl;
            break;
        default:
            std::cout << "无效选择！" << std::endl;
        }
    } while (orderChoice != 6);
}

void MenuManager::displayOrderMenu() {
    std::cout << "\n=== 订单管理 ===" << std::endl;
    std::cout << "1. 生成订单" << std::endl;
    std::cout << "2. 查看我的订单" << std::endl;
    std::cout << "3. 取消订单" << std::endl;
    std::cout << "4. 修改收货地址" << std::endl;
    std::cout << "5. 删除订单" << std::endl;
    std::cout << "6. 返回上级菜单" << std::endl;
    std::cout << "请选择: ";
}

void MenuManager::handleCreateOrder() {
    if (!userManager.isCustomerLoggedIn()) {
        std::cout << "请先登录顾客账号！" << std::endl;
        return;
    }

    Customer* customer = dynamic_cast<Customer*>(userManager.getCurrentUser());
    if (!customer) return;

    ShoppingCart& cart = customer->getCart();
    if (cart.isEmpty()) {
        std::cout << "购物车为空，无法生成订单！" << std::endl;
        return;
    }

    std::cout << "\n=== 购物车内容 ===" << std::endl;
    cart.displayCart();

    double cartTotal = cart.getTotalPrice();
    std::vector<FullReductionPromotion*> fullReductions = promotionManager.getActiveFullReductions();

    double bestReduction = 0.0;
    std::string bestPromotion = "无";
    for (size_t i = 0; i < fullReductions.size(); ++i) {
        FullReductionPromotion* fr = fullReductions[i];
        if (fr && fr->isQualified(cartTotal) && fr->getReduction() > bestReduction) {
            bestReduction = fr->getReduction();
            bestPromotion = fr->getPromotionTag();
        }
    }

    double finalPrice = cartTotal - bestReduction;

    std::cout << "\n=== 订单促销信息 ===" << std::endl;
    std::cout << "购物车总额: ￥" << std::fixed << std::setprecision(2) << cartTotal << std::endl;

    const auto& items = cart.getItems();
    double totalProductDiscount = 0.0;
    for (const auto& item : items) {
        const Product& product = item.second.first;
        int quantity = item.second.second;

        if (product.hasPromotion(&promotionManager)) {
            double originalPrice = product.getPrice() * quantity;
            double discountedPrice = promotionManager.calculateProductPrice(product, quantity);
            double productDiscount = originalPrice - discountedPrice;
            totalProductDiscount += productDiscount;

            std::cout << "  " << product.getName() << " x" << quantity
                << ": 商品折扣￥" << std::fixed << std::setprecision(2) << productDiscount << std::endl;
        }
    }

    if (bestReduction > 0) {
        std::cout << "满减优惠: " << bestPromotion << " (-￥" << std::fixed << std::setprecision(2) << bestReduction << ")" << std::endl;
    }

    std::cout << "总计节省: ￥" << std::fixed << std::setprecision(2) << (totalProductDiscount + bestReduction) << std::endl;

    bool stockAvailable = true;
    for (const auto& item : items) {
        const Product& product = item.second.first;
        int quantity = item.second.second;

        Product* stockProduct = productList.findProduct(product.getName());
        if (!stockProduct) {
            std::cout << "商品 '" << product.getName() << "' 不存在！" << std::endl;
            stockAvailable = false;
            break;
        }

        if (stockProduct->getStock() < quantity) {
            std::cout << "商品 '" << product.getName() << "' 库存不足！" << std::endl;
            std::cout << "需要: " << quantity << "，库存: " << stockProduct->getStock() << std::endl;
            stockAvailable = false;
            break;
        }
    }

    if (!stockAvailable) {
        std::cout << "库存不足，无法生成订单！" << std::endl;
        return;
    }

    std::cout << "\n确认生成订单吗？(y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore();

    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "取消订单生成" << std::endl;
        return;
    }

    std::string address = customer->getAddress();
    if (address.empty()) {
        address = getInput("请输入收货地址: ");
        customer->setAddress(address);
    }

    std::vector<std::pair<Product, int>> orderItems;
    for (const auto& item : items) {
        orderItems.push_back(item.second);
    }

    Order* newOrder = orderList.createOrder(orderItems, address,0.0);
    if (newOrder) {
        newOrder->applyPromotions(&promotionManager);

        for (const auto& item : items) {
            const Product& product = item.second.first;
            int quantity = item.second.second;
            productList.updateProductStock(product.getName(), -quantity);
        }

        cart.clearCart();

        std::cout << "\n订单生成成功！" << std::endl;
        newOrder->displayOrderInfo();
    }
}

void MenuManager::handleViewOrders() {
    if (!userManager.isCustomerLoggedIn()) {
        std::cout << "请先登录顾客账号！" << std::endl;
        return;
    }

    std::cout << "\n=== 我的订单 ===" << std::endl;
    orderList.displayAllOrders();

    std::cout << "\n是否查看订单详情？(y/n): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice == 'y' || choice == 'Y') {
        std::string orderId = getInput("请输入订单编号: ");
        Order* order = orderList.findOrder(orderId);
        if (order) {
            order->displayOrderInfo();
        }
        else {
            std::cout << "订单不存在！" << std::endl;
        }
    }
}

void MenuManager::handleCancelOrder() {
    if (!userManager.isCustomerLoggedIn()) {
        std::cout << "请先登录顾客账号！" << std::endl;
        return;
    }

    std::string orderId = getInput("请输入要取消的订单号: ");

    std::cout << "确定要取消订单 " << orderId << " 吗？(y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore();

    if (confirm == 'y' || confirm == 'Y') {
        if (orderList.cancelOrder(orderId, productList)) {
            std::cout << "订单取消成功！" << std::endl;
        }
    }
    else {
        std::cout << "取消操作已取消" << std::endl;
    }
}

void MenuManager::handleUpdateOrderAddress() {
    if (!userManager.isCustomerLoggedIn()) {
        std::cout << "请先登录顾客账号！" << std::endl;
        return;
    }

    std::string orderId = getInput("请输入要修改的订单号: ");
    std::string newAddress = getInput("请输入新的收货地址: ");

    if (orderList.updateOrderAddress(orderId, newAddress)) {
        Customer* customer = dynamic_cast<Customer*>(userManager.getCurrentUser());
        if (customer) {
            customer->setAddress(newAddress);
        }
    }
}

void MenuManager::handleDeleteOrder() {
    if (!userManager.isCustomerLoggedIn()) {
        std::cout << "请先登录顾客账号！" << std::endl;
        return;
    }

    std::string orderId = getInput("请输入要删除的订单号: ");

    if (orderList.removeOrder(orderId)) {
        std::cout << "订单删除成功！" << std::endl;
    }
}

// ========== 管理员订单管理相关 ==========

void MenuManager::handleAdminOrderManagement() {
    int adminOrderChoice;
    do {
        displayAdminOrderMenu();
        adminOrderChoice = getIntInput("");
        switch (adminOrderChoice) {
        case 1:
            handleAdminViewOrders();
            break;
        case 2:
            handleAdminShipOrder();
            break;
        case 3:
            handleAdminDeliverOrder();
            break;
        case 4:
            handleAdminCancelOrder();
            break;
        case 5:
            std::cout << "返回上级菜单..." << std::endl;
            break;
        default:
            std::cout << "无效选择！" << std::endl;
        }
    } while (adminOrderChoice != 5);
}

void MenuManager::displayAdminOrderMenu() {
    std::cout << "\n=== 管理员订单管理 ===" << std::endl;
    std::cout << "1. 查看所有订单" << std::endl;
    std::cout << "2. 发货订单" << std::endl;
    std::cout << "3. 确认签收" << std::endl;
    std::cout << "4. 取消订单" << std::endl;
    std::cout << "5. 返回上级菜单" << std::endl;
    std::cout << "请选择: ";
}

void MenuManager::handleAdminViewOrders() {
    std::cout << "\n=== 所有订单 ===" << std::endl;
    orderList.displayAllOrders();

    std::cout << "\n是否查看订单详情？(y/n): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice == 'y' || choice == 'Y') {
        std::string orderId = getInput("请输入订单编号: ");
        Order* order = orderList.findOrder(orderId);
        if (order) {
            order->displayOrderInfo();
        }
        else {
            std::cout << "订单不存在！" << std::endl;
        }
    }
}

void MenuManager::handleAdminShipOrder() {
    std::string orderId = getInput("请输入要发货的订单号: ");

    Order* order = orderList.findOrder(orderId);
    if (order) {
        if (order->isPending()) {
            std::cout << "确定要发货订单 " << orderId << " 吗？(y/n): ";
            char confirm;
            std::cin >> confirm;
            std::cin.ignore();

            if (confirm == 'y' || confirm == 'Y') {
                if (orderList.shipOrder(orderId)) {
                    std::cout << "发货成功！" << std::endl;
                }
            }
        }
        else {
            std::cout << "只有待发货的订单可以发货！" << std::endl;
        }
    }
    else {
        std::cout << "订单不存在！" << std::endl;
    }
}

void MenuManager::handleAdminDeliverOrder() {
    std::string orderId = getInput("请输入要签收的订单号: ");

    Order* order = orderList.findOrder(orderId);
    if (order) {
        if (order->isShipped()) {
            std::cout << "确定要签收订单 " << orderId << " 吗？(y/n): ";
            char confirm;
            std::cin >> confirm;
            std::cin.ignore();

            if (confirm == 'y' || confirm == 'Y') {
                if (orderList.deliverOrder(orderId)) {
                    std::cout << "签收成功！" << std::endl;
                }
            }
        }
        else {
            std::cout << "只有已发货的订单可以签收！" << std::endl;
        }
    }
    else {
        std::cout << "订单不存在！" << std::endl;
    }
}

void MenuManager::handleAdminCancelOrder() {
    std::string orderId = getInput("请输入要取消的订单号: ");

    Order* order = orderList.findOrder(orderId);
    if (order) {
        if (order->isPending()) {
            std::cout << "确定要取消订单 " << orderId << " 吗？(y/n): ";
            char confirm;
            std::cin >> confirm;
            std::cin.ignore();

            if (confirm == 'y' || confirm == 'Y') {
                if (orderList.cancelOrder(orderId, productList)) {
                    std::cout << "订单取消成功！" << std::endl;
                }
            }
        }
        else {
            std::cout << "只有待发货的订单可以取消！" << std::endl;
        }
    }
    else {
        std::cout << "订单不存在！" << std::endl;
    }
}

// ========== 顾客查看促销活动 ==========

void MenuManager::handleCustomerViewPromotions() {
    std::cout << "\n=== 促销活动信息 ===" << std::endl;

    promotionManager.displayAllPromotions();

    std::cout << "\n=== 促销商品列表 ===" << std::endl;
    std::vector<Product*> promotionalProducts = productList.getPromotionalProducts(&promotionManager);
    if (promotionalProducts.empty()) {
        std::cout << "暂无促销商品" << std::endl;
    }
    else {
        for (size_t i = 0; i < promotionalProducts.size(); ++i) {
            Product* product = promotionalProducts[i];
            if (product) {
                product->displayWithPromotion(&promotionManager);
            }
        }
    }

    std::cout << "\n=== 满减活动 ===" << std::endl;
    std::vector<FullReductionPromotion*> fullReductions = promotionManager.getActiveFullReductions();
    if (fullReductions.empty()) {
        std::cout << "暂无满减活动" << std::endl;
    }
    else {
        for (size_t i = 0; i < fullReductions.size(); ++i) {
            FullReductionPromotion* fr = fullReductions[i];
            if (fr) {
                std::cout << fr->getPromotionTag() << std::endl;
            }
        }
    }
}

// ========== 主菜单循环 ==========

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
                std::cout << "\n=== 浏览商品 ===" << std::endl;
                productList.displayAllProductsWithPromotion(&promotionManager);
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
                    handleAdminOrderManagement();
                    break;
                case 5:
                    handlePromotionManagement();
                    break;
                case 6:
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
                    handleCartManagement();
                    break;
                case 6:
                    handleOrderManagement();
                    break;
                case 7:
                    handleCustomerViewPromotions();
                    break;
                case 8:
                    userManager.logout();
                    break;
                default:
                    std::cout << "无效选择！" << std::endl;
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}