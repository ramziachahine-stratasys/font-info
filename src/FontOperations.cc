#define WINVER 0x600;

#include <dwrite.h>
#include <unordered_set>

#include <StringUtils.h>
#include <FontDescriptor.h>

char *getString(IDWriteFont *font, DWRITE_INFORMATIONAL_STRING_ID id)
{
    char *result = nullptr;
    IDWriteLocalizedStrings *locStrings;

    BOOL exists;
    font->GetInformationalStrings(id, &locStrings, &exists);

    if(exists)
    {
        unsigned int index;
        unsigned int sz;
        
        locStrings->GetStringLength(index, &sz);
        
        wchar_t *s = new wchar_t[sz + 1];

        locStrings->GetString(index, s, sz + 1);
        result = utf16ToUtf8(s);
        
        // clean up
        delete []s;
        locStrings->Release();
    }

    return result;
}

FontDescriptor *fromIDWriteFont(IDWriteFont *font)
{
    FontDescriptor *fontDescriptor;
    IDWriteFontFace *fontFace;
    IDWriteFontFile *files;
    unsigned int nFiles;

    font->CreateFontFace(&fontFace);
    fontFace->GetFiles(&nFiles, nullptr);
    fontFace->GetFiles(&nFiles, &files);

    if (nFiles > 0)
    {
        IDWriteFontFileLoader *loader;
        IDWriteLocalFontFileLoader *localLoader;

        unsigned int nameSz;
        const void *rKey;
        unsigned int rKeySz;

        wchar_t *name;

        files[0].GetLoader(&loader);

        HRESULT hRes = loader->QueryInterface(__uuidof(IDWriteLocalFontFileLoader), (void **)&localLoader);
        if (SUCCEEDED(hRes))
        {
            files[0].GetReferenceKey(&rKey, &rKeySz);
            localLoader->GetFilePathLengthFromKey(rKey, rKeySz, &nameSz);

            name = new wchar_t[nameSz];
            localLoader->GetFilePathFromKey(rKey, rKeySz, name, nameSz + 1);

            char *family = getString(font, DWRITE_INFORMATIONAL_STRING_WIN32_FAMILY_NAMES);
            char *style = getString(font, DWRITE_INFORMATIONAL_STRING_WIN32_SUBFAMILY_NAMES);

            if(family && style)
            {
                fontDescriptor = new FontDescriptor(
                    utf16ToUtf8(name),
                    family,
                    style
                );
            }

            delete []family;
            delete []style;
            delete []name; 
        }

        localLoader->Release();
        loader->Release();
    }
    
    fontFace->Release();
    files->Release();
    return fontDescriptor;
}

std::vector<FontDescriptor*> *getAvailableFonts() 
{

    std::vector<FontDescriptor *> *result = new std::vector<FontDescriptor*>();
    std::unordered_set<std::string> uniqueFonts;

    IDWriteFactory *factory = nullptr;
    DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED, 
        __uuidof(IDWriteFactory), 
        reinterpret_cast<IUnknown **>(&factory)
    );

    IDWriteFontCollection *collection = nullptr;
    factory->GetSystemFontCollection(&collection);

    int nFamilies = collection->GetFontFamilyCount();

    for(int i = 0; i < nFamilies; i++) 
    {
        IDWriteFontFamily *family;
        collection->GetFontFamily(i, &family);

        int nFonts = family->GetFontCount();
        for(int j = 0; j < nFonts; j++) 
        {
            IDWriteFont *font = nullptr;
            family->GetFont(j, &font);

            FontDescriptor *fontDescriptor = fromIDWriteFont(font);
            if(fontDescriptor && uniqueFonts.count(fontDescriptor->getPath()) == 0)
            {
                uniqueFonts.insert(fontDescriptor->getPath());
                result->push_back(fontDescriptor);
            }
        }

        family->Release();
    }

    collection->Release();
    factory->Release();    
    return result;
} 