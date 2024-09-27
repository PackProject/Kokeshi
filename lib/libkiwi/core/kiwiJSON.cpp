#include <cstring>
#include <libkiwi.h>

namespace kiwi {
namespace json {

/**
 * @name JSON reader
 */
/**@{*/
// Shorthand for accepting input strings
#define MATCH()                                                                \
    /* Consume the input string by applying the new parser position */         \
    rPos = pos;                                                                \
    return true;

// Shorthand for rejecting input strings
#define DONT_MATCH() return false;

/**
 * @brief Constructor
 *
 * @param rOther Element to copy
 */
Element::Element(const Element& rOther) {
    // Don't leak existing memory
    Clear();

    if (rOther.IsUndefined()) {
        return;
    }

    switch (rOther.mType) {
    case EType_Number: {
        Set(rOther.Get<f64>());
        break;
    }

    case EType_String: {
        Set(rOther.Get<String>());
        break;
    }

    case EType_Boolean: {
        Set(rOther.Get<bool>());
        break;
    }

    case EType_Array: {
        Set(rOther.Get<Array>());
        break;
    }

    case EType_Object: {
        Set(rOther.Get<Object>());
        break;
    }

    case EType_Null: {
        Set(json::null);
        break;
    }

    default: {
        K_ASSERT(false);
        break;
    }
    }
}

/**
 * @brief Clears this element's data
 */
void Element::Clear() {
    switch (mType) {
    case EType_String: {
        mpString = nullptr;
        break;
    }

    case EType_Array: {
        mpArray = nullptr;
        break;
    }

    case EType_Object: {
        mpObject = nullptr;
        break;
    }
    }

    // TODO: Better way to memset the union?
    std::memset(&mNumber, 0, sizeof(mNumber));
    mType = EType_Undefined;
}

/**
 * @brief Decodes a JSON string (UTF-8) into element form
 *
 * @param rStr JSON string
 */
void Reader::Decode(const String& rStr) {
    mpFileBuffer = rStr.CStr();
    mFileSize = rStr.Length();
    mEncoding = EEncoding_UTF8;

    DecodeImpl();
}

/**
 * @brief Decodes a JSON string into element form
 *
 * @param pData JSON data buffer
 * @param size Data size
 */
void Reader::Decode(const void* pData, u32 size) {
    K_ASSERT(pData != nullptr);

    mpFileBuffer = static_cast<const char*>(pData);
    mFileSize = size;
    mEncoding = EEncoding_AutoDetect;

    DecodeImpl();
}

/**
 * @brief Decodes JSON data into element form
 */
void Reader::DecodeImpl() {
    K_ASSERT(mpFileBuffer != nullptr);

    // Remove previous data
    mRootElement.Clear();

    if (mpFileBuffer == nullptr || mFileSize == 0) {
        return;
    }

    // Begin from the start
    u32 pos = 0;

    // Need to detect encoding if not specified
    if (mEncoding == EEncoding_AutoDetect) {
        ParseEncoding(pos);
    }

    // TODO: Implement UTF-16 parsing
    K_ASSERT_EX(mEncoding == EEncoding_UTF8,
                "UTF-16 encoding is not yet supported.\n");

    // JSON must begin with an element
    if (!ParseElement(pos, mRootElement)) {
        mRootElement.Clear();
    }

    // Something else is incorrectly written after the main element
    if (pos != mFileSize) {
        mRootElement.Clear();
    }

    K_ASSERT_EX(pos <= mFileSize, "Buffer overrun!");
}

/**
 * @brief Attempts to determine the JSON data's text encoding type
 *
 * @param[in, out] rPos Parser position
 * @return Success
 */
bool Reader::ParseEncoding(u32& rPos) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    if (ParseLiteral("\xEF\xBB\xBF", pos)) {
        mEncoding = EEncoding_UTF8;
        MATCH();
    }

    if (ParseLiteral("\xFF\xFE", pos)) {
        mEncoding = EEncoding_UTF16_LE;
        MATCH();
    }

    if (ParseLiteral("\xFE\xFF", pos)) {
        mEncoding = EEncoding_UTF16_BE;
        MATCH();
    }

