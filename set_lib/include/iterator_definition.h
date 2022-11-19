#include "iterator.h"
#pragma once


template <typename T, typename Comparator>
Iterator<T, Comparator>::Iterator(std::shared_ptr<typename Set<T, Comparator>::Node> node) : node(node) 
{
}

template <typename T, typename Comparator>
Iterator<T, Comparator>& Iterator<T, Comparator>::operator++()
{
    node = node->Next();
    return *this;
}

template <typename T, typename Comparator>
Iterator<T, Comparator> Iterator<T, Comparator>::operator++(int)
{
    Iterator<T, Comparator> tmp(*this);
    operator++();
    return tmp;
}

template <typename T, typename Comparator>
Iterator<T, Comparator>& Iterator<T, Comparator>::operator--()
{
    node = node->Prev();
    return *this;
}

template <typename T, typename Comparator>
Iterator<T, Comparator> Iterator<T, Comparator>::operator--(int)
{
    Iterator<T, Comparator> tmp(*this);
    operator--();
    return tmp;
}

template <typename T, typename Comparator>
const T& Iterator<T, Comparator>::operator*()
{
    return node->data;
}

template <typename T, typename Comparator>
const T* Iterator<T, Comparator>::operator->()
{
    return &(node->data);
}

template <typename T, typename Comparator>
bool Iterator<T, Comparator>::operator==(const Iterator<T, Comparator> &other)
{
    return node == other.node;
}

template <typename T, typename Comparator>
bool Iterator<T, Comparator>::operator!=(const Iterator<T, Comparator> &other)
{
    return !(*this == other);
}

