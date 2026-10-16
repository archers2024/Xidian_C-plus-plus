#include <iostream>
#include <cstdlib>  // 添加这个头文件用于 rand() 和 srand()
#include <ctime>    // 添加这个头文件用于 time()
using namespace std;

// 循环链表节点类
class Node {
public:
    int id;         // 人员编号
    int password;   // 密码
    Node* next;     // 指向下一个节点的指针
    
    // 构造函数
    Node(int id, int password) {
        this->id = id;
        this->password = password;
        this->next = nullptr;
    }
};

// 约瑟夫环类
class JosephCircle {
private:
    Node* head;     // 链表头指针
    Node* tail;     // 链表尾指针
    int size;       // 链表大小

public:
    // 构造函数
    JosephCircle() {
        head = tail = nullptr;
        size = 0;
    }
    
    // 析构函数
    ~JosephCircle() {
        clear();
    }
    
    // 清空链表
    void clear() {
        if (isEmpty()) return;
        
        // 由于是循环链表，需要先断开环
        if (tail != nullptr) {
            tail->next = nullptr;
        }
        
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = tail = nullptr;
        size = 0;
    }
    
    // 判断链表是否为空
    bool isEmpty() {
        return head == nullptr;
    }
    
    // 添加人员到环中
    void addPerson(int id, int password) {
        Node* newNode = new Node(id, password);
        
        if (isEmpty()) {
            // 第一个节点，自己指向自己形成环
            head = tail = newNode;
            newNode->next = head;
        } else {
            // 添加到链表尾部
            tail->next = newNode;
            tail = newNode;
            tail->next = head;  // 形成循环
        }
        size++;
    }
    
    // 解决约瑟夫问题
    void solveJoseph(int initialM) {
        if (isEmpty()) {
            cout << "人员列表为空！" << endl;
            return;
        }
        
        cout << "出列顺序：";
        
        Node* current = head;
        Node* previous = tail;  // 前一个节点，初始为尾节点
        int m = initialM;
        int count = 1;
        bool firstOutput = true;  // 标记是否是第一个输出
        
        // 当环中还有多于一个节点时继续
        while (size > 1) {
            // 报数
            if (count < m) {
                previous = current;
                current = current->next;
                count++;
            } else {
                // 报到m的人出列
                if (!firstOutput) {
                    cout << ", ";
                }
                cout << current->id;
                firstOutput = false;
                
                // 更新m值为出列者的密码
                m = current->password;
                
                // 从环中移除当前节点
                previous->next = current->next;
                
                // 如果删除的是头节点，需要更新头指针
                if (current == head) {
                    head = current->next;
                }
                // 如果删除的是尾节点，需要更新尾指针
                if (current == tail) {
                    tail = previous;
                }
                
                // 删除节点并移动到下一个
                Node* temp = current;
                current = current->next;
                delete temp;
                size--;
                
                // 重置计数器
                count = 1;
            }
        }
        
        // 输出最后剩下的人
        if (size == 1) {
            if (!firstOutput) {
                cout << ", ";
            }
            cout << current->id << endl;
            delete current;
            head = tail = nullptr;
            size = 0;
        } else {
            cout << endl;
        }
    }
    
    // 显示环中所有人员信息
    void display() {
        if (isEmpty()) {
            cout << "环为空！" << endl;
            return;
        }
        
        cout << "当前环中人员：";
        Node* current = head;
        do {
            cout << "[" << current->id << ",密码:" << current->password << "]";
            if (current->next != head) cout << " -> ";
            current = current->next;
        } while (current != head);
        cout << endl;
    }
};

