#pragma once
#include <v8.h>


struct FontDescriptor
{
public:
    const char *getPath() const {
        return path;
    }

    const char *getFamily() const {
        return family;
    }

    const char *getStyle() const {
        return style;
    }

    explicit FontDescriptor(v8::Local<v8::Object> object)
    {
        path = nullptr;
        family = getString(object, "family");
        style = getString(object, "style");
        
    }

    FontDescriptor(const char *path, const char *family, const char *style)
    {
        this->path = copyString(path);
        this->family = copyString(family);
        this->style = copyString(style);
    }

    FontDescriptor(const FontDescriptor &other)
    {
        this->path = copyString(path);
        this->family = copyString(family);
        this->style = copyString(style);
    }

    ~FontDescriptor()
    {
        delete path;
        delete family;
        delete style;

        family = nullptr;
        style = nullptr;
    }

    v8::Local<v8::Object> toJsObject() {
        Nan::EscapableHandleScope scope;
        v8::Local<v8::Object> res = Nan::New<v8::Object>();

        if (path)
        {
            Nan::Set(res, Nan::New<v8::String>("path").ToLocalChecked(), Nan::New<v8::String>(path).ToLocalChecked());
        }
        if (family)
        {
            Nan::Set(res, Nan::New<v8::String>("family").ToLocalChecked(), Nan::New<v8::String>(family).ToLocalChecked());
        }
        if (style)
        {
            Nan::Set(res, Nan::New<v8::String>("style").ToLocalChecked(), Nan::New<v8::String>(style).ToLocalChecked());
        }

        return scope.Escape(res);
    }

private:
    // path to the font file
    const char *path;
    const char *family;
    // style of the font (regular, bold, italic, etc.)
    const char *style;

    static char *copyString(const char *s)
    {
        if (s == nullptr)
        {
            return nullptr;
        }
        char *copy = new char[strlen(s) + 1];
        strcpy(&copy[0], s);
        return copy;
    }

    static char *getString(v8::Local<v8::Object> object, const char *property)
    {
        Nan::HandleScope scope;
        v8::MaybeLocal<v8::Value> val = Nan::Get(object, Nan::New<v8::String>(property).ToLocalChecked());

        if(val.IsEmpty() || !val.ToLocalChecked()->IsString())
        {
            return nullptr;
        }
    
        v8::String::Utf8Value utf8_str(v8::Isolate::GetCurrent(), val.ToLocalChecked());
        if(*utf8_str == nullptr)
        {
            return nullptr;
        }

        std::string str(*utf8_str);
        return copyString(str.c_str());
    }
};