    // Default to UTF-8
    mEncoding = EEncoding_UTF8;
    MATCH();
}

/**
 * @brief Attempts to parse a character literal
 *
 * @param literal Character literal
 * @param[in, out] rPos Parser position
 * @return Success
 */
bool Reader::ParseLiteral(char literal, u32& rPos) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    // Prevent buffer overrun
    if (rPos >= mFileSize) {
        DONT_MATCH();
    }

    if (mpFileBuffer[pos++] == literal) {
        MATCH();
    }

    DONT_MATCH();
}

/**
 * @brief Attempts to parse a character literal
 *
 * @param literal Character literal
 * @param[in, out] rPos Parser position
 * @param[out] rToken Parsed token
 * @return Success
 */
bool Reader::ParseLiteral(char literal, u32& rPos, char& rToken) {
    bool success = ParseLiteral(literal, rPos);

    if (success) {
        rToken = literal;
    } else {
        rToken = 0;
    }

    return success;
}

/**
 * @brief Attempts to parse a string literal
 *
 * @param rLiteral String literal
 * @param[in, out] rPos Parser position
 * @return Success
 */
bool Reader::ParseLiteral(const String& rLiteral, u32& rPos) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;
    u32 len = rLiteral.Length();

    for (u32 i = 0; i < len; pos++, i++) {
        // Prevent buffer overrun
        if (pos >= mFileSize) {
            DONT_MATCH();
        }

        if (mpFileBuffer[pos] != rLiteral[i]) {
            DONT_MATCH();
        }
    }

    MATCH();
}

/**
 * @brief Attempts to parse a string literal
 *
 * @param rLiteral String literal
 * @param[in, out] rPos Parser position
 * @param[out] rToken Parsed token
 * @return Success
 */
bool Reader::ParseLiteral(const String& rLiteral, u32& rPos, String& rToken) {
    bool success = ParseLiteral(rLiteral, rPos);

    if (success) {
        rToken = rLiteral;
    } else {
        rToken.Clear();
    }

    return success;
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Element'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rElement Destination element
 * @return Success
 */
bool Reader::ParseElement(u32& rPos, Element& rElement) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);
    K_ASSERT(rElement.IsUndefined());

    u32 pos = rPos;

    /**
     * Element = WhiteSpace? Value WhiteSpace?
     */
    ParseWhiteSpace(pos);

    if (!ParseValue(pos, rElement)) {
        rElement.Clear();
        DONT_MATCH();
    }

    ParseWhiteSpace(pos);

    MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Elements'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rElement Destination element
 * @return Success
 */
bool Reader::ParseElements(u32& rPos, Element& rElement) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    K_ASSERT_EX(rElement.GetType() == Element::EType_Array,
                "Only arrays can contain elements\n");

    u32 pos = rPos;

    /**
     * Elements = Element (',' Element)*
     */
    Element element;

    while (ParseElement(pos, element)) {
        rElement.Get<Array>().PushBack(element);
        element.Clear();

        ParseWhiteSpace(pos);
        if (!ParseLiteral(',', pos)) {
            // End of element list
            MATCH();
        }
        ParseWhiteSpace(pos);
    }

    // Trailing comma
    rElement.Clear();
    DONT_MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Value'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rElement Destination element
 * @return Success
 */
bool Reader::ParseValue(u32& rPos, Element& rElement) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);
    K_ASSERT(rElement.IsUndefined());

    u32 pos = rPos;

    /**
     * Value = "false" | "null" | "true" | Object | Array | Number | String
     */
    if (ParseLiteral("false", pos)) {
        rElement.Set(false);
        MATCH();
    }

    if (ParseLiteral("null", pos)) {
        rElement.Set(json::null);
        MATCH();
    }

    if (ParseLiteral("true", pos)) {
        rElement.Set(true);
        MATCH();
    }

    if (ParseObject(pos, rElement)) {
        MATCH();
    }

    if (ParseArray(pos, rElement)) {
        MATCH();
    }

    f64 number;
    if (ParseNumber(pos, number)) {
        rElement.Set(number);
        MATCH();
    }

    String string = "";
    if (ParseString(pos, string)) {
        rElement.Set(string);
        MATCH();
    }

    DONT_MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Object'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rElement Destination element
 * @return Success
 */
