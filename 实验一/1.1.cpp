#include <iostream>
using namespace std;

// 定义单链表结点结构
struct Node {           
    int data;       // 结点数据
    Node* next;     // 指向下一结点的指针
    Node(int val) : data(val), next(nullptr) {} // 构造函数
};

// 创建单链表
Node* createList() {
    Node* head = nullptr;
    Node* tail = nullptr;
    int n, val;
    cout << "请输入链表元素个数：";
    cin >> n;
    cout << "请依次输入" << n << "个整数：" << endl;
    for (int i = 0; i < n; ++i) {
        cin >> val;
        Node* newNode = new Node(val);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

// 遍历并输出单链表
void printList(Node* head) {
    Node* p = head;
    while (p != nullptr) {
        cout << p->data << " ";
        p = p->next;
    }
    cout << endl;
}

// 单链表冒泡排序（非递减）
void bubbleSortList(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return; // 空链表或只有一个结点，无需排序
    }
    bool swapped; // 标记本轮是否发生交换
    Node* p;
    Node* tail = nullptr; // 已排序部分的尾边界（无需再比较）
    do {
        swapped = false;
        p = head;
        while (p->next != tail) {
            if (p->data > p->next->data) { // 前结点值大于后结点，交换值
                swap(p->data, p->next->data);
                swapped = true; // 标记发生了交换
            }
            p = p->next;
        }
        tail = p; // 更新尾边界（本轮最大的元素已到尾部）
    } while (swapped); // 若本轮无交换，说明已有序，退出循环
}

int main() {
    Node* head = createList(); // 创建单链表
    cout << "排序前的链表：";
    printList(head);          // 输出排序前的链表
    bubbleSortList(head);     // 对链表进行冒泡排序
    cout << "排序后的链表（非递减）：";
    printList(head);          // 输出排序后的链表

    // 释放链表内存（防止内存泄漏）
    Node* curr = head;
    while (curr != nullptr) {
        Node* temp = curr;
        curr = curr->next;
        delete temp;
    }
    return 0;
}

