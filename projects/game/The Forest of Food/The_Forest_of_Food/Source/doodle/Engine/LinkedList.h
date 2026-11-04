/*
File name : LinkedList.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/



#pragma once
#include <iostream>
#include <cstring>

template <typename T>
class Linked;

template <typename T>
class Node
{
	T object;
	uint32_t code;

	Node* next;

public:
	Node(T&& object, Node* next, uint32_t code) : object(std::move(object)), code(code), next(next) {}
	Node(T&& object, uint32_t name) : object(std::move(object)), code(name), next(nullptr) {}

	Node(Node<T>&& node) : object(std::move(node.object)), code(node.code), next(node.next) {}

	friend class Linked<T>;
};

template <typename T>
class Linked
{
	Node<T>* firstNode;

	Node<T>* bookmark;

public:
	Linked() { firstNode = nullptr; }
	Linked(Node<T>&& node) { firstNode = new Node(node); }

	void AddFirst(Node<T>&& node);
	void AddByCode(Node<T>&& node);
	void AddLast(Node<T>&& node);

	T* GetFirst() { return firstNode; }
	Node<T>* Search(uint32_t code);

	bool isEnd();

	T* GetObject();
	void ResetBookmark();

	void DeleteNode();
	void DeleteNode(uint32_t code);

	~Linked();
};


#include "LinkedList.inl"

template <typename T>
Node<T>* Linked<T>::Search(uint32_t code)
{
	Node<T>* tempBookmark(firstNode);
	while (tempBookmark->code == code || tempBookmark->next == nullptr)
		tempBookmark = tempBookmark->next;

	if (tempBookmark == nullptr)
		std::cout << "cannot found Node";

	return tempBookmark;
}

template <typename T>
void Linked<T>::AddFirst(Node<T>&& node)
{
	if (firstNode == nullptr)
		firstNode = new Node(std::move(node));
	else
	{
		Node<T>* temp = firstNode;
		firstNode = new Node(std::move(node));
		firstNode->next = temp;
	}

	ResetBookmark();
}

template <typename T>
void Linked<T>::AddByCode(Node<T>&& node)
{
	if (firstNode == nullptr)
	{
		firstNode = new Node(std::move(node));
		ResetBookmark();
		return;
	}

	Node<T>* tempBookmark(firstNode);
	while (node.code < tempBookmark->code && tempBookmark->next != nullptr) // while link name is bigger than temp name
		tempBookmark = tempBookmark->next;

	Node<T>* temp = new Node(std::move(node));
	temp->next = tempBookmark->next;
	tempBookmark->next = temp;

	ResetBookmark();
}

template <typename T>
void Linked<T>::AddLast(Node<T>&& node)
{
	if (firstNode == nullptr)
	{
		firstNode = new Node(std::move(node));
		ResetBookmark();
		return;
	}

	Node<T>* tempBookmark(firstNode);
	while (tempBookmark->next != nullptr)
		tempBookmark = tempBookmark->next;

	tempBookmark->next = new Node(std::move(node));

	ResetBookmark();
}

template <typename T>
T* Linked<T>::GetObject()
{
	if (bookmark == nullptr)
	{
		return nullptr;
	}

	Node<T>* temp = bookmark;
	bookmark = bookmark->next;
	return &(temp->object);
}

template <typename T>
bool Linked<T>::isEnd()
{
	if (bookmark == nullptr)
	{
		ResetBookmark();
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
void Linked<T>::ResetBookmark()
{
	bookmark = firstNode;
}

template <typename T>
void Linked<T>::DeleteNode(uint32_t code)
{
	Node<T>* tempBookmark(firstNode);
	Node prev(firstNode);
	while (tempBookmark->code != code && tempBookmark->next != nullptr)
	{
		prev = tempBookmark;
		tempBookmark = tempBookmark->next;
	}

	if (tempBookmark == nullptr)
		std::cout << "cannot found Node";

	prev.next = tempBookmark->next;

	delete tempBookmark;
}

template <typename T>
void Linked<T>::DeleteNode()
{
	Node<T>* temp = bookmark;
	delete bookmark;
	bookmark = temp->next;
}

template <typename T>
Linked<T>::~Linked()
{
	if (firstNode == nullptr)
		return;

	Node<T>* tempBookmark(firstNode);
	do
	{
		Node<T>* temp(tempBookmark);
		delete tempBookmark;
		tempBookmark = temp->next;
	} while (tempBookmark->next != nullptr);
}