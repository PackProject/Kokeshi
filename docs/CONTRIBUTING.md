# Contributing

Thank you for your interest in the Kokeshi project! We ask that you please follow these contribution guidelines to ensure consistency across the codebase and its documentation.

## Project Structure

The Kokeshi repository follows this structure:

```
├── 🎨 assets: Graphics, audio, and other data
├── 📜 base: Base game DOLs and symbol maps
├── 🚧 build: Build process output
├── 📜 include: Header files for interfacing with the games
├── 📚 lib: External library code
│      └── 🥝 libkiwi: Utility library for working with Pack Project games
├── 🔄 loader: Statically-linked code (Kamek loader)
├── 💿 src: Dynamically-linked code (Kamek module)
└── 🔨 tools: Build tools (Kamek linker, CodeWarrior compiler, etc.)
```

Header files that detail game concepts should be placed in the `include` directory. Depending on the module in which the code lies, you should choose the appropriate sub-directory to hold the header files:

```
└── include:
    ├── egg: Nintendo EAD middleware
    ├── MetroTRK: Metrowerks Target Resident Kernel (TRK) Debugger
    ├── MSL: Metrowerks Standard Library
    ├── nw4r: NintendoWare for Revolution (NW4R)
    ├── Pack: Revolution Pack Project
    ├── revolution: Revolution (RVL) SDK
    ├── RVLFaceLib: Revolution Face Library (RFL)
    └── Sports2: Wii Sports 2 (Resort)
```

## File Formats

Please use the following file extensions for Kokeshi code:

| File Extension | Usage                                                |
| -------------- | ---------------------------------------------------- |
| `*.c`          | C function definition(s)                             |
| `*.cpp`        | C++ function/class definition(s)                     |
| `*.h`          | C/C++ function/class declaration(s)                  |
| `*.hpp`        | Large C++ template definitions that must be included |

## Style Guide

Kokeshi contains a `.clang-format` configuration file which handles most of the code formatting. Still, there are some important style elements to conform to:

### Header Guards

Although this project will likely always use the CodeWarrior compiler, which supports the `#pragma once` directive, please use standard include guards instead.

This is done both for readability purposes and to provide context about the file inside the guard:

- `#ifndef EGG_CORE_ARCHIVE_H` -> `include/egg/core/eggArchive.h`
- `#ifndef NW4R_UT_LIST_H` -> `include/nw4r/ut/ut_list.h`

#### Syntax

Please use the following format:
```
#ifndef {library}_{module}_{filename}_H
#define {library}_{module}_{filename}_H

// . . .

#endif
```

Terms separated by case/underscores should both be converted to underscores in the guard name:

- `myClass` -> `MY_CLASS`
- `MyClass` -> `MY_CLASS`
- `My_Class` -> `MY_CLASS`
- `MYClass` -> `MY_CLASS`

#### Special cases

The only exception is for Pack Project (RP) files. The classes exist in the global namespace and often include the module name in both the directory and filename:

- `include/Pack/RPAudio/RPSndAudioMgr.h` -> `#ifndef RP_AUDIO_SND_AUDIO_MGR_H`
- `include/Pack/RPGraphics/RPGrpRenderer.h` -> `#ifndef RP_GRAPHICS_RENDERER_H`
- `include/Pack/RPUtility/RPUtlBaseFsm.h` -> `#ifndef RP_UTILITY_BASE_FSM_H`

### Include Directives

*Include-what-you-use.* Unnecessary `#include` directives only increase the duration of compilation. 

#### Syntax

Please use the `<>` characters over `""` when writing include paths, and use filepaths relative to `include/` or `lib/`:

```cpp
#include "include/Pack/RPGraphics.h" // ❌ Don't do this!!!
#include <include/Pack/RPGraphics.h> // ❌ Don't do this!!!
#include "Pack/RPGraphics.h"         // ❌ Don't do this!!!
#include <Pack/RPGraphics.h>         // ✅ Do this!!!
```

#### Vs. forward declarations

If a forward declaration can resolve the dependency, favor that over `#include`'ing the corresponding file:


