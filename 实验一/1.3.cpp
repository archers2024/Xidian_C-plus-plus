#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <iomanip>
using namespace std;

// 哈夫曼树节点结构
struct HuffmanNode {
    char ch;           // 字符
    double weight;     // 权值
    string code;       // 哈夫曼编码
    HuffmanNode* left;
    HuffmanNode* right;
    
    HuffmanNode(char c, double w) : ch(c), weight(w), code(""), left(NULL), right(NULL) {}
    HuffmanNode(double w) : ch('\0'), weight(w), code(""), left(NULL), right(NULL) {}
};

// 用于优先队列的比较函数
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->weight > b->weight;
    }
};

class HuffmanSystem {
private:
    HuffmanNode* root;
    map<char, string> huffmanCode;  // 字符到编码的映射
    map<string, char> codeToChar;   // 编码到字符的映射
    
    // 释放哈夫曼树内存
    void freeTree(HuffmanNode* node) {
        if (node == NULL) return;
        freeTree(node->left);
        freeTree(node->right);
        delete node;
    }
    
    // 生成哈夫曼编码
    void generateCodes(HuffmanNode* node, string code) {
        if (node == NULL) return;
        
        if (node->ch != '\0') {
            node->code = code;
            huffmanCode[node->ch] = code;
            codeToChar[code] = node->ch;
        }
        
        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
    }
    
public:
    HuffmanSystem() : root(NULL) {}
    
    ~HuffmanSystem() {
        freeTree(root);
    }
    
    // 初始化：建立哈夫曼树
    void initialization() {
        // 释放之前的树
        freeTree(root);
        root = NULL;
        huffmanCode.clear();
        codeToChar.clear();
        
        int n;
        cout << "请输入字符集大小 n: ";
        cin >> n;
        
        vector<char> chars(n);
        vector<double> weights(n);
        
        cout << "请输入 " << n << " 个字符: ";
        for (int i = 0; i < n; i++) {
            cin >> chars[i];
        }
        
        cout << "请输入 " << n << " 个权值: ";
        for (int i = 0; i < n; i++) {
            cin >> weights[i];
        }
        
        buildHuffmanTree(chars, weights);
        generateCodes(root, "");
        saveTreeToFile();
        
        cout << "哈夫曼树构建完成并已保存到 hfmTree 文件中" << endl;
        displayCodes();
    }
    
    // 建立哈夫曼树
    void buildHuffmanTree(const vector<char>& chars, const vector<double>& weights) {
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;
        
        // 创建叶子节点并加入优先队列
        for (int i = 0; i < chars.size(); i++) {
            pq.push(new HuffmanNode(chars[i], weights[i]));
        }
        
        // 构建哈夫曼树
        while (pq.size() > 1) {
            HuffmanNode* left = pq.top(); pq.pop();
            HuffmanNode* right = pq.top(); pq.pop();
            
            HuffmanNode* parent = new HuffmanNode(left->weight + right->weight);
            parent->left = left;
            parent->right = right;
            
            pq.push(parent);
        }
        
        if (!pq.empty()) {
            root = pq.top();
        }
    }
    
    // 保存哈夫曼树到文件
    void saveTreeToFile() {
        ofstream outFile("hfmTree");
        if (!outFile) {
            cerr << "无法打开文件 hfmTree 进行写入" << endl;
            return;
        }
        
        outFile << huffmanCode.size() << endl;
        for (map<char, string>::iterator it = huffmanCode.begin(); it != huffmanCode.end(); it++) {
            outFile << it->first << " " << it->second << endl;
        }
        outFile.close();
    }
    
    // 从文件加载哈夫曼树
    void loadTreeFromFile() {
        ifstream inFile("hfmTree");
        if (!inFile) {
            cerr << "无法打开文件 hfmTree，请先初始化" << endl;
            return;
        }
        
        huffmanCode.clear();
        codeToChar.clear();
        
        int n;
        inFile >> n;
        inFile.ignore(); // 忽略换行符
        
        for (int i = 0; i < n; i++) {
            char ch;
            string code;
            inFile >> ch >> code;
            huffmanCode[ch] = code;
            codeToChar[code] = ch;
        }
        inFile.close();
        
        cout << "哈夫曼树加载成功" << endl;
        displayCodes();
    }
    
    // 编码功能
    void encoding() {
        loadTreeFromFile();
        if (huffmanCode.empty()) {
            cout << "请先初始化哈夫曼树" << endl;
            return;
        }
        
        // 创建测试文件或读取现有文件
        createTestFile();
        
        // 读取待编码文本
        ifstream inFile("ToBeTran");
        if (!inFile) {
            cerr << "无法打开文件 ToBeTran" << endl;
            return;
        }
        
        string text;
        string line;
        while (getline(inFile, line)) {
            text += line;
            if (!inFile.eof()) {
                text += "\n";
            }
        }
        inFile.close();
        
        if (text.empty()) {
            cout << "文件 ToBeTran 为空" << endl;
            return;
        }
        
        // 进行编码
        string encodedText;
        for (int i = 0; i < text.length(); i++) {
            char ch = text[i];
            if (huffmanCode.find(ch) != huffmanCode.end()) {
                encodedText += huffmanCode[ch];
            } else {
                cout << "警告: 字符 '" << ch << "' 没有对应的编码" << endl;
            }
        }
        
        // 保存编码结果
        ofstream outFile("CodeFile");
        if (!outFile) {
            cerr << "无法打开文件 CodeFile 进行写入" << endl;
            return;
        }
        
        outFile << encodedText;
        outFile.close();
        
        cout << "编码完成，结果已保存到 CodeFile 中" << endl;
        cout << "原文长度: " << text.length() << " 字符" << endl;
        cout << "编码后长度: " << encodedText.length() << " 位" << endl;
        if (text.length() > 0) {
            double compressionRatio = (1.0 - (double)encodedText.length() / (text.length() * 8)) * 100;
            cout << "压缩率: " << fixed << setprecision(2) << compressionRatio << "%" << endl;
        }
    }
    
