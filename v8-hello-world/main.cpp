//
//  main.cpp
//  v8-hello-world
//
//  Created by 吕途 on 2017/11/6.
//  Copyright © 2017年 吕途. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/libplatform/libplatform.h"
#include "include/v8.h"
using namespace v8;

/**
 * CallJSFunction(Handle<v8::Object>, string, Handle<Value>, UINT)
 * / Handle of the global that is running the script with desired function
 * / Title of the JS fuction
 * / List of arguments for function
 * / Number of agrguments
 * Returns the return value of the JS function
 **/
Handle<v8::Value> CallJSFunction(Isolate* isolate,Handle<v8::Object> global, std::string funcName, Handle<Value> argList[], unsigned int argCount){
    // Create value for the return of the JS function
    Handle<Value> js_result;
    // Grab JS function out of file
//    Local<Object> global = isolate->GetCurrentContext()->Global();
    Handle<v8::Value> value = global->Get(String::NewFromUtf8(isolate,funcName.c_str()));
    // Cast value to v8::Function
    Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
    // Call function with all set values
    js_result = func->Call(global, argCount, argList);
    // Return value from function
    return js_result;
}

/**
 * Add[DataType]ToArguments(string/double/bool, Handle<Value>, UINT)
 * / [datatype] value to assign to argument list
 * / pass in arguments handle
 * / position in argument list to
 * This function will eaily convert and set the values for an argument list
 * to easily pass into a JS function you are calling from C++
 * JSEx: Func(arg[0], arg[1], ..)
 **/
void AddStringToArguments(Isolate* isolate, std::string str, Handle<Value> argList[], unsigned int argPos){
    argList[argPos] = String::NewFromUtf8(isolate,str.c_str());
}
void AddNumberToArguments(Isolate* isolate, double num, Handle<Value> argList[], unsigned int argPos){
    argList[argPos] = Number::New(isolate,num);
}
void AddBooleanToArguments(Isolate* isolate,bool value, Handle<Value> argList[], unsigned int argPos){
    argList[argPos] = Boolean::New(isolate,value);
}

/*
 * JS Function: myadd(number, ...)
 * /a number value to be added
 * Calculates the sum of all numbers entered
 */
void Add(const v8::FunctionCallbackInfo<v8::Value>& args) {
    int myVal = 0;
    for (int i = 0; i < args.Length(); i++) {        v8::HandleScope handle_scope(args.GetIsolate());        myVal += args[i]->Int32Value();
    }
    printf("%i\n", myVal);
//     This will give the script a return value
    args.GetReturnValue().Set(Number::New(args.GetIsolate(),myVal));
}

int main(int argc, char* argv[]) {
    // Initialize V8.
    V8::InitializeICUDefaultLocation(argv[0]);
    V8::InitializeExternalStartupData(argv[0]);
    Platform* platform = platform::CreateDefaultPlatform();
    V8::InitializePlatform(platform);
    V8::Initialize();
    // Create a new Isolate and make it the current one.
    Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
    v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    Isolate* isolate = Isolate::New(create_params);
    {
        Isolate::Scope isolate_scope(isolate);
        // Create a stack-allocated handle scope.
        HandleScope handle_scope(isolate);
        // Create a new context.
        Local<Context> context = Context::New(isolate);
        // Enter the context for compiling and running the hello world script.
        Context::Scope context_scope(context);
        // Create a string containing the JavaScript source code.
        Local<String> source =
        String::NewFromUtf8(isolate, "function JSrepeat(name, repeat) {     var printthis = '';     for(var i=0; i < repeat; i++){         printthis += name;     }     return printthis; } ;console.log('hello world!')",
                            NewStringType::kNormal).ToLocalChecked();
        // Compile the source code.
        Local<Script> script = Script::Compile(context, source).ToLocalChecked();
        // Run the script to get the result.
        Local<Value> result = script->Run(context).ToLocalChecked();

        // Convert the result to an UTF8 string and print it.
        String::Utf8Value utf8(result);
        printf("%s\n", *utf8);
        
        // c++ call js function
        // Create handle for arguements
        Handle<Value> args[2];
        
        // Create arguments to be passed into JS function
        AddStringToArguments(isolate,"BOSS", args, 0);
        AddNumberToArguments(isolate,5.0, args, 1);
        
        // Call the JS function
        Handle<Value> js_result = CallJSFunction(isolate,context->Global(), "JSrepeat", args, 2);
        String::Utf8Value utf8_2(js_result);
        printf("JSrepeat() returned: %s\n", *utf8_2);

        // js call c++ fuction
        context->Global()->Set(String::NewFromUtf8(isolate,"myadd"), FunctionTemplate::New(isolate,Add)->GetFunction());
        
        js_result = CallJSFunction(isolate,context->Global(), "myadd", args, 2);
        String::Utf8Value utf8_3(js_result);
        printf("JSrepeat() returned: %s\n", *utf8_3);


    }
    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    V8::Dispose();
    V8::ShutdownPlatform();
    delete platform;
    delete create_params.array_buffer_allocator;
    return 0;
}