```cpp
#include <Pack/RPGraphics.h> // ❌ Don't do this!!!

class RPGrpRenderer {
public:
    void AppendDrawObject(IRPGrpDrawObject* pDrawObject);
```

```cpp
// Forward declarations
class IRPGrpDrawObject; // ✅ Do this!!!

class RPGrpRenderer {
public:
    void AppendDrawObject(IRPGrpDrawObject* pDrawObject);
```

The only exception is when dealing with symbols from other libraries:

```cpp
#include <egg/core.h>   // ✅ From different library, so #include is OK.

// Forward declarations
class IRPGrpDrawObject; // ✅ From same library, so forward declare it.

class RPGrpRenderer {
public:
    static void SetEfbClearColor(const EGG::Color& rColor);
    void AppendDrawObject(IRPGrpDrawObject* pDrawObject);
```

#### Public header files

Many libraries/modules have a "public" or "external" header file that will in-turn `#include` all of its respective files.

For example, `include/revolution/NAND.h` is the public header file for the NAND library:
```c
#ifndef RVL_SDK_PUBLIC_NAND_H
#define RVL_SDK_PUBLIC_NAND_H
#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/NAND/NANDCheck.h>
#include <revolution/NAND/NANDCore.h>
#include <revolution/NAND/NANDOpenClose.h>
#include <revolution/NAND/nand.h>

#ifdef __cplusplus
}
#endif
#endif
```

Using public headers is OK as long as:
1. They are from another library
2. You are writing the `#include` directive in an implementation file (`*.c`/`*.cpp`) 

Otherwise, it is easy to form a circular dependency.
Here's an example!

`libkiwi/core/kiwiColor.h`:
```cpp
#include <libkiwi.h>                     // ❌ It will cause a circular dependency!
#include <libkiwi/core/kiwiConsoleOut.h> // ✅ Include only what you use.
#include <revolution/NAND.h>             // ✅ NAND is in another library (RVL SDK).
```

`libkiwi/core/kiwiColor.cpp`:
```cpp
#include <libkiwi.h>                     // ✅ OK because this is the implementation file!
#include <libkiwi/core/kiwiConsoleOut.h> // ✅ Also OK.
#include <revolution/NAND.h>             // ✅ NAND is in another library (RVL SDK).
```

#### libkiwi

All libkiwi header files must at least include the types header file:
```c
#include <libkiwi/k_types.h>
```

### Naming Scheme

#### Functions

Please write function names in PascalCase:
```cpp
void myFunction();  // ❌ Don't do this!!!
void my_function(); // ❌ Don't do this!!!
void MyFunction();  // ✅ Do this!!!
```

#### Data

Please use the following prefixes for data members/function parameters:

| Prefix | Meaning                      |
| ------ | ---------------------------- |
| `m`    | C++ class private member     |
| `s`    | C/C++ static, private member |
| `p`    | C/C++ pointer-typed variable |
| `r`    | C++ reference-typed variable |

If no prefix is required, write the name in camelCase.

These can be combined/stacked in the same order as the table; however, **the member (`m`) and static (`s`) prefixes are mutually exclusive**:

```cpp
class X {
public:
    int var;  // ✅ Do this!!!
    int mVar; // ❌ Don't do this!!! This is a public member!

private:
    int id;  // ❌ Don't do this!!! This is a private member!
    int mID; // ✅ Do this!!!

    int* mNum;    // ❌ Don't do this!!! This is a pointer!
    int* mpNum;   // ✅ Do this!!!
    int** mppNum; // ✅ Do this!!!

    int& mHandle;  // ❌ Don't do this!!! This is a reference!
    int& mrHandle; // ✅ Do this!!!

    static void* mpBuffer;  // ❌ Don't do this!!! This is a static member!
    static void* mspBuffer; // ❌ Don't do this!!! Don't combine 'm' and 's'!
    static void* spBuffer;  // ✅ Do this!!!
};

int MemCopy(void* buffer, u32 size);  // ❌ Don't do this!!! 'buffer' is a pointer!
int MemCopy(void* pBuffer, u32 size); // ✅ Do this!!!

bool GetHandle(int& handle);  // ❌ Don't do this!!! 'handle' is a reference!
bool GetHandle(int& rHandle); // ✅ Do this!!!
```

