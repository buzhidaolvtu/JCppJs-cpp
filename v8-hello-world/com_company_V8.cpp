//
//  com_company_V8.cpp
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
#include "V8JNI.hpp"
using namespace v8;

/*
 * Class:     com_company_V8
 * Method:    initialize0
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_company_V8_initialize0
(JNIEnv *env, jclass thisObj, jstring path){
    jint ret = env->GetJavaVM(&vm);
    if(ret == JNI_OK){
        printf("[cpp]get java vm ok.\n");
    }
    jboolean isCopy;
    const char* pathss = env->GetStringUTFChars(path, &isCopy);
    v8Engin=new V8Engin(pathss);
}

