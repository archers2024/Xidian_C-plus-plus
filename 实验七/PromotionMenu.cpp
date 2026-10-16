#include "PromotionMenu.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <limits>
#include <algorithm>

PromotionMenu::PromotionMenu(PromotionManager& pm, ProductList& pl)
    : promotionManager(pm), productList(pl) {
}

// ========== 输入辅助函数 ==========

std::string PromotionMenu::getInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

double PromotionMenu::getDoubleInput(const std::string& prompt) {
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

int PromotionMenu::getIntInput(const std::string& prompt) {
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

std::time_t PromotionMenu::getDateTimeInput(const std::string& prompt) {
    std::cout << prompt << std::endl;

    std::string dateStr = getInput("请输入日期 (YYYY-MM-DD，留空使用今天): ");
    std::string timeStr = getInput("请输入时间 (HH:MM，留空使用现在): ");

    return parseDateTime(dateStr, timeStr);
}

std::time_t PromotionMenu::parseDateTime(const std::string& dateStr, const std::string& timeStr) {
    std::time_t now = std::time(nullptr);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);

    // 解析日期
    if (!dateStr.empty()) {
        int year, month, day;
        if (sscanf_s(dateStr.c_str(), "%d-%d-%d", &year, &month, &day) == 3) {
            timeinfo.tm_year = year - 1900;
            timeinfo.tm_mon = month - 1;
            timeinfo.tm_mday = day;
        }
    }

    // 解析时间
    if (!timeStr.empty()) {
        int hour, minute;
        if (sscanf_s(timeStr.c_str(), "%d:%d", &hour, &minute) == 2) {
            timeinfo.tm_hour = hour;
            timeinfo.tm_min = minute;
            timeinfo.tm_sec = 0;
        }
    }

    return mktime(&timeinfo);
}

// ========== 菜单显示 ==========

void PromotionMenu::displayPromotionMenu() {
    std::cout << "\n=== 促销活动管理 ===" << std::endl;
    std::cout << "1. 添加限时折扣" << std::endl;
    std::cout << "2. 添加满减活动" << std::endl;
    std::cout << "3. 查看所有促销" << std::endl;
    std::cout << "4. 查看促销商品" << std::endl;
    std::cout << "5. 删除促销活动" << std::endl;
    std::cout << "6. 更新促销状态" << std::endl;
    std::cout << "7. 测试促销计算" << std::endl;
    std::cout << "8. 返回上级菜单" << std::endl;
    std::cout << "请选择: ";
}

void PromotionMenu::displayPromotionDetails(const Promotion* promotion) {
    if (!promotion) return;

    std::cout << "\n=== 促销活动详情 ===" << std::endl;
    std::cout << "活动ID: " << promotion->getPromotionId() << std::endl;
    std::cout << "活动名称: " << promotion->getPromotionName() << std::endl;
    std::cout << "活动标签: " << promotion->getPromotionTag() << std::endl;

    // 显示活动时间
    char buffer[80];
    struct tm startTime, endTime;

    // 修复：将 time_t 转为左值变量
    std::time_t startTimestamp = promotion->getStartTime();
    localtime_s(&startTime, &startTimestamp);

    std::time_t endTimestamp = promotion->getEndTime();
    localtime_s(&endTime, &endTimestamp);

    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &startTime);
    std::cout << "开始时间: " << buffer << std::endl;

    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &endTime);
    std::cout << "结束时间: " << buffer << std::endl;

    // 修复：移除或替换Unicode表情符号
    std::cout << "活动状态: " << (promotion->isActiveNow() ? "进行中" : "已结束") << std::endl;
}
void PromotionMenu::displayProductListForSelection() {
    std::cout << "\n=== 可选商品列表 ===" << std::endl;
    productList.displayAllProducts();
}

// ========== 功能处理 ==========

void PromotionMenu::handleAddDiscount() {
    std::cout << "\n=== 添加限时折扣 ===" << std::endl;

    // 显示商品列表供选择
    displayProductListForSelection();

    std::string productName = getInput("请输入商品名称: ");
    Product* product = productList.findProduct(productName);
    if (!product) {
        std::cout << "商品不存在！" << std::endl;
        return;
    }

    std::string promotionId = getInput("请输入促销ID: ");
    std::string promotionName = getInput("请输入促销名称: ");

    double discountRate = getDoubleInput("请输入折扣率 (0.1-1.0，如0.8表示8折): ");
    if (discountRate <= 0 || discountRate > 1.0) {
        std::cout << "折扣率必须在0.1到1.0之间！" << std::endl;
        return;
    }

    std::cout << "\n设置活动时间:" << std::endl;
    std::time_t startTime = getDateTimeInput("开始时间:");
    std::time_t endTime = getDateTimeInput("结束时间:");

    if (startTime >= endTime) {
        std::cout << "开始时间必须早于结束时间！" << std::endl;
        return;
    }

    // 添加折扣活动
    if (promotionManager.addDiscountPromotion(promotionId, promotionName,
        productName, discountRate,
        startTime, endTime)) {
        std::cout << "限时折扣添加成功！" << std::endl;
    }
    else {
        std::cout << "限时折扣添加失败！" << std::endl;
    }
}

