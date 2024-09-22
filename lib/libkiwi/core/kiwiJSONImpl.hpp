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
template <> inline f64& Element::Get<f64>() {
    K_ASSERT(mType == EType_Number);
    return mNumber;
}
template <> inline const f64& Element::Get<f64>() const {
    K_ASSERT(mType == EType_Number);
    return mNumber;
}

template <> inline void Element::Set<f64>(const f64& rValue) {
    Clear();

    mType = EType_Number;
    mNumber = rValue;
}
template <> inline void Element::Set<s64>(const s64& rValue) {
    Clear();

    mType = EType_Number;
    mNumber = static_cast<f64>(rValue);
}
/**@}*/

/**
 * @name String
 */
/**@{*/
template <> inline String& Element::Get<String>() {
    K_ASSERT(mType == EType_String);
    K_ASSERT(mpString != nullptr);
    return *mpString;
}
template <> inline const String& Element::Get<String>() const {
    K_ASSERT(mType == EType_String);
    K_ASSERT(mpString != nullptr);
    return *mpString;
}

template <> inline void Element::Set<String>(const String& rValue) {
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
template <> inline bool& Element::Get<bool>() {
    K_ASSERT(mType == EType_Boolean);
    return mBoolean;
}
template <> inline const bool& Element::Get<bool>() const {
    K_ASSERT(mType == EType_Boolean);
    return mBoolean;
}

template <> inline void Element::Set<bool>(const bool& rValue) {
    Clear();

    mType = EType_Boolean;
    mBoolean = rValue;
}
/**@}*/

/**
 * @name Array
 */
/**@{*/
template <> inline Array& Element::Get<Array>() {
    K_ASSERT(mType == EType_Array);
    K_ASSERT(mpArray != nullptr);
    return *mpArray;
}
template <> inline const Array& Element::Get<Array>() const {
    K_ASSERT(mType == EType_Array);
    K_ASSERT(mpArray != nullptr);
    return *mpArray;
}

template <> inline void Element::Set<Array>(const Array& rArray) {
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
template <> inline Object& Element::Get<Object>() {
    K_ASSERT(mType == EType_Object);
    K_ASSERT(mpObject != nullptr);
    return *mpObject;
}
template <> inline const Object& Element::Get<Object>() const {
    K_ASSERT(mType == EType_Object);
    K_ASSERT(mpObject != nullptr);
    return *mpObject;
}

template <> inline void Element::Set<Object>(const Object& rValue) {
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
template <> inline Null_t& Element::Get<Null_t>() {
    K_ASSERT(mType == EType_Null);
    // TODO: Better way to solve this problem?
    return const_cast<Null_t&>(json::null);
}
template <> inline const Null_t& Element::Get<Null_t>() const {
    K_ASSERT(mType == EType_Null);
    return json::null;
}

template <> inline void Element::Set<Null_t>(const Null_t& rValue) {
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
template <typename T> inline T Element::GetObj() const {
    K_ASSERT(mType == EType_Object);
    K_ASSERT(mpObject != nullptr);

    // User type is responsible for deserializing
    T obj;
    bool success = obj.Deserialize(*this);

    K_WARN_EX(!success, "Failed to deserialize JSON object\n");
    return success ? obj : T();
}

template <typename T> inline void Element::Set(const T& rValue) {
    Clear();

    mType = EType_Object;
    rValue.Serialize(*this);
}
/**@}*/

} // namespace json
} // namespace kiwi

#endif
