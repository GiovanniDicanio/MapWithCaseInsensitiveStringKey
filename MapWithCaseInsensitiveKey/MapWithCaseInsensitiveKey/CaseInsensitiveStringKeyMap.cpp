//////////////////////////////////////////////////////////////////////////////////////////
//
//                  Test std::map with case insensitive string keys
//
//                              by Giovanni Dicanio
//
//////////////////////////////////////////////////////////////////////////////////////////


//========================================================================================
//                                      Includes
//========================================================================================

#include <crtdbg.h>     // _ASSERTE
#include <Windows.h>    // CompareStringEx

#include <iostream>     // wcout
#include <map>          // map
#include <string>       // wstring

#define ASSERT(expr)    _ASSERTE(expr)


//========================================================================================
//             Infrastructure for Maps with Case Insensitive String Keys
//========================================================================================

// Wrapper around the Windows CompareStringEx C API
inline int CompareStringIgnoreCase(const std::wstring& s1, const std::wstring& s2)
{
    // According to the MSDN documentation, the CompareStringEx function 
    // is optimized for NORM_IGNORECASE and string lengths specified as -1.

    return ::CompareStringEx(
        LOCALE_NAME_INVARIANT,
        NORM_IGNORECASE,
        s1.c_str(),
        -1,
        s2.c_str(),
        -1,
        nullptr,        // reserved
        nullptr,        // reserved
        0               // reserved
    );
}

// Comparison object for std::map, ignoring string case
struct StringIgnoreCaseLess
{
    bool operator()(const std::wstring& s1, const std::wstring& s2) const
    {
        // (s1 < s2) ignoring string case
        return CompareStringIgnoreCase(s1, s2) == CSTR_LESS_THAN;
    }
};

// Alias template for maps with case insensitive string keys
template <typename Value>
using CaseInsensitiveStringMap = std::map<std::wstring, Value, StringIgnoreCaseLess>;


//========================================================================================
// Some tests
//========================================================================================
int main()
{
    using std::map;
    using std::wcout;
    using std::wstring;

    wcout << L"*** Testing Map with Case Insensitive String Keys ***\n\n";

    CaseInsensitiveStringMap<wstring> dictionary
    {
        { L"ciao", L"hi" },
        { L"gatto", L"cat" },
        { L"perch\x00E9", L"because" }
    };


    ASSERT(dictionary[L"ciao"] == L"hi");
    ASSERT(dictionary[L"Ciao"] == L"hi");
    ASSERT(dictionary[L"CIAO"] == L"hi");
    ASSERT(dictionary[L"CIaO"] == L"hi");

    ASSERT(dictionary[L"gatto"] == L"cat");
    ASSERT(dictionary[L"GATTO"] == L"cat");
    ASSERT(dictionary[L"Gatto"] == L"cat");
    ASSERT(dictionary[L"gAtTo"] == L"cat");
    ASSERT(dictionary[L"gatTO"] == L"cat");

    ASSERT(dictionary[L"perch\x00E9"] == L"because");
    ASSERT(dictionary[L"PERch\x00E9"] == L"because");
    ASSERT(dictionary[L"PERCH\x00C9"] == L"because");
    ASSERT(dictionary[L"PeRCh\x00C9"] == L"because");

    wcout << L"All right.\n";
}

//////////////////////////////////////////////////////////////////////////////////////////
