#pragma once
#include <memory>

#include "set.h"

template <typename T, typename Comparator>
Set<T, Comparator>::Set() : m_root(nullptr) 
{	
}

template <typename T, typename Comparator>
Set<T, Comparator>::~Set()
{
}

template <typename T, typename Comparator>
void Set<T, Comparator>::Insert(const T &data)
{
	if (Has(data))
	{
		return;
	}
	m_root = insertInternal(m_root, data);
}
    
template <typename T, typename Comparator>
bool Set<T, Comparator>::Has(const T &data) const
{
    std::shared_ptr<Node> tmp = m_root;
	while (tmp)
	{
		if (tmp->data == data)
		{
			return true;
		}
		else if (m_cmp(tmp->data, data))
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
    
template <typename T, typename Comparator>
void Set<T, Comparator>::Erase(const T &data)
{
	m_root = eraseInternal(m_root, data);
}

template <typename T, typename Comparator>
std::shared_ptr<typename Set<T, Comparator>::Node> Set<T, Comparator>::eraseInternal(std::shared_ptr<Node> node, const T &data)
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
		replace->parent = node->parent; // на выходе из рекурсии редактируем parent
		
		return doBalance(replace);
	}
	return doBalance(node);
}

template <typename T, typename Comparator>
std::shared_ptr<typename Set<T, Comparator>::Node> Set<T, Comparator>::findReplacement(std::shared_ptr<Node> node) const
{
	while (node->left)
    {
		node = node->left;
    }
	return node;
}

template <typename T, typename Comparator>
std::shared_ptr<typename Set<T, Comparator>::Node> Set<T, Comparator>::detachReplacement(std::shared_ptr<Node> node)
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

template <typename T, typename Comparator>
std::shared_ptr<typename Set<T, Comparator>::Node> Set<T, Comparator>::insertInternal(std::shared_ptr<Node> node, const T &data)
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
	// if (node->left)
	// {
	// 	std::cout << "node->left->data  			: " << node->left->data<< "\n";	
	// 	std::cout << "node->left->parent  			: " << node->left->parent << "\n";	
	// 	std::cout << "node->left->parent->data  	: " << node->left->parent->data << "\n";	
	// }
	// if (node->right)
	// {
	// 	std::cout << "node->right->data  			: " << node->right->data<< "\n";	
	// 	std::cout << "node->right->parent  			: " << node->right->parent << "\n";	
	// 	std::cout << "node->right->parent->data  	: " << node->right->parent->data << "\n";
	// }
	// std::cout << "node  : " << node->data << "\n";
	// std::cout << "left  : " << node->left << "\n";
	// std::cout << "right : " << node->right << "\n";
	// std::cout << "paren : " << node->parent << "\n";
	return doBalance(node);
}

template <typename T, typename Comparator>
size_t Set<T, Comparator>::getHeight(std::shared_ptr<Node> node) const
{
	return node ? node->height : 0;
}

template <typename T, typename Comparator>
void Set<T, Comparator>::fixHeight(std::shared_ptr<Node> node)
{
	node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
}

template <typename T, typename Comparator>
std::shared_ptr<typename Set<T, Comparator>::Node> Set<T, Comparator>::rotateLeft(std::shared_ptr<Node> node)
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

template <typename T, typename Comparator>
std::shared_ptr<typename Set<T, Comparator>::Node> Set<T, Comparator>::rotateRight(std::shared_ptr<Node> node)
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

template <typename T, typename Comparator>
int Set<T, Comparator>::getBalance(std::shared_ptr<Node> node) const
{
	return getHeight(node->right) - getHeight(node->left);
}

template <typename T, typename Comparator>
std::shared_ptr<typename Set<T, Comparator>::Node> Set<T, Comparator>::doBalance(std::shared_ptr<Node> node)
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

