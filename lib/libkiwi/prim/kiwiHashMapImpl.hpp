// Implementation header
#ifndef LIBKIWI_PRIM_HASHMAP_IMPL_HPP
#define LIBKIWI_PRIM_HASHMAP_IMPL_HPP

// Declaration header
#ifndef LIBKIWI_PRIM_HASHMAP_H
#include <libkiwi/prim/kiwiHashMap.h>
#endif

namespace kiwi {

/**
 * Pre-increment operator
 */
template <typename TKey, typename TValue>
TMap<TKey, TValue>::ConstIterator&
TMap<TKey, TValue>::ConstIterator::operator++() {
    // Can't iterate
    if (mpIter == NULL) {
        return *this;
    }

    // Increment
    mpIter = mpIter->chained;

    // Find next non-empty chain
    while (true) {
        // End of chain, advance to next bucket
        if (mpIter == NULL) {
            if (++mIndex >= mCapacity) {
                break;
            }

            mpIter = &mpBuckets[mIndex];
        }

        // Did we find an item?
        K_ASSERT(mpIter != NULL);
        if (mpIter->used) {
            break;
        }

        // Keep searching
        mpIter = mpIter->chained;
    }

    return *this;
}

/**
 * @brief Constructor
 *
 * @param other Map to copy
 */
template <typename TKey, typename TValue>
TMap<TKey, TValue>::TMap(const TMap& other) {
    // Copy capacity
    mCapacity = other.mCapacity;
    K_ASSERT(mCapacity > 0);
    K_ASSERT(mCapacity < HASH_MAX);

    // Create buckets for copy
    mpBuckets = new Bucket[mCapacity];
    K_ASSERT(mpBuckets != NULL);

    // Re-insert all members
    for (ConstIterator it = other.Begin(); it != other.End(); ++it) {
        Insert(it.Key(), it.Value());
    }
}

/**
 * @brief Remove a key
 *
 * @param key Key
 * @param[out] removed Removed value
 * @return Success
 */
template <typename TKey, typename TValue>
bool TMap<TKey, TValue>::Remove(const TKey& key, TValue* removed) {
    Bucket* bucket = Search(key);

    // Can't remove, doesn't exist
    if (bucket == NULL) {
        return false;
    }

    // Write out value about to be removed
    if (removed != NULL) {
        *removed = *bucket->value;
    }

    // Just mark as unused
    bucket->used = false;
    mSize--;
    return true;
}

/**
 * @brief Find key in hashmap
 *
 * @param key Key
 */
template <typename TKey, typename TValue>
TMap<TKey, TValue>::Bucket* TMap<TKey, TValue>::Search(const TKey& key) const {
    // Calculate bucket index
    u32 i = Hash(key) % mCapacity;

    // Iterate through chains
    for (Bucket* it = &mpBuckets[i]; it != NULL; it = it->chained) {
        // Unused entry
        if (!it->used) {
            continue;
        }

        // Matches key
        if (*it->key == key) {
            return it;
        }
    }

    return NULL;
}

/**
 * @brief Create key in hashmap
 *
 * @param key Key
 */
template <typename TKey, typename TValue>
TMap<TKey, TValue>::Bucket& TMap<TKey, TValue>::Create(const TKey& key) {
    // Calculate bucket index
    u32 i = Hash(key) % mCapacity;

    // Iterate through chains
    Bucket* last = NULL;
    for (Bucket* it = &mpBuckets[i]; it != NULL; it = it->chained) {
        // Unused entry
        if (!it->used) {
            // Override this entry
            it->key = key;
            it->value.Emplace();
            it->used = true;
            mSize++;
            return *it;
        }

        // Matches key
        if (*it->key == key) {
            return *it;
        }

        last = it;
    }

    // Chain new bucket
    K_ASSERT(last != NULL);
    last->chained = new Bucket();
    K_ASSERT(last->chained != NULL);

    last->chained->key = key;
    last->chained->value.Emplace();
    last->chained->used = true;
    mSize++;
    return *last->chained;
}

} // namespace kiwi

#endif
