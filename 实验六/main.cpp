#include <iostream>
#include "UserManager.hpp"
#include "ProductList.hpp"
#include "OrderList.hpp"
#include "MenuManager.hpp"

using namespace std;

void initializeSampleData(ProductList& productList);

int main() {
    UserManager userManager;
    ProductList productList;
    OrderList orderList;  // 新增
    MenuManager menuManager(userManager, productList, orderList);  // 修改

    initializeSampleData(productList);

    cout << "=== 网上购物系统启动 ===" << endl;
    cout << "默认管理员账号: admin, 密码: admin123" << endl;
    cout << "系统版本: 第四版（完整订单功能）" << endl;

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