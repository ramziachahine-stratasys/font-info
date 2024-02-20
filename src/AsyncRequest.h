#pragma once
#include <nan.h>
#include <uv.h>
#include <v8.h>
#include <vector>

#include "FontDescriptor.h"

struct AsyncRequest {

public:
    AsyncRequest(v8::Local<v8::Value> v) {
        work.data = (void *)this;
        callback = new Nan::Callback(v.As<v8::Function>());
        result = nullptr;
    }

    ~AsyncRequest() {
        delete callback;
        callback = nullptr;
    }

private:
    uv_work_t work;
    std::vector<FontDescriptor*> *result;
    Nan::Callback *callback;
};
