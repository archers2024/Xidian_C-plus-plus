#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

// 记录类
class Record {
public:
    string phoneNumber;   // 电话号码
    string userName;      // 用户名
    string address;       // 地址
    
    Record() : phoneNumber(""), userName(""), address("") {}
    Record(string phone, string name, string addr) 
        : phoneNumber(phone), userName(name), address(addr) {}
    
    void display() const {
        cout << "电话号码: " << phoneNumber << endl;
        cout << "用户名: " << userName << endl;
        cout << "地址: " << address << endl;
    }
};

// 哈希表节点（用于链地址法）
class HashNode {
public:
    Record record;
    HashNode* next;
    
    HashNode(const Record& rec) : record(rec), next(nullptr) {}
};

// 以电话号码为关键字的哈希表（线性探查法）
class PhoneHashTable {
private:
    vector<Record> table;     // 哈希表
    vector<bool> occupied;    // 标记位置是否被占用
    int capacity;             // 哈希表容量
    int size;                 // 当前元素数量
    
    // 哈希函数 - 电话号码
    int hashFunction(const string& phone) {
        unsigned long hash = 0;
        for (char c : phone) {
            if (c >= '0' && c <= '9') {
                hash = hash * 31 + (c - '0');
            }
        }
        return hash % capacity;
    }
    
public:
    PhoneHashTable(int cap = 100) : capacity(cap), size(0) {
        table.resize(capacity);
        occupied.resize(capacity, false);
    }
    
    // 插入记录
    bool insert(const Record& record) {
        if (size >= capacity) {
            cout << "哈希表已满！" << endl;
            return false;
        }
        
        int index = hashFunction(record.phoneNumber);
        int originalIndex = index;
        
        // 线性探查
        while (occupied[index]) {
            // 检查是否已存在相同的电话号码
            if (table[index].phoneNumber == record.phoneNumber) {
                cout << "电话号码 " << record.phoneNumber << " 已存在！" << endl;
                return false;
            }
            index = (index + 1) % capacity;
            if (index == originalIndex) {
                cout << "哈希表已满！" << endl;
                return false;
            }
        }
        
        table[index] = record;
        occupied[index] = true;
        size++;
        return true;
    }
    
    // 查找记录（按电话号码）
    Record* search(const string& phone) {
        int index = hashFunction(phone);
        int originalIndex = index;
        
        while (occupied[index]) {
            if (table[index].phoneNumber == phone) {
                return &table[index];
            }
            index = (index + 1) % capacity;
            if (index == originalIndex) {
                break;
            }
        }
        
        return nullptr;
    }
    
    // 显示哈希表状态
    void displayStatus() {
        cout << "电话号码哈希表状态：" << endl;
        cout << "容量: " << capacity << ", 元素数量: " << size << endl;
        cout << "装载因子: " << (double)size / capacity << endl;
        
        int collisionCount = 0;
        for (int i = 0; i < capacity; i++) {
            if (occupied[i]) {
                int expectedIndex = hashFunction(table[i].phoneNumber);
                if (expectedIndex != i) {
                    collisionCount++;
                }
            }
        }
        cout << "冲突次数: " << collisionCount << endl;
    }
};

// 以用户名为关键字的哈希表（链地址法）
class NameHashTable {
private:
    vector<HashNode*> table;  // 哈希表（链表头指针数组）
    int capacity;             // 哈希表容量
    int size;                 // 当前元素数量
    
    // 哈希函数 - 用户名
    int hashFunction(const string& name) {
        unsigned long hash = 0;
        for (char c : name) {
            hash = hash * 31 + c;
        }
        return hash % capacity;
    }
    
public:
    NameHashTable(int cap = 100) : capacity(cap), size(0) {
        table.resize(capacity, nullptr);
    }
    
    ~NameHashTable() {
        clear();
    }
    
    // 清空哈希表
    void clear() {
        for (int i = 0; i < capacity; i++) {
            HashNode* current = table[i];
            while (current != nullptr) {
                HashNode* temp = current;
                current = current->next;
                delete temp;
            }
            table[i] = nullptr;
        }
        size = 0;
    }
    
    // 插入记录
    bool insert(const Record& record) {
        int index = hashFunction(record.userName);
        
        // 检查是否已存在相同的用户名
        HashNode* current = table[index];
        while (current != nullptr) {
            if (current->record.userName == record.userName) {
                cout << "用户名 " << record.userName << " 已存在！" << endl;
                return false;
            }
            current = current->next;
        }
        
        // 插入到链表头部
        HashNode* newNode = new HashNode(record);
        newNode->next = table[index];
        table[index] = newNode;
        size++;
        return true;
    }
    
    // 查找记录（按用户名）
    Record* search(const string& name) {
        int index = hashFunction(name);
        HashNode* current = table[index];
        
        while (current != nullptr) {
            if (current->record.userName == name) {
                return &(current->record);
            }
            current = current->next;
        }
        
        return nullptr;
    }
    
