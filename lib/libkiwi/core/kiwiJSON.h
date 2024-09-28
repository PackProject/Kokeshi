#ifndef LIBKIWI_CORE_JSON_H
#define LIBKIWI_CORE_JSON_H
#include <libkiwi/k_types.h>
#include <libkiwi/prim/kiwiHashMap.h>
#include <libkiwi/prim/kiwiOptional.h>
#include <libkiwi/prim/kiwiPair.h>
#include <libkiwi/prim/kiwiString.h>
#include <libkiwi/prim/kiwiVector.h>
#include <libkiwi/util/kiwiNonCopyable.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{
namespace json {
//! @addtogroup libkiwi_core
//! @{

// Forward declarations
class Element;

//! JSON array contains zero or more elements
typedef TVector<Element> Array;
//! JSON object contains zero or more key/value pairs
typedef TMap<String, Element> Object;

struct Null_t {};
//! JavaScript 'null' keyword
static const Null_t null = Null_t();

/**
 * @brief Element visitor interface
 */
class IVisitor {
public:
    /**
     * @brief Destructor
     */
    virtual ~IVisitor() {}

    /**
     * @brief Processes the current element
     *
     * @param rElement JSON element
     */
    virtual void Visit(const Element& rElement) = 0;
};

/**
 * @brief Generic JSON element
 */
class Element {
public:
    enum EType {
        EType_Number,   // Double-precision float
        EType_String,   // Character sequence
        EType_Boolean,  // Boolean value
        EType_Array,    // Ordered list of elements
        EType_Object,   // Unordered name/value pairs
        EType_Null,     // Javascript 'null' keyword
        EType_Undefined // No contents
    };

public:
    /**
     * @brief Constructor
     * @note Element is undefined (has no contents) by default
     */
    Element() : mType(EType_Undefined) {}

    /**
     * @brief Constructor
     *
     * @param rOther Element to copy
     */
    Element(const Element& rOther);

    /**
     * @brief Destructor
     * @note Contained tree is destroyed
     */
    ~Element() {
        Clear();
    }

    /**
     * @name Value constructors
     */
    /**@{*/
    // clang-format off
    Element(f64 x)           { Set<f64>(x);    }
    Element(s64 x)           { Set<s64>(x);    }
    Element(bool x)          { Set<bool>(x);   }
    Element(const String& x) { Set<String>(x); }
    Element(const Array& x)  { Set<Array>(x);  }
    Element(const Object& x) { Set<Object>(x); }
    Element(Null_t x)        { Set<Null_t>(x); }
    // clang-format on
    /**@}*/

    /**
     * @name Array access
     */
    /**@{*/
    Element& operator[](int i) {
        K_ASSERT(mType == EType_Array);
        return Get<Array>()[i];
    }
    const Element& operator[](int i) const {
        K_ASSERT(mType == EType_Array);
        return Get<Array>()[i];
    }
    /**@}*/

    /**
     * @name Object access
     */
    /**@{*/
    Element& operator[](const String& rKey) {
        K_ASSERT(mType == EType_Object);
        return Get<Object>()[rKey];
    }
    const Element& operator[](const String& rKey) const {
        K_ASSERT(mType == EType_Object);
        const Element* pElement = Get<Object>().Find(rKey);

        K_ASSERT(pElement != nullptr);
        return *pElement;
    }
    /**@}*/

    /**
     * @brief Accesses this element's type
     */
    EType GetType() const {
        return mType;
    }

    /**
     * @brief Accesses this element's value, by type
     */
    template <typename T> T& Get();
    /**
     * @brief Accesses this element's value, by type
     */
    template <typename T> const T& Get() const;

    /**
     * @brief Deserialises this element's value, by type
     * @note The object type must derive from ISerializable
     *
     * @tparam T Object type
     */
    template <typename T> T GetObj() const;

    /**
     * @brief Overwrites this element's value
     *
     * @param rValue New value (copied)
     */
    template <typename T> void Set(const T& rValue);

    /**
     * @brief Tests whether the element is null
     */
    bool IsNull() const {
        return mType == EType_Null;
    }

    /**
     * @brief Tests whether the element contains valid data
     */
    bool IsValid() const {
        return !IsUndefined();
    }
    /**
     * @brief Tests whether the element is undefined (has no contents)
     */
    bool IsUndefined() const {
        return mType == EType_Undefined;
    }

    /**
     * @brief Clears this element's data
     */
    void Clear();

    /**
     * @brief Recursively process this element and all its children
     *
     * @param rVisitor Element visitor
     */
    void ForEach(IVisitor& rVisitor) const {
        rVisitor.Visit(*this);
    }

private:
    EType mType; // Element type

    union {
        f64 mNumber;      // Double-precision float
        String* mpString; // Character sequence
        bool mBoolean;    // Boolean value
        Array* mpArray;   // Ordered list of elements
        Object* mpObject; // Unordered name/value pairs
    };
};

/**
 * @brief Interface for JSON serializable/deserializable objects
 */
class ISerializable {
public:
    /**
     * @brief Destructor
     */
    virtual ~ISerializable() {}

    /**
     * @brief Encodes this object into JSON element form
     *
     * @param rElem JSON Element to which data will be written
     */
    virtual void Serialize(Element& /* rElem */) const {}

    /**
     * @brief Decodes a JSON element into this object
     *
     * @param rElem JSON element from which data will be read
     * @return Success
     */
    virtual bool Deserialize(const Element& /* rElem */) {
        return true;
    }
};

/**
 * @brief JSON reader
 */
class Reader : private NonCopyable {
public:
    /**
     * @brief Constructor
     */
    Reader()
        : mpFileBuffer(nullptr),
          mFileSize(0),
          mEncoding(EEncoding_AutoDetect) {}

