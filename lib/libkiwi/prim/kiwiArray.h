#ifndef LIBKIWI_PRIM_ARRAY_H
#define LIBKIWI_PRIM_ARRAY_H
#include <libkiwi/debug/kiwiAssert.h>
#include <libkiwi/k_types.h>

namespace kiwi {

/**
 * Templated array with bounds-checking in debug
 *
 * @tparam T Array element type
 * @tparam N Array size
 */
template <typename T, u32 N> class TArray {
public:
    u32 Length() const {
        return N;
    }

    T& At(int i) {
        K_ASSERT(i >= 0 && i < N);
        return mData[i];
    }

    const T& At(int i) const {
        K_ASSERT(i >= 0 && i < N);
        return mData[i];
    }

    T& operator[](int i) {
        return At(i);
    }
    const T& operator[](int i) const {
        return At(i);
    }

    T& operator()(int i) {
        return At(i);
    }
    const T& operator()(int i) const {
        return At(i);
    }

private:
    T mData[N]; // Internal array
};

/**
 * Templated 2D array with bounds-checking in debug
 *
 * @tparam T Array element type
 * @tparam M Outer array size
 * @tparam N Inner array size
 */
template <typename T, u32 M, u32 N> class TArray2D {
public:
    u32 Length() const {
        return M;
    }

    TArray<T, N>& At(int i) {
        K_ASSERT(i >= 0 && i < N);
        return mData[i];
    }

    const TArray<T, N>& At(int i) const {
        K_ASSERT(i >= 0 && i < N);
        return mData[i];
    }

    TArray<T, N>& At(int i, int j) {
        K_ASSERT(i >= 0 && i < M);
        K_ASSERT(j >= 0 && j < N);
        return mData[i][j];
    }

    const TArray<T, N>& At(int i, int j) const {
        K_ASSERT(i >= 0 && i < M);
        K_ASSERT(j >= 0 && j < N);
        return mData[i][j];
    }

    TArray<T, N>& operator[](int i) {
        return At(i);
    }
    const TArray<T, N>& operator[](int i) const {
        return At(i);
    }

    TArray<T, N>& operator()(int i) {
        return At(i);
    }
    const TArray<T, N>& operator()(int i) const {
        return At(i);
    }

    TArray<T, N>& operator()(int i, int j) {
        return At(i).At(j);
    }
    const TArray<T, N>& operator()(int i, int j) const {
        return At(i).At(j);
    }

private:
    TArray<T, N> mData[M]; // Internal array
};

} // namespace kiwi

#endif
