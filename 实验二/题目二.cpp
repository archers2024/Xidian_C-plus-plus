#include <iostream>
#include <string>
using namespace std;

// 汽车信息类
class Car {
public:
    string licensePlate;  // 车牌号码
    int arrivalTime;      // 到达时间
    int position;         // 停车位置（停车场内或便道上的位置）
    
    Car() : licensePlate(""), arrivalTime(0), position(0) {}
    Car(string plate, int time, int pos = 0) 
        : licensePlate(plate), arrivalTime(time), position(pos) {}
};

// 栈类 - 模拟停车场
class ParkingStack {
private:
    Car* spaces;          // 停车位数组
    int capacity;         // 停车场容量
    int top;              // 栈顶指针

public:
    // 构造函数
    ParkingStack(int n) {
        capacity = n;
        spaces = new Car[capacity];
        top = -1;
    }
    
    // 析构函数
    ~ParkingStack() {
        delete[] spaces;
    }
    
    // 获取停车场容量
    int getCapacity() {
        return capacity;
    }
    
    // 判断停车场是否已满
    bool isFull() {
        return top == capacity - 1;
    }
    
    // 判断停车场是否为空
    bool isEmpty() {
        return top == -1;
    }
    
    // 获取当前停车数量
    int getSize() {
        return top + 1;
    }
    
    // 车辆进入停车场
    bool parkCar(const Car& car) {
        if (isFull()) {
            return false;
        }
        top++;
        spaces[top] = car;
        spaces[top].position = top + 1;  // 修正：位置从1开始编号
        return true;
    }
    
    // 车辆离开停车场
    Car leaveParking(int position) {
        if (isEmpty() || position < 1 || position > top + 1) {
            return Car();  // 返回空车表示失败
        }
        
        // 找到要离开的车辆在栈中的索引
        int index = position - 1;
        Car leavingCar = spaces[index];
        
        // 将后面的车辆暂时移出（让路）
        for (int i = index; i < top; i++) {
            spaces[i] = spaces[i + 1];
            spaces[i].position = i + 1;  // 更新位置
        }
        top--;
        
        return leavingCar;
    }
    
    // 根据车牌号查找车辆
    int findCarByLicense(string license) {
        for (int i = 0; i <= top; i++) {
            if (spaces[i].licensePlate == license) {
                return i + 1;  // 返回位置（从1开始）
            }
        }
        return -1;  // 未找到
    }
    
    // 获取栈顶车辆
    Car getTopCar() {
        if (isEmpty()) {
            return Car();
        }
        return spaces[top];
    }
    
    // 显示停车场状态
    void display() {
        cout << "停车场（容量：" << capacity << "，当前：" << getSize() << "辆）" << endl;
        if (isEmpty()) {
            cout << "  停车场为空" << endl;
            return;
        }
        cout << "  位置\t车牌\t到达时间" << endl;
        for (int i = 0; i <= top; i++) {
            cout << "  " << spaces[i].position << "\t" 
                 << spaces[i].licensePlate << "\t" 
                 << spaces[i].arrivalTime << endl;
        }
    }
};

// 辅助栈 - 用于临时存放让路的车辆
class TempStack {
private:
    Car* spaces;          // 临时停车位数组
    int capacity;         // 容量
    int top;              // 栈顶指针

public:
    // 构造函数
    TempStack(int n) {
        capacity = n;
        spaces = new Car[capacity];
        top = -1;
    }
    
    // 析构函数
    ~TempStack() {
        delete[] spaces;
    }
    
    // 判断栈是否已满
    bool isFull() {
        return top == capacity - 1;
    }
    
    // 判断栈是否为空
    bool isEmpty() {
        return top == -1;
    }
    
    // 压入车辆
    bool push(const Car& car) {
        if (isFull()) {
            return false;
        }
        spaces[++top] = car;
        return true;
    }
    
    // 弹出车辆
    Car pop() {
        if (isEmpty()) {
            return Car();
        }
        return spaces[top--];
    }
};

// 队列节点类
class QueueNode {
public:
    Car car;
    QueueNode* next;
    
