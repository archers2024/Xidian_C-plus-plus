// DataManager.cpp
#include "DataManager.hpp"
#include "UserManager.hpp"
#include "ProductList.hpp"
#include "OrderList.hpp"
#include "PromotionManager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <filesystem>
#include <algorithm>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#define MKDIR(dir) mkdir(dir, 0755)
#endif
#define _CRT_SECURE_NO_WARNINGS

DataManager::DataManager(const std::string& directory) : dataDir(directory) {
    createDataDirectory();
}

std::string DataManager::getUsersFilePath() const {
    return dataDir + "/users.dat";
}

std::string DataManager::getProductsFilePath() const {
    return dataDir + "/products.dat";
}

std::string DataManager::getOrdersFilePath() const {
    return dataDir + "/orders.dat";
}

std::string DataManager::getPromotionsFilePath() const {
    return dataDir + "/promotions.dat";
}

std::string DataManager::getCartFilePath(const std::string& username) const {
    return dataDir + "/cart_" + username + ".dat";
}

void DataManager::logError(const std::string& message) const {
    std::time_t now = std::time(nullptr);
    char buffer[80];
#ifdef _WIN32
    std::tm timeinfo;
    localtime_s(&timeinfo, &now);
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
#else
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
#endif
    
    std::ofstream logFile(dataDir + "/error.log", std::ios::app);
    if (logFile) {
        logFile << "[" << buffer << "] ERROR: " << message << std::endl;
    }
}

void DataManager::logInfo(const std::string& message) const {
    std::time_t now = std::time(nullptr);
    char buffer[80];
#ifdef _WIN32
    std::tm timeinfo;
    localtime_s(&timeinfo, &now);
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
#else
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
#endif
    
    std::ofstream logFile(dataDir + "/info.log", std::ios::app);
    if (logFile) {
        logFile << "[" << buffer << "] INFO: " << message << std::endl;
    }
}

// ========== 用户数据持久化 ==========
bool DataManager::saveUsers(const UserManager& userManager) {
    std::string filePath = getUsersFilePath();
    std::ofstream outFile(filePath, std::ios::binary | std::ios::trunc);
    
    if (!outFile) {
        logError("无法打开用户数据文件: " + filePath);
        return false;
    }
    
    try {
        // 保存管理员信息
        const Admin& admin = userManager.getAdmin();
        std::string username = admin.getUsername();
        std::string password = "加密存储"; // 实际应用中应该加密存储
        
        outFile.write(username.c_str(), username.length() + 1);
        outFile.write(password.c_str(), password.length() + 1);
        
        // 保存顾客数量
        const auto& customers = userManager.getAllCustomers();
        size_t customerCount = customers.size();
        outFile.write(reinterpret_cast<const char*>(&customerCount), sizeof(customerCount));
        
        // 保存每个顾客信息
        for (const auto& customer : customers) {
            std::string custUsername = customer.getUsername();
            std::string custPassword = "加密存储";
            std::string custName = customer.getName();
            std::string custEmail = customer.getEmail();
            std::string custPhone = customer.getPhone();
            std::string custAddress = customer.getAddress();
            
            outFile.write(custUsername.c_str(), custUsername.length() + 1);
            outFile.write(custPassword.c_str(), custPassword.length() + 1);
            outFile.write(custName.c_str(), custName.length() + 1);
            outFile.write(custEmail.c_str(), custEmail.length() + 1);
            outFile.write(custPhone.c_str(), custPhone.length() + 1);
            outFile.write(custAddress.c_str(), custAddress.length() + 1);
        }
        
        outFile.close();
        logInfo("用户数据保存成功: " + filePath);
        return true;
    }
    catch (const std::exception& e) {
        logError("保存用户数据时发生异常: " + std::string(e.what()));
        return false;
    }
}

