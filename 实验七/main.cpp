#include <iostream>
#include "UserManager.hpp"
#include "ProductList.hpp"
#include "OrderList.hpp"
#include "PromotionManager.hpp"  // 新增
#include "MenuManager.hpp"

using namespace std;

void initializeSampleData(ProductList& productList);
void initializeSamplePromotions(PromotionManager& pm);  // 新增

int main() {
    UserManager userManager;
    ProductList productList;
    OrderList orderList;
    PromotionManager promotionManager;  // 新增

    // 修改：传递PromotionManager给MenuManager
    MenuManager menuManager(userManager, productList, orderList, promotionManager);

    initializeSampleData(productList);
    initializeSamplePromotions(promotionManager);  // 新增

    cout << "=== 网上购物系统启动 ===" << endl;
    cout << "默认管理员账号: admin, 密码: admin123" << endl;
    cout << "系统版本: 第五版（促销活动功能）" << endl;
    cout << "已加载促销活动模块" << endl;

    menuManager.runMainMenu();

    cout << "谢谢使用网上购物系统！" << endl;
    return 0;
}

void initializeSampleData(ProductList& productList) {
    productList.addProduct(Product("iPhone 15", 5999.0, 20, "手机", "最新款智能手机"));
    productList.addProduct(Product("MacBook Pro", 12999.0, 10, "电脑", "高性能笔记本电脑"));
    productList.addProduct(Product("AirPods Pro", 1999.0, 50, "耳机", "降噪无线耳机"));
    productList.addProduct(Product("iPad Air", 3999.0, 15, "平板", "轻薄便携平板"));
    productList.addProduct(Product("Apple Watch", 2999.0, 30, "手表", "智能运动手表"));
    productList.addProduct(Product("小米手机", 2999.0, 25, "手机", "高性价比手机"));
    productList.addProduct(Product("华为平板", 2599.0, 12, "平板", "国产精品平板"));
    productList.addProduct(Product("索尼耳机", 899.0, 40, "耳机", "入门级有线耳机"));

    cout << "已初始化 " << productList.getAllProducts().size() << " 个示例商品" << endl;
}

void initializeSamplePromotions(PromotionManager& pm) {
    // 获取当前时间
    std::time_t now = std::time(nullptr);

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

    cout << "已初始化促销活动" << endl;
    cout << "包含：" << endl;
    cout << "  - iPhone 15: 9折优惠" << endl;
    cout << "  - AirPods Pro: 85折优惠" << endl;
    cout << "  - 小米手机: 88折优惠" << endl;
    cout << "  - 满1000减100活动" << endl;
    cout << "  - 满3000减300活动" << endl;
    cout << "  - 满5000减500活动" << endl;
}