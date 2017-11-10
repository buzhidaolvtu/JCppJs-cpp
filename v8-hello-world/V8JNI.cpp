//
//  V8.cpp
//  v8-hello-world
//
//  Created by 吕途 on 2017/11/8.
//  Copyright © 2017年 吕途. All rights reserved.
//

#include "V8JNI.hpp"

using namespace v8;

JavaVM *vm;
V8Engin *v8Engin;

/**
 * 待验证
 * 这里似乎有问题，isolate scope在函数结束之后，scope就会退出，然后创建的context也似乎会失效。
 */
JV8Context::JV8Context(jobject jcontext){
    this->jcontext = jcontext;
    Isolate* isolate = v8Engin->GetIsolate();
    Isolate::Scope isolate_scope(isolate);
    HandleScope handlescope(isolate);
    Local<Context> context = Context::New(isolate);
    Persistent<Context> persistent_context(isolate,context);
    this->persistent_context.Reset(isolate, persistent_context);
    persistent_context.Reset();
}

v8::Local<v8::Context> JV8Context::GetContext(){
    Isolate* isolate = v8Engin->GetIsolate();
    EscapableHandleScope handle_scope(isolate);
    return handle_scope.Escape(persistent_context.Get(isolate));
}

#pragma mark V8Engin
V8Engin::V8Engin(const char* path){
    V8::InitializeICUDefaultLocation(path);
    V8::InitializeExternalStartupData(path);
    Platform* platform = platform::CreateDefaultPlatform();
    V8::InitializePlatform(platform);
    V8::Initialize();
    // Create a new Isolate
    p_create_params = new Isolate::CreateParams;
    p_create_params->array_buffer_allocator =
    v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    isolate = Isolate::New(*p_create_params);
}

V8Engin::~V8Engin(){
    isolate->Dispose();
    V8::Dispose();
    V8::ShutdownPlatform();
    delete platform;
    delete p_create_params->array_buffer_allocator;
    delete p_create_params;
}

Isolate* V8Engin::GetIsolate(){
    return isolate;
}

JNIEnv* resetVM(){
    JNIEnv *env;
    jint ret = vm->AttachCurrentThread((void**)&env, NULL);
    if(ret == JNI_OK){
        printf("[cpp]AttachCurrentThread succeed.\n");
    }
    return env;
}





