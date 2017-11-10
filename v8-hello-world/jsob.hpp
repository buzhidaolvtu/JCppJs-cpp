//
//  jsdb.hpp
//  v8HelloWorld
//
//  Created by 吕途 on 2017/11/7.
//  Copyright © 2017年 吕途. All rights reserved.
//

#ifndef jsob_hpp
#define jsob_hpp
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
class CConnection{
public:
    CConnection(const char* url);
    
public:
    char* query(char* sql);
    void close();
    jobject GetJConn();
    
private:
    jobject jconn;
};

#endif /* jsdb_hpp */
