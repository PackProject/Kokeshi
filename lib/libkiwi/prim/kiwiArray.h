#ifndef LIBKIWI_PRIM_ARRAY_H
#define LIBKIWI_PRIM_ARRAY_H
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>

namespace kiwi {
//! @addtogroup libkiwi_prim
//! @{

/**
 * @brief 1D array of fixed size
 *
 * @tparam T Array element type
 * @tparam N Array size
 */
template <typename T, int N> class TArray {
public:
    /**
     * @brief Access array length
     */
    int Length() const {
        return N;
    }

    /**
     * @brief Access array element by index
     *
     * @param i Element index
     */
    T& At(int i) {
        K_ASSERT(i >= 0 && i < N);
        return mData[i];
    }
    /**
     * @brief Access array element by index (read-only)
     *
     * @param i Element index
     */
    const T& At(int i) const {
        K_ASSERT(i >= 0 && i < N);
        return mData[i];
    }

    // clang-format off
    T&       operator[](int i)       { return At(i); }
    T&       operator()(int i)       { return At(i); }
    const T& operator[](int i) const { return At(i); }
    const T& operator()(int i) const { return At(i); }
    // clang-format on

private:
    T mData[N]; // Internal array
};

/**
 * @brief 2D array of fixed size
 *
 * @tparam T Array element type
 * @tparam M Outer array size
 * @tparam N Inner array size
 */
template <typename T, int M, int N> class TArray2D {
public:
    /**
     * @brief Access array length
     */
    int Length() const {
        return M;
    }

    /**
     * @brief Access array element by index
     *
     * @param i Element index
     */
    TArray<T, N>& At(int i) {
        K_ASSERT(i >= 0 && i < N);
        return mData[i];
    }
    /**
     * @brief Access array element by index (read-only)
     *
     * @param i Element index
     */
    const TArray<T, N>& At(int i) const {
        K_ASSERT(i >= 0 && i < N);
        return mData[i];
    }

    /**
     * @brief Access array element by index
     *
     * @param i Element row
     * @param j Element column
     */
    T& At(int i, int j) {
        K_ASSERT(i >= 0 && i < M);
        K_ASSERT(j >= 0 && j < N);
        return mData[i][j];
    }
    /**
     * @brief Access array element by index (read-only)
     *
     * @param i Element row
     * @param j Element column
     */
    const T& At(int i, int j) const {
        K_ASSERT(i >= 0 && i < M);
        K_ASSERT(j >= 0 && j < N);
        return mData[i][j];
    }

    // clang-format off
    TArray<T, N>&       operator[](int i)       { return At(i); }
    TArray<T, N>&       operator()(int i)       { return At(i); }
    const TArray<T, N>& operator[](int i) const { return At(i); }
    const TArray<T, N>& operator()(int i) const { return At(i); }

    TArray<T, N>&       operator()(int i, int j)       { return At(i).At(j); }
    const TArray<T, N>& operator()(int i, int j) const { return At(i).At(j); }
    // clang-format on

private:
    TArray<T, N> mData[M]; // Internal array
};

//! @}
} // namespace kiwi

#endif