bool Reader::ParseObject(u32& rPos, Element& rElement) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);
    K_ASSERT(rElement.IsUndefined());

    u32 pos = rPos;
    rElement.Set(Object());

    /**
     * Object = '{' Members? '}'
     */
    ParseWhiteSpace(pos);
    if (!ParseLiteral('{', pos)) {
        // Not an object
        rElement.Clear();
        DONT_MATCH();
    }
    ParseWhiteSpace(pos);

    if (!ParseMembers(pos, rElement)) {
        // Empty object
        rElement.Set(Object());
    }

    ParseWhiteSpace(pos);
    if (!ParseLiteral('}', pos)) {
        // Missing ending bracket
        rElement.Clear();
        DONT_MATCH();
    }
    ParseWhiteSpace(pos);

    MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Array'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rElement Destination element
 * @return Success
 */
bool Reader::ParseArray(u32& rPos, Element& rElement) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);
    K_ASSERT(rElement.IsUndefined());

    u32 pos = rPos;
    rElement.Set(Array());

    /**
     * Array = '[' Elements? ']'
     */
    ParseWhiteSpace(pos);
    if (!ParseLiteral('[', pos)) {
        // Not an array
        rElement.Clear();
        DONT_MATCH();
    }
    ParseWhiteSpace(pos);

    if (!ParseElements(pos, rElement)) {
        // Empty array
        rElement.Set(Array());
    }

    ParseWhiteSpace(pos);
    if (!ParseLiteral(']', pos)) {
        // Missing ending bracket
        rElement.Clear();
        DONT_MATCH();
    }
    ParseWhiteSpace(pos);

    MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'String'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rString Destination string
 * @return Success
 */
bool Reader::ParseString(u32& rPos, String& rString) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    /**
     * String = '"' Characters? '"'
     */
    if (!ParseLiteral('\"', pos)) {
        rString.Clear();
        DONT_MATCH();
    }

    ParseCharacters(pos, rString);

    if (!ParseLiteral('\"', pos)) {
        rString.Clear();
        DONT_MATCH();
    }

    MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Number'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rNumber Destination number
 * @return Success
 */
bool Reader::ParseNumber(u32& rPos, f64& rNumber) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    /**
     * Number = '-'? Integer Fraction? Exponent?
     */
    bool sign = ParseLiteral('-', pos);

    String integer = "";
    if (!ParseInteger(pos, integer)) {
        rNumber = 0.0;
        DONT_MATCH();
    }

    String fraction = "";
    ParseFraction(pos, fraction);

    String exponent = "";
    ParseExponent(pos, exponent);

    String s = integer + fraction + exponent;
    f64 number = ksl::atof(s.CStr());
    rNumber = sign ? -number : number;

    MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Member'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rKey Destination key
 * @param[out] rValue Destination value
 * @return Success
 */
bool Reader::ParseMember(u32& rPos, String& rKey, Element& rValue) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);
    K_ASSERT(rValue.IsUndefined());

    u32 pos = rPos;

    /**
     * Member = String ':' Element
     */
    if (!ParseString(pos, rKey)) {
        rKey.Clear();
        rValue.Clear();
        DONT_MATCH();
    }

    ParseWhiteSpace(pos);
    if (!ParseLiteral(':', pos)) {
        rKey.Clear();
        rValue.Clear();
        DONT_MATCH();
    }
    ParseWhiteSpace(pos);

    if (!ParseElement(pos, rValue)) {
        rKey.Clear();
        rValue.Clear();
        DONT_MATCH();
    }

    MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Members'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rElement Destination element
 * @return Success
 */
bool Reader::ParseMembers(u32& rPos, Element& rElement) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    K_ASSERT_EX(rElement.GetType() == Element::EType_Object,
                "Only objects can contain members\n");

    u32 pos = rPos;

    /**
     * Members = Member (',' Member)*
     */
    String key = "";
    Element value;

    while (ParseMember(pos, key, value)) {
        // Member names SHOULD be unique, but don't *have* to be
        K_WARN_EX(rElement.Get<Object>().Contains(key),
                  "Duplicate member name: %s\n", key.CStr());

        rElement.Get<Object>().Insert(key, value);
        value.Clear();

        ParseWhiteSpace(pos);
        if (!ParseLiteral(',', pos)) {
            // End of member list
            MATCH();
        }
        ParseWhiteSpace(pos);
    }

    // Trailing comma
    rElement.Clear();
    DONT_MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Characters'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rToken Parsed token
 * @return Success
 */