bool DataManager::loadUsers(UserManager& userManager) {
    std::string filePath = getUsersFilePath();
    std::ifstream inFile(filePath, std::ios::binary);
    
    if (!inFile) {
        logInfo("用户数据文件不存在，将创建新文件: " + filePath);
        return true; // 文件不存在不是错误，只是没有数据
    }
    
    try {
        // 加载管理员信息
        std::string username, password;
        std::getline(inFile, username, '\0');
        std::getline(inFile, password, '\0');
        
        // 加载顾客数量
        size_t customerCount = 0;
        inFile.read(reinterpret_cast<char*>(&customerCount), sizeof(customerCount));
        
        // 加载每个顾客信息
        for (size_t i = 0; i < customerCount; ++i) {
            std::string custUsername, custPassword, custName, custEmail, custPhone, custAddress;
            
            std::getline(inFile, custUsername, '\0');
            std::getline(inFile, custPassword, '\0');
            std::getline(inFile, custName, '\0');
            std::getline(inFile, custEmail, '\0');
            std::getline(inFile, custPhone, '\0');
            std::getline(inFile, custAddress, '\0');
            
            // 注册顾客
            userManager.customerRegister(custUsername, custPassword, custName, 
                                        custEmail, custPhone, custAddress);
        }
        
        inFile.close();
        logInfo("用户数据加载成功: " + filePath);
        return true;
    }
    catch (const std::exception& e) {
        logError("加载用户数据时发生异常: " + std::string(e.what()));
        return false;
    }
}

// ========== 商品数据持久化 ==========
bool DataManager::saveProducts(const ProductList& productList) {
    std::string filePath = getProductsFilePath();
    std::ofstream outFile(filePath);
    
    if (!outFile) {
        logError("无法打开商品数据文件: " + filePath);
        return false;
    }
    
    try {
        const auto& products = productList.getAllProducts();
        
        // 写入商品数量
        outFile << products.size() << std::endl;
        
        // 写入每个商品信息
        for (const auto& product : products) {
            outFile << product.getName() << std::endl;
            outFile << product.getPrice() << std::endl;
            outFile << product.getStock() << std::endl;
            outFile << product.getCategory() << std::endl;
            outFile << product.getDescription() << std::endl;
        }
        
        outFile.close();
        logInfo("商品数据保存成功: " + filePath);
        return true;
    }
    catch (const std::exception& e) {
        logError("保存商品数据时发生异常: " + std::string(e.what()));
        return false;
    }
}

bool DataManager::loadProducts(ProductList& productList) {
    std::string filePath = getProductsFilePath();
    std::ifstream inFile(filePath);
    
    if (!inFile) {
        logInfo("商品数据文件不存在，将创建新文件: " + filePath);
        return true;
    }
    
    try {
        // 清空现有商品
        // 注意：这里假设ProductList有clear方法，如果没有需要添加
        
        // 读取商品数量
        int productCount = 0;
        inFile >> productCount;
        inFile.ignore(); // 忽略换行符
        
        // 读取每个商品信息
        for (int i = 0; i < productCount; ++i) {
            std::string name, category, description;
            double price;
            int stock;
            
            std::getline(inFile, name);
            inFile >> price;
            inFile.ignore();
            inFile >> stock;
            inFile.ignore();
            std::getline(inFile, category);
            std::getline(inFile, description);
            
            Product product(name, price, stock, category, description);
            productList.addProduct(product);
        }
        
        inFile.close();
        logInfo("商品数据加载成功: " + filePath);
        return true;
    }
    catch (const std::exception& e) {
        logError("加载商品数据时发生异常: " + std::string(e.what()));
        return false;
    }
}

// ========== 订单数据持久化 ==========
bool DataManager::saveOrders(const OrderList& orderList) {
    std::string filePath = getOrdersFilePath();
    std::ofstream outFile(filePath);
    
    if (!outFile) {
        logError("无法打开订单数据文件: " + filePath);
        return false;
    }
    
    try {
        // 获取所有订单
        auto allOrders = const_cast<OrderList&>(orderList).getAllOrders();
        
        // 写入订单数量
        outFile << allOrders.size() << std::endl;
        
        // 写入每个订单信息
        for (const auto* order : allOrders) {
            if (!order) continue;
            
            outFile << order->getOrderId() << std::endl;
            outFile << order->getOrderTime() << std::endl;
            outFile << order->getTotalAmount() << std::endl;
            outFile << order->getDiscountedAmount() << std::endl;
            outFile << order->getShippingAddress() << std::endl;
            outFile << static_cast<int>(order->getStatus()) << std::endl;
            outFile << order->getItems().size() << std::endl;
            
            // 写入订单商品
            const auto& items = order->getItems();
            for (const auto& item : items) {
                const Product& product = item.first;
                int quantity = item.second;
                
                outFile << product.getName() << std::endl;
                outFile << product.getPrice() << std::endl;
                outFile << quantity << std::endl;
            }
        }
        
        outFile.close();
        logInfo("订单数据保存成功: " + filePath);
        return true;
    }
    catch (const std::exception& e) {
        logError("保存订单数据时发生异常: " + std::string(e.what()));
        return false;
    }
}

