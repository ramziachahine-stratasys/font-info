#include <unordered_set>
#include <string>
#include <stdlib.h>

#include "StringUtils.h"
#include "FontOperations.h"

char *FontOperations::get_string(IDWriteFont *font, DWRITE_INFORMATIONAL_STRING_ID id)
{
    char *result = nullptr;
    IDWriteLocalizedStrings *loc_strings;

    BOOL exists;
    font->GetInformationalStrings(id, &loc_strings, &exists);

    if(exists)
    {
        unsigned int index;
        unsigned int sz;
        
        loc_strings->GetStringLength(index, &sz);
        
        wchar_t *s = new wchar_t[sz + 1];

        loc_strings->GetString(index, s, sz + 1);
        result = utf16_to_utf8(s);
        
        // clean up
        delete []s;
        loc_strings->Release();
    }

    return result;
}

FontDescriptor *FontOperations::from(IDWriteFont *font)
{
    FontDescriptor *font_descriptor;
    IDWriteFontFace *font_face;
    IDWriteFontFile *files;
    unsigned int n_files;

    font->CreateFontFace(&font_face);
    font_face->GetFiles(&n_files, nullptr);
    font_face->GetFiles(&n_files, &files);

    if (n_files > 0)
    {
        IDWriteFontFileLoader *loader;
        IDWriteLocalFontFileLoader *local_loader;

        unsigned int name_sz;
        const void *r_key;
        unsigned int r_key_sz;

        wchar_t *name;

        files[0].GetLoader(&loader);

        HRESULT h_res = loader->QueryInterface(__uuidof(IDWriteLocalFontFileLoader), (void **)&local_loader);
        if (SUCCEEDED(h_res))
        {
            files[0].GetReferenceKey(&r_key, &r_key_sz);
            local_loader->GetFilePathLengthFromKey(r_key, r_key_sz, &name_sz);

            name = new wchar_t[name_sz];
            local_loader->GetFilePathFromKey(r_key, r_key_sz, name, name_sz + 1);

            char *family = get_string(font, DWRITE_INFORMATIONAL_STRING_WIN32_FAMILY_NAMES);
            char *style = get_string(font, DWRITE_INFORMATIONAL_STRING_WIN32_SUBFAMILY_NAMES);

            if(family && style)
            {
                font_descriptor = new FontDescriptor(
                    utf16_to_utf8(name),
                    family,
                    style
                );
            }

            delete []family;
            delete []style;
            delete []name; 
        }

        local_loader->Release();
        loader->Release();
    }
    
    font_face->Release();
    files->Release();
    return font_descriptor;
}

std::vector<FontDescriptor*> *FontOperations::get_available_fonts()
{

    std::vector<FontDescriptor *> *result = new std::vector<FontDescriptor*>();
    std::unordered_set<std::string> unq_fonts;

    IDWriteFactory *factory = nullptr;
    DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED, 
        __uuidof(IDWriteFactory), 
        reinterpret_cast<IUnknown **>(&factory)
    );

    IDWriteFontCollection *collection = nullptr;
    factory->GetSystemFontCollection(&collection);

    int n_families = collection->GetFontFamilyCount();

    for(int i = 0; i < n_families; i++) 
    {
        IDWriteFontFamily *family;
        collection->GetFontFamily(i, &family);

        int n_fonts = family->GetFontCount();
        for(int j = 0; j < n_fonts; j++) 
        {
            IDWriteFont *font = nullptr;
            family->GetFont(j, &font);

            FontDescriptor *font_descriptor = from(font);
            if(font_descriptor && !unq_fonts.contains(font_descriptor->get_path()))
            {
                unq_fonts.insert(font_descriptor->get_path());
                result->push_back(font_descriptor);
            } else {
                delete font_descriptor;
            }
        }

        family->Release();
    }

    collection->Release();
    factory->Release();    
    return result;
} 