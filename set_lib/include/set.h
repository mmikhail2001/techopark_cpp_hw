#pragma once
#include <iostream>

template <typename T>
class Set
{
    struct Node
    {
        Node(const T &data)
        : data(data), height(1), left(nullptr), right(nullptr)
        {
        }
        
        T 		data;
        size_t 	height;
        Node*	left;
        Node*	right;
    };
    
public:
    Set();
    ~Set();
    void 	Insert(const T &data);
    bool 	Has(const T &data) const;
    void 	Erase(const T &data);
    
private:
    Node*	m_root;
    
    void 	destroyTree(Node *node);

    Node* 	eraseInternal(Node *node, const T &data);
    Node* 	insertInternal(Node *node, const T &data);
    Node* 	removeMin(Node *node);
    Node* 	findMin(Node *node);
	
    Node* 	rotateLeft(Node *node);
    Node* 	rotateRight(Node *node);
	
    size_t 	getHeight(Node *node);
    void 	fixHeight(Node *node);

    int 	getBalance(Node *node);
    Node* 	doBalance(Node *node);
    
};

#include "set_definition.h"

