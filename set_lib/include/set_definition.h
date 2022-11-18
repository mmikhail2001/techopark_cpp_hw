#pragma once

#include "set.h"

template <typename T>
Set<T>::Set() : m_root(nullptr) 
{	
}

template <typename T>
Set<T>::~Set()
{
	destroyTree(m_root);
}

template <typename T>
void Set<T>::Insert(const T &data)
{
	m_root = insertInternal(m_root, data);
}
    
template <typename T>
bool Set<T>::Has(const T &data) const
{
	Node *tmp = m_root;
	while (tmp)
	{
		if (tmp->data == data)
		{
			return true;
		}
		else if (tmp->data < data)
		{
			tmp = tmp->right;
		}
		else
		{
			tmp = tmp->left;
		}
	}
	return false;
}
    
template <typename T>
void Set<T>::Erase(const T &data)
{
	m_root = eraseInternal(m_root, data);
}

template <typename T>
void Set<T>::destroyTree(Node *node)
{
	if (node)
	{
		destroyTree(node->left);
		destroyTree(node->right);
		delete node;
	}
}

template <typename T>
typename Set<T>::Node* Set<T>::eraseInternal(Node *node, const T &data)
{
	if (!node)
		return nullptr;
	if (node->data < data)
	{
		node->right = eraseInternal(node->right, data);
	}
	else if (node->data > data)
	{
		node->left = eraseInternal(node->left, data);
	}
	else
	{
		Node *left = node->left;
		Node *right = node->right;
		
		delete node;
		
		if (!right)
			return left;
		
		Node *min = findMin(right);
		min->right = removeMin(right);
		min->left = left;
		
		return doBalance(min);
	}
	return doBalance(node);
}

template <typename T>
typename Set<T>::Node* Set<T>::findMin(Node *node)
{
	while (node->left)
		node = node->left;
	return node;
}

template <typename T>
typename Set<T>::Node* Set<T>::removeMin(Node *node)
{
	if (!node->left)
		return node->right;
	node->left = removeMin(node->left);
	return doBalance(node);
}

template <typename T>
typename Set<T>::Node* Set<T>::insertInternal(Node *node, const T &data)
{
	if (!node)
		return new Node(data);
	if (node->data <= data)
	{
		node->right = insertInternal(node->right, data);
	}
	else
	{
		node->left = insertInternal(node->left, data);
	}
	return doBalance(node);
}

template <typename T>
size_t Set<T>::getHeight(Node *node)
{
	return node ? node->height : 0;
}

template <typename T>
void Set<T>::fixHeight(Node *node)
{
	node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
}

template <typename T>
typename Set<T>::Node* Set<T>::rotateLeft(Node *node)
{
	Node *tmp = node->right;
	node->right = tmp->left;
	tmp->left = node;
	fixHeight(node);
	fixHeight(tmp);
	return tmp;
}

template <typename T>
typename Set<T>::Node* Set<T>::rotateRight(Node *node)
{
	Node *tmp = node->left;
	node->left = tmp->right;
	tmp->right = node;
	fixHeight(node);
	fixHeight(tmp);
	return tmp;
}

template <typename T>
int Set<T>::getBalance(Node *node)
{
	return getHeight(node->right) - getHeight(node->left);
}

template <typename T>
typename Set<T>::Node* Set<T>::doBalance(Node *node)
{
	fixHeight(node);
	
	switch (getBalance(node))
	{
		case 2:
		{
			if (getBalance(node->right) < 0)
				node->right = rotateRight(node->right);
			return rotateLeft(node);
		}
		case -2:
		{
			if (getBalance(node->left) > 0)
				node->left = rotateLeft(node->left);
			return rotateRight(node);
		}
		default:
			return node;
	}
}

