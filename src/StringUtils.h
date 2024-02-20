#pragma once
#include <cstdlib>
#include <string.h>

/**
 * Convert a UTF-8 string to a UTF-16 string.
 * Allocates memory to Heap.
*/
inline wchar_t *utf8_to_utf16(const char *utf8) {
    const size_t sz = strlen(utf8) + 1;
	auto *utf16 = new wchar_t[sz];
    size_t converted_chars = 0;
    mbstowcs_s(&converted_chars,utf16, sz, utf8, sz - 1);
    return utf16;
}

/**
 * Convert a UTF-16 string to a UTF-8 string.
 * Allocates memory to Heap.
*/
inline char *utf16_to_utf8(const wchar_t *utf16) {
    const size_t sz = (wcslen(utf16) + 1) * 2;
    auto *utf8 = new char[sz];
    int converted_chars = 0;
    wctomb_s(&converted_chars, utf8, sz, *utf16);
    return utf8;
}