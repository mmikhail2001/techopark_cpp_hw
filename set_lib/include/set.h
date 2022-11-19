#pragma once

#include <iostream>
#include <memory>

#include "iterator.h"

template <typename T, typename Comparator = std::less<T>>
class Set
{
    struct Node
    {
        Node(const T &data)
        : data(data), height(1)
        {
        }
        std::shared_ptr<Node> Next();
        std::shared_ptr<Node> Prev();
        
        T 		data;
        size_t 	height;
        std::shared_ptr<Node>	left;	
        std::shared_ptr<Node> 	right;
        std::shared_ptr<Node> 	parent;
        friend class            Iterator<T>;
    };
    
public:

    using key_type          = T;
    using value_type        = T;
    using size_type         = std::size_t;
    using difference_type   = std::ptrdiff_t;
    using reference         = T&;
    using const_reference   = const T&;
    using iterator          = Iterator<T>;
    // using const_iterator = 
    // using reverse_iterator = 
    // using const_reverse_iterator = 
    


    Set();
    ~Set();
    void 	Insert(const T &data);
    bool 	Has(const T &data) const;
    void 	Erase(const T &data);
    std::shared_ptr<Node> begin() const;
    std::shared_ptr<Node> end() const;
    
private:
    std::shared_ptr<Node>	m_root;
    Comparator              m_cmp;
    // friend class            Iterator<T>;
    
    std::shared_ptr<Node> 	eraseInternal(std::shared_ptr<Node> node, const T &data);
    std::shared_ptr<Node> 	insertInternal(std::shared_ptr<Node> node, const T &data);
    
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

