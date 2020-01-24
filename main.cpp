//
//  main.cpp
//  Huffman Encoder-Decoder System
//
//  Created by Starry Night on 2019/11/16.
//  Copyright © 2019 Starry Night. All rights reserved.
//

#include "infrastructure.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
using namespace std;

//比较函数，用于快速排序
bool compare(TreeNode *node1, TreeNode *node2) {
  return node1->val > node2->val;
}

// 8位二进制数转换为十进制数
int Bin2Dec(string binary) {
  int decimal = 0;
  for (int i = 0; i < 8; i++)
    decimal += ((binary[i] == '1') ? pow(2, 7 - i) : 0);
  return decimal;
}

string Dec2Bin(int decimal) {
  string binary;
  for (int i = 0; i < 8; i++) {
    if (decimal < pow(2, 7 - i))
      binary += "0";
    else {
      binary += "1";
      decimal -= pow(2, 7 - i);
    }
  }
  return binary;
}

TreeNode::TreeNode(int ch, int val) {
  this->ch = ch;
  this->val = val;
}

void TreeNode::PrintInfo() {
  cout << "Character: " << (char)this->ch << " Value: " << this->val
       << " Code: " << this->code << endl;
}

TreeStack::TreeStack(int len) {
  this->len = len;
  this->trees =
      new TreeNode *[this->len]; //创建能够包含26个字母和空格键的动态数组
}

TreeNode *TreeStack::Pop() {
  if (top != -1) {
    return this->trees[top];
  } else {
    cout << "Error: poping from empty stack !" << endl;
    system("pause");
    exit(0);
  }
}

void TreeStack::Push(TreeNode *newNode) {
  if (top == len - 1) {
    cout << "Error: push into full stack !" << endl;
    system("pause");
    exit(0);
  }
  TreeNode *temp = new TreeNode;
  *temp = *newNode;
  this->trees[++top] = temp;
}

void TreeStack::Remove(int num) {
  if (this->top == -1) {
    cout << "Error: removing top element of empty stack !" << endl;
    system("pause");
    exit(0);
  }
  for (int i = 0; i < num; i++) {
    this->trees[top--] = new TreeNode;
  }
}

//使用快速排序对栈中元素按权重降序排序
void TreeStack::Sort() {
  if (this->top == -1) {
    cout << "Error: sorting empty stack !" << endl;
    system("pause");
    exit(0);
  }
  sort(this->trees, (this->trees) + top + 1, compare);
}

/* 赫夫曼树的操作函数 */
//从文件中读取待编码字符及权值
void initStack(TreeStack *stack) {
  ifstream inFile("hfmTree.txt", ios::in);
  if (!inFile) {
    cout << "Error: opening source file failed !" << endl;
    system("pause");
    exit(0);
  }
  TreeNode *temp = new TreeNode;
  while (inFile >> temp->ch >> temp->val) {
    stack->Push(temp);
  }
  stack->Sort();
  inFile.close();
}

//提取栈顶（权值最小）的两个结点进行组合
void combineNodes(TreeStack *stack) {
  TreeNode *parent = new TreeNode(-1, -1); //创建非叶子结点，用于连接子结点
  parent->right = stack->Pop();
  stack->Remove(1);
  parent->left = stack->Pop();
  stack->Remove(1);
  parent->val = parent->left->val + parent->right->val;
  stack->Push(parent);
  stack->Sort();
}

//根据字符及权值创建赫夫曼树
TreeNode *createTree(TreeStack *stack) {
  while (stack->top > 0)
    combineNodes(stack);
  return stack->trees[0];
}

//递归遍历二叉树所有元素，显示信息
void Traversal(TreeNode *root) {
  if (root != nullptr) {
    if (root->ch != -1) {
      root->PrintInfo();
    }
    Traversal(root->left);
    Traversal(root->right);
  }
}

//递归遍历二叉树所有元素，为输入元素编码
void Traversal(TreeNode *root, int ch, string *code) {
  if (root != nullptr) {
    if (root->ch != -1) {
      if (root->ch == ch) {
        *code = root->code;
      }
    }
    Traversal(root->left, ch, code);
    Traversal(root->right, ch, code);
  }
}

//为编码赋值
void traversalCode(TreeNode *root) {
  if (root != nullptr) {
    if (root->left != nullptr && !root->left->isCoded) {
      root->left->code += (root->code + "0");
    }
    traversalCode(root->left);
    if (root->right != nullptr && !root->right->isCoded) {
      root->right->code += (root->code + "1");
    }
    traversalCode(root->right);
  }
}

//初始化赫夫曼编码
void initCode(TreeNode *root) {
  string code;
  root->code = code;
  traversalCode(root);
}

/*编码操作*/
string Encode(TreeNode *root, string text) {
  string encoded;
  string addition;
  long int len = text.length();
  for (int i = 0; i < len; i++) {
    Traversal(root, (int)text[i], &addition);
    encoded += addition;
  }
  return encoded;
}