    // 译码功能
    void decoding() {
        loadTreeFromFile();
        if (codeToChar.empty()) {
            cout << "请先初始化哈夫曼树" << endl;
            return;
        }
        
        // 读取编码文件
        ifstream inFile("CodeFile");
        if (!inFile) {
            cerr << "无法打开文件 CodeFile" << endl;
            return;
        }
        
        string encodedText;
        getline(inFile, encodedText);
        inFile.close();
        
        if (encodedText.empty()) {
            cout << "文件 CodeFile 为空" << endl;
            return;
        }
        
        // 进行译码
        string decodedText;
        string currentCode;
        
        for (int i = 0; i < encodedText.length(); i++) {
            currentCode += encodedText[i];
            if (codeToChar.find(currentCode) != codeToChar.end()) {
                decodedText += codeToChar[currentCode];
                currentCode.clear();
            }
        }
        
        if (!currentCode.empty()) {
            cout << "警告: 存在无法译码的剩余位: " << currentCode << endl;
        }
        
        // 保存译码结果
        ofstream outFile("TextFile");
        if (!outFile) {
            cerr << "无法打开文件 TextFile 进行写入" << endl;
            return;
        }
        
        outFile << decodedText;
        outFile.close();
        
        cout << "译码完成，结果已保存到 TextFile 中" << endl;
    }
    
    // 验证文件内容
    void verifyFiles() {
        ifstream file1("ToBeTran");
        ifstream file2("TextFile");
        
        if (!file1 || !file2) {
            cerr << "无法打开待比较的文件" << endl;
            return;
        }
        
        string content1, content2;
        string line;
        
        while (getline(file1, line)) {
            content1 += line;
            if (!file1.eof()) {
                content1 += "\n";
            }
        }
        while (getline(file2, line)) {
            content2 += line;
            if (!file2.eof()) {
                content2 += "\n";
            }
        }
        
        file1.close();
        file2.close();
        
        // 移除最后一个多余的换行符
        if (!content1.empty() && content1[content1.length()-1] == '\n') {
            content1 = content1.substr(0, content1.length()-1);
        }
        if (!content2.empty() && content2[content2.length()-1] == '\n') {
            content2 = content2.substr(0, content2.length()-1);
        }
        
        if (content1 == content2) {
            cout << "? 验证成功: ToBeTran 和 TextFile 内容完全相同" << endl;
        } else {
            cout << "? 验证失败: 两个文件内容不同" << endl;
            cout << "ToBeTran 长度: " << content1.length() << endl;
            cout << "TextFile 长度: " << content2.length() << endl;
        }
    }
    
    // 显示编码表
    void displayCodes() {
        if (huffmanCode.empty()) {
            cout << "编码表为空，请先初始化" << endl;
            return;
        }
        
        cout << "\n=== 哈夫曼编码表 ===" << endl;
        cout << left << setw(10) << "字符" << setw(15) << "编码" << endl;
        cout << string(25, '-') << endl;
        
        for (map<char, string>::iterator it = huffmanCode.begin(); it != huffmanCode.end(); it++) {
            if (it->first == '\n') {
                cout << left << setw(10) << "\\n" << setw(15) << it->second << endl;
            } else if (it->first == ' ') {
                cout << left << setw(10) << "空格" << setw(15) << it->second << endl;
            } else {
                cout << left << setw(10) << it->first << setw(15) << it->second << endl;
            }
        }
        cout << endl;
    }
    
    // 创建测试文件
    void createTestFile() {
        ofstream outFile("ToBeTran");
        if (!outFile) {
            cerr << "无法创建测试文件" << endl;
            return;
        }
        
        cout << "请输入测试文本: ";
        cin.ignore(); // 清除输入缓冲区
        string text;
        getline(cin, text);
        
        outFile << text;
        outFile.close();
        
        cout << "测试文件 ToBeTran 创建成功" << endl;
    }
};

// 显示菜单
void displayMenu() {
    cout << "\n=== 哈夫曼编/译码系统 ===" << endl;
    cout << "I. 初始化哈夫曼树" << endl;
    cout << "E. 编码" << endl;
    cout << "D. 译码" << endl;
    cout << "V. 验证文件" << endl;
    cout << "T. 创建测试文件" << endl;
    cout << "S. 显示编码表" << endl;
    cout << "Q. 退出" << endl;
    cout << "请选择操作: ";
}

int main() {
    HuffmanSystem huffmanSystem;
    char choice;
    
    cout << "哈夫曼编/译码系统" << endl;
    
    while (true) {
        displayMenu();
        cin >> choice;
        choice = toupper(choice);
        
        switch (choice) {
            case 'I':
                huffmanSystem.initialization();
                break;
            case 'E':
                huffmanSystem.encoding();
                break;
            case 'D':
                huffmanSystem.decoding();
                break;
            case 'V':
                huffmanSystem.verifyFiles();
                break;
            case 'T':
                huffmanSystem.createTestFile();
                break;
            case 'S':
                huffmanSystem.displayCodes();
                break;
            case 'Q':
                cout << "感谢使用，再见！" << endl;
                return 0;
            default:
                cout << "无效选择，请重新输入" << endl;
        }
    }
    
    return 0;
}

