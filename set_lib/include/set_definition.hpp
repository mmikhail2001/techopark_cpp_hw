#pragma once

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
	for (auto &&elem : init_list)
	{
		insert(elem);
	}
}

template <typename T, typename Cmp>
template <typename Cmp_>
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
Set<T, Cmp>::~Set()
{
	// итерируемся по списку
	Node *node = m_first;
	while (node)
	{
		Node *tmp = node->next;
		delete node;
		node = tmp;
	}
}

template <typename T, typename Cmp>
Set<T, Cmp> &Set<T, Cmp>::operator=(Set other)
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
void Set<T, Cmp>::insert(const T &data)
{
	if (findInternal(data) != nullptr)
	{
		return;
	}
	++m_size;
	m_root = insertInternal(m_root, data);
	auto ptr = findInternal(data);
	// обновление m_first
	if (!m_first_value || m_cmp(ptr->data, *m_first_value))
	{
		m_first = ptr;
		m_first_value = ptr->data;
	}
	// обновление m_last
	if (!m_last_value || m_cmp(*m_last_value, ptr->data))
	{
		m_last = ptr;
		m_last_value = ptr->data;
	}
	// установка prev и next
	ptr->prev = prevInternal(ptr);
	if (ptr->prev)
	{
		ptr->prev->next = ptr;
	}
	ptr->next = nextInternal(ptr);
	if (ptr->next)
	{
		ptr->next->prev = ptr;
	}
}

template <typename T, typename Cmp>
Iterator<T, Cmp> Set<T, Cmp>::find(const T &data) const
{
	auto ptr = findInternal(data);
	return ptr ? Iterator<T, Cmp>(ptr, m_root) : end();
}

template <typename T, typename Cmp>
Iterator<T, Cmp> Set<T, Cmp>::lower_bound(const T &data) const
{
	auto ptr = lower_boundInternal(data);
	return ptr ? Iterator<T, Cmp>(ptr, m_root) : end();
}

template <typename T, typename Cmp>
typename Set<T, Cmp>::Node *Set<T, Cmp>::findInternal(const T &data) const
{
	Node *node = m_root;
	while (node)
	{
		if (!m_cmp(node->data, data) && !m_cmp(data, node->data))
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
typename Set<T, Cmp>::Node *Set<T, Cmp>::lower_boundInternal(const T &data) const
{
	Node *node = m_root;
	Node *result = nullptr;
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
	auto ptr = findInternal(data);
	if (ptr != nullptr)
	{
		auto prevNode = prevInternal(ptr);
		auto nextNode = nextInternal(ptr);
		// извлекаем из списка
		if (!(prevNode == nullptr && nextNode == nullptr))
		{
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
		}
		--m_size;
		// извлекаем из дерева и удаляем
		m_root = eraseInternal(m_root, data);
		m_first = getLeftMost(m_root);
	}
}

template <typename T, typename Cmp>
typename Set<T, Cmp>::Node *Set<T, Cmp>::eraseInternal(Node *node, const T &data)
{
	if (!node) return nullptr;
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
		Node *left = node->left;
		Node *right = node->right;

		if (!right)
		{
			delete node;
			return left;
		}

		Node *replace = findReplacement(right);
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
		delete node;

		return doBalance(replace);
	}
	return doBalance(node);
}

template <typename T, typename Cmp>
typename Set<T, Cmp>::Node *Set<T, Cmp>::findReplacement(Node *node) const
{
	if (!node) return nullptr;
	while (node->left)
	{
		node = node->left;
	}
	return node;
}

template <typename T, typename Cmp>
typename Set<T, Cmp>::Node *Set<T, Cmp>::detachReplacement(Node *node)
{
	if (!node) return nullptr;
	if (!node->left)
	{
		return node->right;
	}
	node->left = detachReplacement(node->left);
	return doBalance(node);
}

template <typename T, typename Cmp>
typename Set<T, Cmp>::Node *Set<T, Cmp>::insertInternal(Node *node, const T &data)
{
	if (!node)
	{
		return new Node(data);
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
size_t Set<T, Cmp>::getHeight(Node *node) const
{
	return node ? node->height : 0;
}

template <typename T, typename Cmp>
void Set<T, Cmp>::fixHeight(Node *node)
{
	node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
}

template <typename T, typename Cmp>
typename Set<T, Cmp>::Node *Set<T, Cmp>::rotateLeft(Node *node)
{
	if (!node) return nullptr;
	Node *tmp = node->right;
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
typename Set<T, Cmp>::Node *Set<T, Cmp>::rotateRight(Node *node)
{
	if (!node) return nullptr;
	Node *tmp = node->left;
	node->left = tmp->right;
	if (tmp->right)
	{
		tmp->right->parent = node;
	}
	tmp->right = node;
	tmp->parent = node->parent;
	node->parent = tmp;

	fixHeight(node);
	fixHeight(tmp);
	return tmp;
}

template <typename T, typename Cmp>
int Set<T, Cmp>::getBalance(Node *node) const
{
	return getHeight(node->right) - getHeight(node->left);
}

template <typename T, typename Cmp>
typename Set<T, Cmp>::Node *Set<T, Cmp>::doBalance(Node *node)
{
	if (!node) return nullptr;
	fixHeight(node);

	switch (getBalance(node))
	{
	case 2: {
		if (getBalance(node->right) < 0)
		{
			node->right = rotateRight(node->right);
		}
		return rotateLeft(node);
	}
	case -2: {
		if (getBalance(node->left) > 0)
		{
			node->left = rotateLeft(node->left);
		}
		return rotateRight(node);
	}
	default: {
		return node;
	}
	}
}

template <typename T, typename Cmp>
Iterator<T, Cmp> Set<T, Cmp>::begin() const
{
	if (!m_root)
	{
		return end();
	}
	return Iterator<T, Cmp>(m_first, m_root);
}

template <typename T, typename Cmp>
Iterator<T, Cmp> Set<T, Cmp>::end() const
{
	return Iterator<T, Cmp>(nullptr, m_root, true);
}

template <typename T, typename Cmp>
typename Set<T, Cmp>::Node *Set<T, Cmp>::nextInternal(Node *node) const
{
	if (!node) return nullptr;

	if (node->right)
	{
		node = node->right;
		return getLeftMost(node);
	}
	else
	{
		Node *needed_parent = node->parent;
		while (needed_parent && needed_parent->left != node)
		{
			node = needed_parent;
			needed_parent = node->parent;
		}
		return needed_parent;
	}
}

template <typename T, typename Cmp>
typename Set<T, Cmp>::Node *Set<T, Cmp>::prevInternal(Node *node) const
{
	if (!node) return nullptr;
	if (node->left)
	{
		node = node->left;
		return getRightMost(node);
	}
	else
	{
		Node *needed_parent = node->parent;
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

template <typename T, typename Cmp>
bool Set<T, Cmp>::operator==(Set const &other) const
{
	auto itOther = other.begin();
	for (auto it = begin(); it != end(); ++it)
	{
		if (*itOther != *it)
		{
			return false;
		}
		itOther++;
	}
	return true;
}

template <typename T, typename Cmp>
bool Set<T, Cmp>::operator!=(Set const &other) const
{
	return !(*this == other);
}