    /**
     * @brief Decodes a JSON string (UTF-8) into element form
     *
     * @param rStr JSON string
     */
    void Decode(const String& rStr);
    /**
     * @brief Decodes a JSON string into element form
     *
     * @param pData JSON data buffer
     * @param size Data size
     */
    void Decode(const void* pData, u32 size);

    /**
     * @brief Gets the current state of the root element
     */
    const Element& Get() const {
        return mRootElement;
    }

private:
    /**
     * @brief Text encoding type
     */
    enum EEncoding {
        EEncoding_AutoDetect, // Auto-detect from contents
        EEncoding_UTF8,       // UTF-8
        EEncoding_UTF16_BE,   // UTF-16 (Big Endian)
        EEncoding_UTF16_LE    // UTF-16 (Little Endian)
    };

    // Escaped unicode character codes are required to be four digits
    static const int UNICODE_ESCAPE_DIGITS = 4;

private:
    /**
     * @brief Decodes JSON data into element form
     */
    void DecodeImpl();

    /**
     * @name Grammar functions
     */
    /**@{*/
    bool ParseEncoding(u32& rPos);

    bool ParseElement(u32& rPos, Element& rElement);
    bool ParseElements(u32& rPos, Element& rElement);

    bool ParseValue(u32& rPos, Element& rElement);
    bool ParseObject(u32& rPos, Element& rElement);
    bool ParseArray(u32& rPos, Element& rElement);
    bool ParseString(u32& rPos, String& rString);
    bool ParseNumber(u32& rPos, f64& rNumber);

    bool ParseMember(u32& rPos, String& rKey, Element& rValue);
    bool ParseMembers(u32& rPos, Element& rElement);

    bool ParseCharacters(u32& rPos, String& rToken);
    bool ParseCharacter(u32& rPos, String& rToken);
    bool ParseUnEscaped(u32& rPos, char& rToken);
    bool ParseEscaped(u32& rPos, String& rToken);
    bool ParseHex(u32& rPos, char& rToken);
    bool ParseInteger(u32& rPos, String& rToken);
    bool ParseDigits(u32& rPos, String& rToken);
    bool ParseDigit(u32& rPos, char& rToken);
    bool ParseOneNine(u32& rPos, char& rToken);
    bool ParseFraction(u32& rPos, String& rToken);
    bool ParseExponent(u32& rPos, String& rToken);
    bool ParseWhiteSpace(u32& rPos);

    bool ParseLiteral(char literal, u32& rPos);
    bool ParseLiteral(char literal, u32& rPos, char& rToken);

    bool ParseLiteral(const String& rLiteral, u32& rPos);
    bool ParseLiteral(const String& rLiteral, u32& rPos, String& rToken);
    /**@}*/

private:
    //! JSON data buffer
    const char* mpFileBuffer;
    //! JSON data size
    u32 mFileSize;

    //! JSON text encoding
    EEncoding mEncoding;

    //! JSON tree root
    Element mRootElement;
};

/**
 * @brief JSON writer
 */
class Writer : private NonCopyable {
public:
    /**
     * @brief Encodes a JSON element into string form (UTF-8)
     *
     * @param rElem JSON element
     * @param pretty Whether to pretty-print
     */
    void Encode(const Element& rElem, bool pretty = false);

    /**
     * @brief Gets the current state of the JSON string (UTF-8)
     */
    const String& Get() const {
        return mTextBuffer;
    }

private:
    //! JSON text buffer
    String mTextBuffer;
};

/**
 * @name Deserialization utilities
 */
/**@{*/
/**
 * @brief Decodes a JSON string (UTF-8) into element form
 *
 * @param rStr JSON string
 */
K_INLINE Element loads(const String& rStr) {
    Reader reader;
    reader.Decode(rStr);
    return reader.Get();
}

/**
 * @brief Decodes a JSON string (UTF-8) into object form
 * @note The object type must derive from ISerializable.
 *
 * @tparam T Object type
 * @param rStr JSON string
 */
template <typename T> K_INLINE Optional<T> loads(const String& rStr) {
    Reader reader;
    reader.Decode(rStr);

    const Element& rElem = reader.Get();
    if (rElem.IsUndefined()) {
        return kiwi::nullopt;
    }

    T value;
    if (!value.Deserialize(rElem)) {
        return kiwi::nullopt;
    }

    return value;
}
/**@}*/

/**
 * @name Serialization utilities
 */
/**
 * @brief Encodes a JSON element into string form (UTF-8)
 *
 * @param rElem JSON element
 * @param pretty Whether to pretty-print
 */
K_INLINE String dumps(const Element& rElem, bool pretty = false) {
    Writer writer;
    writer.Encode(rElem);
    return writer.Get();
}

/**
 * @brief Encodes an object into JSON string form (UTF-8)
 * @note The object type must derive from ISerializable.
 *
 * @param rObj Object
 * @param pretty Whether to pretty-print
 */
K_INLINE String dumps(const ISerializable& rObj, bool pretty = false) {
    Element elem;
    rObj.Serialize(elem);

    Writer writer;
    writer.Encode(elem);
    return writer.Get();
}
/**@}*/

//! @}
} // namespace json

//! Shorthand
typedef json::Element JSON;

//! @}
} // namespace kiwi

// Implementation header
#ifndef LIBKIWI_CORE_JSON_IMPL_HPP
#include <libkiwi/core/kiwiJSONImpl.hpp>
#endif

#endif
