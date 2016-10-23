
#pragma once

#include "stdafx.h"
#include "Heap.h"
#include <string>
using namespace std;

class HuffmanTreeNode
{
public:
	unsigned char data;
	bool tag; //是否为叶节点，Huffman树中存非叶节点的为false，其他未true
	int weight;
	HuffmanTreeNode *leftChild, *rightChild, *parent;
	HuffmanTreeNode(bool Tag = false);
	HuffmanTreeNode(int w, HuffmanTreeNode *left = NULL, HuffmanTreeNode *right = NULL, HuffmanTreeNode *pr = NULL, bool Tag = false);
	bool operator <= (HuffmanTreeNode& j);
	bool operator >= (HuffmanTreeNode& j);
	bool operator > (HuffmanTreeNode& j);
	bool operator < (HuffmanTreeNode& j);
};

class HuffmanTree
{
public:
	HuffmanTree(int w[], int n);
	HuffmanTree();
	~HuffmanTree();
	HuffmanTreeNode* GetRoot();
	void Clear();
	void CreatHuffmanTree(int w[], int n);
	void Find(unsigned char& item, string& result, int& value);
	HuffmanTreeNode* Parent(HuffmanTreeNode *current);
protected:
	HuffmanTreeNode* Parent(HuffmanTreeNode* subTree, HuffmanTreeNode* current);
	HuffmanTreeNode* Find(HuffmanTreeNode* subTree, unsigned char &x);
	HuffmanTreeNode* root;
	void Destroy(HuffmanTreeNode* subTree);
};

HuffmanTreeNode* HuffmanTree::GetRoot()
{
	return root;
}

void HuffmanTree::Find(unsigned char& item, string& result, int& value)		//HUFFMAN树的左侧为1 右侧0 函数输出item节点的编码resut
{
	result = "";
	value = 0;
	HuffmanTreeNode* current = Find(root, item);
	for (int i = 0;; i++)
	{
		HuffmanTreeNode* parent = NULL;
		parent = Parent(current);
		if (parent == NULL)
			break;
		if (parent->leftChild == current)
		{
			result.insert(0, "1");
			value += pow(2, i);
		}
		else if (parent->rightChild == current)
		{
			result.insert(0, "0");
		}
		current = parent;
	}
}

HuffmanTreeNode* HuffmanTree::Parent(HuffmanTreeNode *current)
{
	return(root == NULL || root == current) ? NULL : Parent(root, current);
}

HuffmanTreeNode* HuffmanTree::Parent(HuffmanTreeNode* subTree, HuffmanTreeNode* current)
{
	if (subTree == NULL)
		return NULL;
	if (subTree->leftChild == current || subTree->rightChild == current)
		return subTree;
	HuffmanTreeNode* temp;
	temp = Parent(subTree->leftChild, current);
	if (temp != NULL)
		return temp;
	else
		return Parent(subTree->rightChild, current);
}

HuffmanTreeNode* HuffmanTree::Find(HuffmanTreeNode* subTree, unsigned char &x)
{
	if (subTree == NULL)
		return NULL;
	else if (subTree->data == x&&subTree->tag)	//只有当节点为叶节点的时候才返回
		return subTree;
	else if (subTree->rightChild == NULL&&subTree->rightChild == NULL)
		return NULL;

	HuffmanTreeNode* ptr = NULL;

	ptr = Find(subTree->leftChild, x);
	if (ptr == NULL)
	{
		ptr = Find(subTree->rightChild, x);
		return ptr;
	}
	else
		return ptr;
}

void HuffmanTree::CreatHuffmanTree(int w[], int n)
{
	MinHeap<HuffmanTreeNode> heap;
	HuffmanTreeNode *LastParents = NULL;
	int sum = 0;
	for (int i = 0; i < n; i++)
	{
		if (w[i] != 0)
		{
			HuffmanTreeNode* temp = new HuffmanTreeNode(true);	//建立叶节点
			temp->weight = w[i];
			temp->data = (unsigned char)i;
			temp->leftChild = NULL;
			temp->rightChild = NULL;
			temp->parent = NULL;
			heap.Insert(*temp);
			sum++;
		}
	}
	for (int i = 0; i < sum - 1; i++)
	{
		HuffmanTreeNode* least = new HuffmanTreeNode(true);//建立叶节点
		HuffmanTreeNode* less = new HuffmanTreeNode(true);//建立叶节点
		heap.Remove(*least);
		heap.Remove(*less);

		HuffmanTreeNode *parent = new HuffmanTreeNode(false);	//建立非叶节点
		parent->leftChild = least;
		parent->rightChild = less;

		parent->weight = parent->leftChild->weight + parent->rightChild->weight;

		parent->leftChild->parent = parent;
		parent->rightChild->parent = parent;
		heap.Insert(*parent);
		if (i == sum - 2)
			LastParents = parent;
	}
	root = LastParents;
}

HuffmanTree::HuffmanTree()
{
	root = NULL;
}

HuffmanTree::HuffmanTree(int w[], int n)
{
	MinHeap<HuffmanTreeNode> heap;
	HuffmanTreeNode *LastParents = NULL;
	int sum = 0;
	for (int i = 0; i < n; i++)
	{
		if (w[i] != 0)
		{
			HuffmanTreeNode* temp = new HuffmanTreeNode(true);
			temp->weight = w[i];
			temp->data = (unsigned char)i;
			temp->leftChild = NULL;
			temp->rightChild = NULL;
			temp->parent = NULL;
			heap.Insert(*temp);
			sum++;
		}
	}
	for (int i = 0; i < sum - 1; i++)
	{
		HuffmanTreeNode* least = new HuffmanTreeNode(true);//建立叶节点
		HuffmanTreeNode* less = new HuffmanTreeNode(true);//建立叶节点
		heap.Remove(*least);
		heap.Remove(*less);

		HuffmanTreeNode *parent = new HuffmanTreeNode(false);	//建立非叶节点
		parent->leftChild = least;
		parent->rightChild = less;

		parent->weight = parent->leftChild->weight + parent->rightChild->weight;

		parent->leftChild->parent = parent;
		parent->rightChild->parent = parent;
		heap.Insert(*parent);
		if (i == sum - 2)
			LastParents = parent;
	}
	root = LastParents;
}

void HuffmanTree::Destroy(HuffmanTreeNode* subTree)
{
	if (subTree != NULL)
	{
		Destroy(subTree->leftChild);
		Destroy(subTree->rightChild);
		delete subTree;
	}
}

void HuffmanTree::Clear()
{
	Destroy(root);
	root = NULL;
}

HuffmanTree::~HuffmanTree()
{
	Destroy(root);
}

HuffmanTreeNode::HuffmanTreeNode(bool Tag)
{
	tag = Tag;
	leftChild = NULL;
	rightChild = NULL;
	parent = NULL;
	data = NULL;
}

HuffmanTreeNode::HuffmanTreeNode(int w, HuffmanTreeNode *left, HuffmanTreeNode *right, HuffmanTreeNode *pr, bool Tag)
{
	tag = Tag;
	weight = w;
	leftChild = left;
	rightChild = right;
	parent = pr;
}

bool HuffmanTreeNode::operator <= (HuffmanTreeNode& j)
{
	return weight <= j.weight;
}

bool HuffmanTreeNode::operator >= (HuffmanTreeNode& j)
{
	return weight >= j.weight;
}

bool HuffmanTreeNode::operator > (HuffmanTreeNode& j)
{
	return weight > j.weight;
}

bool HuffmanTreeNode::operator < (HuffmanTreeNode& j)
{
	return weight < j.weight;
}
