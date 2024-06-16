// Implementation header
#ifndef LIBKIWI_PRIM_HASHMAP_IMPL_HPP
#define LIBKIWI_PRIM_HASHMAP_IMPL_HPP

// Declaration header
#ifndef LIBKIWI_PRIM_HASHMAP_H
#include <libkiwi/prim/kiwiHashMap.h>
#endif

namespace kiwi {

/**
 * @brief Pre-increment operator
 */
template <typename TKey, typename TValue>
TMap<TKey, TValue>::ConstIterator&
TMap<TKey, TValue>::ConstIterator::operator++() {
    // Can't iterate
    if (mpIter == nullptr) {
        return *this;
    }

    // Increment
    mpIter = mpIter->pChained;

    // Find next non-empty chain
    while (true) {
        // End of chain, advance to next bucket
        if (mpIter == nullptr) {
            if (++mIndex >= mCapacity) {
                break;
            }

            mpIter = &mpBuckets[mIndex];
        }

        // Did we find an item?
        K_ASSERT(mpIter != nullptr);
        if (mpIter->used) {
            break;
        }

        // Keep searching
        mpIter = mpIter->pChained;
    }

    return *this;
}

/**
 * @brief Constructor
 * @details Copy constructor
 *
 * @param rOther Map to copy
 */
template <typename TKey, typename TValue>
TMap<TKey, TValue>::TMap(const TMap& rOther) : mCapacity(rOther.mCapacity) {
    K_ASSERT(mCapacity > 0);
    K_ASSERT(mCapacity < HASH_MAX);

    // Create buckets for copy
    mpBuckets = new Bucket[mCapacity];
    K_ASSERT(mpBuckets != nullptr);

    // Re-insert all members
    for (ConstIterator it = rOther.Begin(); it != rOther.End(); ++it) {
        Insert(it.Key(), it.Value());
    }
}

/**
 * @brief Remove a key
 *
 * @param rKey Key
 * @param[out] pRemoved Removed value
 * @return Success
 */
template <typename TKey, typename TValue>
bool TMap<TKey, TValue>::Remove(const TKey& rKey, TValue* pRemoved) {
    Bucket* pBucket = Search(rKey);

    // Can't remove, doesn't exist
    if (pBucket == nullptr) {
        return false;
    }

    // Write out value about to be removed
    if (pRemoved != nullptr) {
        *pRemoved = *pBucket->value;
    }

    // Just mark as unused
    pBucket->used = false;
    mSize--;
    return true;
}

/**
 * @brief Find key in hashmap
 *
 * @param rKey Key
 */
template <typename TKey, typename TValue>
TMap<TKey, TValue>::Bucket* TMap<TKey, TValue>::Search(const TKey& rKey) const {
    // Calculate bucket index
    u32 i = Hash(rKey) % mCapacity;

    // Iterate through chains
    for (Bucket* pIt = &mpBuckets[i]; pIt != nullptr; pIt = pIt->pChained) {
        // Unused entry
        if (!pIt->used) {
            continue;
        }

        // Matches key
        if (*pIt->key == rKey) {
            return pIt;
        }
    }

    return nullptr;
}

/**
 * @brief Create key in hashmap
 *
 * @param rKey Key
 */
template <typename TKey, typename TValue>
TMap<TKey, TValue>::Bucket& TMap<TKey, TValue>::Create(const TKey& rKey) {
    // Calculate bucket index
    u32 i = Hash(rKey) % mCapacity;

    // Iterate through chains
    Bucket* pLast = nullptr;
    for (Bucket* pIt = &mpBuckets[i]; pIt != nullptr; pIt = pIt->pChained) {
        // Unused entry
        if (!pIt->used) {
            // Override this entry
            pIt->key = rKey;
            pIt->value.Emplace();
            pIt->used = true;
            mSize++;
            return *pIt;
        }

        // Matches key
        if (*pIt->key == rKey) {
            return *pIt;
        }

        pLast = pIt;
    }

    // Chain new bucket
    K_ASSERT(pLast != nullptr);
    pLast->pChained = new Bucket();
    K_ASSERT(pLast->pChained != nullptr);

    pLast->pChained->key = rKey;
    pLast->pChained->value.Emplace();
    pLast->pChained->used = true;
    mSize++;
    return *pLast->pChained;
}

} // namespace kiwi

#endif
