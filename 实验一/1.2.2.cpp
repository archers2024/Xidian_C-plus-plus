#include <iostream>
#include <iomanip>
#include <cmath>  // 添加这行来使用abs函数
using namespace std;

// 多项式节点结构
struct PolyNode {
    double coef;  // 系数
    int exp;      // 指数
    PolyNode* next;
    
    PolyNode(double c, int e) : coef(c), exp(e), next(NULL) {}
};

// 多项式类
class Polynomial {
private:
    PolyNode* head;
    string name;
    
public:
    // 构造函数
    Polynomial(string n = "") : name(n) {
        head = new PolyNode(0, -1);  // 头节点
    }
    
    // 析构函数
    ~Polynomial() {
        clear();
        delete head;
    }
    
    // 清空多项式
    void clear() {
        PolyNode* current = head->next;
        while (current != NULL) {
            PolyNode* temp = current;
            current = current->next;
            delete temp;
        }
        head->next = NULL;
    }
    
    // 插入项
    void insertTerm(double coef, int exp) {
        if (coef == 0) return;
        
        PolyNode* prev = head;
        PolyNode* current = head->next;
        
        // 寻找插入位置
        while (current != NULL && current->exp > exp) {
            prev = current;
            current = current->next;
        }
        
        // 合并同类项
        if (current != NULL && current->exp == exp) {
            current->coef += coef;
            if (current->coef == 0) {
                prev->next = current->next;
                delete current;
            }
            return;
        }
        
        // 插入新节点
        PolyNode* newNode = new PolyNode(coef, exp);
        newNode->next = current;
        prev->next = newNode;
    }
    
    // 输入多项式
    void input() {
        clear();
        int n;
        cout << "请输入多项式 " << name << " 的项数: ";
        cin >> n;
        
        if (n <= 0) return;
        
        cout << "请依次输入各项的系数和指数（用空格分隔）:" << endl;
        for (int i = 0; i < n; i++) {
            double coef;
            int exp;
            cout << "第 " << (i + 1) << " 项: ";
            cin >> coef >> exp;
            insertTerm(coef, exp);
        }
        cout << "多项式 " << name << " 输入完成！" << endl;
    }
    
    // 输出标准格式
    void outputStandard() {
        PolyNode* current = head->next;
        
        // 计算项数
        int count = 0;
        PolyNode* temp = current;
        while (temp != NULL) {
            count++;
            temp = temp->next;
        }
        
        // 输出标准格式
        cout << count;
        while (current != NULL) {
            cout << " " << fixed << setprecision(1) << current->coef 
                 << " " << current->exp;
            current = current->next;
        }
        cout << endl;
    }
    
    // 输出数学格式
    void outputMath() {
        PolyNode* current = head->next;
        
        if (current == NULL) {
            cout << "0";
            return;
        }
        
        bool first = true;
        while (current != NULL) {
            // 处理符号
            if (!first) {
                if (current->coef > 0) cout << " + ";
                else cout << " - ";
            } else if (current->coef < 0) {
                cout << "-";
            }
            
            double absCoef = abs(current->coef);  // 现在abs函数可以正常使用了
            
            // 输出项 - 修复这里的错误
            if (current->exp == 0) {
                cout << absCoef;
            } else if (current->exp == 1) {
                if (absCoef == 1) cout << "x";
                else cout << absCoef << "x";
            } else {
                if (absCoef == 1) cout << "x^" << current->exp;  // 修复：使用current->exp
                else cout << absCoef << "x^" << current->exp;    // 修复：使用current->exp
            }
            
            first = false;
            current = current->next;
        }
        cout << endl;
    }
    
    // 显示详细信息
    void display() {
        cout << "\n=== 多项式 " << name << " ===" << endl;
        cout << "标准格式: ";
        outputStandard();
        cout << "数学格式: ";
        outputMath();
    }
    
    // 判断是否为空
    bool isEmpty() {
        return head->next == NULL;
    }
    