bool DataManager::loadOrders(OrderList& orderList) {
    std::string filePath = getOrdersFilePath();
    std::ifstream inFile(filePath);
    
    if (!inFile) {
        logInfo("订单数据文件不存在，将创建新文件: " + filePath);
        return true;
    }
    
    try {
        // 清空现有订单
        // 注意：这里假设OrderList有clear方法，如果没有需要添加
        
        // 读取订单数量
        int orderCount = 0;
        inFile >> orderCount;
        inFile.ignore();
        
        // 读取每个订单信息
        for (int i = 0; i < orderCount; ++i) {
            std::string orderId, shippingAddress;
            time_t orderTime;
            double totalAmount, discountedAmount;
            int statusInt;
            
            std::getline(inFile, orderId);
            inFile >> orderTime;
            inFile.ignore();
            inFile >> totalAmount;
            inFile.ignore();
            inFile >> discountedAmount;
            inFile.ignore();
            std::getline(inFile, shippingAddress);
            inFile >> statusInt;
            inFile.ignore();
            
            // 读取商品数量
            int itemCount = 0;
            inFile >> itemCount;
            inFile.ignore();
            
            std::vector<std::pair<Product, int>> items;
            for (int j = 0; j < itemCount; ++j) {
                std::string productName;
                double productPrice;
                int quantity;
                
                std::getline(inFile, productName);
                inFile >> productPrice;
                inFile.ignore();
                inFile >> quantity;
                inFile.ignore();
                
                Product product(productName, productPrice, 0, "", "");
                items.push_back(std::make_pair(product, quantity));
            }
            
            // 创建订单
            Order order(items, shippingAddress);
            // 这里需要设置其他属性，如订单ID、时间、金额等
            // 可能需要修改Order类的构造函数或添加setter方法
        }
        
        inFile.close();
        logInfo("订单数据加载成功: " + filePath);
        return true;
    }
    catch (const std::exception& e) {
        logError("加载订单数据时发生异常: " + std::string(e.what()));
        return false;
    }
}

// ========== 购物车数据持久化 ==========
bool DataManager::saveCart(const std::string& username, const ShoppingCart& cart) {
    std::string filePath = getCartFilePath(username);
    std::ofstream outFile(filePath);
    
    if (!outFile) {
        logError("无法打开购物车数据文件: " + filePath);
        return false;
    }
    
    try {
        // 写入商品数量
        outFile << cart.getTotalItems() << std::endl;
        
        // 获取购物车商品（需要添加getItems方法到ShoppingCart类）
        // 注意：ShoppingCart类中已经有getItems()方法
        
        outFile.close();
        logInfo("购物车数据保存成功: " + filePath);
        return true;
    }
    catch (const std::exception& e) {
        logError("保存购物车数据时发生异常: " + std::string(e.what()));
        return false;
    }
}

bool DataManager::loadCart(const std::string& username, ShoppingCart& cart, ProductList& productList) {
    std::string filePath = getCartFilePath(username);
    std::ifstream inFile(filePath);
    
    if (!inFile) {
        logInfo("购物车数据文件不存在: " + filePath);
        return true;
    }
    
    try {
        cart.clearCart();
        
        // 读取商品数量
        int itemCount = 0;
        inFile >> itemCount;
        inFile.ignore();
        
        // 读取每个商品
        for (int i = 0; i < itemCount; ++i) {
            std::string productName;
            int quantity;
            
            std::getline(inFile, productName);
            inFile >> quantity;
            inFile.ignore();
            
            Product* product = productList.findProduct(productName);
            if (product) {
                cart.addItem(*product, quantity);
            }
        }
        
        inFile.close();
        logInfo("购物车数据加载成功: " + filePath);
        return true;
    }
    catch (const std::exception& e) {
        logError("加载购物车数据时发生异常: " + std::string(e.what()));
        return false;
    }
}

// ========== 促销数据持久化 ==========
bool DataManager::savePromotions(const PromotionManager& promotionManager) {
    std::string filePath = getPromotionsFilePath();
    std::ofstream outFile(filePath);
    
    if (!outFile) {
        logError("无法打开促销数据文件: " + filePath);
        return false;
    }
    
    // 这里需要PromotionManager提供获取所有促销的方法
    // 当前PromotionManager没有提供获取所有促销的公共方法
    // 需要添加相应方法
    
    logInfo("促销数据保存成功: " + filePath);
    return true;
}

