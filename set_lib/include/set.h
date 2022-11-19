#pragma once

#include <iostream>
#include <memory>

template <typename T, typename Comparator>
class Iterator;

template <typename T, typename Comparator = std::less<T>>
class Set
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
    using iterator          = Iterator<T, Comparator>;
private:
    struct Node
    {
        Node(const T &data)
        : data(data), height(1)
        {
        }
        std::shared_ptr<Node> Next();
        std::shared_ptr<Node> Prev();

        T 		                data;
        size_t 	                height;
        std::shared_ptr<Node>	left;	
        std::shared_ptr<Node> 	right;
        std::shared_ptr<Node> 	parent;
    };
public:
    Set();
    ~Set();
    size_type   size() const;
    bool        empty() const;
    void 	    Insert(const_reference);
    bool 	    Has(const_reference) const;
    void 	    Erase(const_reference);
    iterator    begin() const;
    iterator    end() const;
    
private:
    friend class Iterator<T, Comparator>;
    std::shared_ptr<Node>	m_root;
    Comparator              m_cmp;
    size_type               m_size = 0;

    std::shared_ptr<Node> 	eraseInternal(std::shared_ptr<Node> node, const_reference data);
    std::shared_ptr<Node> 	insertInternal(std::shared_ptr<Node> node, const_reference data);
    
    std::shared_ptr<Node> 	detachReplacement(std::shared_ptr<Node> node);
    std::shared_ptr<Node> 	findReplacement(std::shared_ptr<Node> node) const;
	
    std::shared_ptr<Node> 	rotateLeft(std::shared_ptr<Node> node);
    std::shared_ptr<Node> 	rotateRight(std::shared_ptr<Node> node);
	
    size_t 	getHeight(std::shared_ptr<Node> node) const;
    void 	fixHeight(std::shared_ptr<Node> node);

    int 	getBalance(std::shared_ptr<Node> node) const;
    std::shared_ptr<Node> 	doBalance(std::shared_ptr<Node> node);
    
};

#include "set_definition.h"

