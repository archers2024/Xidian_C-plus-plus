#ifndef PROMOTIONMENU_HPP
#define PROMOTIONMENU_HPP

#include "PromotionManager.hpp"
#include "ProductList.hpp"
#include <string>
#include <ctime>

class PromotionMenu {
private:
    PromotionManager& promotionManager;
    ProductList& productList;

    // 输入辅助函数
    std::string getInput(const std::string& prompt);
    double getDoubleInput(const std::string& prompt);
    int getIntInput(const std::string& prompt);

    // 时间输入
    std::time_t getDateTimeInput(const std::string& prompt);
    std::time_t parseDateTime(const std::string& dateStr, const std::string& timeStr);

    // 菜单显示
    void displayPromotionMenu();

    // 功能处理
    void handleAddDiscount();
    void handleAddFullReduction();
    void handleViewPromotions();
    void handleViewPromotionalProducts();
    void handleRemovePromotion();
    void handleUpdatePromotionStatus();
    void handleTestPromotionCalculation();

    // 显示辅助
    void displayPromotionDetails(const Promotion* promotion);
    void displayProductListForSelection();

public:
    PromotionMenu(PromotionManager& pm, ProductList& pl);
    void run();
};

#endif