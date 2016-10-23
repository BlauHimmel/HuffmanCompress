
#pragma once

#include "stdafx.h"
#include "HuffmanTree.h"
#include <fstream>
#include <cstring>
using namespace std;

class Compression
{
public:
	Compression();
	bool FileCompress(ifstream &IN_FILE, ofstream &OUT_FILE);
	bool FileDecompress(ifstream &IN_FILE, ofstream &OUT_FILE);

private:
	void GetFrequency(ifstream &IN_FILE);
	int frequency[256];
	HuffmanTree CompressionHuffmanTree;
};


bool Compression::FileDecompress(ifstream &IN_FILE, ofstream &OUT_FILE)		//文件解压
{
	int weight[256];
	IN_FILE.read((char*)&weight, 256 * sizeof(int));
	CompressionHuffmanTree.CreatHuffmanTree(weight, 256);	//读取频数信息并构建Huffman树

	int FileSize;
	IN_FILE.read((char*)&FileSize, sizeof(FileSize));

	unsigned int buffer = 0;
	HuffmanTreeNode* current = CompressionHuffmanTree.GetRoot();
	for (int i = 0; i < FileSize;)						//HUFFMAN树的左侧为1 右侧0	i记录已写入的字节数
	{
		IN_FILE.read((char*)&buffer, sizeof(unsigned int));		//一次读入32位

		/////////////////////////////////////////////////////
		TAG = false;
		CUR = ((double)i*1.0) / (double)FileSize*100.0;
		TAG = true;
		/////////////////////////////////////////////////////

		for (int p = 0; p < 32 && i < FileSize; p++)	//解码一个32位数据
		{
			unsigned int temp = buffer;
			temp <<= p;
			temp >>= 31;
			if (temp == 1)	//左
			{
				current = current->leftChild;
				if (current->rightChild == NULL&&current->leftChild == NULL)	//遇到了子叶节点	则current指针归位并将节点的data写入文件中
				{
					unsigned char write_temp = current->data;
					OUT_FILE.write((char*)&write_temp, sizeof(unsigned char));
					current = CompressionHuffmanTree.GetRoot();
					i++;		//写入的字节数加一
					continue;
				}
			}
			else if (temp == 0)	//右
			{
				current = current->rightChild;
				if (current->rightChild == NULL&&current->leftChild == NULL)	//遇到了子叶节点	则current指针归位并将节点的data写入文件中
				{
					unsigned char write_temp = current->data;
					OUT_FILE.write((char*)&write_temp, sizeof(unsigned char));
					current = CompressionHuffmanTree.GetRoot();
					i++;		//写入的字节数加一
					continue;
				}
			}
		}
	}
	CUR = 100.0;
	IN_FILE.close();
	OUT_FILE.close();
	CompressionHuffmanTree.Clear();
	return true;
}

