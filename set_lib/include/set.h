#pragma once

#include <iostream>
#include <memory>
#include <type_traits>

template <typename T, typename Cmp>
class Iterator;

template <typename T, typename Cmp = std::less<T>>
class Set
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
    using iterator          = Iterator<T, Cmp>;
private:
    struct Node
    {
        Node(const T &data)
        : data(data), height(1)
        {
        }

        T 		                data;
        size_t 	                height;
        // для АВЛ-дерева
        std::shared_ptr<Node>	left;	
        std::shared_ptr<Node> 	right;
        std::shared_ptr<Node> 	parent;
        // для двусвязного спика
        std::shared_ptr<Node> 	next;
        std::shared_ptr<Node> 	prev;
    };
public:
    Set(Cmp cmp = Cmp{});
    template <typename InputIt, typename = typename std::enable_if<std::is_same<typename   std::iterator_traits<InputIt>::value_type, T>::value>::type>
    Set(InputIt first, InputIt last, Cmp cmp = Cmp{});
    Set(std::initializer_list<T> const &init_list);
    template<typename Cmp_ = Cmp>
    Set(Set<T, Cmp_> const &other);
    Set(Set const &other);
    Set &operator=(Set other);
    ~Set();
    size_type       size() const;
    bool            empty() const;
    // проверяет наличие прежде чем insertInternal
    void 	        insert(const_reference data);
    // проверяет наличие прежде чем eraseInternal
    iterator 	    find(const_reference data) const;
    iterator 	    lower_bound(const_reference data) const;
    void 	        erase(const_reference data);
    iterator        begin() const;
    iterator        end() const;
    
private:
    friend class Iterator<T, Cmp>;
    // корень дерева
    std::shared_ptr<Node>	m_root;
    // узлы двусвязного списка
    std::shared_ptr<Node>	m_first;
    std::shared_ptr<Node>	m_last;
    Cmp                     m_cmp;
    size_type               m_size = 0;

    std::shared_ptr<Node> 	findInternal(const_reference data) const;
    std::shared_ptr<Node> 	lower_boundInternal(const_reference data) const;
    std::shared_ptr<Node> 	eraseInternal(std::shared_ptr<Node> node, const_reference data);
    std::shared_ptr<Node> 	insertInternal(std::shared_ptr<Node> node, const_reference data);
    
    std::shared_ptr<Node> 	detachReplacement(std::shared_ptr<Node> node);
    std::shared_ptr<Node> 	findReplacement(std::shared_ptr<Node> node) const;
	
    std::shared_ptr<Node> 	rotateLeft(std::shared_ptr<Node> node);
    std::shared_ptr<Node> 	rotateRight(std::shared_ptr<Node> node);
	
    size_t 	                getHeight(std::shared_ptr<Node> node) const;
    void 	                fixHeight(std::shared_ptr<Node> node);

    int 	                getBalance(std::shared_ptr<Node> node) const;
    std::shared_ptr<Node> 	doBalance(std::shared_ptr<Node> node);

    std::shared_ptr<Node>   nextInternal(std::shared_ptr<Node> node) const;
    std::shared_ptr<Node>   prevInternal(std::shared_ptr<Node> node) const;

    void                    Swap(Set &other);
    
};

#include "set_definition.h"

