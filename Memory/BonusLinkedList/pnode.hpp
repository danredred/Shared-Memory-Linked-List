#pragma once
#include <Windows.h>

/// <summary>
/// A class that mannages the convertion between the shared memory buffer to pointer
/// </summary>
template<class T>
class PNode
{
public:
    static T* buffer;
    PNode(DWORD index = 0) :_index(index) {};
    PNode& operator=(const PNode& other) { _index = other.getIndex();return *this; };
    PNode& operator=(const DWORD other) { _index = other;return *this; };
    PNode& operator=(const T* other) { _index =  other- buffer;return *this; };
    T& operator*() const { return buffer[_index]; };
    T* operator->() const { return &buffer[_index]; };
    bool operator==(const T* other) const { return &**this == other; };
    bool operator!=(const T* other) const { return &**this != other; };
    DWORD getIndex() const { return _index; };
private:
    DWORD _index;
};

template<class T>T* PNode<T>::buffer = nullptr;
