#include "DataManager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <filesystem>
#include <algorithm>
#include <stdexcept>
#include "TimeUtils.hpp"

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#define MKDIR(dir) mkdir(dir, 0755)
#endif

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
    std::time_t now = TimeUtils::getCurrentTime();
    char buffer[80];

    struct tm timeinfo;
    if (TimeUtils::safeLocaltime(&now, &timeinfo)) {
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

        std::ofstream logFile(dataDir + "/error.log", std::ios::app);
        if (logFile) {
            logFile << "[" << buffer << "] ERROR: " << message << std::endl;
        }
    }
}

void DataManager::logInfo(const std::string& message) const {
    std::time_t now = TimeUtils::getCurrentTime();
    char buffer[80];

    struct tm timeinfo;
    if (TimeUtils::safeLocaltime(&now, &timeinfo)) {
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

        std::ofstream logFile(dataDir + "/info.log", std::ios::app);
        if (logFile) {
            logFile << "[" << buffer << "] INFO: " << message << std::endl;
        }
    }
}

bool DataManager::createDataDirectory() const {
    if (std::filesystem::exists(dataDir)) {
        return true;
    }

    int result = MKDIR(dataDir.c_str());
    if (result == 0) {
        logInfo("创建数据目录: " + dataDir);
        std::cout << "创建数据目录: " << dataDir << std::endl;
        return true;
    }
    else {
        logError("无法创建数据目录: " + dataDir);
        std::cerr << "无法创建数据目录: " << dataDir << std::endl;
        return false;
    }
}

// ========== 用户数据持久化 - 修复版 ==========
bool DataManager::saveUsers(const UserManager& userManager) {
    std::string filePath = getUsersFilePath();

    // 确保目录存在
    createDataDirectory();

    std::ofstream outFile(filePath, std::ios::trunc);

    if (!outFile) {
        std::string errorMsg = "无法打开用户数据文件: " + filePath;
        logError(errorMsg);
        std::cerr << errorMsg << std::endl;
        return false;
    }

    try {
        // 保存管理员信息
        const Admin& admin = userManager.getAdmin();
        outFile << "ADMIN:"
            << admin.getUsername() << "|"
            << admin.getPassword() << std::endl;

        // 保存顾客信息
        const auto& customers = userManager.getAllCustomers();
        std::cout << "正在保存 " << customers.size() << " 个顾客..." << std::endl;

        for (const auto& customer : customers) {
            // 构建顾客数据字符串
            std::stringstream ss;
            ss << customer.getUsername() << "|"
                << customer.getPassword() << "|"
                << customer.getName() << "|"
                << customer.getEmail() << "|"
                << customer.getPhone() << "|"
                << customer.getAddress();

            outFile << "CUSTOMER:" << ss.str() << std::endl;

            // 调试信息
            logInfo("保存顾客: " + customer.getUsername());
        }

        outFile.close();

        // 验证保存的文件
        std::ifstream testFile(filePath);
        int lineCount = 0;
        std::string line;
        while (std::getline(testFile, line)) {
            lineCount++;
        }
        testFile.close();

        std::string successMsg = "用户数据保存成功: " + filePath + "，共" + std::to_string(lineCount) + "行";
        logInfo(successMsg);
        std::cout << successMsg << std::endl;

        return true;
    }
    catch (const std::exception& e) {
        std::string errorMsg = "保存用户数据时发生异常: " + std::string(e.what());
        logError(errorMsg);
        std::cerr << errorMsg << std::endl;
        return false;
    }
}

bool DataManager::loadUsers(UserManager& userManager) {
    std::string filePath = getUsersFilePath();

    // 检查文件是否存在
    if (!std::filesystem::exists(filePath)) {
        std::cout << "用户数据文件不存在: " << filePath << std::endl;
        logInfo("用户数据文件不存在: " + filePath);
        return false;
    }

    std::ifstream inFile(filePath);

    if (!inFile) {
        std::string errorMsg = "无法打开用户数据文件: " + filePath;
        logError(errorMsg);
        std::cerr << errorMsg << std::endl;
        return false;
    }

    try {
        // 先清空现有顾客
        userManager.clearCustomers();

        std::string line;
        int adminCount = 0;
        int customerCount = 0;

        std::cout << "正在加载用户数据..." << std::endl;

        while (std::getline(inFile, line)) {
            if (line.empty()) continue;

            // 解析用户类型
            size_t colonPos = line.find(':');
            if (colonPos == std::string::npos) {
                logError("无效的用户数据行（缺少冒号）: " + line);
                continue;
            }

            std::string userType = line.substr(0, colonPos);
            std::string userData = line.substr(colonPos + 1);

            if (userType == "ADMIN") {
                // 解析管理员数据
                size_t pipePos = userData.find('|');
                if (pipePos == std::string::npos) {
                    logError("无效的管理员数据格式: " + userData);
                    continue;
                }

                std::string username = userData.substr(0, pipePos);
                std::string password = userData.substr(pipePos + 1);

                // 更新管理员信息
                Admin& admin = userManager.getAdmin();
                admin.setPassword(password);

                adminCount++;
                std::cout << "加载管理员: " << username << std::endl;
                logInfo("加载管理员: " + username);
            }
            else if (userType == "CUSTOMER") {
                // 解析顾客数据
                std::vector<std::string> parts;
                size_t start = 0;
                size_t end = userData.find('|');

                while (end != std::string::npos) {
                    parts.push_back(userData.substr(start, end - start));
                    start = end + 1;
                    end = userData.find('|', start);
                }
                parts.push_back(userData.substr(start));

                if (parts.size() >= 4) {
                    std::string username = parts[0];
                    std::string password = parts[1];
                    std::string name = parts[2];
                    std::string email = parts[3];

                    std::string phone = (parts.size() > 4) ? parts[4] : "";
                    std::string address = (parts.size() > 5) ? parts[5] : "";

                    // 使用addCustomerDirectly直接添加，避免重复检查
                    Customer customer(username, password, name, email, phone, address);
                    userManager.addCustomerDirectly(customer);

                    customerCount++;
                    std::cout << "加载顾客: " << username << std::endl;
                    logInfo("加载顾客: " + username);
                }
                else {
                    logError("无效的顾客数据格式，字段数不足: " + userData);
                }
            }
            else {
                logError("未知的用户类型: " + userType);
            }
        }

        inFile.close();

        std::string successMsg = "用户数据加载完成: " + std::to_string(adminCount) +
            " 个管理员, " + std::to_string(customerCount) + " 个顾客";
        logInfo(successMsg);
        std::cout << successMsg << std::endl;

        return true;
    }
    catch (const std::exception& e) {
        std::string errorMsg = "加载用户数据时发生异常: " + std::string(e.what());
        logError(errorMsg);
        std::cerr << errorMsg << std::endl;
        return false;
    }
}

// ========== 商品数据持久化 ==========
bool DataManager::saveProducts(const ProductList& productList) {
    std::string filePath = getProductsFilePath();
    createDataDirectory();

    std::ofstream outFile(filePath, std::ios::trunc);

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
            outFile << product.getName() << "|"
                << product.getPrice() << "|"
                << product.getStock() << "|"
                << product.getCategory() << "|"
                << product.getDescription() << std::endl;
        }

        outFile.close();
        logInfo("商品数据保存成功: " + filePath + "，共" + std::to_string(products.size()) + "条记录");
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
        return false;
    }

    try {
        productList.clearProducts();

        // 读取商品数量
        int productCount = 0;
        std::string countLine;
        if (std::getline(inFile, countLine)) {
            productCount = std::stoi(countLine);
        }

        // 读取每个商品信息
        for (int i = 0; i < productCount; ++i) {
            std::string productData;
            if (std::getline(inFile, productData)) {
                // 解析商品数据
                std::vector<std::string> parts;
                size_t start = 0;
                size_t end = productData.find('|');

                while (end != std::string::npos) {
                    parts.push_back(productData.substr(start, end - start));
                    start = end + 1;
                    end = productData.find('|', start);
                }
                parts.push_back(productData.substr(start));

                if (parts.size() >= 5) {
                    std::string name = parts[0];
                    double price = std::stod(parts[1]);
                    int stock = std::stoi(parts[2]);
                    std::string category = parts[3];
                    std::string description = parts[4];

                    Product product(name, price, stock, category, description);
                    productList.addProductDirectly(product);
                }
            }
        }

        inFile.close();
        logInfo("商品数据加载成功: " + filePath + "，共" + std::to_string(productCount) + "条记录");
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
    std::ofstream outFile(filePath, std::ios::trunc);

    if (!outFile) {
        logError("无法打开订单数据文件: " + filePath);
        return false;
    }

    try {
        const auto& orders = orderList.getOrders();

        // 写入订单数量
        outFile << orders.size() << std::endl;

        // 写入每个订单信息
        for (const auto& order : orders) {
            outFile << order.getOrderId() << "|"
                << order.getOrderTime() << "|"
                << order.getTotalAmount() << "|"
                << order.getDiscountedAmount() << "|"
                << order.getShippingAddress() << "|"
                << static_cast<int>(order.getStatus()) << "|"
                << order.getOrderTime() << "|"  // 发货时间（简化处理）
                << order.getOrderTime() << std::endl;  // 签收时间（简化处理）
        }

        outFile.close();
        logInfo("订单数据保存成功: " + filePath + "，共" + std::to_string(orders.size()) + "条记录");
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
        orderList.clearOrders();

        // 读取订单数量
        int orderCount = 0;
        std::string countLine;
        if (std::getline(inFile, countLine)) {
            orderCount = std::stoi(countLine);
        }

        // 读取每个订单信息
        for (int i = 0; i < orderCount; ++i) {
            std::string orderData;
            if (std::getline(inFile, orderData)) {
                // 简化处理：只保存基本信息
            }
        }

        inFile.close();
        logInfo("订单数据加载成功: " + filePath + "，共" + std::to_string(orderCount) + "条记录");
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
    std::ofstream outFile(filePath, std::ios::trunc);

    if (!outFile) {
        logError("无法打开购物车数据文件: " + filePath);
        return false;
    }

    try {
        const auto& items = cart.getItems();

        // 写入商品数量
        outFile << items.size() << std::endl;

        // 写入每个商品信息
        for (const auto& item : items) {
            const Product& product = item.second.first;
            int quantity = item.second.second;

            outFile << product.getName() << "|"
                << product.getPrice() << "|"
                << quantity << std::endl;
        }

        outFile.close();
        logInfo("购物车数据保存成功: " + filePath + "，共" + std::to_string(items.size()) + "件商品");
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
        std::string countLine;
        if (std::getline(inFile, countLine)) {
            itemCount = std::stoi(countLine);
        }

        // 读取每个商品
        for (int i = 0; i < itemCount; ++i) {
            std::string itemData;
            if (std::getline(inFile, itemData)) {
                size_t pos1 = itemData.find('|');
                size_t pos2 = itemData.find('|', pos1 + 1);

                if (pos1 != std::string::npos && pos2 != std::string::npos) {
                    std::string productName = itemData.substr(0, pos1);
                    double price = std::stod(itemData.substr(pos1 + 1, pos2 - pos1 - 1));
                    int quantity = std::stoi(itemData.substr(pos2 + 1));

                    // 从商品列表中查找商品
                    Product* product = productList.findProduct(productName);
                    if (product) {
                        cart.addItem(*product, quantity);
                    }
                }
            }
        }

        inFile.close();
        logInfo("购物车数据加载成功: " + filePath + "，共" + std::to_string(itemCount) + "件商品");
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
    std::ofstream outFile(filePath, std::ios::trunc);

    if (!outFile) {
        logError("无法打开促销数据文件: " + filePath);
        return false;
    }

    try {
        const auto& promotions = promotionManager.getAllPromotions();

        // 写入促销数量
        outFile << promotions.size() << std::endl;

        // 写入每个促销信息
        for (const auto& promotion : promotions) {
            if (auto* discount = dynamic_cast<const DiscountPromotion*>(promotion.get())) {
                outFile << "DISCOUNT:" << discount->getPromotionId() << "|"
                    << discount->getPromotionName() << "|"
                    << discount->getProductId() << "|"
                    << discount->getDiscountRate() << "|"
                    << discount->getStartTime() << "|"
                    << discount->getEndTime() << std::endl;
            }
            else if (auto* fullReduction = dynamic_cast<const FullReductionPromotion*>(promotion.get())) {
                outFile << "FULLREDUCTION:" << fullReduction->getPromotionId() << "|"
                    << fullReduction->getPromotionName() << "|"
                    << fullReduction->getThreshold() << "|"
                    << fullReduction->getReduction() << "|"
                    << fullReduction->getStartTime() << "|"
                    << fullReduction->getEndTime() << std::endl;
            }
        }

        outFile.close();
        logInfo("促销数据保存成功: " + filePath + "，共" + std::to_string(promotions.size()) + "条记录");
        return true;
    }
    catch (const std::exception& e) {
        logError("保存促销数据时发生异常: " + std::string(e.what()));
        return false;
    }
}

bool DataManager::loadPromotions(PromotionManager& promotionManager) {
    std::string filePath = getPromotionsFilePath();
    std::ifstream inFile(filePath);

    if (!inFile) {
        logInfo("促销数据文件不存在，将创建新文件: " + filePath);
        return true;
    }

    try {
        promotionManager.clearAllPromotions();

        // 读取促销数量
        int promotionCount = 0;
        std::string countLine;
        if (std::getline(inFile, countLine)) {
            promotionCount = std::stoi(countLine);
        }

        // 读取每个促销信息
        for (int i = 0; i < promotionCount; ++i) {
            std::string promotionData;
            if (std::getline(inFile, promotionData)) {
                size_t colonPos = promotionData.find(':');
                if (colonPos == std::string::npos) continue;

                std::string promoType = promotionData.substr(0, colonPos);
                std::string promoDetails = promotionData.substr(colonPos + 1);

                std::vector<std::string> parts;
                size_t start = 0;
                size_t end = promoDetails.find('|');

                while (end != std::string::npos) {
                    parts.push_back(promoDetails.substr(start, end - start));
                    start = end + 1;
                    end = promoDetails.find('|', start);
                }
                parts.push_back(promoDetails.substr(start));

                if (parts.size() >= 6) {
                    std::string id = parts[0];
                    std::string name = parts[1];
                    std::time_t startTime = std::stoll(parts[4]);
                    std::time_t endTime = std::stoll(parts[5]);

                    if (promoType == "DISCOUNT") {
                        std::string productId = parts[2];
                        double discountRate = std::stod(parts[3]);

                        auto discount = std::make_unique<DiscountPromotion>(
                            id, name, productId, discountRate, startTime, endTime);
                        promotionManager.addPromotionDirectly(std::move(discount));
                    }
                    else if (promoType == "FULLREDUCTION") {
                        double threshold = std::stod(parts[2]);
                        double reduction = std::stod(parts[3]);

                        auto fullReduction = std::make_unique<FullReductionPromotion>(
                            id, name, threshold, reduction, startTime, endTime);
                        promotionManager.addPromotionDirectly(std::move(fullReduction));
                    }
                }
            }
        }

        inFile.close();
        logInfo("促销数据加载成功: " + filePath + "，共" + std::to_string(promotionCount) + "条记录");
        return true;
    }
    catch (const std::exception& e) {
        logError("加载促销数据时发生异常: " + std::string(e.what()));
        return false;
    }
}

// ========== 批量操作 ==========
bool DataManager::saveAll(const UserManager& userManager, const ProductList& productList,
    const OrderList& orderList, const PromotionManager& promotionManager) {
    bool success = true;

    success = success && saveUsers(userManager);
    success = success && saveProducts(productList);
    success = success && saveOrders(orderList);
    success = success && savePromotions(promotionManager);

    // 保存所有顾客的购物车
    const auto& customers = userManager.getAllCustomers();
    for (const auto& customer : customers) {
        success = success && saveCart(customer.getUsername(), customer.getCart());
    }

    if (success) {
        logInfo("所有数据保存成功");
        std::cout << "所有数据保存成功！" << std::endl;
        backupData();
    }
    else {
        logError("部分数据保存失败");
        std::cerr << "警告：部分数据保存失败！" << std::endl;
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

    // 加载所有顾客的购物车
    const auto& customers = userManager.getAllCustomers();
    for (auto& customer : const_cast<std::vector<Customer>&>(customers)) {
        success = success && loadCart(customer.getUsername(), customer.getCart(), productList);
    }

    if (success) {
        logInfo("所有数据加载成功");
        std::cout << "数据加载完成！" << std::endl;
    }
    else {
        logError("部分数据加载失败");
        std::cerr << "警告：部分数据加载失败！" << std::endl;
    }

    return success;
}

bool DataManager::backupData() const {
    std::time_t now = TimeUtils::getCurrentTime();
    char buffer[80];

    struct tm timeinfo;
    if (!TimeUtils::safeLocaltime(&now, &timeinfo)) {
        return false;
    }

    std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", &timeinfo);

    std::string backupDir = dataDir + "/backup";
    if (!std::filesystem::exists(backupDir)) {
        MKDIR(backupDir.c_str());
    }

    std::string backupPath = backupDir + "/backup_" + std::string(buffer) + ".dat";

    try {
        std::ofstream backupFile(backupPath);
        if (!backupFile) {
            logError("无法创建备份文件: " + backupPath);
            return false;
        }

        char timeStr[80];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
        backupFile << "=== 数据备份 " << buffer << " ===" << std::endl;
        backupFile << "备份时间: " << timeStr << std::endl;

        backupFile.close();
        logInfo("数据备份创建: " + backupPath);
        return true;
    }
    catch (const std::exception& e) {
        logError("创建备份时发生异常: " + std::string(e.what()));
        return false;
    }
}

void DataManager::displayDataStats() const {
    std::cout << "\n=== 数据统计 ===" << std::endl;

    auto printFileStats = [&](const std::string& filePath, const std::string& label) {
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (file) {
            std::streamsize size = file.tellg();
            std::cout << label << ": " << std::fixed << std::setprecision(2)
                << size / 1024.0 << " KB" << std::endl;
            file.close();
        }
        else {
            std::cout << label << ": 文件不存在" << std::endl;
        }
        };

    printFileStats(getUsersFilePath(), "用户数据");
    printFileStats(getProductsFilePath(), "商品数据");
    printFileStats(getOrdersFilePath(), "订单数据");
    printFileStats(getPromotionsFilePath(), "促销数据");

    int cartCount = 0;
    try {
        for (const auto& entry : std::filesystem::directory_iterator(dataDir)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if (filename.find("cart_") == 0) {
                    cartCount++;
                }
            }
        }
    }
    catch (...) {
        // 忽略目录访问错误
    }

    std::cout << "购物车文件: " << cartCount << " 个" << std::endl;
}

bool DataManager::checkDataIntegrity() const {
    bool allOk = true;

    auto checkFile = [&](const std::string& filePath, const std::string& label) {
        std::ifstream file(filePath);
        if (!file) {
            std::cout << label << ": 文件不存在或无法访问" << std::endl;
            return false;
        }

        file.seekg(0, std::ios::end);
        std::streamsize size = file.tellg();
        file.close();

        if (size == 0) {
            std::cout << label << ": 文件为空" << std::endl;
            return false;
        }

        std::cout << label << ": OK (" << size << " 字节)" << std::endl;
        return true;
        };

    std::cout << "\n=== 数据完整性检查 ===" << std::endl;

    allOk = allOk && checkFile(getUsersFilePath(), "用户数据");
    allOk = allOk && checkFile(getProductsFilePath(), "商品数据");
    allOk = allOk && checkFile(getOrdersFilePath(), "订单数据");
    allOk = allOk && checkFile(getPromotionsFilePath(), "促销数据");

    return allOk;
}

// ========== 调试功能 ==========
void DataManager::displayUserFileContents() const {
    std::string filePath = getUsersFilePath();
    std::ifstream inFile(filePath);

    if (!inFile) {
        std::cout << "无法打开用户文件: " << filePath << std::endl;
        return;
    }

    std::cout << "\n=== 用户文件内容 ===" << std::endl;
    std::string line;
    int lineNum = 1;
    while (std::getline(inFile, line)) {
        std::cout << lineNum << ": " << line << std::endl;
        lineNum++;
    }
    inFile.close();
}

void DataManager::displayLoadedUsers(const UserManager& userManager) const {
    std::cout << "\n=== 已加载用户 ===" << std::endl;

    // 管理员
    const Admin& admin = userManager.getAdmin();
    std::cout << "管理员: " << admin.getUsername() << std::endl;

    // 顾客
    const auto& customers = userManager.getAllCustomers();
    std::cout << "顾客数量: " << customers.size() << std::endl;

    for (size_t i = 0; i < customers.size(); ++i) {
        std::cout << "顾客 " << i + 1 << ": "
            << customers[i].getUsername() << " ("
            << customers[i].getName() << ", "
            << customers[i].getEmail() << ")" << std::endl;
    }
}

bool DataManager::validateLoadedData(const UserManager& userManager,
    const ProductList& productList,
    const OrderList& orderList,
    const PromotionManager& promotionManager) const {

    std::cout << "\n=== 数据验证 ===" << std::endl;

    bool isValid = true;

    const auto& customers = userManager.getAllCustomers();
    std::cout << "顾客数量: " << customers.size() << std::endl;

    const auto& products = productList.getAllProducts();
    std::cout << "商品数量: " << products.size() << std::endl;

    if (products.empty()) {
        std::cout << "警告: 商品数据为空！" << std::endl;
        isValid = false;
    }

    const auto& orders = orderList.getOrders();
    std::cout << "订单数量: " << orders.size() << std::endl;

    const auto& promotions = promotionManager.getAllPromotions();
    std::cout << "促销数量: " << promotions.size() << std::endl;

    return isValid;
}