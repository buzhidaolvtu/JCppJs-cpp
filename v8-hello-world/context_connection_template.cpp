//
//  context_template.cpp
//  v8HelloWorld
//
//  Created by 吕途 on 2017/11/10.
//  Copyright © 2017年 吕途. All rights reserved.
//

#include "context_connection_template.hpp"
#include "jsob.hpp"
#include "V8JNI.hpp"

using namespace v8;
extern Local<ObjectTemplate> gFuncObTpl;


void _Connection(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::HandleScope handle_scope(args.GetIsolate());
    Local<String> url = args[0]->ToString();
    String::Utf8Value urlstr(url);
    printf("[cpp] _Connection url : %s\n",*urlstr);
    //这里调用Java函数
    Local<Object> ret = gFuncObTpl->NewInstance();
    CConnection *conn = new CConnection(*urlstr);
    ret->SetInternalField(0, External::New(args.GetIsolate(),conn));
    // This will give the script a return value
    args.GetReturnValue().Set(ret);
}

# pragma mark _ConnectionConstructor
void _ConnectionConstructor(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if(args.IsConstructCall()){
        v8::HandleScope handle_scope(args.GetIsolate());
        Local<String> url = args[0]->ToString();
        String::Utf8Value urlstr(url);
        printf("[cpp] _ConnectionConstructor url : %s\n",*urlstr);
        //这里调用Java函数
        CConnection *conn = new CConnection(*urlstr);
        Local<Object> obj = args.This();
        int c = obj->InternalFieldCount();
        printf("[cpp] %i\n",c);
        obj->SetInternalField(0, External::New(args.GetIsolate(),conn));
    }else{
        _Connection(args);
    }
}

#pragma mark 从Java查询数据并返回给javascript
void _query(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::HandleScope handle_scope(args.GetIsolate());
    Local<String> sql = args[0]->ToString();
    String::Utf8Value sqlstr(sql);
    printf("[cpp] sql : %s\n",*sqlstr);
    Local<Object> thisConn = args.This();
    Local<External> wrap = Local<External>::Cast(thisConn->GetInternalField(0));
    CConnection* conn = static_cast<CConnection*>(wrap->Value());
    //这里内部调用java函数
    char* str = conn->query(*sqlstr);
    args.GetReturnValue().Set(String::NewFromUtf8(args.GetIsolate(), str));
}