template <typename T, typename Comparator>
std::shared_ptr<typename Set<T, Comparator>::Node> Set<T, Comparator>::NextInternal(std::shared_ptr<Node> node) const
{
	if (!node)
	{
		return nullptr;
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

template <typename T, typename Comparator>
std::shared_ptr<typename Set<T, Comparator>::Node> Set<T, Comparator>::PrevInternal(std::shared_ptr<Node> node) const
{
	if (node == end())
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

template <typename T, typename Comparator>
std::shared_ptr<typename Set<T, Comparator>::Node> Set<T, Comparator>::begin() const
{
	if (!m_root)
	{
		return nullptr;
	}
	std::shared_ptr<Node> node = m_root;
	while (node->left)
    {
		node = node->left;
    }
	return node;
}

template <typename T, typename Comparator>
std::shared_ptr<typename Set<T, Comparator>::Node> Set<T, Comparator>::end() const
{
	return nullptr;
}

// template <typename T, typename Comparator>
// void Set<T, Comparator>::Node::Next()
// {
// 	if (right)
// 	{
// 		while (left)
// 		{
// 			Node *this_node = left.get();
// 			this = this_node;
// 			// right 	= left->right;
// 			// left 	= left->left;
// 			// parent.reset(this); 
// 		}
// 	}
// 	else
// 	{
// 		while (parent && parent->left.get() != this)
// 		{
// 			Node *this_node = parent.get();
// 			this = this_node;
// 			// this = parent.get();
// 		}
// 		Node *this_node = parent.get();
// 		this = this_node;
// 		// this = parent.get();
// 	}
// } 

// template <typename T, typename Comparator>
// void Set<T, Comparator>::Node::Next() 
// {
// 	if (right)
// 	{
// 		while (left)
// 		{
// 			// this = left.get();
// 			right 	= left->right;
// 			left 	= left->left;
// 			parent.reset(this); 
// 		}
// 	}
// 	else
// 	{
// 		while (parent && parent->left.get() != this)
// 		{
// 			left = parent->left;
// 			right = parent->right;
// 			parent = parent->parent;
// 			// this = parent.get();
// 		}
// 		if (parent)
// 		{
// 			left = parent->left;
// 			right = parent->right;
// 			parent = parent->parent;
// 		}
// 		else
// 		{
// 			left = right = parent = nullptr;
// 		}
// 	}
// } 

// template <typename T, typename Comparator>
// std::shared_ptr<typename Set<T, Comparator>::Node> Set<T, Comparator>::Node::Next() 
// {
// 	Node *node = this;
// 	if (node->right)
// 	{
// 		node = node->right.get();
// 		while (node->left)
// 		{
// 			node = node->left.get();
// 		}
// 		return std::make_shared<Node>(*node);
// 		// return std::shared_ptr<Node>(node);
// 	}
// 	else
// 	{
// 		std::shared_ptr<Node> needed_parent = node->parent;
// 		while (needed_parent && needed_parent->left.get() != node)
// 		{
// 			node = needed_parent.get(); 
// 			needed_parent = needed_parent->parent;
// 		}
// 		return needed_parent;
// 	}
// } 

template <typename T, typename Comparator>
std::shared_ptr<typename Set<T, Comparator>::Node> Set<T, Comparator>::Node::Next() 
{
	std::shared_ptr<Node> node;
	if (right)
	{
		node = right;
	// if (node->right)
	// {
		// node = node->right.get();
		while (node->left)
		{
			node = node->left;
		}
		return node;
		// return std::make_shared<Node>(*node);
		// return std::shared_ptr<Node>(node);
	}
	else
	{
		if (!parent)
		{
			return nullptr;
		}
		else
		{
			node = parent;
			if (node->left.get() == this)
			{
				return node;
			}
		}
		// std::shared_ptr<Node> child = node->parent;
		std::shared_ptr<Node> needed_parent = node->parent;
		while (needed_parent && needed_parent->left != node)
		{
			node = needed_parent; 
			needed_parent = needed_parent->parent;
		}
		return needed_parent;
	}
} 

// template <typename T, typename Comparator>
// void Set<T, Comparator>::Node::Prev() const
// {
// 	if (left)
// 	{
// 		while (right)
// 		{
// 			this = right.get();
// 			// right 	= left->right;
// 			// left 	= left->left;
// 			// parent.reset(this); 
// 		}
// 	}
// 	else
// 	{
// 		while (parent && parent->right.get() != this)
// 		{
// 			this = parent.get();
// 		}
// 		this = parent.get();
// 	}
// } 

