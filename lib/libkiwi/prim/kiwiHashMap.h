#ifndef LIBKIWI_PRIM_HASHMAP_H
#define LIBKIWI_PRIM_HASHMAP_H
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiLinkList.h>
#include <libkiwi/prim/kiwiOptional.h>

namespace kiwi {
//! @addtogroup libkiwi_prim
//! @{

// Hash value type
typedef u32 hash_t;
// Largest representable hash
static const hash_t HASH_MAX = (1 << (sizeof(hash_t) * 8)) - 1;

/**
 * @brief Hashes data of a specified size
 * @details MurmurHash3 algorithm
 *
 * @param pKey Key
 * @param len Key length
 */
hash_t HashImpl(const void* pKey, s32 len);

/**
 * @brief Hashes a key of any type
 * @note Specialize this for your custom types
 *
 * @param rKey Key
 */
template <typename TKey> K_INLINE hash_t Hash(const TKey& rKey) {
    return HashImpl(&rKey, sizeof(TKey));
}

/**
 * @brief Key/value pair map
 *
 * @todo Track load factor, expand + re-hash when high
 */
template <typename TKey, typename TValue> class TMap {
    friend class ConstIterator; // Access 'Bucket' structure

public:
    // Default bucket count
    static const u32 scDefaultCapacity = 32;

private:
    struct Bucket {
        /**
         * @brief Constructor
         */
        Bucket() : used(false), pChained(nullptr) {}

        /**
         * @brief Destructor
         */
        ~Bucket() {
            // Recurse
            delete pChained;
        }

        Optional<TKey> key;
        Optional<TValue> value;

        bool used;        // Bucket is in use
        Bucket* pChained; // Chains
    };

public:
    class ConstIterator {
        template <typename, typename> friend class TMap;

    private:
        /**
         * @brief Constructor
         *
         * @param capacity Map capacity
         * @param pBuckets Array of buckets
         */
        ConstIterator(u32 capacity, const Bucket* pBuckets)
            : mIndex(0),
              mCapacity(capacity),
              mpBuckets(pBuckets),
              mpIter(mpBuckets) {
            // Find first non-empty value
            if (mpIter != nullptr && !mpIter->used) {
                ++*this;
            }
        }

    public:
        /**
         * @brief Pre-increment operator
         */
        ConstIterator& operator++();
        /**
         * @brief Post-increment operator
         */
        ConstIterator operator++(int) {
            ConstIterator clone(*this);
            ++*this;
            return clone;
        }

        /**
         * @brief Get key from this element
         */
        const TKey& Key() const {
            K_ASSERT(mpIter != nullptr);
            return *mpIter->key;
        }
        /**
         * @brief Get value from this element
         */
        const TValue& Value() const {
            K_ASSERT(mpIter != nullptr);
            return *mpIter->value;
        }

        /**
         * @brief Gets pointer to this value
         */
        const TValue* operator->() const {
            return &Value();
        }
        /**
         * @brief Gets reference to this value
         */
        const TValue& operator*() const {
            return Value();
        }

        bool operator==(ConstIterator rhs) const {
            return mpIter == rhs.mpIter;
        }
        bool operator!=(ConstIterator rhs) const {
            return mpIter != rhs.mpIter;
        }

    private:
        u32 mIndex;    // Current bucket index
        u32 mCapacity; // Maximum bucket index

        const Bucket* mpBuckets; // Current bucket root
        const Bucket* mpIter;    // Current bucket chain
    };

public:
    /**
     * @brief Constructor
     *
     * @param capacity Starting number of buckets
     */
    TMap(u32 capacity = scDefaultCapacity)
        : mSize(0), mCapacity(capacity), mpBuckets(nullptr) {
        K_ASSERT(mCapacity > 0);
        K_ASSERT(mCapacity < HASH_MAX);

        mpBuckets = new Bucket[mCapacity];
        K_ASSERT(mpBuckets != nullptr);
    }

    /**
     * @brief Constructor
     * @details Copy constructor
     *
     * @param rOther Map to copy
     */
    TMap(const TMap& rOther);

    /**
     * @brief Destructor
     */
    ~TMap() {
        delete[] mpBuckets;
    }

    /**
     * @brief Access a value by key
     * @note Inserts key if it does not already exist
     *
     * @param rKey Key
     * @return Existing value, or new entry
     */
    TValue& operator[](const TKey& rKey) {
        return *Create(rKey).value;
    }

    /**
     * @brief Insert a new key or update an existing value
     *
     * @param rKey Key
     * @param rValue Value
     */
    void Insert(const TKey& rKey, const TValue& rValue) {
        Create(rKey).value = rValue;
    }

    /**
     * @brief Remove a key
     *
     * @param rKey Key
     * @param[out] pRemoved Removed value
     * @return Success
     */
    bool Remove(const TKey& rKey, TValue* pRemoved = nullptr);

    /**
     * @brief Look for the value corresponding to a key
     *
     * @param rKey Key
     * @return Value if it exists
     */
    TValue* Find(const TKey& rKey) const {
        Bucket* pBucket = Search(rKey);

        if (pBucket != nullptr) {
            return &*pBucket->value;
        }

        return nullptr;
    }

    /**
     * @brief Check whether a key exists
     *
     * @param rKey Key
     */
    bool Contains(const TKey& rKey) const {
        return Find(rKey) != nullptr;
    }

    /**
     * @brief Get number of elements in the map
     */
    u32 Size() const {
        return mSize;
    }

    /**
     * @brief Check whether the map contains no elements
     */
    bool Empty() const {
        return Size() == 0;
    }

    /**
     * @brief Gets list of keys in the map
     */
    TList<TKey> Keys() const {
        TList<TKey> keys;

        for (ConstIterator it = Begin(); it != End(); ++it) {
            keys.PushBack(it.Key());
        }

        return keys;
    }

    /**
     * @brief Gets list of values in the map
     */
    TList<TValue> Values() const {
        TList<TValue> values;

        for (ConstIterator it = Begin(); it != End(); ++it) {
            values.PushBack(it.Value());
        }

        return values;
    }

    /**
     * @brief Gets iterator to beginning of map (const view)
     */
    ConstIterator Begin() const {
        return ConstIterator(mCapacity, mpBuckets);
    }

    /**
     * @brief Gets iterator to end of map (const-view)
     */
    ConstIterator End() const {
        return ConstIterator(0, nullptr);
    }

private:
    Bucket* Search(const TKey& rKey) const;
    Bucket& Create(const TKey& rKey);

private:
    u32 mSize;         // Number of elements
    u32 mCapacity;     // Maximum number of elements
    Bucket* mpBuckets; // Element buckets
};

//! @}
} // namespace kiwi

// Implementation header
#ifndef LIBKIWI_PRIM_HASHMAP_IMPL_HPP
#include <libkiwi/prim/kiwiHashMapImpl.hpp>
#endif

#endif
