#ifndef LIBKIWI_PRIM_OPTIONAL_H
#define LIBKIWI_PRIM_OPTIONAL_H
#include <algorithm>
#include <libkiwi/k_types.h>

namespace kiwi {
//! @addtogroup libkiwi_prim
//! @{

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
     */
    Optional() : mHasValue(false) {}

    /**
     * @brief Constructor
     * @details Null constructor
     */
    Optional(NullOpt) : mHasValue(false) {}

    /**
     * @brief Constructor
     * @details Copy constructor
     *
     * @param rOther Optional value
     */
    Optional(const Optional& rOther) : mHasValue(rOther.HasValue()) {
        if (mHasValue) {
            new (mBuffer) T(*rOther);
        }
    }

    /**
     * @brief Constructor
     * @details Copy constructor
     *
     * @param rValue Value
     */
    Optional(const T& rValue) : mHasValue(true) {
        new (mBuffer) T(rValue);
    }

#ifdef LIBKIWI_CPP1X
    /**
     * @brief Constructor
     * @details Move constructor
     *
     * @param rOther Optional value
     */
    Optional(const Optional&& rOther) : mHasValue(rOther.HasValue()) {
        if (mHasValue) {
            new (mBuffer) T(std::move(*rOther));
        }
    }

    /**
     * @brief Constructor
     * @details Move constructor
     *
     * @param rValue Optional value
     */
    Optional(const T&& rValue) {
        new (mBuffer) T(std::move(rValue));
    }
#endif

    /**
     * @brief Destructor
     */
    ~Optional() {
        Reset();
    }

    /**
     * @brief Value assignment
     *
     * @param rValue New value
     */
    Optional& operator=(const T& rValue) {
        new (mBuffer) T(rValue);
        mHasValue = true;
        return *this;
    }

#ifdef LIBKIWI_CPP1X
    /**
     * @brief Value assignment
     *
     * @param rValue New value
     */
    Optional& operator=(const T&& rValue) {
        new (mBuffer) T(std::move(rValue));
        mHasValue = true;
        return *this;
    }
#endif

    /**
     * @brief Tests whether a value is present
     */
    bool HasValue() const {
        return mHasValue;
    }

    /**
     * @brief Accesses optional value
     */
    T& Value() {
        K_ASSERT_EX(mHasValue, "Please check HasValue");
        return reinterpret_cast<T&>(mBuffer);
    }
    /**
     * @brief Accesses optional value (read-only)
     */
    const T& Value() const {
        K_ASSERT_EX(mHasValue, "Please check HasValue");
        return reinterpret_cast<const T&>(mBuffer);
    }

    /**
     * @brief Accesses optional value (or default value if not present)
     *
     * @param rValue Default value to use when this contains no value
     */
    const T& ValueOr(const T& rValue) const {
        return mHasValue ? Value() : rValue;
    }

    /**
     * @brief Constructs value in-place
     */
    T& Emplace() {
        K_ASSERT(!mHasValue);

        new (mBuffer) T();
        mHasValue = true;
        return Value();
    }

    /**
     * @brief Destroys existing value
     */
    void Reset() {
        if (mHasValue) {
            Value().~T();
        }

        mHasValue = false;
    }

    // clang-format off
    operator bool() const { return HasValue(); }
    
    T&       operator*()       { return Value(); }
    const T& operator*() const { return Value(); }

    T*       operator->()       { return &Value(); }
    const T* operator->() const { return &Value(); }
    // clang-format on

private:
    u8 mBuffer[sizeof(T)]; // Don't automatically construct T
    bool mHasValue;        // Whether the optional has a value
};

namespace {
//! @addtogroup libkiwi_prim
//! @{

/**
 * @brief Optional construction helper
 *
 * @param rValue Value
 */
template <typename T> Optional<T> MakeOptional(const T& rValue) {
    return Optional<T>(rValue);
}

/**
 * @brief Optional construction helper
 *
 * @param pValue Value (may be nullptr)
 */
template <typename T> Optional<T> MakeOptional(const T* pValue) {
    return pValue ? Optional<T>(*pValue) : kiwi::nullopt;
}

//! @}
} // namespace
//! @}
} // namespace kiwi

#endif
