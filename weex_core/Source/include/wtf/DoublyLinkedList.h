/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
#ifndef DoublyLinkedList_h
#define DoublyLinkedList_h

namespace WTF {

// This class allows nodes to share code without dictating data member layout.
template<typename T> class DoublyLinkedListNode {
public:
    DoublyLinkedListNode();
    
    void setPrev(T*);
    void setNext(T*);
    
    T* prev() const;
    T* next() const;
};

template<typename T> inline DoublyLinkedListNode<T>::DoublyLinkedListNode()
{
    setPrev(0);
    setNext(0);
}

template<typename T> inline void DoublyLinkedListNode<T>::setPrev(T* prev)
{
    static_cast<T*>(this)->m_prev = prev;
}

template<typename T> inline void DoublyLinkedListNode<T>::setNext(T* next)
{
    static_cast<T*>(this)->m_next = next;
}

template<typename T> inline T* DoublyLinkedListNode<T>::prev() const
{
    return static_cast<const T*>(this)->m_prev;
}

template<typename T> inline T* DoublyLinkedListNode<T>::next() const
{
    return static_cast<const T*>(this)->m_next;
}

template<typename T> class DoublyLinkedList {
public:
    DoublyLinkedList();
    
    bool isEmpty() const;
    size_t size() const; // This is O(n).
    void clear();

    T* head() const;
    T* removeHead();

    T* tail() const;

    void push(T*);
    void append(T*);
    void remove(T*);
    void append(DoublyLinkedList<T>&);

private:
    T* m_head;
    T* m_tail;
};

template<typename T> inline DoublyLinkedList<T>::DoublyLinkedList()
    : m_head(0)
    , m_tail(0)
{
}

template<typename T> inline bool DoublyLinkedList<T>::isEmpty() const
{
    return !m_head;
}

template<typename T> inline size_t DoublyLinkedList<T>::size() const
{
    size_t size = 0;
    for (T* node = m_head; node; node = node->next())
        ++size;
    return size;
}

template<typename T> inline void DoublyLinkedList<T>::clear()
{
    m_head = 0;
    m_tail = 0;
}

template<typename T> inline T* DoublyLinkedList<T>::head() const
{
    return m_head;
}

template<typename T> inline T* DoublyLinkedList<T>::tail() const
{
    return m_tail;
}

template<typename T> inline void DoublyLinkedList<T>::push(T* node)
{
    if (!m_head) {
        ASSERT(!m_tail);
        m_head = node;
        m_tail = node;
        node->setPrev(0);
        node->setNext(0);
        return;
    }

    ASSERT(m_tail);
    m_head->setPrev(node);
    node->setNext(m_head);
    node->setPrev(0);
    m_head = node;
}

template<typename T> inline void DoublyLinkedList<T>::append(T* node)
{
    if (!m_tail) {
        ASSERT(!m_head);
        m_head = node;
        m_tail = node;
        node->setPrev(0);
        node->setNext(0);
        return;
    }

    ASSERT(m_head);
    m_tail->setNext(node);
    node->setPrev(m_tail);
    node->setNext(0);
    m_tail = node;
}

template<typename T> inline void DoublyLinkedList<T>::remove(T* node)
{
    if (node->prev()) {
        ASSERT(node != m_head);
        node->prev()->setNext(node->next());
    } else {
        ASSERT(node == m_head);
        m_head = node->next();
    }

    if (node->next()) {
        ASSERT(node != m_tail);
        node->next()->setPrev(node->prev());
    } else {
        ASSERT(node == m_tail);
        m_tail = node->prev();
    }
}

template<typename T> inline T* DoublyLinkedList<T>::removeHead()
{
    T* node = head();
    if (node)
        remove(node);
    return node;
}

template<typename T> inline void DoublyLinkedList<T>::append(DoublyLinkedList<T>& other)
{
    if (!other.head())
        return;

    if (!head()) {
        m_head = other.head();
        m_tail = other.tail();
        other.clear();
        return;
    }

    ASSERT(tail());
    ASSERT(other.head());
    T* otherHead = other.head();
    T* otherTail = other.tail();
    other.clear();

    ASSERT(!m_tail->next());
    m_tail->setNext(otherHead);
    ASSERT(!otherHead->prev());
    otherHead->setPrev(m_tail);
    m_tail = otherTail;
}

} // namespace WTF

using WTF::DoublyLinkedListNode;
using WTF::DoublyLinkedList;

#endif
