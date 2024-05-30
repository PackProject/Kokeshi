// Implementation header
#ifndef LIBKIWI_PRIM_LINKLIST_IMPL_HPP
#define LIBKIWI_PRIM_LINKLIST_IMPL_HPP

// Declaration header
#ifndef LIBKIWI_PRIM_LINKLIST_H
#include <libkiwi/prim/kiwiLinkList.h>
#endif

namespace kiwi {

/**
 * Inserts node at iterator
 *
 * @param iter Iterator at which to insert node
 * @param node Node to insert
 * @return Iterator to new node
 */
template <typename T>
TList<T>::Iterator TList<T>::Insert(Iterator iter, TListNode<T>* node) {
    K_ASSERT(node != NULL);

    TListNode<T>* next = iter.mpNode;
    TListNode<T>* prev = next->mpPrev;

    // prev <- node -> next
    node->mpNext = next;
    node->mpPrev = prev;
    // prev <-> node <-> next
    next->mpPrev = node;
    prev->mpNext = node;

    mSize++;

    return Iterator(node);
}

/**
 * Erases node from list
 *
 * @param node Node to erase
 * @return Iterator to next node
 */
template <typename T> TList<T>::Iterator TList<T>::Erase(TListNode<T>* node) {
    K_ASSERT(node != NULL);

    TListNode<T>* next = node->mpNext;
    TListNode<T>* prev = node->mpPrev;

    // Remove connections to node
    next->mpPrev = prev;
    prev->mpNext = next;
    // Isolate node
    node->mpNext = NULL;
    node->mpPrev = NULL;
    // Free memory
    delete node;

    mSize--;

    return Iterator(next);
}

/**
 * Erases range of nodes
 *
 * @param begin Beginning of range (inclusive)
 * @param end End of range (exclusive)
 * @return Iterator to end of range
 */
template <typename T>
TList<T>::Iterator TList<T>::Erase(Iterator begin, Iterator end) {
    TListNode<T>* pCur = begin.mpNode;
    TListNode<T>* pEnd = end.mpNode;

    while (pCur != pEnd) {
        // Preserve next node before erasing pointers
        TListNode<T>* pNext = pCur->mpNext;
        // Erase current node
        Erase(pCur);
        pCur = pNext;
    }

    return Iterator(pEnd);
}

} // namespace kiwi

#endif
