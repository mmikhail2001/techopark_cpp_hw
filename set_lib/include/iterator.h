#include "set.h"
#pragma once

template <typename T, typename Comparator>
class Iterator
{
public:
    using key_type          = T;
    using value_type        = T;
    using size_type         = std::size_t;
    using difference_type   = std::ptrdiff_t;
    using key_compare       = Comparator;
    using value_compare     = Comparator;
    using reference         = T&;
    using const_reference   = const T&;
    using pointer           = T*;
    using const_pointer     = const T*;
    using iterator_category = std::bidirectional_iterator_tag;

    Iterator() = default;
    Iterator(std::shared_ptr<typename Set<T, Comparator>::Node> node);
    Iterator(const Iterator &) = default;

    Iterator&   operator=(const Iterator &) = default;
 
    Iterator&   operator++();
    Iterator    operator++(int);
    Iterator&   operator--();
    Iterator    operator--(int);
 
    const_reference operator*();
    const_pointer   operator->();
 
    bool        operator==(const Iterator &);
    bool        operator!=(const Iterator &);
private:
    std::shared_ptr<typename  Set<T, Comparator>::Node> node;
};

#include "iterator_definition.h"