bool Reader::ParseCharacters(u32& rPos, String& rToken) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    /**
     * Characters = Character+
     */
    String character = "";
    if (!ParseCharacter(pos, character)) {
        // Need at least one character
        rToken.Clear();
        DONT_MATCH();
    }

    rToken = character;

    // Any number of characters may follow
    while (ParseCharacter(pos, character)) {
        rToken += character;
    }

    MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Character'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rToken Parsed token
 * @return Success
 */
bool Reader::ParseCharacter(u32& rPos, String& rToken) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    /**
     * Character = UnEscaped | Escaped
     */
    char unescape;
    if (ParseUnEscaped(pos, unescape)) {
        rToken = unescape;
        MATCH();
    }

    String escape = "";
    if (ParseEscaped(pos, escape)) {
        rToken = escape;
        MATCH();
    }

    DONT_MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'UnEscaped'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rToken Parsed token
 * @return Success
 */
bool Reader::ParseUnEscaped(u32& rPos, char& rToken) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    // Prevent buffer overrun
    if (rPos >= mFileSize) {
        rToken = 0;
        DONT_MATCH();
    }

    /**
     * UnEscaped = [0x20-0x21] | [0x23-0x5B] | [0x5D-0x10FFFF]
     */
    bool valid = false;

    // Need unsigned char for >/< comparisons
    u8 ch = mpFileBuffer[pos++];
    valid |= ch == 0x20 /* (' ') */ || ch == 0x21 /* ('!') */;
    valid |= ch >= 0x23 /* ('#') */ && ch <= 0x5B /* ('[') */;
    valid |= ch >= 0x5D /* (']') */;

    if (!valid) {
        // Not an unescaped character
        rToken = 0;
        DONT_MATCH();
    }

    rToken = ch;
    MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Escaped'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rToken Parsed token
 * @return Success
 */
bool Reader::ParseEscaped(u32& rPos, String& rToken) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    /**
     * Escaped = '\' ('"' | '\' | '/' | 'b' | 'f' | 'n' | 'r' | 't'
     *                | ('u' Hex{4}))
     */
    if (!ParseLiteral('\\', pos)) {
        // Not an escape sequence
        rToken.Clear();
        DONT_MATCH();
    }

    // Prevent buffer overrun
    if (rPos >= mFileSize) {
        rToken.Clear();
        DONT_MATCH();
    }

    char ch = mpFileBuffer[pos++];
    if (ch == '\"' || ch == '\\' || ch == '/' || ch == 'b' || ch == 'f' ||
        ch == 'n' || ch == 'r' || ch == 't') {
        rToken = ch;
        MATCH();
    }

    // Must be a unicode sequence at this point
    if (ch != 'u') {
        rToken.Clear();
        DONT_MATCH();
    }

    // Need to build hex character code
    String code = "u";

    for (int i = 0; i < UNICODE_ESCAPE_DIGITS; i++) {
        char hex;
        if (!ParseHex(pos, hex)) {
            rToken.Clear();
            DONT_MATCH();
        }

        code += hex;
    }

    rToken = code;
    MATCH();
};

/**
 * @brief Attempts to parse the JSON grammar structure 'Hex'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rToken Parsed token
 * @return Success
 */
