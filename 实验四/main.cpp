#include <iostream>
#include "UserManager.hpp"
#include "ProductList.hpp"
#include "MenuManager.hpp"

using namespace std;

void initializeSampleData(ProductList& productList);

int main() {
    UserManager userManager;
    ProductList productList;
    MenuManager menuManager(userManager, productList);

    initializeSampleData(productList);

    cout << "=== 网上购物系统启动 ===" << endl;
    cout << "默认管理员账号: admin, 密码: admin123" << endl;

    menuManager.runMainMenu();

    cout << "谢谢使用网上购物系统！" << endl;
    return 0;
}

void initializeSampleData(ProductList& productList) {
    productList.addProduct(Product("iPhone 15", 5999.0, 10, "手机", "最新款智能手机"));
    productList.addProduct(Product("MacBook Pro", 12999.0, 5, "电脑", "高性能笔记本电脑"));
    productList.addProduct(Product("AirPods", 1299.0, 20, "耳机", "无线蓝牙耳机"));
    productList.addProduct(Product("iPad Air", 3999.0, 8, "平板", "轻薄便携平板"));
    productList.addProduct(Product("Apple Watch", 2999.0, 15, "手表", "智能运动手表"));
}