#### Macros

Please write macros in uppercase SNAKE_CASE.

#### Exceptions

The only exceptions to these rules are identifiers that must be named otherwise due to the decompilation, such as functions/data with known symbols, and the `OSIsMEM` family of macros.

### Declaration Order

Please use the following order when writing C++ classes:

1. Public members
2. Protected members
3. Private members

#### Order per section
In each section (public/protected/private), use the following order of declarations:

- Type aliases (`friend`, `typedef`, `using`)
- Nested types
- Static constants
- ### **[Re-declare the access specifier here]**
- Constructor(s) and assignment operator(s)
- Destructor
- Static functions
- Non-static functions
- ### **[Re-declare the access specifier here]**
- Non-constant, static members
- All other data members

```cpp
class FruitBasket : public IBasket {
// 1. PUBLIC ACCESS
public:
    // 1. Type aliases (`friend`, `typedef`, `using`)
    friend class BasketMgr;
    
    typedef TList<FruitBasker> FruitBasketList;
    using IBasket::Foo;

    // 2. Nested types
    struct Fruit : public IObject {
        String name;
        Color color;
    };

    // 3. Static constants
    static const int FRUIT_MAX = 10;

// Re-declare access specifier
public:
    // 4. Constructor(s) and assignment operator(s)
    FruitBasket();
    FruitBasket& operator=(const FruitBasket& rOther);
    // 5. Destructor
    virtual ~FruitBasket();

    // 6. Static functions
    static String GetName() {
        return "FruitBasket";
    }

    // 7. Non-static functions
    void AddFruit(const Fruit& rFruit) {
        mFruits.PushBack(rFruit);
    }

// 2. PROTECTED ACCESS
protected:
    // 7. Non-static functions
    const Fruit& GetFruit(int i) const {
        return mFruits[i];
    }

// 3. PRIVATE ACCESS
private:
    // 7. Non-static functions
    void Clear();

    // 8. Non-constant, static members
    static TList<FruitBasket> sInstances;

    // 9. All other data members
    TList<Fruit> mFruits;
};
```

#### Exceptions

The only exceptions to these rules are classes where certain members must be declared higher up because of the placement of the class's virtual table. This is only important for ensuring interoperability with classes in the DOL.

### Header Implementation

With older compilers it was common to implement function bodies in a header file so that they can be inlined across translation units. Because Kokeshi relies on the CodeWarrior compiler, this is still is a potential optimization.

However, Kokeshi code is compiled with size optimizations, not speed optimizations. As a result, the compiler will often not inline functions unless they are one or two instructions.

So, please only implement functions in-header **if they are a single C/C++ statement** (`return *this` in operators does not count). This does not include ASSERT macros, as they are stripped from release builds.

Don't bother implementing virtual function bodies in-header, as they will never be inlined unless the call is manually devirtualized.

**NOTE: These restrictions does not apply to templated functions, although templated *member functions* should go in a .hpp file (see `libkiwi/prim/kiwiVectorImpl.hpp`).**

### Doxygen
Kokeshi uses Doxygen to offer a website view of the codebase.

