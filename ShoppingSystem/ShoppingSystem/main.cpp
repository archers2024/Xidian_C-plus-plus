#include <iostream>
#include <filesystem>
#include "UserManager.hpp"
#include "ProductList.hpp"
#include "OrderList.hpp"
#include "PromotionManager.hpp"
#include "MenuManager.hpp"
#include "DataManager.hpp"
#include "TimeUtils.hpp"

using namespace std;

void initializeSampleData(ProductList& productList);
void initializeSamplePromotions(PromotionManager& pm);
void checkAndInitializeData(DataManager& dataManager,
    UserManager& userManager,
    ProductList& productList,
    OrderList& orderList,
    PromotionManager& promotionManager);

int main() {
    cout << "=== 网上购物系统启动 ===" << endl;
    cout << "默认管理员账号: admin, 密码: admin123" << endl;
    cout << "=========================" << endl;

    // 创建数据管理器
    DataManager dataManager("./data");

    // 创建用户管理器并关联数据管理器
    UserManager userManager;
    userManager.setDataManager(&dataManager);

    ProductList productList;
    OrderList orderList;
    PromotionManager promotionManager;

    // 检查和初始化数据
    checkAndInitializeData(dataManager, userManager, productList, orderList, promotionManager);

    // 创建菜单管理器
    MenuManager menuManager(userManager, productList, orderList, promotionManager);

    // 显示系统状态
    cout << "\n=== 系统状态 ===" << endl;
    cout << "商品数量: " << productList.getAllProducts().size() << " 个" << endl;
    cout << "顾客数量: " << userManager.getAllCustomers().size() << " 个" << endl;
    cout << "促销数量: " << promotionManager.getAllPromotions().size() << " 个" << endl;

    // 显示数据统计
    dataManager.displayDataStats();

    // 运行主菜单
    menuManager.runMainMenu();

    // 退出前保存所有数据
    cout << "\n正在保存所有数据..." << endl;
    bool saveSuccess = dataManager.saveAll(userManager, productList, orderList, promotionManager);
    if (saveSuccess) {
        cout << "所有数据保存成功！" << endl;
    }
    else {
        cout << "警告：部分数据保存失败！" << endl;
    }

    cout << "\n谢谢使用网上购物系统！" << endl;
    return 0;
}

void initializeSampleData(ProductList& productList) {
    cout << "\n正在初始化示例商品数据..." << endl;

    // 先清空现有商品
    productList.clearProducts();

    // 添加示例商品
    productList.addProduct(Product("iPhone 15", 5999.0, 20, "手机", "最新款智能手机"));
    productList.addProduct(Product("MacBook Pro", 12999.0, 10, "电脑", "高性能笔记本电脑"));
    productList.addProduct(Product("AirPods Pro", 1999.0, 50, "耳机", "降噪无线耳机"));
    productList.addProduct(Product("iPad Air", 3999.0, 15, "平板", "轻薄便携平板"));
    productList.addProduct(Product("Apple Watch", 2999.0, 30, "手表", "智能运动手表"));
    productList.addProduct(Product("小米手机", 2999.0, 25, "手机", "高性价比手机"));
    productList.addProduct(Product("华为平板", 2599.0, 12, "平板", "国产精品平板"));
    productList.addProduct(Product("索尼耳机", 899.0, 40, "耳机", "入门级有线耳机"));

    cout << "示例商品初始化完成，共 " << productList.getAllProducts().size() << " 个商品" << endl;
}

void initializeSamplePromotions(PromotionManager& pm) {
    cout << "\n正在初始化示例促销数据..." << endl;

    // 获取当前时间
    std::time_t now = TimeUtils::getCurrentTime();

    // 添加折扣活动（有效期为1个月）
    std::time_t endTime = now + 30 * 24 * 60 * 60;  // 30天后

    // iPhone限时9折
    pm.addDiscountPromotion("D001", "iPhone限时折扣", "iPhone 15", 0.9, now, endTime);

    // AirPods Pro限时85折
    pm.addDiscountPromotion("D002", "耳机特惠", "AirPods Pro", 0.85, now, endTime);

    // 小米手机88折
    pm.addDiscountPromotion("D003", "小米特惠", "小米手机", 0.88, now, endTime);

    // 添加满减活动
    pm.addFullReductionPromotion("F001", "国庆满减", 1000.0, 100.0, now, endTime);
    pm.addFullReductionPromotion("F002", "双十一大促", 3000.0, 300.0, now, endTime);
    pm.addFullReductionPromotion("F003", "超值满减", 5000.0, 500.0, now, endTime);

    cout << "示例促销初始化完成" << endl;
    cout << "包含：" << endl;
    cout << "  - iPhone 15: 9折优惠" << endl;
    cout << "  - AirPods Pro: 85折优惠" << endl;
    cout << "  - 小米手机: 88折优惠" << endl;
    cout << "  - 满1000减100活动" << endl;
    cout << "  - 满3000减300活动" << endl;
    cout << "  - 满5000减500活动" << endl;
}

void checkAndInitializeData(DataManager& dataManager,
    UserManager& userManager,
    ProductList& productList,
    OrderList& orderList,
    PromotionManager& promotionManager) {

    bool loadSuccess = false;

    // 检查数据目录是否存在
    if (!std::filesystem::exists("./data")) {
        cout << "数据目录不存在，将创建新目录并初始化数据..." << endl;
        dataManager.createDataDirectory();
        loadSuccess = false;
    }
    else {
        cout << "正在加载数据..." << endl;
        loadSuccess = dataManager.loadAll(userManager, productList, orderList, promotionManager);

        if (loadSuccess) {
            cout << "数据加载完成" << endl;
        }
        else {
            cout << "数据加载失败或部分失败，将重新初始化..." << endl;
        }
    }

    // 如果加载失败或没有数据，初始化示例数据
    if (!loadSuccess || productList.getAllProducts().empty()) {
        cout << "\n正在初始化示例数据..." << endl;

        // 初始化商品数据
        if (productList.getAllProducts().empty()) {
            cout << "商品数据为空，初始化示例商品..." << endl;
            initializeSampleData(productList);
        }

        // 初始化促销数据
        if (promotionManager.getAllPromotions().empty()) {
            cout << "促销数据为空，初始化示例促销..." << endl;
            initializeSamplePromotions(promotionManager);
        }

        // 保存初始化后的数据
        cout << "\n保存初始化数据..." << endl;
        bool saveInitSuccess = dataManager.saveAll(userManager, productList, orderList, promotionManager);
        if (saveInitSuccess) {
            cout << "初始化数据保存成功！" << endl;
        }
        else {
            cout << "初始化数据保存失败！" << endl;
        }
    }
    else {
        cout << "\n使用已保存的数据：" << endl;
        cout << "商品数量: " << productList.getAllProducts().size() << " 个" << endl;
        cout << "促销数量: " << promotionManager.getAllPromotions().size() << " 个" << endl;
        cout << "顾客数量: " << userManager.getAllCustomers().size() << " 个" << endl;
        cout << "订单数量: " << orderList.getOrders().size() << " 个" << endl;
    }

    // 验证数据
    dataManager.validateLoadedData(userManager, productList, orderList, promotionManager);
}