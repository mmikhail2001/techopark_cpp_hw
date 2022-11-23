#pragma once

#include "set.h"


template <typename T, typename Cmp>
class Iterator
{
public:
    using key_type          = T;
    using value_type        = T;
    using size_type         = std::size_t;
    using difference_type   = std::ptrdiff_t;
    using key_compare       = Cmp;
    using value_compare     = Cmp;
    using reference         = T&;
    using const_reference   = const T&;
    using pointer           = T*;
    using const_pointer     = const T*;
    using iterator_category = std::bidirectional_iterator_tag;

    Iterator(typename  Set<T, Cmp>::Node* node, 
        typename  Set<T, Cmp>::Node* root, 
        bool after_end = false, bool before_begin = false);
    Iterator() = default;

    Iterator&       operator=(const Iterator &) = default;
 
    Iterator&       operator++();
    Iterator        operator++(int);
    Iterator&       operator--();
    Iterator        operator--(int);
 
    const_reference operator*();
    const_pointer   operator->();
 
    bool            operator==(const Iterator &);
    bool            operator!=(const Iterator &);
private:
    typename  Set<T, Cmp>::Node* node;
    typename  Set<T, Cmp>::Node* root;
    // признаки местоположения итератора перед началом и после конца
    bool before_begin   = false;
    bool after_end      = false; 
};

#include "iterator_definition.h"