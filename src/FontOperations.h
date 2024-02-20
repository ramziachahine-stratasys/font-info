#pragma once
#define WINVER 0x600;
#include <dwrite.h>
#include "FontDescriptor.h"

class FontOperations 
{
    public:
        static char *get_string(IDWriteFont *font, DWRITE_INFORMATIONAL_STRING_ID id);
        static FontDescriptor *from(IDWriteFont *font);
        static std::vector<FontDescriptor*> *get_available_fonts();
};
