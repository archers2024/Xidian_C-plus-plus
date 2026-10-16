#include <iostream>
using namespace std;

// 多项式结点结构：系数 + 指数 + 下一结点指针
struct PolyNode {
    int coef;        // 系数
    int exp;         // 指数
    PolyNode* next;  // 指向下一项的指针
    PolyNode(int c, int e) : coef(c), exp(e), next(nullptr) {}
};

// 创建多项式（输入系数和指数，构建链表）
PolyNode* createPoly() {
    PolyNode* head = new PolyNode(0, 0); // 头结点（不存储实际项，方便操作）
    PolyNode* tail = head;
    int n;
    cout << "请输入多项式的项数：";
    cin >> n;
    cout << "请依次输入每一项的系数和指数（系数 指数）：" << endl;
    for (int i = 0; i < n; i++) {
        int c, e;
        cin >> c >> e;
        PolyNode* newNode = new PolyNode(c, e);
        tail->next = newNode;
        tail = newNode;
    }
    return head;
}

// 输出多项式：格式为“项数 系数1 指数1 系数2 指数2 ...”
void printPoly(PolyNode* head) {
    PolyNode* p = head->next;
    int count = 0;
    // 先统计项数
    while (p != nullptr) {
        count++;
        p = p->next;
    }
    cout << count; // 输出项数
    p = head->next;
    // 输出每一项的系数和指数
    while (p != nullptr) {
        cout << " " << p->coef << " " << p->exp;
        p = p->next;
    }
    cout << endl;
}

// 多项式相加：A + B → C
PolyNode* addPoly(PolyNode* A, PolyNode* B) {
    PolyNode* headC = new PolyNode(0, 0); // 结果多项式的头结点
    PolyNode* tailC = headC;
    PolyNode* pA = A->next;
    PolyNode* pB = B->next;

    while (pA != nullptr && pB != nullptr) {
        if (pA->exp == pB->exp) { // 指数相等：系数相加
            int sum = pA->coef + pB->coef;
            if (sum != 0) { // 系数和不为 0，才加入结果
                tailC->next = new PolyNode(sum, pA->exp);
                tailC = tailC->next;
            }
            pA = pA->next;
            pB = pB->next;
        } else if (pA->exp > pB->exp) { // A 的指数大，取 A 的项
            tailC->next = new PolyNode(pA->coef, pA->exp);
            tailC = tailC->next;
            pA = pA->next;
        } else { // B 的指数大，取 B 的项
            tailC->next = new PolyNode(pB->coef, pB->exp);
            tailC = tailC->next;
            pB = pB->next;
        }
    }
    // 处理剩余项（A 或 B 还有未遍历的项）
    while (pA != nullptr) {
        tailC->next = new PolyNode(pA->coef, pA->exp);
        tailC = tailC->next;
        pA = pA->next;
    }
    while (pB != nullptr) {
        tailC->next = new PolyNode(pB->coef, pB->exp);
        tailC = tailC->next;
        pB = pB->next;
    }
    return headC;
}

// 多项式相减：A - B → D（等价于 A + (-B)）
PolyNode* subPoly(PolyNode* A, PolyNode* B) {
    // 复制 B 并将系数取反，得到 -B
    PolyNode* headNegB = new PolyNode(0, 0);
    PolyNode* tailNegB = headNegB;
    PolyNode* pB = B->next;
    while (pB != nullptr) {
        tailNegB->next = new PolyNode(-pB->coef, pB->exp);
        tailNegB = tailNegB->next;
        pB = pB->next;
    }
    // 调用相加函数，计算 A + (-B)
    PolyNode* headD = addPoly(A, headNegB);
    // 释放 -B 的内存
    PolyNode* curr = headNegB;
    while (curr != nullptr) {
        PolyNode* temp = curr;
        curr = curr->next;
        delete temp;
    }
    return headD;
}

int main() {
    cout << "=== 输入多项式 A ===" << endl;
    PolyNode* A = createPoly();
    cout << "多项式 A：";
    printPoly(A);

    cout << "=== 输入多项式 B ===" << endl;
    PolyNode* B = createPoly();
    cout << "多项式 B：";
    printPoly(B);

    // 多项式相加
    PolyNode* C = addPoly(A, B);
    cout << "A + B 的结果：";
    printPoly(C);

    // 多项式相减
    PolyNode* D = subPoly(A, B);
    cout << "A - B 的结果：";
    printPoly(D);

    // 释放所有多项式的内存（防止内存泄漏）
    auto freePoly = [](PolyNode* head) {
        PolyNode* curr = head;
        while (curr != nullptr) {
            PolyNode* temp = curr;
            curr = curr->next;
            delete temp;
        }
    };
    freePoly(A);
    freePoly(B);
    freePoly(C);
    freePoly(D);

    return 0;
}
