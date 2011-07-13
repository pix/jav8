/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class lu_flier_script_V8ScriptEngineFactory */

#ifndef _Included_lu_flier_script_V8ScriptEngineFactory
#define _Included_lu_flier_script_V8ScriptEngineFactory
#ifdef __cplusplus
extern "C" {
#endif
/* Inaccessible static: names */
/* Inaccessible static: mimeTypes */
/* Inaccessible static: extensions */
/*
 * Class:     lu_flier_script_V8ScriptEngineFactory
 * Method:    getParameter
 * Signature: (Ljava/lang/String;)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_lu_flier_script_V8ScriptEngineFactory_getParameter
  (JNIEnv *, jobject, jstring);

/*
 * Class:     lu_flier_script_V8ScriptEngineFactory
 * Method:    initialize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_lu_flier_script_V8ScriptEngineFactory_initialize
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
/* Header for class lu_flier_script_V8CompiledScript */

#ifndef _Included_lu_flier_script_V8CompiledScript
#define _Included_lu_flier_script_V8CompiledScript
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     lu_flier_script_V8CompiledScript
 * Method:    internalCompile
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_lu_flier_script_V8CompiledScript_internalCompile
  (JNIEnv *, jobject, jstring);

/*
 * Class:     lu_flier_script_V8CompiledScript
 * Method:    internalRelease
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_lu_flier_script_V8CompiledScript_internalRelease
  (JNIEnv *, jobject, jlong);

/*
 * Class:     lu_flier_script_V8CompiledScript
 * Method:    internalExecute
 * Signature: (JLjavax/script/ScriptContext;)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_lu_flier_script_V8CompiledScript_internalExecute
  (JNIEnv *, jobject, jlong, jobject);

#ifdef __cplusplus
}
#endif
#endif
/* Header for class lu_flier_script_V8Context */

#ifndef _Included_lu_flier_script_V8Context
#define _Included_lu_flier_script_V8Context
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     lu_flier_script_V8Context
 * Method:    getEntered
 * Signature: ()Llu/flier/script/V8Context;
 */
JNIEXPORT jobject JNICALL Java_lu_flier_script_V8Context_getEntered
  (JNIEnv *, jclass);

/*
 * Class:     lu_flier_script_V8Context
 * Method:    getCurrent
 * Signature: ()Llu/flier/script/V8Context;
 */
JNIEXPORT jobject JNICALL Java_lu_flier_script_V8Context_getCurrent
  (JNIEnv *, jclass);

/*
 * Class:     lu_flier_script_V8Context
 * Method:    getCalling
 * Signature: ()Llu/flier/script/V8Context;
 */
JNIEXPORT jobject JNICALL Java_lu_flier_script_V8Context_getCalling
  (JNIEnv *, jclass);

/*
 * Class:     lu_flier_script_V8Context
 * Method:    inContext
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_lu_flier_script_V8Context_inContext
  (JNIEnv *, jclass);

/*
 * Class:     lu_flier_script_V8Context
 * Method:    internalCreate
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_lu_flier_script_V8Context_internalCreate
  (JNIEnv *, jclass);

/*
 * Class:     lu_flier_script_V8Context
 * Method:    internalRelease
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_lu_flier_script_V8Context_internalRelease
  (JNIEnv *, jobject, jlong);

/*
 * Class:     lu_flier_script_V8Context
 * Method:    internalEnter
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_lu_flier_script_V8Context_internalEnter
  (JNIEnv *, jobject, jlong);

/*
 * Class:     lu_flier_script_V8Context
 * Method:    internalLeave
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_lu_flier_script_V8Context_internalLeave
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
/* Header for class lu_flier_script_V8Object */

#ifndef _Included_lu_flier_script_V8Object
#define _Included_lu_flier_script_V8Object
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     lu_flier_script_V8Object
 * Method:    internalRelease
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_lu_flier_script_V8Object_internalRelease
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