bool Reader::ParseHex(u32& rPos, char& rToken) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    /**
     * Hex = Digit | ['A'-'F'] | ['a'-'f']
     */
    char digit;
    if (ParseDigit(pos, digit)) {
        rToken = digit;
        MATCH();
    }

    // Prevent buffer overrun
    if (rPos >= mFileSize) {
        rToken = 0;
        DONT_MATCH();
    }

    char ch = mpFileBuffer[pos++];
    if ((ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f')) {
        rToken = ch;
        MATCH();
    }

    rToken = 0;
    DONT_MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Integer'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rToken Parsed token
 * @return Success
 */
bool Reader::ParseInteger(u32& rPos, String& rToken) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    /**
     * Integer = '0' | (OneNine Digits?)
     */
    if (ParseLiteral('0', pos)) {
        rToken = "0";
        MATCH();
    }

    char onenine;
    if (!ParseOneNine(pos, onenine)) {
        // Not a number
        rToken.Clear();
        DONT_MATCH();
    }

    rToken = onenine;

    // Further digits are optional
    String digits = "";
    if (ParseDigits(pos, digits)) {
        rToken += digits;
    }

    MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Digits'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rToken Parsed token
 * @return Success
 */
bool Reader::ParseDigits(u32& rPos, String& rToken) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    /**
     * Digits = Digit+
     */
    char digit;
    if (!ParseDigit(pos, digit)) {
        // Need at least one digit
        rToken.Clear();
        DONT_MATCH();
    }

    rToken = digit;

    // Any number of digits may follow
    while (ParseDigit(pos, digit)) {
        rToken += digit;
    }

    MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Digit'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rToken Parsed token
 * @return Success
 */
bool Reader::ParseDigit(u32& rPos, char& rToken) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    // Prevent buffer overrun
    if (rPos >= mFileSize) {
        rToken = 0;
        DONT_MATCH();
    }

    /**
     * Digit = ['0'-'9']
     */
    char ch = mpFileBuffer[pos++];
    if (ch >= '0' && ch <= '9') {
        rToken = ch;
        MATCH();
    }

    rToken = 0;
    DONT_MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'OneNine'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rToken Parsed token
 * @return Success
 */
bool Reader::ParseOneNine(u32& rPos, char& rToken) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    // Prevent buffer overrun
    if (rPos >= mFileSize) {
        rToken = 0;
        DONT_MATCH();
    }

    /**
     * OneNine = ['1'-'9']
     */
    char ch = mpFileBuffer[pos++];
    if (ch >= '1' && ch <= '9') {
        rToken = ch;
        MATCH();
    }

    rToken = 0;
    DONT_MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Fraction'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rToken Parsed token
 * @return Success
 */
bool Reader::ParseFraction(u32& rPos, String& rToken) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    /**
     * Fraction = '.' Digits
     */
    if (!ParseLiteral('.', pos)) {
        // Not a fraction
        rToken.Clear();
        DONT_MATCH();
    }

    String digits = "";
    if (!ParseDigits(pos, digits)) {
        // Trailing decimal point
        rToken.Clear();
        DONT_MATCH();
    }

    rToken = '.' + digits;
    MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'Exponent'
 *
 * @param[in, out] rPos Parser position
 * @param[out] rToken Parsed token
 * @return Success
 */
bool Reader::ParseExponent(u32& rPos, String& rToken) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    /**
     * Exponent = ('e' | 'E') ('-' | '+')? Digits
     */
    char prefix;
    if (!ParseLiteral('E', pos, prefix) && !ParseLiteral('e', pos, prefix)) {
        // Not an exponent
        rToken.Clear();
        DONT_MATCH();
    }

    char sign;
    if (!ParseLiteral('-', pos, sign)) {
        ParseLiteral('+', pos, sign);
    }

    String digits = "";
    if (!ParseDigits(pos, digits)) {
        // Need at least one digit
        rToken.Clear();
        DONT_MATCH();
    }

    rToken = prefix + (sign + digits);
    MATCH();
}

/**
 * @brief Attempts to parse the JSON grammar structure 'WhiteSpace'
 *
 * @param[in, out] rPos Parser position
 * @return Success
 */
bool Reader::ParseWhiteSpace(u32& rPos) {
    K_ASSERT(mpFileBuffer != nullptr);
    K_ASSERT(rPos >= 0 && rPos <= mFileSize);

    u32 pos = rPos;

    /**
     * WhiteSpace = (' ' | '\t' | '\n' | '\r')*
     */
    while (ParseLiteral(' ', pos) || ParseLiteral('\t', pos) ||
           ParseLiteral('\n', pos) || ParseLiteral('\r', pos)) {
        ;
    }

    // Empty string
    MATCH();
}
/**@}*/

/**
 * @name JSON writer
 */
/**@{*/
/**
 * Pretty-print element visitor
 */
