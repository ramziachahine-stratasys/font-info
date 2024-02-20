#include <stdlib.h>
#include <string.h>

/**
 * Convert a UTF-8 string to a UTF-16 string.
 * Allocates memory to Heap.
*/
wchar_t *utf8_to_utf16(const char *utf8) {
    const size_t sz = strlen(utf8) + 1;
    wchar_t* utf16 = new wchar_t[sz];
    mbstowcs(utf16, utf8, sz);
    return utf16;
}

/**
 * Convert a UTF-16 string to a UTF-8 string.
 * Allocates memory to Heap.
*/
char *utf16_to_utf8(const wchar_t *utf16) {
    const size_t sz = (wcslen(utf16) + 1) * 2;
    char *utf8 = new char[sz];
    int converted_chars = 0;
    wctomb_s(&converted_chars, utf8, sz, *utf16);
    return utf8;
}