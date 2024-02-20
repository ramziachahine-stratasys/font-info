#pragma once
#include <nan.h>
#include <v8.h>

struct FontDescriptor
{
public:
    [[nodiscard]]
    const char *get_path() const {
        return path;
    }
    [[nodiscard]]
    const char *get_family() const {
        return family;
    }
    [[nodiscard]]
    const char *get_style() const {
        return style;
    }

    explicit FontDescriptor(v8::Local<v8::Object> object)
    {
        path = nullptr;
        family = get_string(object, "family");
        style = get_string(object, "style");
        
    }

    FontDescriptor(const char *path, const char *family, const char *style)
    {
        this->path = copy_string(path);
        this->family = copy_string(family);
        this->style = copy_string(style);
    }

    FontDescriptor(const FontDescriptor &other)
    {
        this->path = copy_string(path);
        this->family = copy_string(family);
        this->style = copy_string(style);
    }

    ~FontDescriptor()
    {
        delete []path;
        delete []family;
        delete []style;
    }

    v8::Local<v8::Object> to_js_object() {
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

    v8::Local<v8::Value> wrap_font()
    {
        Nan::EscapableHandleScope scope;
        v8::Local<v8::Object> res = this->to_js_object();
        return scope.Escape(res);
    }

    static v8::Local<v8::Array> collect(std::vector<FontDescriptor *> *results)
    {
        Nan::EscapableHandleScope scope;
        v8::Local<v8::Array> res = Nan::New<v8::Array>(results->size());
        for (auto it = results->begin(); it != results->end(); ++it)
        {
            Nan::Set(res, it - results->begin(), (*it)->to_js_object());
        }
        return scope.Escape(res);
    }

private:
    // path to the font file
    const char *path;
    const char *family;
    // style of the font (regular, bold, italic, etc.)
    const char *style;

    static char *copy_string(const char *s)
    {
        if (s == nullptr)
        {
            return nullptr;
        }
        auto *copy = new char[strlen(s) + 1];
        strcpy(&copy[0], s);
        return copy;
    }

    static char *get_string(v8::Local<v8::Object> object, const char *property)
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

        const std::string str(*utf8_str);
        return copy_string(str.c_str());
    }
};
