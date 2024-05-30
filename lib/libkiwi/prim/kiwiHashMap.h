#ifndef LIBKIWI_PRIM_HASHMAP_H
#define LIBKIWI_PRIM_HASHMAP_H
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiLinkList.h>
#include <libkiwi/prim/kiwiOptional.h>

namespace kiwi {

// Hash value type
typedef u32 hash_t;
// Largest representable hash
static const hash_t HASH_MAX = (1 << (sizeof(hash_t) * 8)) - 1;

hash_t HashImpl(const void* key, s32 len);

/**
 * @brief Key hasher
 * @note Specialize for your custom types
 */
template <typename TKey> inline hash_t Hash(const TKey& key) {
    return HashImpl(&key, sizeof(TKey));
}

/**
 * @brief Key/value map
 *
 * TODO: Track load factor, expand + re-hash when high
 */
template <typename TKey, typename TValue> class TMap {
    friend class ConstIterator; // Access 'Bucket' structure

public:
    // Default bucket count
    static const u32 DEFAULT_CAPACITY = 32;

private:
    struct Bucket {
        /**
         * @brief Constructor
         */
        Bucket() : used(false), chained(NULL) {}

        /**
         * @brief Destructor
         */
        ~Bucket() {
            // Recurse
            delete chained;
        }

        // Key/value pair
        Optional<TKey> key;
        Optional<TValue> value;

        bool used;       // Bucket is in use
        Bucket* chained; // Chains
    };

public:
    class ConstIterator {
        template <typename, typename> friend class TMap;

    private:
        /**
         * @brief Constructor
         *
         * @param capacity Map capacity
         * @param buckets Array of buckets
         */
        ConstIterator(u32 capacity, const Bucket* buckets)
            : mIndex(0),
              mCapacity(capacity),
              mpBuckets(buckets),
              mpIter(mpBuckets) {
            // Find first non-empty value
            if (mpIter != NULL && !mpIter->used) {
                ++*this;
            }
        }

    public:
        /**
         * Pre-increment operator
         */
        ConstIterator& operator++();
        /**
         * Post-increment operator
         */
        ConstIterator operator++(int) {
            ConstIterator clone(*this);
            ++*this;
            return clone;
        }

        /**
         * Get key from this element
         */
        const TKey& Key() const {
            K_ASSERT(mpIter != NULL);
            return *mpIter->key;
        }
        /**
         * Get value from this element
         */
        const TValue& Value() const {
            K_ASSERT(mpIter != NULL);
            return *mpIter->value;
        }

        /**
         * Gets pointer to this value
         */
        const TValue* operator->() const {
            return &Value();
        }
        /**
         * Gets reference to this value
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
    TMap(u32 capacity = DEFAULT_CAPACITY)
        : mSize(0), mCapacity(capacity), mpBuckets(NULL) {
        K_ASSERT(mCapacity > 0);
        K_ASSERT(mCapacity < HASH_MAX);

        mpBuckets = new Bucket[mCapacity];
        K_ASSERT(mpBuckets != NULL);
    }

    /**
     * @brief Constructor
     *
     * @param other Map to copy
     */
    TMap(const TMap& other);

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
     * @param key Key
     * @return Existing value, or new entry
     */
    TValue& operator[](const TKey& key) {
        return *Create(key).value;
    }

    /**
     * @brief Insert a new key or update an existing value
     *
     * @param key Key
     * @param value Value
     */
    void Insert(const TKey& key, const TValue& value) {
        Create(key).value = value;
    }

    /**
     * @brief Remove a key
     *
     * @param key Key
     * @param[out] removed Removed value
     * @return Success
     */
    bool Remove(const TKey& key, TValue* removed = NULL);

    /**
     * @brief Look for the value corresponding to a key
     *
     * @param key Key
     * @return Value if it exists
     */
    TValue* Find(const TKey& key) const {
        Bucket* bucket = Search(key);
        return bucket != NULL ? &*bucket->value : NULL;
    }

    /**
     * @brief Check whether a key exists
     *
     * @param key Key
     */
    bool Contains(const TKey& key) const {
        return Find(key) != NULL;
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
     * Gets iterator to beginning of map (const view)
     */
    ConstIterator Begin() const {
        return ConstIterator(mCapacity, mpBuckets);
    }

    /**
     * Gets iterator to end of map (const-view)
     */
    ConstIterator End() const {
        return ConstIterator(0, NULL);
    }

private:
    Bucket* Search(const TKey& key) const;
    Bucket& Create(const TKey& key);

private:
    u32 mSize;         // Number of elements
    u32 mCapacity;     // Maximum number of elements
    Bucket* mpBuckets; // Element buckets
};

} // namespace kiwi

// Implementation header
#ifndef LIBKIWI_PRIM_HASHMAP_IMPL_HPP
#include <libkiwi/prim/kiwiHashMapImpl.hpp>
#endif

#endif
