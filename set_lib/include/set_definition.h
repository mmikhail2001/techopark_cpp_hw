#pragma once
#include <iostream>
#include <memory>

#include "set.h"

template <typename T, typename Cmp>
Set<T, Cmp>::Set(Cmp cmp) : m_cmp(cmp), m_root(nullptr) 
{	
}

template <typename T, typename Cmp>
template <typename InputIt, typename>
Set<T, Cmp>::Set(InputIt first, InputIt last, Cmp cmp) : m_cmp(cmp) 
{	
	for (; first != last; ++first)
	{
		insert(*first);
	}
}

template <typename T, typename Cmp>
Set<T, Cmp>::Set(std::initializer_list<T> const &init_list)
{
	for (auto && elem : init_list)
	{
		insert(elem);
	}
}

template <typename T, typename Cmp>
template<typename Cmp_>
Set<T, Cmp>::Set(Set<T, Cmp_> const &other)
{
	for (auto elem : other)
	{
		insert(elem);
	}
}


template <typename T, typename Cmp>
Set<T, Cmp>::Set(Set const &other)
{
	for (auto elem : other)
	{
		insert(elem);
	}
}

template <typename T, typename Cmp>
Set<T, Cmp>& Set<T, Cmp>::operator=(Set other)
{
	Swap(other);
	return *this;
}

template <typename T, typename Cmp>
void Set<T, Cmp>::Swap(Set &other)
{
	std::swap(m_root, other.m_root);
	std::swap(m_first, other.m_first);
	std::swap(m_last, other.m_last);
	std::swap(m_cmp, other.m_cmp);
	std::swap(m_size, other.m_size);
}

template <typename T, typename Cmp>
Set<T, Cmp>::~Set()
{
}

template <typename T, typename Cmp>
void Set<T, Cmp>::insert(const T &data)
{
	if (findInternal(data) != nullptr)
	{
		return;
	}
	++m_size;
	m_root = insertInternal(m_root, data);
	auto it = findInternal(data);
	it->prev = prevInternal(it);
	if (it->prev)
	{
		it->prev->next = it;
	}
	it->next = nextInternal(it);
	if (it->next)
	{
		it->next->prev = it;
	}
}
    
template <typename T, typename Cmp>
Iterator<T, Cmp>  Set<T, Cmp>::find(const T &data) const
{
    auto ptr = findInternal(data);
	return ptr ? Iterator<T, Cmp>(ptr) : end();
}

template <typename T, typename Cmp>
Iterator<T, Cmp> Set<T, Cmp>::lower_bound(const T &data) const
{
    auto ptr = lower_boundInternal(data);
	return ptr ? Iterator<T, Cmp>(ptr) : end();
}

template <typename T, typename Cmp>
std::shared_ptr<typename Set<T, Cmp>::Node> Set<T, Cmp>::findInternal(const T &data) const
{
    std::shared_ptr<Node> node = m_root;
	while (node)
	{
		if (node->data == data)
		{
			return node;
		}
		else if (m_cmp(node->data, data))
		{
			node = node->right;
		}
		else
		{
			node = node->left;
		}
	}
	return nullptr;
}

template <typename T, typename Cmp>
std::shared_ptr<typename Set<T, Cmp>::Node> Set<T, Cmp>::lower_boundInternal(const T &data) const
{
    std::shared_ptr<Node> node = m_root;
	std::shared_ptr<Node> result = nullptr;
	while (node)
	{
		if (m_cmp(node->data, data))
		{
			node = node->right;
		}
		else
		{
			result = node; 
			node = node->left;
		}
	}
	return result;
}
    
template <typename T, typename Cmp>
void Set<T, Cmp>::erase(const T &data)
{	
	auto it = findInternal(data);
	if (it != nullptr)
	{
		// отдельная функция - удалить из списка
		auto prevNode = prevInternal(it);
		auto nextNode = nextInternal(it);
		if (prevNode == nullptr)
		{
			nextNode->prev = nullptr;
		}
		if (nextNode == nullptr)
		{
			prevNode->next = nullptr;
		}
		if (prevNode && nextNode)
		{
			prevNode->next = nextNode;
			nextNode->prev = prevNode;
		}
		// prevNode->next = nextNode;
		--m_size;
		m_root = eraseInternal(m_root, data);
	}
}

template <typename T, typename Cmp>
std::shared_ptr<typename Set<T, Cmp>::Node> Set<T, Cmp>::eraseInternal(std::shared_ptr<Node> node, const T &data)
{
	if (!node)
    {
		return nullptr;
    }
	if (m_cmp(node->data, data))
	{
		node->right = eraseInternal(node->right, data);
		if (node->right)
		{
			node->right->parent = node;
		}
	}
	else if (m_cmp(data, node->data))
	{
		node->left = eraseInternal(node->left, data);
		if (node->left)
		{
			node->right->parent = node;
		}
	}
	else
	{
		std::shared_ptr<Node> left = node->left;
		std::shared_ptr<Node> right = node->right;
		
		if (!right)
        {
			return left;
        }
		
		std::shared_ptr<Node> replace = findReplacement(right);
		replace->right = detachReplacement(right);
		if (replace->right)
		{
			replace->right->parent = replace;
		}
		replace->left = left;
		if (replace->left)
		{
			replace->left->parent = replace;
		}
		// на выходе из рекурсии редактируем parent
		replace->parent = node->parent; 
		
		return doBalance(replace);
	}
	return doBalance(node);
}

template <typename T, typename Cmp>
std::shared_ptr<typename Set<T, Cmp>::Node> Set<T, Cmp>::findReplacement(std::shared_ptr<Node> node) const
{
	while (node->left)
    {
		node = node->left;
    }
	return node;
}

