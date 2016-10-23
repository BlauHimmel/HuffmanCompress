
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


bool Compression::FileDecompress(ifstream &IN_FILE, ofstream &OUT_FILE)		//�ļ���ѹ
{
	int weight[256];
	IN_FILE.read((char*)&weight, 256 * sizeof(int));
	CompressionHuffmanTree.CreatHuffmanTree(weight, 256);	//��ȡƵ����Ϣ������Huffman��

	int FileSize;
	IN_FILE.read((char*)&FileSize, sizeof(FileSize));

	unsigned int buffer = 0;
	HuffmanTreeNode* current = CompressionHuffmanTree.GetRoot();
	for (int i = 0; i < FileSize;)						//HUFFMAN�������Ϊ1 �Ҳ�0	i��¼��д����ֽ���
	{
		IN_FILE.read((char*)&buffer, sizeof(unsigned int));		//һ�ζ���32λ

		/////////////////////////////////////////////////////
		TAG = false;
		CUR = ((double)i*1.0) / (double)FileSize*100.0;
		TAG = true;
		/////////////////////////////////////////////////////

		for (int p = 0; p < 32 && i < FileSize; p++)	//����һ��32λ����
		{
			unsigned int temp = buffer;
			temp <<= p;
			temp >>= 31;
			if (temp == 1)	//��
			{
				current = current->leftChild;
				if (current->rightChild == NULL&&current->leftChild == NULL)	//��������Ҷ�ڵ�	��currentָ���λ�����ڵ��dataд���ļ���
				{
					unsigned char write_temp = current->data;
					OUT_FILE.write((char*)&write_temp, sizeof(unsigned char));
					current = CompressionHuffmanTree.GetRoot();
					i++;		//д����ֽ�����һ
					continue;
				}
			}
			else if (temp == 0)	//��
			{
				current = current->rightChild;
				if (current->rightChild == NULL&&current->leftChild == NULL)	//��������Ҷ�ڵ�	��currentָ���λ�����ڵ��dataд���ļ���
				{
					unsigned char write_temp = current->data;
					OUT_FILE.write((char*)&write_temp, sizeof(unsigned char));
					current = CompressionHuffmanTree.GetRoot();
					i++;		//д����ֽ�����һ
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

bool Compression::FileCompress(ifstream &IN_FILE, ofstream &OUT_FILE)	//�ļ�ѹ��	�� �ļ�������-�ļ���-�ַ�Ƶ������-�ļ�����-ѹ������д���ļ�
{

	GetFrequency(IN_FILE);
	IN_FILE.clear();
	IN_FILE.seekg(0, ios::end);
	int FileSize = IN_FILE.tellg();
	IN_FILE.seekg(0, ios::beg);

	CompressionHuffmanTree.CreatHuffmanTree(frequency, 256);	//����Huffman��

	unsigned char read_buffer[1024];
	unsigned int temp = 0;	//�洢��ʱ������λ��,�ѱ�д���ļ���
	unsigned int buffer = 0;	//��������λ��
	int writtenBit = 0;	//��¼��д��λ��
	int leftBit = 0;	//��һ���ж����λ�Ĳ��ֵ�λ��
	OUT_FILE.write((char*)&frequency, 256 * sizeof(int));		//д����ַ���Ƶ��(�Թ���ѹʱ����Huffman��)
	OUT_FILE.write((char*)&FileSize, sizeof(FileSize));			//д���ļ�����

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
		int FileLeft = EndPos - CurrentPos;		//���ʣ���ļ��Ĵ�С��byte��

		/////////////////////////////////////////////////////
		TAG = false;
		CUR = ((double)FileSize - (double)FileLeft) / (double)FileSize*100.0;
		TAG = true;
		/////////////////////////////////////////////////////

		int read_length;
		bool isEnd = false;	//����Ƿ�����ļ������һ����

		if (FileLeft > 1024)
			read_length = 1024;
		else
		{
			read_length = FileLeft;
			isEnd = true;
		}


		IN_FILE.read((char*)read_buffer, 1024 * sizeof(unsigned char));
		for (int i = 0; i < read_length; i++)		//��������1KB����
		{
			int value = 0;	//�洢����huffman�����ʮ������ֵ
			string code = "";	//�洢����huffman����Ķ�������ֵ	

			if (leftBit>0)//����һ���ж��ಿ�ֲ�����һ��
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

			if (isEnd&&i == read_length - 1)	//�ļ�β������
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
					int inBit = code.length() - (writtenBit - 32);	//Ҫд�뵱ǰ���λ��
					leftBit = writtenBit - 32;						//Ҫд����һ���λ��
					unsigned int temp_value = 0;					//�洢Ҫд����һ���ֵ
					for (int i = inBit, j = 0; i > 0; i--, j++)		//���������ַ����ֿ�������Ҫд����һ���ֵ
					{
						char temp_char = code[i - 1];
						int temp_bit = temp_char - '0';
						temp_value += temp_bit* pow(2, j);
					}

					temp <<= inBit;
					temp |= temp_value;
					OUT_FILE.write((char*)&temp, sizeof(temp));

					buffer = 0;

					for (int i = code.length(), j = 0; i > inBit; i--, j++)		//bufferΪ����Ĳ��ֵ�ֵ
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

			if (writtenBit <= 32)		//һ�������
			{
				temp <<= code.length();
				temp |= value;
				if (writtenBit == 32)	//�Ѿ�����32λ��д���ļ�
				{
					OUT_FILE.write((char*)&temp, sizeof(temp));
					writtenBit = 0;					//����д��λ����0������32λд���ļ�
					temp = 0;
				}
			}

			else   //if(writtenBit>32)	//����������µ��м䲿��
			{
				int inBit = code.length() - (writtenBit - 32);	//Ҫд�뵱ǰ���λ��
				leftBit = writtenBit - 32;						//Ҫд����һ���λ��
				unsigned int temp_value = 0;					//�洢Ҫд����һ���ֵ
				for (int i = inBit, j = 0; i > 0; i--, j++)		//���������ַ����ֿ�������Ҫд����һ���ֵ
				{
					char temp = code[i - 1];
					int temp_bit = temp - '0';
					temp_value += temp_bit* pow(2, j);
				}

				temp <<= inBit;
				temp |= temp_value;

				for (int i = code.length(), j = 0; i > inBit; i--, j++)		//bufferΪ����Ĳ��ֵ�ֵ
				{
					char temp_char = code[i - 1];
					int temp_bit = temp_char - '0';
					buffer += temp_bit* pow(2, j);
				}

				writtenBit = 0;					//����д��λ����0������32λд���ļ�
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

