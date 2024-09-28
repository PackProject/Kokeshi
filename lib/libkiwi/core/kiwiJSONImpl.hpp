// Implementation header
#ifndef LIBKIWI_CORE_JSON_IMPL_HPP
#define LIBKIWI_CORE_JSON_IMPL_HPP

// Declaration header
#ifndef LIBKIWI_CORE_JSON_H
#include <libkiwi/core/kiwiJSON.h>
#endif

namespace kiwi {
namespace json {

/**
 * @name Number
 */
/**@{*/
template <> K_INLINE f64& Element::Get<f64>() {
    K_ASSERT(mType == EType_Number);
    return mNumber;
}
template <> K_INLINE const f64& Element::Get<f64>() const {
    K_ASSERT(mType == EType_Number);
    return mNumber;
}

template <> K_INLINE void Element::Set<f64>(const f64& rValue) {
    Clear();

    mType = EType_Number;
    mNumber = rValue;
}
template <> K_INLINE void Element::Set<s64>(const s64& rValue) {
    Clear();

    mType = EType_Number;
    mNumber = static_cast<f64>(rValue);
}
/**@}*/

/**
 * @name String
 */
/**@{*/
template <> K_INLINE String& Element::Get<String>() {
    K_ASSERT(mType == EType_String);
    K_ASSERT(mpString != nullptr);
    return *mpString;
}
template <> K_INLINE const String& Element::Get<String>() const {
    K_ASSERT(mType == EType_String);
    K_ASSERT(mpString != nullptr);
    return *mpString;
}

template <> K_INLINE void Element::Set<String>(const String& rValue) {
    Clear();

    mpString = new String(rValue);
    K_ASSERT(mpString != nullptr);

    mType = EType_String;
}
/**@}*/

/**
 * @name Boolean
 */
/**@{*/
template <> K_INLINE bool& Element::Get<bool>() {
    K_ASSERT(mType == EType_Boolean);
    return mBoolean;
}
template <> K_INLINE const bool& Element::Get<bool>() const {
    K_ASSERT(mType == EType_Boolean);
    return mBoolean;
}

template <> K_INLINE void Element::Set<bool>(const bool& rValue) {
    Clear();

    mType = EType_Boolean;
    mBoolean = rValue;
}
/**@}*/

/**
 * @name Array
 */
/**@{*/
template <> K_INLINE Array& Element::Get<Array>() {
    K_ASSERT(mType == EType_Array);
    K_ASSERT(mpArray != nullptr);
    return *mpArray;
}
template <> K_INLINE const Array& Element::Get<Array>() const {
    K_ASSERT(mType == EType_Array);
    K_ASSERT(mpArray != nullptr);
    return *mpArray;
}

template <> K_INLINE void Element::Set<Array>(const Array& rArray) {
    Clear();

    mpArray = new Array(rArray);
    K_ASSERT(mpArray != nullptr);

    mType = EType_Array;
}
/**@}*/

/**
 * @name Object
 */
/**@{*/
template <> K_INLINE Object& Element::Get<Object>() {
    K_ASSERT(mType == EType_Object);
    K_ASSERT(mpObject != nullptr);
    return *mpObject;
}
template <> K_INLINE const Object& Element::Get<Object>() const {
    K_ASSERT(mType == EType_Object);
    K_ASSERT(mpObject != nullptr);
    return *mpObject;
}

template <> K_INLINE void Element::Set<Object>(const Object& rValue) {
    Clear();

    mpObject = new Object(rValue);
    K_ASSERT(mpObject != nullptr);

    mType = EType_Object;
}
/**@}*/

/**
 * @name Null
 */
/**@{*/
template <> K_INLINE Null_t& Element::Get<Null_t>() {
    K_ASSERT(mType == EType_Null);
    // TODO: Better way to solve this problem?
    return const_cast<Null_t&>(json::null);
}
template <> K_INLINE const Null_t& Element::Get<Null_t>() const {
    K_ASSERT(mType == EType_Null);
    return json::null;
}

template <> K_INLINE void Element::Set<Null_t>(const Null_t& rValue) {
#pragma unused(rValue)

    Clear();
    mType = EType_Null;
}
/**@}*/

/**
 * @name Serializable types
 */
/**@{*/
/**
 * @brief Deserialises this element's value, by type
 *
 * @tparam T Object type
 */
template <typename T> K_INLINE T Element::GetObj() const {
    K_ASSERT(mType == EType_Object);
    K_ASSERT(mpObject != nullptr);

    // User type is responsible for deserializing
    T obj;
    bool success = obj.Deserialize(*this);

    K_WARN_EX(!success, "Failed to deserialize JSON object\n");
    return success ? obj : T();
}

template <typename T> K_INLINE void Element::Set(const T& rValue) {
    Clear();

    mType = EType_Object;
    rValue.Serialize(*this);
}
/**@}*/

} // namespace json
} // namespace kiwi

#endif