    QueueNode(const Car& c) : car(c), next(nullptr) {}
};

// 队列类 - 模拟便道
class WaitingQueue {
private:
    QueueNode* front;     // 队首指针
    QueueNode* rear;      // 队尾指针
    int count;            // 便道上的车辆数

public:
    // 构造函数
    WaitingQueue() {
        front = rear = nullptr;
        count = 0;
    }
    
    // 析构函数
    ~WaitingQueue() {
        clear();
    }
    
    // 清空队列
    void clear() {
        while (front != nullptr) {
            QueueNode* temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
        count = 0;
    }
    
    // 判断便道是否为空
    bool isEmpty() {
        return front == nullptr;
    }
    
    // 获取便道上的车辆数
    int getSize() {
        return count;
    }
    
    // 车辆进入便道等待
    void enqueue(const Car& car) {
        QueueNode* newNode = new QueueNode(car);
        newNode->car.position = count + 1;  // 便道位置从1开始编号
        
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        count++;
    }
    
    // 车辆离开便道（进入停车场）
    Car dequeue() {
        if (isEmpty()) {
            return Car();
        }
        
        QueueNode* temp = front;
        Car firstCar = front->car;
        front = front->next;
        
        if (front == nullptr) {
            rear = nullptr;
        }
        
        delete temp;
        count--;
        
        // 更新剩余车辆的位置
        QueueNode* current = front;
        int newPosition = 1;
        while (current != nullptr) {
            current->car.position = newPosition++;
            current = current->next;
        }
        
        return firstCar;
    }
    
    // 显示便道状态
    void display() {
        cout << "便道（当前：" << count << "辆等待）" << endl;
        if (isEmpty()) {
            cout << "  便道为空" << endl;
            return;
        }
        cout << "  位置\t车牌\t到达时间" << endl;
        QueueNode* current = front;
        while (current != nullptr) {
            cout << "  " << current->car.position << "\t" 
                 << current->car.licensePlate << "\t" 
                 << current->car.arrivalTime << endl;
            current = current->next;
        }
    }
};

// 停车场管理系统类
class ParkingManagementSystem {
private:
    ParkingStack* parkingLot;     // 停车场
    TempStack* tempParking;       // 临时停车场（用于让路）
    WaitingQueue* waitingLine;    // 便道
    int hourlyRate;               // 每小时费用

public:
    // 构造函数
    ParkingManagementSystem(int n, int rate = 5) {
        parkingLot = new ParkingStack(n);
        tempParking = new TempStack(n);  // 临时栈容量与停车场相同
        waitingLine = new WaitingQueue();
        hourlyRate = rate;
        
        cout << "停车场已创建，容量：" << n << "辆车，费率：" << rate << "元/小时" << endl;
    }
    
    // 析构函数
    ~ParkingManagementSystem() {
        delete parkingLot;
        delete tempParking;
        delete waitingLine;
    }
    
    // 处理车辆到达
    void handleArrival(string license, int time) {
        Car newCar(license, time);
        
        // 尝试将车辆停入停车场
        if (parkingLot->parkCar(newCar)) {
            // 获取实际停车位置
            int actualPosition = parkingLot->findCarByLicense(license);
            cout << "? 车辆 " << license << " 已停入停车场位置 " 
                 << actualPosition << "（到达时间：" << time << "）" << endl;
        } else {
            // 停车场已满，进入便道等待
            waitingLine->enqueue(newCar);
            cout << "→ 车辆 " << license << " 在便道位置 " 
                 << newCar.position << " 等待（到达时间：" << time << "）" << endl;
        }
        
        displayStatus();
    }
    
