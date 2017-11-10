//
//  jsdb.cpp
//  v8HelloWorld
//
//  Created by 吕途 on 2017/11/7.
//  Copyright © 2017年 吕途. All rights reserved.
//

#include "jsob.hpp"
#include "V8JNI.hpp"

#pragma mark java对象和CPP对象互相存储
jobject createConnection(const char* url){
    JNIEnv* env = resetVM();
    jclass cls2 = env->FindClass("com/company/HelloJNI");
    if(cls2 == nullptr) {
        printf("[cpp]ERROR: class not found !\n");
        return nullptr;
    }
    printf("[cpp]Class MyTest found\n");
    jmethodID mid = env->GetStaticMethodID(cls2, "createConn", "(Ljava/lang/String;)Lcom/company/JConnection;");  // find method
    if(mid == nullptr){
        printf("[cpp]ERROR: method void mymain() not found !");
        return nullptr;
    }else {
        jobject job = env->CallStaticObjectMethod(cls2, mid,env->NewStringUTF(url));
        return job;
    }
}


CConnection::CConnection(const char* url){
    try{
    jobject jconn = createConnection(url);
    this->jconn = jconn;
    }
    catch(v8::Exception e){
        printf("[cpp]CConnection new error.\n");
    }
}

char* CConnection::query(char *sql){
    printf("[cpp]%s\n",sql);
    
    try
    {
        jobject jconn = this->GetJConn();
        JNIEnv* env = resetVM();
        jclass cls = env->GetObjectClass(jconn);
        jmethodID mid = env->GetMethodID(cls, "query", "(Ljava/lang/String;)Ljava/lang/String;");
        if(mid == nullptr){
            printf("[cpp]ERROR: method query not found !");
            return nullptr;
        }
        jstring str = (jstring)env->CallObjectMethod(jconn, mid,env->NewStringUTF(sql) );
        jboolean isCopy;
        return strdup(env->GetStringUTFChars(str, &isCopy));
    }
    catch(v8::Exception e){
        printf("[cpp]get native pointer from java object error.\n");
    }
    
    return "";
}

void CConnection::close(){
    printf("close connection.\n");
}

jobject CConnection::GetJConn(){
    return this->jconn;
}
