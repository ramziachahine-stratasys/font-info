#pragma once
#include <nan.h>
#include <uv.h>
#include <v8.h>
#include <vector>

struct AsyncRequest {
    uv_work_t work;
    std::vector<FontDescriptor *> *result;
    Nan::Callback *callback;
    const char *name;

    AsyncRequest( v8::Local<v8::Function> callback, const char *name = "font-info:async-callback") {
        work.data = (void *)this;
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
        v8::Local<v8::Value> info[1];

        if (result) {
            info[0] = FontDescriptor::collect(result);
        } else {
            info[0] = Nan::Null();
        }

        this->callback->Call(1, info, &async);
    }
};
