#include "iterator.h"
#pragma once


template <typename T, typename Cmp>
Iterator<T, Cmp>::Iterator(std::shared_ptr<typename Set<T, Cmp>::Node> node) : node(node) 
{
}

template <typename T, typename Cmp>
Iterator<T, Cmp>& Iterator<T, Cmp>::operator++()
{
    // node = node->Next();
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
    // node = node->Prev();
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
    return node->data;
}

template <typename T, typename Cmp>
const T* Iterator<T, Cmp>::operator->()
{
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

