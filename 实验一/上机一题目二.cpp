#include <stdio.h>
#include <stdlib.h>

// 多项式节点结构
typedef struct PolyNode {
    float coef;           // 系数
    int exp;              // 指数
    struct PolyNode *next; // 下一个节点指针
} PolyNode, *Polynomial;

// 函数声明
Polynomial createPolynomial();
void printPolynomial(Polynomial P);
Polynomial addPolynomial(Polynomial A, Polynomial B);
Polynomial subPolynomial(Polynomial A, Polynomial B);
void destroyPolynomial(Polynomial P);
void insertNode(Polynomial *P, float coef, int exp);

// 创建多项式链表
Polynomial createPolynomial() {
    Polynomial head = (Polynomial)malloc(sizeof(PolyNode));
    head->next = NULL;
    
    int n;
    printf("请输入多项式的项数: ");
    scanf("%d", &n);
    
    if (n <= 0) {
        return head;
    }
    
    printf("请按指数降序输入各项的系数和指数（每行一组，用空格分隔）:\n");
    for (int i = 0; i < n; i++) {
        float coef;
        int exp;
        scanf("%f %d", &coef, &exp);
        insertNode(&head, coef, exp);
    }
    
    return head;
}

// 插入节点（保持降序排列）
void insertNode(Polynomial *P, float coef, int exp) {
    if (coef == 0) return; // 系数为0不插入
    
    Polynomial newNode = (Polynomial)malloc(sizeof(PolyNode));
    newNode->coef = coef;
    newNode->exp = exp;
    newNode->next = NULL;
    
    Polynomial prev = *P;
    Polynomial curr = (*P)->next;
    
    // 寻找插入位置
    while (curr != NULL && curr->exp > exp) {
        prev = curr;
        curr = curr->next;
    }
    
    // 如果指数相同，合并系数
    if (curr != NULL && curr->exp == exp) {
        curr->coef += coef;
        free(newNode);
        if (curr->coef == 0) {
            prev->next = curr->next;
            free(curr);
        }
        return;
    }
    
    // 插入新节点
    newNode->next = curr;
    prev->next = newNode;
}

// 输出多项式
void printPolynomial(Polynomial P) {
    Polynomial curr = P->next;
    
    if (curr == NULL) {
        printf("0\n");
        return;
    }
    
    // 先计算项数
    int count = 0;
    Polynomial temp = curr;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    
    // 输出项数和各项系数指数
    printf("%d", count);
    while (curr != NULL) {
        printf(" %.1f %d", curr->coef, curr->exp);
        curr = curr->next;
    }
    printf("\n");
}

// 多项式相加
Polynomial addPolynomial(Polynomial A, Polynomial B) {
    Polynomial C = (Polynomial)malloc(sizeof(PolyNode));
    C->next = NULL;
    
    Polynomial p1 = A->next;
    Polynomial p2 = B->next;
    
    while (p1 != NULL && p2 != NULL) {
        if (p1->exp > p2->exp) {
            insertNode(&C, p1->coef, p1->exp);
            p1 = p1->next;
        } else if (p1->exp < p2->exp) {
            insertNode(&C, p2->coef, p2->exp);
            p2 = p2->next;
        } else {
            float sum = p1->coef + p2->coef;
            if (sum != 0) {
                insertNode(&C, sum, p1->exp);
            }
            p1 = p1->next;
            p2 = p2->next;
        }
    }
    
    // 处理剩余项
    while (p1 != NULL) {
        insertNode(&C, p1->coef, p1->exp);
        p1 = p1->next;
    }
    
    while (p2 != NULL) {
        insertNode(&C, p2->coef, p2->exp);
        p2 = p2->next;
    }
    
    return C;
}

// 多项式相减
Polynomial subPolynomial(Polynomial A, Polynomial B) {
    Polynomial D = (Polynomial)malloc(sizeof(PolyNode));
    D->next = NULL;
    
    Polynomial p1 = A->next;
    Polynomial p2 = B->next;
    
    while (p1 != NULL && p2 != NULL) {
        if (p1->exp > p2->exp) {
            insertNode(&D, p1->coef, p1->exp);
            p1 = p1->next;
        } else if (p1->exp < p2->exp) {
            insertNode(&D, -p2->coef, p2->exp);
            p2 = p2->next;
        } else {
            float diff = p1->coef - p2->coef;
            if (diff != 0) {
                insertNode(&D, diff, p1->exp);
            }
            p1 = p1->next;
            p2 = p2->next;
        }
    }
    
    // 处理剩余项
    while (p1 != NULL) {
        insertNode(&D, p1->coef, p1->exp);
        p1 = p1->next;
    }
    
    while (p2 != NULL) {
        insertNode(&D, -p2->coef, p2->exp);
        p2 = p2->next;
    }
    
    return D;
}

// 销毁多项式链表
void destroyPolynomial(Polynomial P) {
    Polynomial curr = P;
    while (curr != NULL) {
        Polynomial temp = curr;
        curr = curr->next;
        free(temp);
    }
}

// 主函数
int main() {
    printf("=== 一元多项式加减运算器 ===\n\n");
    
    // 创建多项式A
    printf("创建多项式A:\n");
    Polynomial A = createPolynomial();
    printf("多项式A: ");
    printPolynomial(A);
    
    // 创建多项式B
    printf("\n创建多项式B:\n");
    Polynomial B = createPolynomial();
    printf("多项式B: ");
    printPolynomial(B);
    
    // 相加运算
    printf("\n多项式A + 多项式B:\n");
    Polynomial C = addPolynomial(A, B);
    printf("结果多项式C: ");
    printPolynomial(C);
    
    // 相减运算
    printf("\n多项式A - 多项式B:\n");
    Polynomial D = subPolynomial(A, B);
    printf("结果多项式D: ");
    printPolynomial(D);
    
    // 清理内存
    destroyPolynomial(A);
    destroyPolynomial(B);
    destroyPolynomial(C);
    destroyPolynomial(D);
    
    return 0;
}

