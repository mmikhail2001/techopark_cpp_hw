#pragma once

#include <exception>

#include "iterator.h"

template <typename T, typename Cmp>
Iterator<T, Cmp>::Iterator(typename Set<T, Cmp>::Node *node, typename Set<T, Cmp>::Node *root, bool after_end,
						   bool before_begin)
	: node(node), root(root), before_begin(before_begin), after_end(after_end)
{
}

template <typename T, typename Cmp>
Iterator<T, Cmp> &Iterator<T, Cmp>::operator++()
{
	if (node)
	{
		node = node->next;
		if (!node)
		{
			after_end = true;
		}
	}
	else if (before_begin)
	{
		node = Set<T, Cmp>::getLeftMost(root);
		before_begin = false;
	}
	return *this;
}

template <typename T, typename Cmp>
Iterator<T, Cmp> Iterator<T, Cmp>::operator++(int)
{
	Iterator<T, Cmp> tmp(*this);
	++(*this);
	return tmp;
}

template <typename T, typename Cmp>
Iterator<T, Cmp> &Iterator<T, Cmp>::operator--()
{
	if (node)
	{
		node = node->prev;
		if (!node)
		{
			before_begin = true;
		}
	}
	else if (after_end)
	{
		node = Set<T, Cmp>::getRightMost(root);
		after_end = false;
	}
	return *this;
}

template <typename T, typename Cmp>
Iterator<T, Cmp> Iterator<T, Cmp>::operator--(int)
{
	Iterator<T, Cmp> tmp(*this);
	--(*this);
	return tmp;
}

template <typename T, typename Cmp>
const T &Iterator<T, Cmp>::operator*()
{
	if (!node)
	{
		throw std::runtime_error("operator*() to nullptr");
	}
	return node->data;
}

template <typename T, typename Cmp>
const T *Iterator<T, Cmp>::operator->()
{
	if (!node)
	{
		throw std::runtime_error("operator->() to nullptr");
	}
	return &(node->data);
}

template <typename T, typename Cmp>
bool Iterator<T, Cmp>::operator==(const Iterator<T, Cmp> &other)
{
	return node == other.node;
}

template <typename T, typename Cmp>
bool Iterator<T, Cmp>::operator!=(const Iterator<T, Cmp> &other)
{
	return !(*this == other);
}
