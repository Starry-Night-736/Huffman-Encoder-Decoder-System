//
//  infrastructure.h
//  Huffman Encoder-Decoder System
//
//  Created by Starry Night on 2019/11/16.
//  Copyright © 2019 Starry Night. All rights reserved.
//
#include <iostream>
#ifndef infrastructure_h
#define infrastructure_h

//赫夫曼树结点
class TreeNode {
public:
  int ch;                    //结点对应的字符，以ASCII码储存
  int val;                   //结点对应的权值
  std::string code;          //结点对应的编码
  bool isCoded = false;      //是否已被编码，用于遍历初始化编码
  TreeNode *left = nullptr;  //左侧结点，表示长子
  TreeNode *right = nullptr; //右侧结点，表示右兄弟
  TreeNode(){};
  TreeNode(int ch, int val);
  void PrintInfo();
};

//赫夫曼树结点栈，用于赫夫曼树的建立
class TreeStack {
public:
  TreeNode **trees; //结点栈
  int top = -1;
  int len;
  TreeStack(int len);
  TreeNode *Pop();
  void Push(TreeNode *newTree);
  void Remove(int num);
  void Sort();
};

#endif /* infrastructure_h */