template <typename T, typename Cmp>
std::shared_ptr<typename Set<T, Cmp>::Node> Set<T, Cmp>::detachReplacement(std::shared_ptr<Node> node)
{
	if (!node->left)
    {
		return node->right;
    }
	node->left = detachReplacement(node->left);
	if (node->left)
	{
		node->left->parent = node;
	}
	return doBalance(node);
}

template <typename T, typename Cmp>
std::shared_ptr<typename Set<T, Cmp>::Node> Set<T, Cmp>::insertInternal(std::shared_ptr<Node> node, const T &data)
{
	if (!node)
    {
		return std::make_shared<Node>(data);
    }
	if (m_cmp(data, node->data))
	{
		node->left = insertInternal(node->left, data);
		// nullptr не возвращается, т.к. вставка обязательна
		node->left->parent = node; 
	}
	else if (m_cmp(node->data, data))
	{
		node->right = insertInternal(node->right, data);
		node->right->parent = node; 
	}

	return doBalance(node);
}

template <typename T, typename Cmp>
size_t Set<T, Cmp>::getHeight(std::shared_ptr<Node> node) const
{
	return node ? node->height : 0;
}

template <typename T, typename Cmp>
void Set<T, Cmp>::fixHeight(std::shared_ptr<Node> node)
{
	node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
}

template <typename T, typename Cmp>
std::shared_ptr<typename Set<T, Cmp>::Node> Set<T, Cmp>::rotateLeft(std::shared_ptr<Node> node)
{
	std::shared_ptr<Node> tmp = node->right;
	node->right = tmp->left;
	if (tmp->left)
	{
		tmp->left->parent = node;
	}
	tmp->left = node;
	tmp->parent = node->parent;
	node->parent = tmp;
	fixHeight(node);
	fixHeight(tmp);
	return tmp;
}

template <typename T, typename Cmp>
std::shared_ptr<typename Set<T, Cmp>::Node> Set<T, Cmp>::rotateRight(std::shared_ptr<Node> node)
{
	// node = a
	// tmp = node->left = b
	std::shared_ptr<Node> tmp = node->left;
	node->left = tmp->right; // теперь a->left = C
	if (tmp->right)
	{
		tmp->right->parent = node;
	}
	tmp->right = node; // теперь b->right = a
	tmp->parent = node->parent;
	node->parent = tmp;
	
	fixHeight(node);
	fixHeight(tmp);
	return tmp;
}

template <typename T, typename Cmp>
int Set<T, Cmp>::getBalance(std::shared_ptr<Node> node) const
{
	return getHeight(node->right) - getHeight(node->left);
}

template <typename T, typename Cmp>
std::shared_ptr<typename Set<T, Cmp>::Node> Set<T, Cmp>::doBalance(std::shared_ptr<Node> node)
{
	fixHeight(node);
	
	switch (getBalance(node))
	{
		case 2:
		{
			if (getBalance(node->right) < 0)
            {
				node->right = rotateRight(node->right);
            }
			return rotateLeft(node);
		}
		case -2:
		{
			if (getBalance(node->left) > 0)
            {
				node->left = rotateLeft(node->left);
            }
			return rotateRight(node);
		}
		default:
        {
			return node;
        }
	}
}


template <typename T, typename Cmp>
Iterator<T, Cmp>  Set<T, Cmp>::begin() const
{
	if (!m_root)
	{
		return end();
	}
	// можно оптимизировать
	// при вставке (insert) обновлять минимальный (максимальный) элемент
	std::shared_ptr<Node> node = m_root;
	while (node->left)
    {
		node = node->left;
    }
	return Iterator<T, Cmp>(node);
}

template <typename T, typename Cmp>
Iterator<T, Cmp>  Set<T, Cmp>::end() const
{
	return Iterator<T, Cmp>(nullptr);
}

template <typename T, typename Cmp>
std::shared_ptr<typename Set<T, Cmp>::Node> Set<T, Cmp>::nextInternal(std::shared_ptr<Node> node) const
{
	// findMin() ?
	if (node == nullptr)
	{
		std::shared_ptr<Node> node = m_root;
		while (node->left)
		{
			node = node->left;
		}
		return node;
	}
	if (node->right)
	{
		node = node->right;
		while (node->left)
		{
			node = node->left;
		}
		return node;
	}
	else
	{
		std::shared_ptr<Node> needed_parent = node->parent;
		while (needed_parent && needed_parent->left != node)
		{
			node = needed_parent; 
			needed_parent = node->parent;
		}
		return needed_parent;
	}
}

template <typename T, typename Cmp>
std::shared_ptr<typename Set<T, Cmp>::Node> Set<T, Cmp>::prevInternal(std::shared_ptr<Node> node) const
{
	// findMax() ?
	if (node == nullptr)
	{
		std::shared_ptr<Node> node = m_root;
		while (node->right)
		{
			node = node->right;
		}
		return node;
	}
	if (node->left)
	{
		node = node->left;
		while (node->right)
		{
			node = node->right;
		}
		return node;
	}
	else
	{
		std::shared_ptr<Node> needed_parent = node->parent;
		while (needed_parent && needed_parent->right != node)
		{
			node = needed_parent; 
			needed_parent = node->parent;
		}
		return needed_parent;
	}
}

template <typename T, typename Cmp>
std::size_t Set<T, Cmp>::size() const
{
	return m_size;
}

template <typename T, typename Cmp>
bool Set<T, Cmp>::empty() const
{
	return m_size == 0;
}