// 演示一些不同的测试用例
void demonstrateExamples() {
    cout << "=== 约瑟夫问题演示 ===" << endl;
    
    // 示例1：简单情况
    {
        JosephCircle circle1;
        cout << "\n示例1 - 简单情况：" << endl;
        cout << "初始m=3, 3个人，密码都是2" << endl;
        circle1.addPerson(1, 2);
        circle1.addPerson(2, 2);
        circle1.addPerson(3, 2);
        circle1.display();
        circle1.solveJoseph(3);
    }
    
    // 示例2：不同密码
    {
        JosephCircle circle2;
        cout << "\n示例2 - 不同密码：" << endl;
        cout << "初始m=4, 5个人，密码分别为1,3,2,5,4" << endl;
        circle2.addPerson(1, 1);
        circle2.addPerson(2, 3);
        circle2.addPerson(3, 2);
        circle2.addPerson(4, 5);
        circle2.addPerson(5, 4);
        circle2.display();
        circle2.solveJoseph(4);
    }
    
    // 示例3：较大数字
    {
        JosephCircle circle3;
        cout << "\n示例3 - 较大数字：" << endl;
        cout << "初始m=10, 4个人，密码分别为5,8,3,6" << endl;
        circle3.addPerson(1, 5);
        circle3.addPerson(2, 8);
        circle3.addPerson(3, 3);
        circle3.addPerson(4, 6);
        circle3.display();
        circle3.solveJoseph(10);
    }
}

// 主函数
int main() {
    cout << "=== 约瑟夫问题求解程序 ===" << endl;
    
    int choice;
    cout << "请选择模式：" << endl;
    cout << "1. 手动输入数据" << endl;
    cout << "2. 查看演示示例" << endl;
    cout << "3. 使用随机数据" << endl;
    cout << "请选择(1-3): ";
    cin >> choice;
    
    if (choice == 1) {
        // 手动输入模式
        JosephCircle circle;
        int initialM, n;
        
        cout << "\n=== 手动输入模式 ===" << endl;
        cout << "请输入初始报数上限值m：";
        cin >> initialM;
        
        cout << "请输入人数n：";
        cin >> n;
        
        if (n <= 0) {
            cout << "人数必须大于0！" << endl;
            return 1;
        }
        
        cout << "请依次输入" << n << "个人的密码：" << endl;
        for (int i = 1; i <= n; i++) {
            int password;
            cout << "第" << i << "个人的密码：";
            cin >> password;
            if (password <= 0) {
                cout << "密码必须为正整数！" << endl;
                return 1;
            }
            circle.addPerson(i, password);
        }
        
        // 显示初始环状态
        cout << "\n初始状态：" << endl;
        circle.display();
        
        // 解决约瑟夫问题
        cout << "\n开始模拟约瑟夫问题..." << endl;
        circle.solveJoseph(initialM);
        
    } else if (choice == 2) {
        // 演示示例模式
        demonstrateExamples();
        
    } else if (choice == 3) {
        // 随机数据模式
        JosephCircle circle;
        int initialM, n;
        
        cout << "\n=== 随机数据模式 ===" << endl;
        cout << "请输入初始报数上限值m：";
        cin >> initialM;
        
        cout << "请输入人数n：";
        cin >> n;
        
        if (n <= 0) {
            cout << "人数必须大于0！" << endl;
            return 1;
        }
        
        cout << "自动生成" << n << "个人的密码（1-10之间的随机数）：" << endl;
        srand(time(0));  // 设置随机种子
        
        for (int i = 1; i <= n; i++) {
            int password = rand() % 10 + 1;  // 1-10的随机数
            cout << "第" << i << "个人的密码：" << password << endl;
            circle.addPerson(i, password);
        }
        
        // 显示初始环状态
        cout << "\n初始状态：" << endl;
        circle.display();
        
        // 解决约瑟夫问题
        cout << "\n开始模拟约瑟夫问题..." << endl;
        circle.solveJoseph(initialM);
        
    } else {
        cout << "无效选择！" << endl;
        return 1;
    }
    
    return 0;
}
// 节点类
//class Node {
//    int id;         // 人员编号
//    int password;   // 密码
//    Node* next;     // 下一个节点指针
//};
//
//// 约瑟夫环类
//class JosephCircle {
//private:
//    Node* head;     // 头指针
//    Node* tail;     // 尾指针
//    int size;       // 环大小
//    
//public:
//    // 核心方法
//    void addPerson(int id, int password);
//    void solveJoseph(int initialM);
//    void display();
//    bool isEmpty();
//    void clear();
//};