    // 多项式加法
    Polynomial add(Polynomial& other) {
        Polynomial result(name + "+" + other.name);
        PolyNode* p1 = this->head->next;
        PolyNode* p2 = other.head->next;
        
        while (p1 != NULL && p2 != NULL) {
            if (p1->exp > p2->exp) {
                result.insertTerm(p1->coef, p1->exp);
                p1 = p1->next;
            } else if (p1->exp < p2->exp) {
                result.insertTerm(p2->coef, p2->exp);
                p2 = p2->next;
            } else {
                double sum = p1->coef + p2->coef;
                if (sum != 0) {
                    result.insertTerm(sum, p1->exp);
                }
                p1 = p1->next;
                p2 = p2->next;
            }
        }
        
        // 处理剩余项
        while (p1 != NULL) {
            result.insertTerm(p1->coef, p1->exp);
            p1 = p1->next;
        }
        
        while (p2 != NULL) {
            result.insertTerm(p2->coef, p2->exp);
            p2 = p2->next;
        }
        
        return result;
    }
    
    // 多项式减法
    Polynomial subtract(Polynomial& other) {
        Polynomial result(name + "-" + other.name);
        PolyNode* p1 = this->head->next;
        PolyNode* p2 = other.head->next;
        
        while (p1 != NULL && p2 != NULL) {
            if (p1->exp > p2->exp) {
                result.insertTerm(p1->coef, p1->exp);
                p1 = p1->next;
            } else if (p1->exp < p2->exp) {
                result.insertTerm(-p2->coef, p2->exp);
                p2 = p2->next;
            } else {
                double diff = p1->coef - p2->coef;
                if (diff != 0) {
                    result.insertTerm(diff, p1->exp);
                }
                p1 = p1->next;
                p2 = p2->next;
            }
        }
        
        // 处理剩余项
        while (p1 != NULL) {
            result.insertTerm(p1->coef, p1->exp);
            p1 = p1->next;
        }
        
        while (p2 != NULL) {
            result.insertTerm(-p2->coef, p2->exp);
            p2 = p2->next;
        }
        
        return result;
    }
};

// 显示菜单
void showMenu() {
    cout << "\n========== 一元多项式计算器 ==========" << endl;
    cout << "1. 输入多项式A" << endl;
    cout << "2. 输入多项式B" << endl;
    cout << "3. 显示多项式A" << endl;
    cout << "4. 显示多项式B" << endl;
    cout << "5. 计算 A + B" << endl;
    cout << "6. 计算 A - B" << endl;
    cout << "7. 计算 B - A" << endl;
    cout << "8. 显示所有多项式" << endl;
    cout << "0. 退出程序" << endl;
    cout << "======================================" << endl;
    cout << "请选择操作: ";
}

int main() {
    Polynomial A("A");
    Polynomial B("B");
    int choice;
    
    cout << "欢迎使用一元多项式加减运算器！" << endl;
    cout << "版本: Dev-C++ 兼容版" << endl;
    
    do {
        showMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                A.input();
                break;
            case 2:
                B.input();
                break;
            case 3:
                A.display();
                break;
            case 4:
                B.display();
                break;
            case 5: {
                if (A.isEmpty() && B.isEmpty()) {
                    cout << "请先输入多项式A和B！" << endl;
                } else {
                    Polynomial C = A.add(B);
                    cout << "\n=== 加法结果 A + B ===" << endl;
                    cout << "标准格式: ";
                    C.outputStandard();
                    cout << "数学格式: ";
                    C.outputMath();
                }
                break;
            }
            case 6: {
                if (A.isEmpty() && B.isEmpty()) {
                    cout << "请先输入多项式A和B！" << endl;
                } else {
                    Polynomial C = A.subtract(B);
                    cout << "\n=== 减法结果 A - B ===" << endl;
                    cout << "标准格式: ";
                    C.outputStandard();
                    cout << "数学格式: ";
                    C.outputMath();
                }
                break;
            }
            case 7: {
                if (A.isEmpty() && B.isEmpty()) {
                    cout << "请先输入多项式A和B！" << endl;
                } else {
                    Polynomial C = B.subtract(A);
                    cout << "\n=== 减法结果 B - A ===" << endl;
                    cout << "标准格式: ";
                    C.outputStandard();
                    cout << "数学格式: ";
                    C.outputMath();
                }
                break;
            }
            case 8:
                cout << "\n=== 所有多项式 ===" << endl;
                A.display();
                B.display();
                break;
            case 0:
                cout << "感谢使用，再见！" << endl;
                break;
            default:
                cout << "无效选择，请重新输入！" << endl;
        }
        
        // 暂停一下让用户看到结果
        if (choice != 0) {
            cout << "\n按回车键继续...";
            cin.ignore();
            cin.get();
        }
        
    } while (choice != 0);
    
    return 0;
}

