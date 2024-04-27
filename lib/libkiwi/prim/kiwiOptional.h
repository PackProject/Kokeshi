#ifndef LIBKIWI_PRIM_OPTIONAL_H
#define LIBKIWI_PRIM_OPTIONAL_H
#include <types.h>

namespace kiwi {

/**
 * @brief Helper for Optional with no value
 */
struct NullOpt {};
static const NullOpt nullopt = NullOpt();

/**
 * @brief std::optional for C++03
 */
template <typename T> class Optional {
public:
    /**
     * @brief Constructor
     *
     * @param t Optional value
     */
    Optional(const T& t) : mHasValue(true) {
        // Copy construct in-place
        new (mBuffer) T(t);
    }

    /**
     * @brief Constructor
     */
    Optional() : mHasValue(false) {}

    /**
     * @brief Constructor
     */
    Optional(NullOpt) : mHasValue(false) {}

    /**
     * @brief Copy constructor
     *
     * @param other Optional value
     */
    Optional(const Optional& other) : mHasValue(other.HasValue()) {
        // Copy construct in-place
        if (mHasValue) {
            new (mBuffer) T(*other);
        }
    }

    /**
     * @brief Value assignment
     *
     * @param t New value
     */
    Optional& operator=(const T& t) {
        new (mBuffer) T(t);
        mHasValue = true;
        return *this;
    }

    // Check for optional value
    bool HasValue() const {
        return mHasValue;
    }
    operator bool() const {
        return HasValue();
    }

    // Access optional value
    T& Value() {
        K_ASSERT_EX(mHasValue, "Please check HasValue");
        return reinterpret_cast<T&>(mBuffer);
    }
    const T& Value() const {
        K_ASSERT_EX(mHasValue, "Please check HasValue");
        return reinterpret_cast<const T&>(mBuffer);
    }

    // Access optional value (or 'val' if none)
    T& ValueOr(const T& val) {
        return HasValue() ? Value() : val;
    }
    const T& ValueOr(const T& val) const {
        return HasValue() ? Value() : val;
    }

    // Destroy value
    void Reset() {
        if (HasValue()) {
            Value().~T();
        }

        mHasValue = false;
    }

    // Dereference access
    T& operator*() {
        return Value();
    }
    const T& operator*() const {
        return Value();
    }

    // Pointer access
    T* operator->() {
        return &this->Value();
    }
    const T* operator->() const {
        return &this->Value();
    }

private:
    // Don't automatically construct T
    u8 mBuffer[sizeof(T)];
    // Whether the optional has a value
    bool mHasValue;
};

} // namespace kiwi

#endif
