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

    /*
        - Принимаем root, чтобы можно было добраться до last, если содержим ноду-маркер с is_end = Y
        - Наверное, можно в качестве маркера сделать Node с указателями такими же, как у root, но с is_end = Y
    */
    Iterator(std::shared_ptr<typename Set<T, Cmp>::Node> node, 
        std::shared_ptr<typename Set<T, Cmp>::Node> root, 
        bool after_end = false, bool before_begin = false);

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
    std::shared_ptr<typename  Set<T, Cmp>::Node> node;
    std::shared_ptr<typename  Set<T, Cmp>::Node> root;
    bool before_begin   = false;
    bool after_end      = false; 
};

#include "iterator_definition.h"