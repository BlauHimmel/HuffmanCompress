#pragma once

#include "stdafx.h"
#define DEFAULTSIZE 256

template <typename T>
class MinHeap
{
public:
	MinHeap(int size = DEFAULTSIZE);
	MinHeap(T elements[], int size);
	~MinHeap();
	bool Insert(T &x);
	bool Remove(T &x);
	bool IsEmpty();
	bool IsFull();
	void Clear();
private:
	T* elements;
	int currentSize;
	int heapSize;
	void siftDown(int start, int m);
	void siftUp(int start);
};

template <typename T>
MinHeap<T>::MinHeap(int size)
{
	heapSize = (size > DEFAULTSIZE) ? size : DEFAULTSIZE;
	elements = new T[heapSize];
	currentSize = 0;
}

template <typename T>
MinHeap<T>::MinHeap(T elements[], int size)
{
	heapSize = (size > DEFAULTSIZE) ? size : DEFAULTSIZE;
	elements = new T[heapSize];
	for (int i = 0; i < n; i++)
	{
		this->elements[i] = elements[i];
	}
	currentSize = size;
	int currentPosition = (currentSize - 2) / 2;
	while (currentPosition >= 0)
		siftDown(currentPosition--, currentSize - 1);
}

template <typename T>
MinHeap<T>::~MinHeap()
{
	delete[] elements;
}

template <typename T>
bool MinHeap<T>::Insert(T &x)
{
	if (currentSize == heapSize)
		return false;
	elements[currentSize] = x;
	siftUp(currentSize++);
	return true;
}

template <typename T>
bool MinHeap<T>::Remove(T &x)
{
	if (IsEmpty())
		return false;
	x = elements[0];
	elements[0] = elements[currentSize - 1];
	currentSize--;
	siftDown(0, currentSize - 1);
	return true;
}


template <typename T>
bool MinHeap<T>::IsEmpty()
{
	if (currentSize == 0)
		return true;
	else
		return false;
}

template <typename T>
bool MinHeap<T>::IsFull()
{
	if (currentSize == heapSize)
		return true;
	else
		return false;
}

template <typename T>
void MinHeap<T>::Clear()
{
	currentSize = 0;
}

template <typename T>
void MinHeap<T>::siftDown(int start, int m)
{
	int head = start, tail = 2 * head + 1;
	T temp = elements[head];		//´íÎóµã
	while (tail <= m)
	{
		if (tail < m&&elements[tail] > elements[tail + 1])
			tail++;

		if (temp < elements[tail])
			break;
		else
		{
			elements[head] = elements[tail];
			head = tail;
			tail = 2 * head + 1;
		}
	}
	elements[head] = temp;
}

template <typename T>
void MinHeap<T>::siftUp(int start)
{
	int tail = start, head = (tail - 1) / 2;
	T temp = elements[tail];
	while (tail > 0)
	{
		if (elements[head] < temp)
			break;
		else
		{
			elements[tail] = elements[head];
			tail = head;
			head = (tail - 1) / 2;
		}
	}
	elements[tail] = temp;
}