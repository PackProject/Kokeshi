#ifndef LIBKIWI_PRIM_LINKLIST_H
#define LIBKIWI_PRIM_LINKLIST_H
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>
#include <libkiwi/util/kiwiNonCopyable.h>

namespace kiwi {
//! @addtogroup libkiwi_prim
//! @{

/**
 * @brief Templated linked-list node
 * @note List node DOES NOT OWN ELEMENT
 */
template <typename T> class TListNode : private NonCopyable {
    // List node should be completely hidden outside list implementation
    template <typename> friend class TList;
    friend class TList<T>::Iterator;
    friend class TList<T>::ConstIterator;

private:
    /**
     * @brief Constructor
     *
     * @param pElem Node element
     */
    explicit TListNode(T* pElem)
        : mpNext(nullptr), mpPrev(nullptr), mpElement(pElem) {}

    TListNode<T>* mpNext; // Next node in the linked-list
    TListNode<T>* mpPrev; // Previous node in the linked-list
    T* mpElement;         // Underlying element
};

/**
 * @brief Templated linked-list
 */
template <typename T> class TList : private NonCopyable {
public:
    // Forward declarations
    class ConstIterator;

    /**
     * @brief Linked-list iterator
     */
    class Iterator {
        template <typename> friend class TList;
        friend class ConstIterator;

    public:
        /**
         * @brief Constructor
         *
         * @param pNode Iterator node
         */
        explicit Iterator(TListNode<T>* pNode) : mpNode(pNode) {
            K_ASSERT(mpNode);
        }

        /**
         * @brief Pre-increment operator
         */
        Iterator& operator++() {
            mpNode = mpNode->mpNext;
            return *this;
        }
        /**
         * @brief Post-increment operator
         */
        Iterator operator++(int) {
            Iterator clone(*this);
            ++*this;
            return clone;
        }

        /**
         * @brief Pre-decrement operator
         */
        Iterator& operator--() {
            mpNode = mpNode->mpPrev;
            return *this;
        }
        /**
         * @brief Post-decrement operator
         */
        Iterator operator--(int) {
            Iterator clone(*this);
            --*this;
            return clone;
        }

        // clang-format off
        T* operator->() const { return mpNode->mpElement; }
        T& operator*()  const { K_ASSERT(mpNode->mpElement != nullptr); return *mpNode->mpElement; }

        bool operator==(Iterator rhs) const { return mpNode == rhs.mpNode; }
        bool operator!=(Iterator rhs) const { return mpNode != rhs.mpNode; }
        // clang-format on

    private:
        TListNode<T>* mpNode;
    };

    /**
     * @brief Linked-list iterator (const view)
     */
    class ConstIterator {
        template <typename> friend class TList;

    public:
        /**
         * @brief Constructor
         *
         * @param pNode Iterator node
         */
        explicit ConstIterator(TListNode<T>* pNode) : mpNode(pNode) {
            K_ASSERT(mpNode != nullptr);
        }

        /**
         * @brief Constructor
         *
         * @param iter Iterator
         */
        ConstIterator(Iterator iter) : mpNode(iter.mpNode) {
            K_ASSERT(mpNode != nullptr);
        }

        /**
         * @brief Pre-increment operator
         */
        ConstIterator& operator++() {
            mpNode = mpNode->mpNext;
            return *this;
        }
        /**
         * @brief Post-increment operator
         */
        ConstIterator operator++(int) {
            ConstIterator clone(*this);
            ++*this;
            return clone;
        }

        /**
         * @brief Pre-decrement operator
         */
        ConstIterator& operator--() {
            mpNode = mpNode->mpPrev;
            return *this;
        }
        /**
         * @brief Post-decrement operator
         */
        ConstIterator operator--(int) {
            ConstIterator clone(*this);
            --*this;
            return clone;
        }

        // clang-format off
        const T* operator->() const { return mpNode->mpElement; }
        const T& operator*()  const { K_ASSERT(mpNode->mpElement != nullptr); return *mpNode->mpElement; }

        bool operator==(ConstIterator rhs) const { return mpNode == rhs.mpNode; }
        bool operator!=(ConstIterator rhs) const { return mpNode != rhs.mpNode; }
        // clang-format on

    private:
        TListNode<T>* mpNode;
    };

    typedef void (*ForEachFunc)(T& rElem);
    typedef void (*ForEachFuncConst)(const T& rElem);

public:
    /**
     * @brief Constructor
     */
    TList() : mSize(0), mEndNode(nullptr) {
        mEndNode.mpNext = &mEndNode;
        mEndNode.mpPrev = &mEndNode;
    }
    /**
     * @brief Destructor
     */
    ~TList() {
        Clear();
    }

    /**
     * @brief Erase all list elements
     */
    void Clear() {
        Erase(Begin(), End());
    }

    /**
     * @brief Gets iterator to beginning of list
     */
    Iterator Begin() {
        return Iterator(mEndNode.mpNext);
    }
    /**
     * @brief Gets iterator to beginning of list (const view)
     */
    ConstIterator Begin() const {
        return ConstIterator(const_cast<TList*>(this)->Begin());
    }

    /**
     * @brief Gets iterator to end of list
     */
    Iterator End() {
        return Iterator(&mEndNode);
    }
    /**
     * @brief Gets iterator to end of list (const-view)
     */
    ConstIterator End() const {
        return ConstIterator(const_cast<TList*>(this)->End());
    }

    /**
     * @brief Gets list size
     */
    u32 Size() const {
        return mSize;
    }

    /**
     * @brief Tests whether list is empty
     */
    bool Empty() const {
        return Size() == 0;
    }

    /**
     * @brief Erases beginning element from list
     */
    void PopFront() {
        Erase(Begin());
    }

    /**
     * @brief Erases end element from list
     */
    void PopBack() {
        Erase(--End());
    }

    /**
     * @brief Prepends element to front of list
     *
     * @param pElem New element
     */
    void PushFront(T* pElem) {
        Insert(Begin(), new TListNode<T>(pElem));
    }

    /**
     * @brief Appends element to end of list
     *
     * @param pElem New element
     */
    void PushBack(T* pElem) {
        Insert(End(), new TListNode<T>(pElem));
    }

    /**
     * @brief Gets reference to first element of list
     */
    T& Front() {
        K_ASSERT(!Empty());
        return *Begin();
    }
    /**
     * @brief Gets reference to first element of list (const-view)
     */
    const T& Front() const {
        K_ASSERT(!Empty());
        return *Begin();
    }

    /**
     * @brief Gets reference to last element of list
     */
    T& Back() {
        K_ASSERT(!Empty());
        return *--End();
    }
    /**
     * @brief Gets reference to last element of list (const-view)
     */
    const T& Back() const {
        K_ASSERT(!Empty());
        return *--End();
    }

    /**
     * @brief Inserts node at iterator
     *
     * @param iter Iterator at which to insert node
     * @param pNode Node to insert
     * @return Iterator to new node
     */
    Iterator Insert(Iterator iter, TListNode<T>* pNode);

    /**
     * @brief Erases node at iterator
     *
     * @param iter Iterator at which to erase node
     * @return Iterator to next node
     */
    Iterator Erase(Iterator iter) {
        Iterator copy(iter);
        return Erase(iter, ++copy);
    }

    /**
     * @brief Erases node from list
     *
     * @param pNode Node to erase
     * @return Iterator to next node
     */
    Iterator Erase(TListNode<T>* pNode);

    /**
     * @brief Erases range of nodes
     *
     * @param begin Beginning of range (inclusive)
     * @param end End of range (exclusive)
     * @return Iterator to end of range
     */
    Iterator Erase(Iterator begin, Iterator end);

    /**
     * @brief Removes first occurrence of element from list
     *
     * @param pElem
     */
    void Remove(const T* pElem) {
        for (Iterator it = Begin(); it != End(); ++it) {
            if (&*it == pElem) {
                Erase(it);
            }
        }
    }

    /**
     * @brief Applies the specified function to every element in the list
     *
     * @param pFunc For-each function
     */
    void ForEach(ForEachFunc pFunc) {
        K_ASSERT(pFunc != nullptr);

        for (Iterator it = Begin(); it != End(); ++it) {
            pFunc(it);
        }
    }

    /**
     * @brief Applies the specified function to every element in the list
     * (read-only)
     *
     * @param pFunc For-each function (const-view)
     */
    void ForEach(ForEachFuncConst pFunc) {
        K_ASSERT(pFunc != nullptr);

        for (Iterator it = Begin(); it != End(); ++it) {
            pFunc(it);
        }
    }

private:
    u32 mSize;             // List size
    TListNode<T> mEndNode; // List end node
};

//! @}
} // namespace kiwi

// Implementation header
#ifndef LIBKIWI_PRIM_LINKLIST_IMPL_HPP
#include <libkiwi/prim/kiwiLinkListImpl.hpp>
#endif

#endif
