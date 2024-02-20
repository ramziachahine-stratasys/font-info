#pragma once
#include <nan.h>
#include <uv.h>
#include <v8.h>
#include <vector>

#include "FontDescriptor.h"

struct AsyncRequest {
    uv_work_t work;
    std::vector<FontDescriptor *> *result;
    Nan::Callback *callback;
    const char *name;

    explicit AsyncRequest(v8::Local<v8::Function> callback, const char *name = "font-info:async-callback") {
        work.data = static_cast<void*>(this);
        this->name = name ;
        this->callback = new Nan::Callback(callback);
        result = nullptr;
    }

    ~AsyncRequest() {
        delete callback;
        delete []name;
    }

    void execute() {
        Nan::HandleScope scope;
        Nan::AsyncResource async(name);
        v8::Local<v8::Value> info[1] = {
        	info[0] = result ? FontDescriptor::collect(result) : Nan::Null()
        };

        this->callback->Call(1, info, &async);
    }
};
