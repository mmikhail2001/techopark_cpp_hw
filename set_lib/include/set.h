#pragma once

#include <iostream>
#include <memory>
#include <type_traits>
#include <optional>


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
        : data(data), height(1), left(nullptr), right(nullptr), parent(nullptr), next(nullptr), prev(nullptr)
        {
        }
        ~Node()
        {
            left = right = parent = next = prev = nullptr;
        }

        T 		                data;
        size_t 	                height;
        // для АВЛ-дерева
        Node*	left;	
        Node* 	right;
        Node* 	parent;
        // для двусвязного спика
        Node* 	next;
        Node* 	prev;
    };
public:
    void show();
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

    void 	        insert(const_reference data);
    void 	        erase(const_reference data);
    
    iterator 	    find(const_reference data) const;
    iterator 	    lower_bound(const_reference data) const;
    

    iterator        begin() const;
    iterator        end() const;
    
    bool            operator==(Set<T, Cmp> const &other) const;
    bool            operator!=(Set<T, Cmp> const &other) const;
private:
    friend class Iterator<T, Cmp>;
    // корень дерева
    Node*	m_root = nullptr;
    // узлы двусвязного списка
    Node*	m_first = nullptr;
    std::optional<key_type> m_first_value = std::nullopt;
    Node*	m_last = nullptr;
    std::optional<key_type> m_last_value = std::nullopt;

    Cmp                     m_cmp;
    size_type               m_size = 0;

    void                    setNodeEnd();

    Node* 	findInternal(const_reference data) const;
    Node* 	lower_boundInternal(const_reference data) const;
    Node* 	eraseInternal(Node* node, const_reference data);
    Node* 	insertInternal(Node* node, const_reference data);
    
    Node* 	detachReplacement(Node* node);
    Node* 	findReplacement(Node* node) const;
	
    Node* 	rotateLeft(Node* node);
    Node* 	rotateRight(Node* node);
	
    size_t 	                getHeight(Node* node) const;
    void 	                fixHeight(Node* node);

    int 	                getBalance(Node* node) const;
    Node* 	doBalance(Node* node);

    Node*   nextInternal(Node* node) const;
    Node*   prevInternal(Node* node) const;

    void                    Swap(Set &other);

    static Node*   getLeftMost(Node* node)
    {
        if (!node)
        {
            return nullptr;
        }
        while (node->left)
        {
            node = node->left;
        }
        return node;
    }
    static Node*   getRightMost(Node* node)
    {
        if (!node)
        {
            return nullptr;
        }
        while (node->right)
        {
            node = node->right;
        }
        return node;
    }


    
};


#include "set_definition.h"