    // 显示哈希表状态
    void displayStatus() {
        cout << "用户名哈希表状态：" << endl;
        cout << "容量: " << capacity << ", 元素数量: " << size << endl;
        cout << "装载因子: " << (double)size / capacity << endl;
        
        int maxChainLength = 0;
        int nonEmptyBuckets = 0;
        
        for (int i = 0; i < capacity; i++) {
            int chainLength = 0;
            HashNode* current = table[i];
            while (current != nullptr) {
                chainLength++;
                current = current->next;
            }
            if (chainLength > 0) {
                nonEmptyBuckets++;
                if (chainLength > maxChainLength) {
                    maxChainLength = chainLength;
                }
            }
        }
        
        cout << "非空桶数量: " << nonEmptyBuckets << endl;
        cout << "最长链长度: " << maxChainLength << endl;
        cout << "平均链长度: " << (nonEmptyBuckets > 0 ? (double)size / nonEmptyBuckets : 0) << endl;
    }
};

// 哈希表管理系统
class HashTableSystem {
private:
    PhoneHashTable phoneTable;    // 电话号码哈希表
    NameHashTable nameTable;      // 用户名哈希表
    
public:
    HashTableSystem(int capacity = 100) : phoneTable(capacity), nameTable(capacity) {}
    
    // 从文件加载数据 - 改进版本
    bool loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "错误：无法打开文件 '" << filename << "'" << endl;
            cout << "请检查文件是否存在或路径是否正确" << endl;
            return false;
        }
        
        cout << "正在从文件 '" << filename << "' 加载数据..." << endl;
        
        string line;
        int lineCount = 0;
        int recordCount = 0;
        Record currentRecord;
        
        while (getline(file, line)) {
            lineCount++;
            
            // 跳过空行
            if (line.empty()) continue;
            
            // 根据行号确定数据字段
            switch (lineCount % 3) {
                case 1: // 第一行：电话号码
                    currentRecord.phoneNumber = line;
                    break;
                case 2: // 第二行：用户名
                    currentRecord.userName = line;
                    break;
                case 0: // 第三行：地址
                    currentRecord.address = line;
                    
                    // 插入记录到两个哈希表
                    if (phoneTable.insert(currentRecord) && nameTable.insert(currentRecord)) {
                        recordCount++;
                        cout << "已加载记录: " << currentRecord.userName << " - " << currentRecord.phoneNumber << endl;
                    } else {
                        cout << "加载记录失败: " << currentRecord.userName << " - " << currentRecord.phoneNumber << endl;
                    }
                    break;
            }
        }
        
        file.close();
        
        // 处理最后一条不完整的记录
        if (lineCount % 3 != 0) {
            cout << "警告：文件可能格式不正确，最后一条记录不完整" << endl;
        }
        
        cout << "成功加载 " << recordCount << " 条记录" << endl;
        return recordCount > 0;
    }
    
    // 从CSV格式文件加载数据（备用方法）
    bool loadFromCSV(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "错误：无法打开文件 '" << filename << "'" << endl;
            return false;
        }
        
        cout << "正在从CSV文件 '" << filename << "' 加载数据..." << endl;
        
        string line;
        int recordCount = 0;
        
        // 跳过标题行（如果有）
        getline(file, line);
        
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            stringstream ss(line);
            string phone, name, address;
            
            // 假设格式：电话,姓名,地址
            if (getline(ss, phone, ',') && getline(ss, name, ',') && getline(ss, address)) {
                Record record(phone, name, address);
                
                if (phoneTable.insert(record) && nameTable.insert(record)) {
                    recordCount++;
                    cout << "已加载记录: " << name << " - " << phone << endl;
                }
            }
        }
        
        file.close();
        cout << "成功加载 " << recordCount << " 条记录" << endl;
        return recordCount > 0;
    }
    
    // 按电话号码查找
    void searchByPhone(const string& phone) {
        Record* record = phoneTable.search(phone);
        if (record != nullptr) {
            cout << "查找成功！" << endl;
            record->display();
        } else {
            cout << "未找到电话号码为 " << phone << " 的记录" << endl;
        }
    }
    
    // 按用户名查找
    void searchByName(const string& name) {
        Record* record = nameTable.search(name);
        if (record != nullptr) {
            cout << "查找成功！" << endl;
            record->display();
        } else {
            cout << "未找到用户名为 " << name << " 的记录" << endl;
        }
    }
    
    // 显示系统状态
    void displayStatus() {
        cout << "\n=== 系统状态 ===" << endl;
        phoneTable.displayStatus();
        cout << endl;
        nameTable.displayStatus();
        cout << "================" << endl;
    }
    
    // 手动添加记录
    void addRecord() {
        string phone, name, address;
        
        cout << "请输入电话号码: ";
        cin >> phone;
        cout << "请输入用户名: ";
        cin.ignore();
        getline(cin, name);
        cout << "请输入地址: ";
        getline(cin, address);
        
        Record record(phone, name, address);
        
        if (phoneTable.insert(record) && nameTable.insert(record)) {
            cout << "记录添加成功！" << endl;
        } else {
            cout << "记录添加失败！" << endl;
        }
    }
    
    // 检查文件是否存在
    bool checkFileExists(const string& filename) {
        ifstream file(filename);
        bool exists = file.is_open();
        file.close();
        return exists;
    }
};

