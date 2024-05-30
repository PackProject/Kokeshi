#ifndef LIBKIWI_PRIM_LINKLIST_H
#define LIBKIWI_PRIM_LINKLIST_H
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiNonCopyable.h>

namespace kiwi {

/**
 * Templated linked-list node
 * @note List node DOES NOT OWN ELEMENT
 */
template <typename T> class TListNode : private NonCopyable {
    // List node should be completely hidden outside list implementation
    template <typename> friend class TList;
    friend class TList<T>::Iterator;
    friend class TList<T>::ConstIterator;

private:
    /**
     * Constructor
     *
     * @param elem Node element
     */
    explicit TListNode(T* elem) : mpNext(NULL), mpPrev(NULL), mpElement(elem) {}

    TListNode<T>* mpNext; // Next node in the linked-list
    TListNode<T>* mpPrev; // Previous node in the linked-list
    T* mpElement;         // Underlying element
};

/**
 * Templated linked-list
 */
template <typename T> class TList : private NonCopyable {
public:
    /**
     * Linked-list iterator
     */
    class Iterator {
        template <typename> friend class TList;
        friend class ConstIterator;

    public:
        /**
         * Constructor
         *
         * @param node Iterator node
         */
        explicit Iterator(TListNode<T>* node) : mpNode(node) {
            K_ASSERT(mpNode);
        }

        /**
         * Pre-increment operator
         */
        Iterator& operator++() {
            mpNode = mpNode->mpNext;
            return *this;
        }
        /**
         * Post-increment operator
         */
        Iterator operator++(int) {
            Iterator clone(*this);
            ++*this;
            return clone;
        }

        /**
         * Pre-decrement operator
         */
        Iterator& operator--() {
            mpNode = mpNode->mpPrev;
            return *this;
        }
        /**
         * Post-decrement operator
         */
        Iterator operator--(int) {
            Iterator clone(*this);
            --*this;
            return clone;
        }

        /**
         * Gets pointer to element
         */
        T* operator->() const {
            return mpNode->mpElement;
        }

        /**
         * Gets reference to element
         */
        T& operator*() const {
            K_ASSERT(mpNode->mpElement != NULL);
            return *mpNode->mpElement;
        }

        bool operator==(Iterator rhs) const {
            return mpNode == rhs.mpNode;
        }
        bool operator!=(Iterator rhs) const {
            return mpNode != rhs.mpNode;
        }

    private:
        TListNode<T>* mpNode;
    };

    /**
     * Linked-list iterator (const view)
     */
    class ConstIterator {
        template <typename> friend class TList;

    public:
        /**
         * Constructor
         *
         * @param node Iterator node
         */
        explicit ConstIterator(TListNode<T>* node) : mpNode(node) {
            K_ASSERT(mpNode);
        }

        /**
         * Constructor
         *
         * @param iter Iterator
         */
        ConstIterator(Iterator iter) : mpNode(iter.mpNode) {
            K_ASSERT(mpNode);
        }

        /**
         * Pre-increment operator
         */
        ConstIterator& operator++() {
            mpNode = mpNode->mpNext;
            return *this;
        }
        /**
         * Post-increment operator
         */
        ConstIterator operator++(int) {
            ConstIterator clone(*this);
            ++*this;
            return clone;
        }

        /**
         * Pre-decrement operator
         */
        ConstIterator& operator--() {
            mpNode = mpNode->mpPrev;
            return *this;
        }
        /**
         * Post-decrement operator
         */
        ConstIterator operator--(int) {
            ConstIterator clone(*this);
            --*this;
            return clone;
        }

        /**
         * Gets pointer to element
         */
        const T* operator->() const {
            return mpNode->mpElement;
        }

        /**
         * Gets reference to element
         */
        const T& operator*() const {
            return *mpNode->mpElement;
        }

        bool operator==(ConstIterator rhs) const {
            return mpNode == rhs.mpNode;
        }
        bool operator!=(ConstIterator rhs) const {
            return mpNode != rhs.mpNode;
        }

    private:
        TListNode<T>* mpNode;
    };

public:
    /**
     * Constructor
     */
    explicit TList() : mSize(0), mEndNode(NULL) {
        mEndNode.mpNext = &mEndNode;
        mEndNode.mpPrev = &mEndNode;
    }
    /**
     * Destructor
     */
    ~TList() {
        Clear();
    }

    /**
     * Erase all list elements
     */
    void Clear() {
        Erase(Begin(), End());
    }

    /**
     * Gets iterator to beginning of list
     */
    Iterator Begin() {
        return Iterator(mEndNode.mpNext);
    }
    /**
     * Gets iterator to beginning of list (const view)
     */
    ConstIterator Begin() const {
        return ConstIterator(Begin());
    }

    /**
     * Gets iterator to end of list
     */
    Iterator End() {
        return Iterator(&mEndNode);
    }
    /**
     * Gets iterator to end of list (const-view)
     */
    ConstIterator End() const {
        return ConstIterator(End());
    }

    /**
     * Gets list size
     */
    u32 GetSize() const {
        return mSize;
    }

    /**
     * Tests whether list is empty
     */
    bool Empty() const {
        return GetSize() == 0;
    }

    /**
     * Erases beginning element from list
     */
    void PopFront() {
        Erase(Begin());
    }

    /**
     * Erases end element from list
     */
    void PopBack() {
        Erase(--End());
    }

    /**
     * Prepends element to front of list
     *
     * @param elem New element
     */
    void PushFront(T* elem) {
        Insert(Begin(), new TListNode<T>(elem));
    }

    /**
     * Appends element to end of list
     *
     * @param elem New element
     */
    void PushBack(T* elem) {
        Insert(End(), new TListNode<T>(elem));
    }

    /**
     * Gets reference to first element of list
     */
    T& Front() {
        K_ASSERT(!Empty());
        return *Begin();
    }
    /**
     * Gets reference to first element of list (const-view)
     */
    const T& Front() const {
        K_ASSERT(!Empty());
        return *Begin();
    }

    /**
     * Gets reference to last element of list
     */
    T& Back() {
        K_ASSERT(!Empty());
        return *--End();
    }
    /**
     * Gets reference to last element of list (const-view)
     */
    const T& Back() const {
        K_ASSERT(!Empty());
        return *--End();
    }

    /**
     * Inserts node at iterator
     *
     * @param iter Iterator at which to insert node
     * @param node Node to insert
     * @return Iterator to new node
     */
    Iterator Insert(Iterator iter, TListNode<T>* node);

    /**
     * Erases node at iterator
     *
     * @param iter Iterator at which to erase node
     * @return Iterator to next node
     */
    Iterator Erase(Iterator iter) {
        Iterator copy(iter);
        return Erase(iter, ++copy);
    }

    /**
     * Erases node from list
     *
     * @param node Node to erase
     * @return Iterator to next node
     */
    Iterator Erase(TListNode<T>* node);

    /**
     * Erases range of nodes
     *
     * @param begin Beginning of range (inclusive)
     * @param end End of range (exclusive)
     * @return Iterator to end of range
     */
    Iterator Erase(Iterator begin, Iterator end);

    /**
     * Removes first occurrence of element from list
     *
     * @param elem
     */
    void Remove(const T* elem) {
        for (Iterator it = Begin(); it != End(); it++) {
            if (&*it == elem) {
                Erase(it);
            }
        }
    }

private:
    u32 mSize;             // List size
    TListNode<T> mEndNode; // List end node
};

} // namespace kiwi

// Implementation header
#ifndef LIBKIWI_PRIM_LINKLIST_IMPL_HPP
#include <libkiwi/prim/kiwiLinkListImpl.hpp>
#endif

#endif
