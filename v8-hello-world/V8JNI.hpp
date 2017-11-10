//
//  V8.hpp
//  v8-hello-world
//
//  Created by 吕途 on 2017/11/8.
//  Copyright © 2017年 吕途. All rights reserved.
//

#ifndef V8jni_hpp
#define V8jni_hpp

#include <stdio.h>
#include <jni.h>
#include "include/libplatform/libplatform.h"
#include "include/v8.h"

class V8Engin;

extern JavaVM *vm;
extern V8Engin *v8Engin;

JNIEnv* resetVM();

class V8Engin{
public:
    V8Engin(const char* path);
    ~V8Engin();
    
public:
    v8::Isolate* GetIsolate();
    
private:
    v8::Platform* platform;
    v8::Isolate::CreateParams *p_create_params;
    v8::Isolate *isolate;
};

//和JSContext实例一一对应，是java.JSContext在cpp的封装表示。
class JV8Context{
public:
    JV8Context(jobject jcontext);
    
public:
    v8::Local<v8::Context> GetContext();

private:
    v8::Persistent<v8::Context> persistent_context;
    jobject jcontext;
};

#endif /* V8_hpp */