string Decode(TreeNode *root, string codestr) {
  TreeNode *find;
  string decoded;
  long int len = codestr.length();
  int index = 0;
  for (; index < len;) {
    find = root;
    while (true) {
      if (codestr[index] == '0')
        find = find->left;
      else if (codestr[index] == '1')
        find = find->right;
      index++;
      if (find->ch != -1) {
        decoded += (char)(find->ch);
        break;
      }
    }
  }
  return decoded;
}

/*保存编码和译码结果*/
void saveEncoded(string encoded) {
  ofstream outFile("CodeFile.txt", ios::out);
  outFile << encoded << endl;
  outFile.close();
}

void saveDecoded(string decoded) {
  ofstream outFile("TextFile.txt", ios::out);
  outFile << decoded << endl;
  outFile.close();
}

void saveBinary(string binary) {
  //计算序列长度并添加符号指示位
  unsigned long length = binary.length();
  string sign;
  for (int i = 0; i < 8; i++) {
    if (length / pow(2, 7 - i) < 1)
      sign += "0";
    else {
      sign += "1";
      length -= pow(2, 7 - i);
    }
  }
  binary = sign + binary;

  //补全不足8位的数字
  int count = binary.length() % 8;
  for (int i = 0; i < 8 - count; i++)
    binary += "0";

  //重新将序列转换为十进制ASCII码
  length = binary.length();
  unsigned long size = length / 8;
  int *numbers = new int[size];
  numbers[0] = Bin2Dec(binary.substr(0, 8));
  for (int i = 1; i < size; i++)
    numbers[i] = Bin2Dec(binary.substr(8 * i, 8 * i + 7));

  //将新的ASCII码写入文件
  ofstream outFile("BinaryFile.bin", ios::out | ios::binary);
  for (int i = 0; i < size; i++)
    outFile << (char)numbers[i];
  cout << "Binary-Format Text Saved to 'BinaryFile.txt'" << endl;
  cout << "Length of Binary-Format Text: " << size << endl;
  outFile.close();
}

string readBinary() {
  ifstream inFile("BinaryFile.bin", ios::in | ios::binary);

  //读取原二进制码长度
  int length;
  string text;
  char temp;
  inFile >> text;
  temp = text[0];
  length = (int)temp; //原二进制码长度

  //将文本字符转换为二进制码
  string binary;
  for (int i = 1; i < text.length(); i++)
    binary += Dec2Bin((int)text[i]);

  //去除末尾多余的0
  for (int i = 0; i < text.length() * 8 - length - 8; i++)
    binary.pop_back();

  return binary;
}

int main() {
  cout << "Huffman Encoder-Decoder System V1.0" << endl;
  cout << "Copyright © 2019 Starry Night. All rights reserved." << endl << endl;
  TreeStack *stack = new TreeStack(27);
  cout << "initializing stack..." << endl;
  initStack(stack);
  cout << "stack initialized !" << endl;
  cout << "creating Huffman Tree..." << endl;
  TreeNode *root = createTree(stack);
  cout << "Huffman Tree Created !" << endl;
  cout << "initializing Huffman Code..." << endl;
  initCode(root);
  cout << "Huffman Code Initialized !" << endl << endl;
  char sel;
  string encoded;
  string decoded;
Sel1:
  cout << "Show Huffman Code ? (Y/n)" << endl;
  cin >> sel;
  cout << endl;
  switch (sel) {
  case 'Y':
    Traversal(root);
    cout << endl;
    break;
  case 'n':
    break;
  default:
    goto Sel1;
    break;
  }
  string text = "THIS PROGRAM IS MY FAVORITE";
  cout << "Text to be Encoded: " << text << endl;
  cout << "Text Length before encoding: " << text.length() << endl;
Sel2:
  cout << "Start Encoding ? (Y)" << endl;
  cin >> sel;
  switch (sel) {
  case 'Y':
    encoded = Encode(root, text);
    break;
  default:
    goto Sel2;
    break;
  }
  cout << "Encoding Finished !" << endl << "Encoded Text: " << encoded << endl;
  saveEncoded(encoded);
  cout << "Encoded Text Saved to 'CodeFile.txt'" << endl << endl;
Sel3:
  cout << "Save Binary-Format Text ? (Y/n)" << endl;
  cin >> sel;
  switch (sel) {
  case 'Y':
    saveBinary(encoded);
    break;
  case 'n':
    break;
  default:
    goto Sel3;
    break;
  }
Sel4:
  cout << endl << "Start Decoding ? (Y/n)" << endl;
  cin >> sel;
  switch (sel) {
  case 'Y':
    decoded = Decode(root, encoded);
    break;
  case 'n':
    break;
  default:
    goto Sel4;
    break;
  }
  cout << "Decoding Finished !" << endl << "Decoded Text: " << decoded << endl;
  saveDecoded(decoded);
  cout << "Decoded Text Saved to 'TextFile.txt'" << endl << endl;

  return 0;
}
