/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_company_HelloJNI */

#ifndef _Included_com_company_HelloJNI
#define _Included_com_company_HelloJNI
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_company_HelloJNI
 * Method:    sayHello
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_company_HelloJNI_sayHello__
  (JNIEnv *, jobject);

/*
 * Class:     com_company_HelloJNI
 * Method:    sayHello
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_company_HelloJNI_sayHello__Ljava_lang_String_2
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_company_HelloJNI
 * Method:    executeScript
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_company_HelloJNI_executeScript
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif