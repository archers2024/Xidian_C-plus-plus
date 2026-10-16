#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <string>
#include <vector>
#include <memory>
#include "UserManager.hpp"
#include "ProductList.hpp"
#include "OrderList.hpp"
#include "PromotionManager.hpp"

class DataManager {
private:
    std::string dataDir;

    // 文件路径
    std::string getUsersFilePath() const;
    std::string getProductsFilePath() const;
    std::string getOrdersFilePath() const;
    std::string getPromotionsFilePath() const;
    std::string getCartFilePath(const std::string& username) const;

    // 日志记录
    void logError(const std::string& message) const;
    void logInfo(const std::string& message) const;

public:
    DataManager(const std::string& directory = "./data");

    // 用户数据持久化
    bool saveUsers(const UserManager& userManager);
    bool loadUsers(UserManager& userManager);

    // 商品数据持久化
    bool saveProducts(const ProductList& productList);
    bool loadProducts(ProductList& productList);

    // 订单数据持久化
    bool saveOrders(const OrderList& orderList);
    bool loadOrders(OrderList& orderList);

    // 促销数据持久化
    bool savePromotions(const PromotionManager& promotionManager);
    bool loadPromotions(PromotionManager& promotionManager);

    // 购物车数据持久化
    bool saveCart(const std::string& username, const ShoppingCart& cart);
    bool loadCart(const std::string& username, ShoppingCart& cart, ProductList& productList);

    // 批量操作
    bool saveAll(const UserManager& userManager, const ProductList& productList,
        const OrderList& orderList, const PromotionManager& promotionManager);
    bool loadAll(UserManager& userManager, ProductList& productList,
        OrderList& orderList, PromotionManager& promotionManager);

    // 辅助函数
    bool createDataDirectory() const;
    bool backupData() const;

    // 统计信息
    void displayDataStats() const;

    // 错误检查
    bool checkDataIntegrity() const;

    // 调试功能
    void displayUserFileContents() const;
    void displayLoadedUsers(const UserManager& userManager) const;

    // 验证数据
    bool validateLoadedData(const UserManager& userManager,
        const ProductList& productList,
        const OrderList& orderList,
        const PromotionManager& promotionManager) const;
};

#endif // DATAMANAGER_HPP