class PrintVisitor : public IVisitor {
public:
    /**
     * @brief Constructor
     *
     * @param[in, out] rBuffer Output text buffer
     * @param pretty Whether to pretty-print
     */
    PrintVisitor(String& rBuffer, bool pretty = false);

    /**
     * @brief Processes the current element
     *
     * @param rElement JSON element
     */
    virtual void Visit(const Element& rElement);

private:
    //! Default string buffer size
    static const int DEFAULT_BUFFER_SIZE = 1024;
    //! Indent width, in spaces
    static const int INDENT_WIDTH = 4;

private:
    /**
     * @brief Appends text to the text buffer
     *
     * @param rText New text to append
     */
    void Print(const String& rText);

private:
    //! JSON text buffer
    String& mrTextBuffer;

    //! Pretty-print flag
    bool mIsPretty;
    //! Current indent level
    int mIndent;
};

/**
 * @brief Constructor
 *
 * @param[in, out] rBuffer Output text buffer
 * @param pretty Whether to pretty-print
 */
PrintVisitor::PrintVisitor(String& rBuffer, bool pretty)
    : mrTextBuffer(rBuffer), mIsPretty(pretty), mIndent(0) {
    if (rBuffer.Empty()) {
        rBuffer.Reserve(DEFAULT_BUFFER_SIZE);
    }
}

/**
 * @brief Appends text to the text buffer
 *
 * @param rText New text to append
 */
void PrintVisitor::Print(const String& rText) {
    // Need to indent this text for pretty-print
    if (mIsPretty) {
        for (int i = 0; i < mIndent * INDENT_WIDTH; i++) {
            mrTextBuffer += ' ';
        }
    }

    mrTextBuffer += rText;
}

/**
 * @brief Processes the current element
 *
 * @param rElement JSON element
 */
void PrintVisitor::Visit(const Element& rElement) {
    if (rElement.IsUndefined()) {
        Print("undefined");
        return;
    }

    switch (rElement.GetType()) {
    case Element::EType_Number: {
        Print(kiwi::ToString(rElement.Get<f64>()));
        break;
    }

    case Element::EType_String: {
        Print('\"' + rElement.Get<String>() + '\"');
        break;
    }

    case Element::EType_Boolean: {
        Print(kiwi::ToString(rElement.Get<bool>()));
        break;
    }

    case Element::EType_Array: {
        Print("[");

        if (mIsPretty) {
            mrTextBuffer += '\n';
        }

        // Nested structure will indent text
        mIndent++;
        {
            u32 size = rElement.Get<Array>().Size();

            for (u32 i = 0; i < size; i++) {
                Visit(rElement.Get<Array>()[i]);

                // Values are comma separated
                if (i != size - 1) {
                    mrTextBuffer += ',';
                }

                mrTextBuffer += '\n';
            }
        }
        mIndent--;

        Print("]");
        break;
    }

    case Element::EType_Object: {
        Print("{");

        if (mIsPretty) {
            mrTextBuffer += '\n';
        }

        // Nested structure will indent text
        mIndent++;
        {
            u32 size = rElement.Get<Object>().Size();
            u32 i = 0;

            for (Object::ConstIterator it = rElement.Get<Object>().Begin();
                 it != rElement.Get<Object>().End(); ++it, i++) {
                Print('\"' + (it.Key() + "\": "));

                // TODO: Fix this somehow?
                Visit(it.Value());

                // Values are comma separated
                if (i != size - 1) {
                    mrTextBuffer += ',';
                }

                mrTextBuffer += '\n';
            }
        }
        mIndent--;

        Print("}");
        break;
    }

    case Element::EType_Null: {
        Print("null");
        break;
    }

    default: {
        K_ASSERT_EX(false, "Should never happen!\n");
        break;
    }
    }
}

/**
 * @brief Encodes a JSON element into string form
 *
 * @param rElem JSON element
 * @param pretty Whether to pretty-print
 */
void Writer::Encode(const Element& rElem, bool pretty) {
    PrintVisitor visitor(mTextBuffer, pretty);
    rElem.ForEach(visitor);

    if (pretty) {
        mTextBuffer += '\n';
    }
}
/**@}*/

} // namespace json
} // namespace kiwi