// 创建测试数据文件 - 改进版本
void createTestDataFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "错误：无法创建测试数据文件 '" << filename << "'" << endl;
        return;
    }
    
    // 测试数据 - 每3行一组
    vector<string> testData = {
        "13800138000", "张三", "北京市海淀区中关村大街1号",
        "13900139000", "李四", "上海市浦东新区陆家嘴金融中心",
        "13600136000", "王五", "广州市天河区珠江新城",
        "13700137000", "赵六", "深圳市南山区科技园",
        "13500135000", "钱七", "杭州市西湖区文三路",
        "15800158000", "孙八", "南京市鼓楼区中山路",
        "15900159000", "周九", "成都市武侯区天府大道",
        "15000150000", "吴十", "武汉市武昌区东湖路",
        "15100151000", "郑十一", "西安市雁塔区长安路",
        "15200152000", "王十二", "重庆市渝中区解放碑"
    };
    
    for (size_t i = 0; i < testData.size(); i++) {
        file << testData[i] << endl;
        // 每组数据后加空行分隔（可选）
        if ((i + 1) % 3 == 0 && i != testData.size() - 1) {
            file << endl;
        }
    }
    
    file.close();
    cout << "测试数据文件已创建: " << filename << endl;
    cout << "文件格式：每3行一组数据（电话、姓名、地址）" << endl;
}

// 创建CSV格式测试文件（备用）
void createTestCSVFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "错误：无法创建CSV测试文件 '" << filename << "'" << endl;
        return;
    }
    
    // CSV标题
    file << "Phone,Name,Address" << endl;
    
    // 测试数据
    vector<Record> testRecords = {
        {"13800138000", "张三", "北京市海淀区中关村大街1号"},
        {"13900139000", "李四", "上海市浦东新区陆家嘴金融中心"},
        {"13600136000", "王五", "广州市天河区珠江新城"},
        {"13700137000", "赵六", "深圳市南山区科技园"},
        {"13500135000", "钱七", "杭州市西湖区文三路"},
        {"15800158000", "孙八", "南京市鼓楼区中山路"},
        {"15900159000", "周九", "成都市武侯区天府大道"},
        {"15000150000", "吴十", "武汉市武昌区东湖路"},
        {"15100151000", "郑十一", "西安市雁塔区长安路"},
        {"15200152000", "王十二", "重庆市渝中区解放碑"}
    };
    
    for (const auto& record : testRecords) {
        file << record.phoneNumber << ","
             << record.userName << ","
             << record.address << endl;
    }
    
    file.close();
    cout << "CSV测试数据文件已创建: " << filename << endl;
}

// 主函数
int main() {
    cout << "=== 哈希表设计与应用系统 ===" << endl;
    
    HashTableSystem system(50);  // 创建哈希表系统，容量50
    
    string filename = "data.txt";
    string csvFilename = "data.csv";
    
    // 检查文件是否存在
    if (!system.checkFileExists(filename)) {
        cout << "数据文件 '" << filename << "' 不存在，正在创建测试数据..." << endl;
        createTestDataFile(filename);
    }
    
    // 从文件加载数据
    if (!system.loadFromFile(filename)) {
        cout << "尝试使用CSV格式..." << endl;
        if (!system.checkFileExists(csvFilename)) {
            createTestCSVFile(csvFilename);
        }
        system.loadFromCSV(csvFilename);
    }
    
    int choice;
    string searchKey;
    
    while (true) {
        cout << "\n=== 菜单 ===" << endl;
        cout << "1. 按电话号码查找" << endl;
        cout << "2. 按用户名查找" << endl;
        cout << "3. 添加新记录" << endl;
        cout << "4. 显示系统状态" << endl;
        cout << "5. 重新加载数据" << endl;
        cout << "0. 退出" << endl;
        cout << "请选择操作: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "请输入要查找的电话号码: ";
                cin >> searchKey;
                system.searchByPhone(searchKey);
                break;
                
            case 2:
                cout << "请输入要查找的用户名: ";
                cin.ignore();
                getline(cin, searchKey);
                system.searchByName(searchKey);
                break;
                
            case 3:
                system.addRecord();
                break;
                
            case 4:
                system.displayStatus();
                break;
                
            case 5:
                if (system.loadFromFile(filename)) {
                    cout << "数据重新加载成功！" << endl;
                } else {
                    cout << "数据重新加载失败！" << endl;
                }
                break;
                
            case 0:
                cout << "程序结束，谢谢使用！" << endl;
                return 0;
                
            default:
                cout << "无效选择，请重新输入！" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
        }
    }
    
    return 0;
}

