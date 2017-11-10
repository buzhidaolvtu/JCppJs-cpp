//
//  com_company_JSContext.cpp
//  v8HelloWorld
//
//  Created by 吕途 on 2017/11/10.
//  Copyright © 2017年 吕途. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/libplatform/libplatform.h"
#include "include/v8.h"

#include "com_company_V8.h"
#include "com_company_JSContext.h"
#include "V8JNI.hpp"
using namespace v8;

//每一个这样的变量对应一个java context的实例，但是这里作为全局变量出现，有问题，需要修改
JV8Context* jV8CurrentContext;
//每一个这样的变量对应一个context_template.
Local<ObjectTemplate> gFuncObTpl;

void _ConnectionConstructor(const v8::FunctionCallbackInfo<v8::Value>& args);
void _query(const v8::FunctionCallbackInfo<v8::Value>& args);
Local<Value> ExecuteScript(Local<String> script_str);

/*
 * Class:     com_company_JSContext
 * Method:    initialize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_company_JSContext_initialize
(JNIEnv *env, jobject thisObj)
{
    // 初始化v8的context
    JV8Context* jV8Context = new JV8Context(env->NewGlobalRef(thisObj));
    
    //把jV8Context保存到对应的java对象中，这样就可以形成一一对应的关系。
    jclass JSContext_class = env->GetObjectClass(thisObj);
    jlong jv8ContextPointer = (jlong)jV8Context;
    jfieldID jv8ContextPointer_fieldID = env->GetFieldID(JSContext_class, "jv8ContextPointer", "J");
    env->SetLongField(JSContext_class, jv8ContextPointer_fieldID, jv8ContextPointer);
    
    Isolate* isolate = v8Engin->GetIsolate();
    
    Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);
    Local<Context> context = jV8Context->GetContext();
    Context::Scope context_scope(context);
    //这里实现Connection
    //implement Connection function
    Local<FunctionTemplate> FuncTpl = FunctionTemplate::New(isolate,_ConnectionConstructor);
    //实现Connection.query函数
    Local<ObjectTemplate> FuncObTpl = FuncTpl->InstanceTemplate();
    gFuncObTpl = FuncObTpl;//这个变量作为把对象作为函数调用时的模板变量，请参照使用细节。
    FuncObTpl->Set(String::NewFromUtf8(isolate,"query"), FunctionTemplate::New(isolate,_query));
    FuncObTpl->SetInternalFieldCount(1);
    //这个函数一定要在上面配置完之后才能配置，否则InstanceTemplate在初始化connection时会不生效。
    context->Global()->Set(String::NewFromUtf8(isolate,"Connection"),FuncTpl->GetFunction());
    //------------------------------------
}

/*
 * Class:     com_company_JSContext
 * Method:    executeScript
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_company_JSContext_executeScript
(JNIEnv *env, jobject obj, jstring str){
    printf("[cpp] start execute script.\n");
    
    jclass JSContext_class = env->GetObjectClass(obj);
    jfieldID jv8ContextPointer_fieldID = env->GetFieldID(JSContext_class, "jv8ContextPointer", "J");
    jlong jv8ContextPointer = env->GetLongField(JSContext_class, jv8ContextPointer_fieldID);
    JV8Context* jV8Context = (JV8Context*)jv8ContextPointer;
    
    jV8CurrentContext = jV8Context;
    
    Isolate* isolate = v8Engin->GetIsolate();
    
    Isolate::Scope isolate_scope(isolate);
    HandleScope handle_scope(isolate);
    Local<Context> context = jV8Context->GetContext();
    Context::Scope context_scope(context);
    
    jboolean isCopy;
    const char* ss = env->GetStringUTFChars(str, &isCopy);
    printf("[cpp] script : %s\n", ss);
    Local<Value> value = ExecuteScript(String::NewFromUtf8(isolate, ss));
    String::Utf8Value utf8(value);
    printf("[cpp] result: %s\n", *utf8);
    return env->NewStringUTF(*utf8);
}

/*
 * Class:     com_company_JSContext
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_company_JSContext_close
(JNIEnv *env, jobject thisObj){
    printf("[cpp] close v8 engin.");
}

Local<Value> ExecuteScript(Local<String> script_str){
    Isolate *isolate = Isolate::GetCurrent();
    EscapableHandleScope handle_scope(isolate);
    
    Local<Context> context = isolate->GetCurrentContext();
    
    Local<Script> script = Script::Compile(context, script_str).ToLocalChecked();
    // Run the script to get the result.
    Local<Value> result = script->Run(context).ToLocalChecked();
    
    return handle_scope.Escape(result);
}