void PromotionMenu::handleAddFullReduction() {
    std::cout << "\n=== 添加满减活动 ===" << std::endl;

    std::string promotionId = getInput("请输入促销ID: ");
    std::string promotionName = getInput("请输入促销名称: ");

    double threshold = getDoubleInput("请输入满减门槛 (例如 300): ");
    if (threshold <= 0) {
        std::cout << "满减门槛必须大于0！" << std::endl;
        return;
    }

    double reduction = getDoubleInput("请输入减免金额 (例如 50): ");
    if (reduction <= 0) {
        std::cout << "减免金额必须大于0！" << std::endl;
        return;
    }

    if (reduction >= threshold) {
        std::cout << "减免金额不能大于或等于门槛金额！" << std::endl;
        return;
    }

    std::cout << "\n设置活动时间:" << std::endl;
    std::time_t startTime = getDateTimeInput("开始时间:");
    std::time_t endTime = getDateTimeInput("结束时间:");

    if (startTime >= endTime) {
        std::cout << "开始时间必须早于结束时间！" << std::endl;
        return;
    }

    // 添加满减活动
    if (promotionManager.addFullReductionPromotion(promotionId, promotionName,
        threshold, reduction,
        startTime, endTime)) {
        std::cout << "满减活动添加成功！" << std::endl;
        std::cout << "活动规则: 满" << threshold << "减" << reduction << std::endl;
    }
    else {
        std::cout << "满减活动添加失败！" << std::endl;
    }
}

void PromotionMenu::handleViewPromotions() {
    promotionManager.displayAllPromotions();
}

void PromotionMenu::handleViewPromotionalProducts() {
    std::cout << "\n=== 促销商品列表 ===" << std::endl;
    productList.displayAllProductsWithPromotion(&promotionManager);
}

void PromotionMenu::handleRemovePromotion() {
    std::string promotionId = getInput("请输入要删除的促销ID: ");

    std::cout << "确定要删除促销活动 " << promotionId << " 吗？(y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore();

    if (confirm == 'y' || confirm == 'Y') {
        if (promotionManager.removePromotion(promotionId)) {
            std::cout << "促销活动删除成功！" << std::endl;
        }
    }
    else {
        std::cout << "取消删除操作" << std::endl;
    }
}

void PromotionMenu::handleUpdatePromotionStatus() {
    promotionManager.updatePromotionStatus();
    std::cout << "促销状态已更新！" << std::endl;
}

void PromotionMenu::handleTestPromotionCalculation() {
    std::cout << "\n=== 促销计算测试 ===" << std::endl;

    // 选择一个商品测试折扣
    displayProductListForSelection();
    std::string productName = getInput("请输入要测试的商品名称: ");
    Product* product = productList.findProduct(productName);
    if (!product) {
        std::cout << "商品不存在！" << std::endl;
        return;
    }

    int quantity = getIntInput("请输入购买数量: ");
    if (quantity <= 0) {
        std::cout << "数量必须大于0！" << std::endl;
        return;
    }

    double originalPrice = product->getPrice() * quantity;
    double discountedPrice = promotionManager.calculateProductPrice(*product, quantity);

    std::cout << "\n=== 计算结果 ===" << std::endl;
    std::cout << "商品: " << product->getName() << std::endl;
    std::cout << "原价: ￥" << std::fixed << std::setprecision(2) << originalPrice << std::endl;
    std::cout << "折后价: ￥" << std::fixed << std::setprecision(2) << discountedPrice << std::endl;

    if (discountedPrice < originalPrice) {
        std::cout << "节省: ￥" << std::fixed << std::setprecision(2)
            << (originalPrice - discountedPrice) << std::endl;
    }
}

// ========== 主运行函数 ==========

void PromotionMenu::run() {
    int choice;

    do {
        displayPromotionMenu();
        choice = getIntInput("");

        switch (choice) {
        case 1:
            handleAddDiscount();
            break;
        case 2:
            handleAddFullReduction();
            break;
        case 3:
            handleViewPromotions();
            break;
        case 4:
            handleViewPromotionalProducts();
            break;
        case 5:
            handleRemovePromotion();
            break;
        case 6:
            handleUpdatePromotionStatus();
            break;
        case 7:
            handleTestPromotionCalculation();
            break;
        case 8:
            std::cout << "返回上级菜单..." << std::endl;
            break;
        default:
            std::cout << "无效选择！" << std::endl;
        }

    } while (choice != 8);
}