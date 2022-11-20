#include <exception>

#include "iterator.h"
#pragma once


template <typename T, typename Cmp>
Iterator<T, Cmp>::Iterator(std::shared_ptr<typename Set<T, Cmp>::Node> node, std::shared_ptr<typename Set<T, Cmp>::Node> root) : node(node), root(root) 
{
}

template <typename T, typename Cmp>
Iterator<T, Cmp>& Iterator<T, Cmp>::operator++()
{
    if (node->next)
    {
        node = node->next;
    }
    else
    {
        node = std::make_shared<typename  Set<T, Cmp>::Node>(0, Y);
    }
    return *this;
}

template <typename T, typename Cmp>
Iterator<T, Cmp> Iterator<T, Cmp>::operator++(int)
{
    Iterator<T, Cmp> tmp(*this);
    operator++();
    return tmp;
}

template <typename T, typename Cmp>
Iterator<T, Cmp>& Iterator<T, Cmp>::operator--()
{
    if (node->is_end == N)
    {
        node = node->prev;
    }
    else
    {
        node = Set<T, Cmp>::getRightMost(root);
    }
    return *this;
}

template <typename T, typename Cmp>
Iterator<T, Cmp> Iterator<T, Cmp>::operator--(int)
{
    Iterator<T, Cmp> tmp(*this);
    operator--();
    return tmp;
}

template <typename T, typename Cmp>
const T& Iterator<T, Cmp>::operator*()
{
    if (!node)
    {
        throw std::runtime_error("operator*() to nullptr");
    }
    return node->data;
}

template <typename T, typename Cmp>
const T* Iterator<T, Cmp>::operator->()
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
    if (node->is_end == Y && other.node->is_end == Y)
    {
        return true;
    }
    return node == other.node;
}

template <typename T, typename Cmp>
bool Iterator<T, Cmp>::operator!=(const Iterator<T, Cmp> &other)
{
    return !(*this == other);
}

