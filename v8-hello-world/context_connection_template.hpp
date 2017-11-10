//
//  context_template.hpp
//  v8HelloWorld
//
//  Created by 吕途 on 2017/11/10.
//  Copyright © 2017年 吕途. All rights reserved.
//

#ifndef context_connection_template_hpp
#define context_connection_template_hpp

#include <stdio.h>
#include "include/libplatform/libplatform.h"
#include "include/v8.h"

void _Connection(const v8::FunctionCallbackInfo<v8::Value>& args);
void _ConnectionConstructor(const v8::FunctionCallbackInfo<v8::Value>& args);
void _query(const v8::FunctionCallbackInfo<v8::Value>& args);


#endif /* context_connection_template_hpp */
