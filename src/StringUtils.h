#include <stdlib.h>
#include <string.h>

/**
 * Convert a UTF-8 string to a UTF-16 string.
 * Allocates memory to Heap.
*/
wchar_t *utf8ToUtf16(const char *utf8) {
    const size_t cSize = strlen(utf8) + 1;
    wchar_t* utf16 = new wchar_t[cSize];
    mbstowcs(utf16, utf8, cSize);
    return utf16;
}

/**
 * Convert a UTF-16 string to a UTF-8 string.
 * Allocates memory to Heap.
*/
char *utf16ToUtf8(const wchar_t *utf16) {
    const size_t cSize = (wcslen(utf16) + 1) * 2;
    char *utf8 = new char[cSize];
    int convertedChars = 0;
    wctomb_s(&convertedChars, utf8, cSize, *utf16);
    return utf8;
}