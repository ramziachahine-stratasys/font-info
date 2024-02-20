#include <nan.h>
#include <node.h>
#include <uv.h>
#include <v8.h>

#include "AsyncRequest.h"
#include "FontDescriptor.h"
#include "FontOperations.h"

void get_available_fonts(uv_work_t *work)
{
    AsyncRequest *request = static_cast<AsyncRequest*>(work->data);
    request->result = FontOperations::get_available_fonts();
}

template<bool Async>
NAN_METHOD(get_available_fonts)
{
    if(Async)
    {
        if(info.Length() < 1 || !info[0]->IsFunction())
        {
            return Nan::ThrowTypeError("First argument must be a callback function");
        }

        AsyncRequest *request = new AsyncRequest(info[0].As<v8::Function>(), "font-info:getAvailableFonts");
        uv_queue_work(uv_default_loop(), &request->work, get_available_fonts, (uv_after_work_cb)req->execute);
        return;
    }

    info.GetReturnValue().Set(FontDescriptor::collect(FontOperations::get_available_fonts()));
}

NAN_MODULE_INIT(Init)
{
    Nan::Export(target, "getAvailableFonts", get_available_fonts<true>);
    Nan::Export(target, "getAvailableFontsSync", get_available_fonts<false>);
}

NODE_MODULE(fontinfo, Init);