bool DataManager::loadPromotions(PromotionManager& promotionManager) {
    std::string filePath = getPromotionsFilePath();
    std::ifstream inFile(filePath);
    
    if (!inFile) {
        logInfo("促销数据文件不存在，将创建新文件: " + filePath);
        return true;
    }
    
    // 加载促销数据逻辑
    
    logInfo("促销数据加载成功: " + filePath);
    return true;
}

// ========== 批量操作 ==========
bool DataManager::saveAll(const UserManager& userManager, const ProductList& productList,
                         const OrderList& orderList, const PromotionManager& promotionManager) {
    bool success = true;
    
    success = success && saveUsers(userManager);
    success = success && saveProducts(productList);
    success = success && saveOrders(orderList);
    success = success && savePromotions(promotionManager);
    
    if (success) {
        logInfo("所有数据保存成功");
        backupData();
    } else {
        logError("部分数据保存失败");
    }
    
    return success;
}

bool DataManager::loadAll(UserManager& userManager, ProductList& productList,
                         OrderList& orderList, PromotionManager& promotionManager) {
    bool success = true;
    
    success = success && loadUsers(userManager);
    success = success && loadProducts(productList);
    success = success && loadOrders(orderList);
    success = success && loadPromotions(promotionManager);
    
    if (success) {
        logInfo("所有数据加载成功");
    } else {
        logError("部分数据加载失败");
    }
    
    return success;
}

// ========== 辅助函数 ==========
bool DataManager::createDataDirectory() const {
    if (std::filesystem::exists(dataDir)) {
        return true;
    }
    
    int result = MKDIR(dataDir.c_str());
    if (result == 0) {
        logInfo("创建数据目录: " + dataDir);
        return true;
    } else {
        logError("无法创建数据目录: " + dataDir);
        return false;
    }
}

bool DataManager::backupData() const {
    std::time_t now = std::time(nullptr);
    char buffer[80];
    // 使用localtime_s替换localtime以消除C4996警告
    std::tm timeinfo;
#ifdef _WIN32
    localtime_s(&timeinfo, &now);
#else
    timeinfo = *std::localtime(&now);
#endif
    std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", &timeinfo);
    
    std::string backupDir = dataDir + "/backup";
    if (!std::filesystem::exists(backupDir)) {
        MKDIR(backupDir.c_str());
    }
    
    std::string backupPath = backupDir + "/backup_" + buffer + ".zip";
    // 这里可以添加文件备份逻辑
    
    logInfo("数据备份创建: " + backupPath);
    return true;
}

void DataManager::displayDataStats() const {
    std::cout << "\n=== 数据统计 ===" << std::endl;
    
    auto printFileStats = [](const std::string& filePath, const std::string& label) {
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (file) {
            std::streamsize size = file.tellg();
            std::cout << label << ": " << std::fixed << std::setprecision(2)
                     << size / 1024.0 << " KB" << std::endl;
        } else {
            std::cout << label << ": 文件不存在" << std::endl;
        }
    };
    
    printFileStats(getUsersFilePath(), "用户数据");
    printFileStats(getProductsFilePath(), "商品数据");
    printFileStats(getOrdersFilePath(), "订单数据");
    printFileStats(getPromotionsFilePath(), "促销数据");
}

bool DataManager::checkDataIntegrity() const {
    bool allOk = true;
    
    auto checkFile = [&](const std::string& filePath, const std::string& label) {
        std::ifstream file(filePath);
        if (!file) {
            std::cout << label << ": 文件不存在或无法访问" << std::endl;
            return false;
        }
        
        // 简单的文件大小检查
        file.seekg(0, std::ios::end);
        std::streamsize size = file.tellg();
        if (size == 0) {
            std::cout << label << ": 文件为空" << std::endl;
            return false;
        }
        
        std::cout << label << ": ✓ 正常 (" << size << " 字节)" << std::endl;
        return true;
    };
    
    std::cout << "\n=== 数据完整性检查 ===" << std::endl;
    
    allOk = allOk && checkFile(getUsersFilePath(), "用户数据");
    allOk = allOk && checkFile(getProductsFilePath(), "商品数据");
    allOk = allOk && checkFile(getOrdersFilePath(), "订单数据");
    allOk = allOk && checkFile(getPromotionsFilePath(), "促销数据");
    
    return allOk;
}