    // 处理车辆离开
    void handleDeparture(string license, int time) {
        // 在停车场中查找车辆
        int position = parkingLot->findCarByLicense(license);
        
        if (position == -1) {
            cout << "? 错误：车辆 " << license << " 不在停车场中" << endl;
            return;
        }
        
        // 处理让路：将目标车辆之后的车辆移到临时栈
        int targetPosition = position;
        int carsToMove = parkingLot->getSize() - targetPosition;
        
        for (int i = 0; i < carsToMove; i++) {
            Car movedCar = parkingLot->leaveParking(parkingLot->getSize());
            tempParking->push(movedCar);
        }
        
        // 目标车辆离开
        Car leavingCar = parkingLot->leaveParking(targetPosition);
        int parkingTime = time - leavingCar.arrivalTime;
        int fee = parkingTime * hourlyRate;
        
        cout << "? 车辆 " << license << " 离开：" << endl;
        cout << "  停车时间: " << parkingTime << " 小时" << endl;
        cout << "  应缴费用: " << fee << " 元" << endl;
        
        // 将临时栈中的车辆重新停回停车场
        while (!tempParking->isEmpty()) {
            Car movedCar = tempParking->pop();
            parkingLot->parkCar(movedCar);
        }
        
        // 如果便道上有等待车辆，让第一辆进入停车场
        if (!waitingLine->isEmpty()) {
            Car nextCar = waitingLine->dequeue();
            nextCar.arrivalTime = time;  // 更新进入停车场的时间
            if (parkingLot->parkCar(nextCar)) {
                int actualPosition = parkingLot->findCarByLicense(nextCar.licensePlate);
                cout << "→ 车辆 " << nextCar.licensePlate << " 从便道进入停车场位置 " 
                     << actualPosition << endl;
            }
        }
        
        displayStatus();
    }
    
    // 显示系统状态
    void displayStatus() {
        cout << "\n--- 当前系统状态 ---" << endl;
        parkingLot->display();
        waitingLine->display();
        cout << "-------------------\n" << endl;
    }
    
    // 获取停车场容量
    int getParkingCapacity() {
        return parkingLot->getCapacity();
    }
};

// 主函数 - 交互式模式
int main() {
    cout << "=== 停车场管理系统 ===" << endl;
    
    int capacity, rate;
    cout << "请输入停车场容量: ";
    cin >> capacity;
    cout << "请输入每小时费率: ";
    cin >> rate;
    
    ParkingManagementSystem system(capacity, rate);
    
    char operation;
    string license;
    int time;
    
    cout << "\n操作说明：" << endl;
    cout << "A 车牌号 时间 - 车辆到达" << endl;
    cout << "D 车牌号 时间 - 车辆离开" << endl;
    cout << "E - 结束程序" << endl;
    cout << "=====================\n" << endl;
    
    while (true) {
        cout << "请输入操作: ";
        cin >> operation;
        
        if (operation == 'E' || operation == 'e') {
            cout << "程序结束" << endl;
            break;
        }
        
        cin >> license >> time;
        
        if (operation == 'A' || operation == 'a') {
            system.handleArrival(license, time);
        } else if (operation == 'D' || operation == 'd') {
            system.handleDeparture(license, time);
        } else {
            cout << "无效操作！请使用 A, D 或 E" << endl;
            // 清除无效输入
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    
    return 0;
}
//// 汽车信息类
//class Car {
//    string licensePlate;
//    int arrivalTime;
//    int position;
//};
//
//// 停车场栈类
//class ParkingStack {
//private:
//    Car* spaces;
//    int capacity;
//    int top;
//    
//public:
//    bool parkCar(const Car& car);
//    Car leaveParking(int position);
//    int findCarByLicense(string license);
//};

//// 临时栈类
//class TempStack {
//    // 用于存放让路的车辆
//};
//
//// 便道队列节点类
//class QueueNode {
//    Car car;
//    QueueNode* next;
//};
//
//// 便道队列类
//class WaitingQueue {
//private:
//    QueueNode* front;
//    QueueNode* rear;
//    int count;
//    
//public:
//    void enqueue(const Car& car);
//    Car dequeue();
//};
//
//// 停车场管理系统类
//class ParkingManagementSystem {
//private:
//    ParkingStack* parkingLot;
//    TempStack* tempParking;
//    WaitingQueue* waitingLine;
//    int hourlyRate;
//    
//public:
//    void handleArrival(string license, int time);
//    void handleDeparture(string license, int time);
//    void displayStatus();
//};