bool Compression::FileCompress(ifstream &IN_FILE, ofstream &OUT_FILE)	//文件压缩	按 文件名长度-文件名-字符频数数组-文件长度-压缩内容写入文件
{

	GetFrequency(IN_FILE);
	IN_FILE.clear();
	IN_FILE.seekg(0, ios::end);
	int FileSize = IN_FILE.tellg();
	IN_FILE.seekg(0, ios::beg);

	CompressionHuffmanTree.CreatHuffmanTree(frequency, 256);	//构建Huffman树

	unsigned char read_buffer[1024];
	unsigned int temp = 0;	//存储临时产生的位码,已备写入文件中
	unsigned int buffer = 0;	//处理多余的位数
	int writtenBit = 0;	//记录已写的位数
	int leftBit = 0;	//上一块中多余的位的部分的位数
	OUT_FILE.write((char*)&frequency, 256 * sizeof(int));		//写入个字符的频数(以供解压时建立Huffman树)
	OUT_FILE.write((char*)&FileSize, sizeof(FileSize));			//写入文件长度

	///////////////////////////////////////////////////////////////////
	struct Info
	{
		string code;
		int value;
	};
	Info info[256];
	for (int i = 0; i < 256; i++)
	{
		unsigned char temp = i;
		CompressionHuffmanTree.Find(temp, info[i].code, info[i].value);
	}
	///////////////////////////////////////////////////////////////////

	while (!IN_FILE.eof())
	{
		int CurrentPos = IN_FILE.tellg();
		IN_FILE.seekg(0, ios::end);
		int EndPos = IN_FILE.tellg();
		IN_FILE.seekg(CurrentPos, ios::beg);
		int FileLeft = EndPos - CurrentPos;		//获得剩余文件的大小（byte）

		/////////////////////////////////////////////////////
		TAG = false;
		CUR = ((double)FileSize - (double)FileLeft) / (double)FileSize*100.0;
		TAG = true;
		/////////////////////////////////////////////////////

		int read_length;
		bool isEnd = false;	//标记是否读到文件的最后一部分

		if (FileLeft > 1024)
			read_length = 1024;
		else
		{
			read_length = FileLeft;
			isEnd = true;
		}


		IN_FILE.read((char*)read_buffer, 1024 * sizeof(unsigned char));
		for (int i = 0; i < read_length; i++)		//处理读入的1KB数据
		{
			int value = 0;	//存储生成huffman编码的十进制数值
			string code = "";	//存储生成huffman编码的二进制数值	

			if (leftBit>0)//将上一块中多余部分补到这一块
			{
				temp = 0;
				temp <<= leftBit;
				temp |= buffer;
				writtenBit += leftBit;
				buffer = 0;
				leftBit = 0;
			}

			code = info[read_buffer[i]].code;
			value = info[read_buffer[i]].value;

			writtenBit += code.length();

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			if (isEnd&&i == read_length - 1)	//文件尾部数据
			{
				if (writtenBit <= 32)
				{
					temp <<= code.length();
					temp |= value;
					temp <<= 32 - writtenBit;
					OUT_FILE.write((char*)&temp, sizeof(temp));
					break;
				}
				else if (writtenBit > 32)
				{
					int inBit = code.length() - (writtenBit - 32);	//要写入当前块的位数
					leftBit = writtenBit - 32;						//要写入下一块的位数
					unsigned int temp_value = 0;					//存储要写入这一块的值
					for (int i = inBit, j = 0; i > 0; i--, j++)		//将二进制字符串分开并计算要写入这一块的值
					{
						char temp_char = code[i - 1];
						int temp_bit = temp_char - '0';
						temp_value += temp_bit* pow(2, j);
					}

					temp <<= inBit;
					temp |= temp_value;
					OUT_FILE.write((char*)&temp, sizeof(temp));

					buffer = 0;

					for (int i = code.length(), j = 0; i > inBit; i--, j++)		//buffer为多余的部分的值
					{
						char temp_char = code[i - 1];
						int temp_bit = temp_char - '0';
						buffer += temp_bit* pow(2, j);
					}

					temp = 0;
					temp <<= leftBit;
					temp |= buffer;
					temp <<= (32 - leftBit);

					OUT_FILE.write((char*)&temp, sizeof(temp));
					continue;
				}
			}

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			if (writtenBit <= 32)		//一般情况下
			{
				temp <<= code.length();
				temp |= value;
				if (writtenBit == 32)	//已经读满32位，写入文件
				{
					OUT_FILE.write((char*)&temp, sizeof(temp));
					writtenBit = 0;					//将已写入位数置0，并将32位写入文件
					temp = 0;
				}
			}

			else   //if(writtenBit>32)	//处理承上启下的中间部分
			{
				int inBit = code.length() - (writtenBit - 32);	//要写入当前块的位数
				leftBit = writtenBit - 32;						//要写入下一块的位数
				unsigned int temp_value = 0;					//存储要写入这一块的值
				for (int i = inBit, j = 0; i > 0; i--, j++)		//将二进制字符串分开并计算要写入这一块的值
				{
					char temp = code[i - 1];
					int temp_bit = temp - '0';
					temp_value += temp_bit* pow(2, j);
				}

				temp <<= inBit;
				temp |= temp_value;

				for (int i = code.length(), j = 0; i > inBit; i--, j++)		//buffer为多余的部分的值
				{
					char temp_char = code[i - 1];
					int temp_bit = temp_char - '0';
					buffer += temp_bit* pow(2, j);
				}

				writtenBit = 0;					//将已写入位数置0，并将32位写入文件
				OUT_FILE.write((char*)&temp, sizeof(temp));
				temp = 0;
			}
		}
	}
	CUR = 100.0;
	IN_FILE.close();
	OUT_FILE.close();
	CompressionHuffmanTree.Clear();
	return true;
}

Compression::Compression()
{
	for (int i = 0; i < 256; i++)
	{
		frequency[i] = 0;
	}
	CompressionHuffmanTree.CreatHuffmanTree(frequency, 256);
}

void Compression::GetFrequency(ifstream &IN_FILE)
{
	while (!IN_FILE.eof())
	{
		unsigned char buffer[1024];
		IN_FILE.read((char*)buffer, 1024 * sizeof(unsigned char));
		for (int i = 0; i < 1024; i++)
		{
			frequency[buffer[i]]++;
		}
	}
}

