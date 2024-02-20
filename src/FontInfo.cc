#include <nan.h>
#include <node.h>
#include <stdlib.h>
#include <uv.h>
#include <v8.h>

#include "FontDescriptor.h"


v8::Local<v8::Array> collect(std::vector<FontDescriptor*> *results)
{
    Nan::EscapableHandleScope scope;
    v8::Local<v8::Array> res = Nan::New<v8::Array>(results->size());

    for (std::vector<FontDescriptor*>::iterator it = results->begin(); it != results->end(); ++it)
    {
        Nan::Set(res, it - results->begin(), (*it)->toJsObject());
    }

    delete results;
    results = nullptr;

    return scope.Escape(res);
}

v8::Local<v8::Value> wrapFont(FontDescriptor *result)
{
	Nan::EscapableHandleScope scope;

	if(result == nullptr)
	{
		return scope.Escape(Nan::Null());
	}

	v8::Local<v8::Object> res = result->toJsObject();
	delete result;
	result = nullptr;
	return scope.Escape(res);
}