At the time of writing, this documentation lives at [packproject.dev/Kokeshi](https://packproject.dev/Kokeshi) and is updated as the main branch changes.

#### Syntax

- Use the `@` prefix to issue Doxygen commands, rather than `\`.
- Use `//!` for single-line Doxygen comments, rather than  `///`.

#### Groups

The only required Doxygen grouping is for modules that are not distinguished by namespaces. This is especially true for C code where there is no such construct as a "namespace".

For example, the Doxygen group hierarchy `NW4R` -> `ut` is represented by the contents of the namespace `nw4r::ut`.

For some libraries this relation does not occur. EGG and libkiwi have many modules but only one namespace. To get around this, the contents of their header files are divided into Doxygen groups depending on the directory the file is in.

Below is the outline of `libkiwi/core/kiwiColor.h`. It exists in the `core` module of libkiwi, but must be explicitly placed into a group because it only uses the root `kiwi` namespace:

```cpp
#ifndef LIBKIWI_CORE_COLOR_H
#define LIBKIWI_CORE_COLOR_H
#include <libkiwi/k_types.h>
#include <nw4r/ut.h>

namespace kiwi {
//! @addtogroup libkiwi_core
//! @{

    // File contents . . .

//! @}
} // namespace kiwi

#endif
```

**To see a list of all Doxygen groups, please see `include/__doxygen.hpp`.**

#### Comments

Comments do not have heavy restrictions but should follow a consistent format to generate readable Doxygen pages:

##### Syntax

For functions, write the brief description such that it could be prefixed with "This function" and still make sense:

```
// ❌ [This function] **clear** the memory buffer
/**
 * @brief Clear the memory buffer
 */
void Clear();

// ✅ [This function] **clears** the memory buffer
/**
 * @brief Clears the memory buffer
 */
void Clear();
```

If the function is a simple check that returns a boolean result, start the description with "Tests whether":

```
/**
 * @brief Tests whether the buffer is empty
 */
bool Empty() const;
```

##### Structures

Structures (`struct`/`class`) should at least provide a brief description:
```
/**
 * @brief Interface for binary file serialization/deserialization
 */
class IBinary {
public:
```

##### Enumerations

Enumerations should at least provide a brief description:
```
/**
 * @brief Stream seek origin
 */
enum ESeekDir { ESeekDir_Begin, ESeekDir_Current, ESeekDir_End };
```

##### Functions

Functions should at least provide a brief description, documenting all parameters:

```
/**
 * @brief Advances this stream's position
 *
 * @param dir Seek direction
 * @param offset Seek offset
 */
void Seek(ESeekDir dir, s32 offset);
```

If the return value is not obvious, explain its purpose with the `@return` command:

```
/**
 * @brief Writes data to this stream (internal implementation)
 *
 * @param pSrc Source buffer
 * @param size Number of bytes to write
 * @return Number of bytes written, or DVD error code
 */
s32 WriteImpl(const void* pSrc, u32 size);
```

If a pointer/reference typed parameter is meant to be given an initial value through the function, mark it as an *out* parameter, like so:

```
/**
 * @brief Gets the console's IP address
 *
 * @param rAddr[out] IPv4 address
 */
void GetHostAddr(SockAddr4& rAddr)
```

##### Macros

Macros can be documented like functions, but also a single-line comment will suffice when there are no parameters, or their purpose is obvious:

```
/**
 * @brief Helper for creating version numbers
 * 
 * @param major Major version number
 * @param minor Minor version number
 */
#define K_VERSION(major, minor) ((major << 8) | minor)

//! Helper for creating version numbers
#define K_VERSION(major, minor) ((major << 8) | minor)
```

##### Data

Data members should provide a single-line brief description. It does not matter whether this is a comment above or after the declaration:

```
//! Stream open flag
bool mIsOpen; 
//! Position in data 
u32 mPosition;

bool mIsOpen;  //!< Stream open flag
u32 mPosition; //!< Position in data
```

##### Constructors/Destructors

The brief description for constructors should be "Constructor", and the brief description for destructors should be "Destructor":

```
/**
 * @brief Constructor
 */
IStream();

/**
 * @brief Destructor
 */
virtual ~IStream();
```

If there are many constructor overloads for purposes such as moving/copying, feel free to specify the constructor "type" in the Doxygen detailed description:

```
/**
 * @brief Constructor
 */
Optional();

/**
 * @brief Constructor
 * @details Null constructor
 */
Optional(NullOpt);

/**
 * @brief Constructor
 * @details Copy constructor
 *
 * @param rOther Optional value
 */
Optional(const Optional& rOther);

/**
 * @brief Constructor
 * @details Copy constructor
 *
 * @param rValue Value
 */
Optional(const T& rValue);

#ifdef LIBKIWI_CPP1X
/**
 * @brief Constructor
 * @details Move constructor
 *
 * @param rOther Optional value
 */
Optional(const Optional&& rOther);

/**
 * @brief Constructor
 * @details Move constructor
 *
 * @param rValue Optional value
 */
Optional(const T&& rValue);
#endif
```
