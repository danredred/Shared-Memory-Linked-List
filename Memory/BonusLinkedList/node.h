#pragma once
#include "pnode.hpp"
#include <iostream>
#include <string>

class Node
{
public:
    int _value;
    PNode<Node> _next;
    std::string print(const int currentIndex = -1) const;
};

Node* PNode<Node>::buffer;

void insert(PNode<Node>& head, const int value, const int index)
{
    if (index <= 0 || head.getIndex() == 0) {
        PNode<Node> nNode(++PNode<Node>::buffer->_value);
        nNode->_next = head;
        nNode->_value = value;
        head = nNode;
        return;
    }
    insert(head->_next, value, index - 1);
}

void remove(PNode<Node>& head, const int index) {
    if (head.getIndex() == 0) { std::cout << "Nothing to remove!" << std::endl;return; } // end if reached end (work on empty list)
    if (index <= 0 || head->_next.getIndex() == 0) {
        PNode<Node> delNode(head);
        head = delNode->_next; // removed from list
        PNode<Node>* l;
        for (l = &PNode<Node>::buffer->_next; (*l)->_next.getIndex() != 0; l = &(*l)->_next); // get the last node
        delNode->_value = (*l)->_value; // move the last node to the location that had been freed
        *l = delNode;
        delNode->_next = (DWORD)0;
        PNode<Node>::buffer->_value--;
        return;
    }
    remove(head->_next, index - 1);
}

std::string Node::print(const int currentIndex) const {
    return ((currentIndex == -1) ? "Size: " : "\n[" + std::to_string(currentIndex) + "] - ") + std::to_string(_value ) + ((_next.getIndex() == 0) ? "\n-----------" : _next->print(currentIndex + 1));
}
