#include "Wrapper.h"

#include <string.h>

#include "Utils.h"

namespace jni {

CJavaObject::~CJavaObject(void)
{
  for (fields_t::const_iterator it = m_fields.begin(); it != m_fields.end(); it++)
  {
    m_pEnv->DeleteGlobalRef(it->second);
  }

  m_fields.clear();

  for (methods_t::const_iterator it = m_methods.begin(); it != m_methods.end(); it++)
  {
    CJavaFunction::ReleaseMethods(m_pEnv, it->second);
  }
}

void CJavaObject::CacheNames(void) 
{
  jni::Env env(m_pEnv);

  static jmethodID midGetFields = env.GetMethodID("java/lang/Class", "getFields", "()[Ljava/lang/reflect/Field;");
  jobjectArray fields = (jobjectArray) m_pEnv->CallObjectMethod(env->GetObjectClass(m_obj), midGetFields);
  
  static jmethodID midGetFieldName = env.GetMethodID("java/lang/reflect/Field", "getName", "()Ljava/lang/String;");
  for (size_t i=0; i<env->GetArrayLength(fields); i++)
  {
    jobject field = env->GetObjectArrayElement(fields, i);

    std::string name = env.GetString((jstring) env->CallObjectMethod(field, midGetFieldName));
    
    m_fields[name] = env->NewGlobalRef(field);

    env->DeleteLocalRef(field);
  }

  static jmethodID midGetMethods = env.GetMethodID("java/lang/Class", "getMethods", "()[Ljava/lang/reflect/Method;");
  jobjectArray methods = (jobjectArray) m_pEnv->CallObjectMethod(env->GetObjectClass(m_obj), midGetMethods);

  static jmethodID midGetMethodName = env.GetMethodID("java/lang/reflect/Method", "getName", "()Ljava/lang/String;");
  
  for (size_t i=0; i<env->GetArrayLength(methods); i++)
  {
    jobject method = env->GetObjectArrayElement(methods, i);
    
    std::string name = env.GetString((jstring) env->CallObjectMethod(method, midGetMethodName));

    const types_t& types = CJavaFunction::GetParameterTypes(env, method);

    m_methods[name].push_back(std::make_pair(env->NewGlobalRef(method), types));

    env->DeleteLocalRef(method);
  }
}

v8::Handle<v8::Value> CJavaObject::NamedGetter(
  v8::Local<v8::String> prop, const v8::AccessorInfo& info)
{
  CJavaObject& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  v8::String::Utf8Value name(prop);

  {
    fields_t::const_iterator it = obj.m_fields.find(*name);

    if (it != obj.m_fields.end()) {
      static jmethodID mid = env.GetMethodID("java/lang/reflect/Field", "get", "(Ljava/lang/Object;)Ljava/lang/Object;");    

      return env.Close(env.Wrap(env->CallObjectMethod(it->second, mid, obj.m_obj)));
    }
  }

  {
    methods_t::const_iterator it = obj.m_methods.find(*name);

    if (it != obj.m_methods.end()) {
      return env.Close(CJavaFunction::Wrap(obj.m_pEnv, it->second));
    }
  }

  return __base__::NamedGetter(prop, info);
}
v8::Handle<v8::Value> CJavaObject::NamedSetter(
  v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
  CJavaObject& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  v8::String::Utf8Value name(prop);

  {
    fields_t::const_iterator it = obj.m_fields.find(*name);

    if (it != obj.m_fields.end()) {
      static jmethodID mid = env.GetMethodID("java/lang/reflect/Field", "set", "(Ljava/lang/Object;Ljava/lang/Object;)V");     

      env->CallVoidMethod(it->second, mid, obj.m_obj, env.Wrap(value));

      return value;
    }
  }

  return __base__::NamedSetter(prop, value, info);
}
v8::Handle<v8::Integer> CJavaObject::NamedQuery(
  v8::Local<v8::String> prop, const v8::AccessorInfo& info)
{
  CJavaObject& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  v8::String::Utf8Value name(prop);

  {
    fields_t::const_iterator it = obj.m_fields.find(*name);

    if (it != obj.m_fields.end()) {
      return env.Close(v8::Integer::New(v8::None));
    }
  }

  {
    methods_t::const_iterator it = obj.m_methods.find(*name);

    if (it != obj.m_methods.end()) {
      return env.Close(v8::Integer::New(v8::None));
    }
  }

  return __base__::NamedQuery(prop, info);
}
v8::Handle<v8::Array> CJavaObject::NamedEnumerator(const v8::AccessorInfo& info)
{
  CJavaObject& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  v8::Handle<v8::Array> result = v8::Array::New(obj.m_fields.size() + obj.m_methods.size());
  uint32_t idx = 0;
  
  for (fields_t::const_iterator it = obj.m_fields.begin(); it != obj.m_fields.end(); it++)
  {
    result->Set(idx++, v8::String::New(it->first.c_str(), it->first.size()));
  }
  
  for (methods_t::const_iterator it = obj.m_methods.begin(); it != obj.m_methods.end(); it++)
  {
    result->Set(idx++, v8::String::New(it->first.c_str(), it->first.size()));
  }

  return env.Close(result);
}


v8::Handle<v8::Value> CJavaArray::NamedGetter(
  v8::Local<v8::String> prop, const v8::AccessorInfo& info)
{
  CJavaArray& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  v8::String::Utf8Value name(prop);

  if (strcmp("length", *name) == 0) {
    return env.Close(v8::Uint32::New(obj.GetLength()));
  }

  return __base__::NamedGetter(prop, info);
}
v8::Handle<v8::Integer> CJavaArray::NamedQuery(
  v8::Local<v8::String> prop, const v8::AccessorInfo& info)
{
  CJavaArray& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  v8::String::Utf8Value name(prop);

  if (strcmp("length", *name) == 0) {
    return env.Close(v8::Integer::New(v8::None));
  }

  return __base__::NamedQuery(prop, info);
}

v8::Handle<v8::Value> CJavaArray::IndexedGetter(
  uint32_t index, const v8::AccessorInfo& info)
{
  CJavaArray& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  static jclass clazz = env.FindClass("java/lang/reflect/Array");
  static jmethodID mid = env.GetStaticMethodID("java/lang/reflect/Array", "get", "(Ljava/lang/Object;I)Ljava/lang/Object;");

  return env.Close(env.Wrap(env->CallStaticObjectMethod(clazz, mid, obj.m_obj, index)));
}
v8::Handle<v8::Value> CJavaArray::IndexedSetter(
  uint32_t index, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
  CJavaArray& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  static jclass clazz = env.FindClass("java/lang/reflect/Array");
  static jmethodID mid = env.GetStaticMethodID(clazz, "set", "(Ljava/lang/Object;ILjava/lang/Object;)V");

  env->CallStaticVoidMethod(clazz, mid, obj.m_obj, index, env.Wrap(value));

  return env.Close(value);
}
v8::Handle<v8::Integer> CJavaArray::IndexedQuery(
  uint32_t index, const v8::AccessorInfo& info)
{
  CJavaArray& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  if (index < obj.GetLength()) {
    return env.Close(v8::Integer::New(v8::None));
  }

  return __base__::IndexedQuery(index, info);
}
v8::Handle<v8::Array> CJavaArray::IndexedEnumerator(const v8::AccessorInfo& info)
{
  CJavaArray& obj = Unwrap(info.Holder());

  jni::V8Env env(obj.m_pEnv);

  size_t length = obj.GetLength();
  v8::Handle<v8::Array> result = v8::Array::New(length);

  for (size_t i=0; i<length; i++)
  {
    result->Set(v8::Uint32::New(i), v8::Int32::New(i)->ToString());
  }

  return env.Close(result);
}
CJavaFunction::CJavaFunction(JNIEnv *pEnv, jobject obj)
  : m_pEnv(pEnv)
{
  m_methods.push_back(std::make_pair(m_pEnv->NewGlobalRef(obj), GetParameterTypes(m_pEnv, obj)));
}
void CJavaFunction::ReleaseMethods(JNIEnv *pEnv, const methods_t& methods)
{
  for (size_t i=0; i<methods.size(); i++)
  {
    pEnv->DeleteGlobalRef(methods[i].first);

    const types_t& types = methods[i].second;

    for (size_t j=0; j<types.size(); j++)
    {
      pEnv->DeleteGlobalRef(types[j]);
    }
  }
}
const CJavaFunction::types_t CJavaFunction::GetParameterTypes(JNIEnv *pEnv, jobject method)
{
  jni::Env env(pEnv);

  static jmethodID mid = env.GetMethodID("java/lang/reflect/Method", "getParameterTypes", "()[Ljava/lang/Class;");

  jobjectArray classes = (jobjectArray) env->CallObjectMethod(method, mid);

  types_t types(env->GetArrayLength(classes));

  for (size_t j=0; j<types.size(); j++)
  {
    jobject type = env->GetObjectArrayElement(classes, j);

    types[j] = (jclass) env->NewGlobalRef(type);

    env->DeleteLocalRef(type);
  }

  return types;
}
jobject CJavaFunction::GetMethod(const v8::Arguments& args)
{
  jni::V8Env env(m_pEnv);

  jobject method = NULL;

  if (m_methods.size() > 1)
  {
    for (size_t i=0; i<m_methods.size(); i++)
    {      
      const types_t& types = m_methods[i].second;

      if (types.size() == args.Length())
      {
        bool same = true;

        method = m_methods[i].first;

        for (size_t j=0; j<types.size(); j++)
        {
          if (!CanConvert(types[j], args[j]))
          {
            same = false;
            break;
          }
        }

        if (same) break;
      }
    }
  }
  else
  {
    method = m_methods[0].first;
  }

  return method;
}
bool CJavaFunction::CanConvert(jclass clazz, v8::Handle<v8::Value> value)
{
  jni::V8Env env(m_pEnv);

  if (value->IsTrue() || value->IsFalse() || value->IsBoolean())
  {
    return env.IsAssignableFrom("java/lang/Boolean", clazz);
  }
  else if (value->IsInt32() || value->IsUint32())
  {
    return env.IsAssignableFrom("java/lang/Long", clazz) ||
           env.IsAssignableFrom("java/lang/Integer", clazz) ||
           env.IsAssignableFrom("java/lang/Short", clazz) ||
           env.IsAssignableFrom("java/lang/Byte", clazz);
  }
  else if (value->IsNumber())
  {
    return env.IsAssignableFrom("java/lang/Double", clazz) ||
           env.IsAssignableFrom("java/lang/Float", clazz);
  }
  else if (value->IsString())
  {
    return env.IsAssignableFrom("java/lang/String", clazz);
  }
  else if (value->IsDate())
  {
    return env.IsAssignableFrom("java/util/Date", clazz);
  }
  else if (value->IsArray())
  {
    return env.IsAssignableFrom("lu/flier/script/V8Array", clazz);
  }
  else if (value.IsEmpty() || value->IsNull() || value->IsUndefined() || value->IsObject())
  {
    return true;
  }
  
  return false;
}

v8::Handle<v8::Value> CJavaFunction::Caller(const v8::Arguments& args) 
{
  CJavaFunction& func = *static_cast<CJavaFunction *>(v8::Handle<v8::External>::Cast(args.Data())->Value());

  jni::V8Env env(func.GetEnv());

  bool hasThiz = CManagedObject::IsWrapped(args.This()->ToObject());
  jobject thiz = hasThiz ? CManagedObject::Unwrap(args.This()->ToObject()).GetObject() : NULL;
  
  jobjectArray params = (jobjectArray) env.NewObjectArray(args.Length());

  for (size_t i=0; i<args.Length(); i++)
  {
    env->SetObjectArrayElement(params, i, env.Wrap(args[i]));
  }
  
  jobject method = func.GetMethod(args);
  static jmethodID mid = env.GetMethodID("java/lang/reflect/Method", "invoke", "(Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;");
  
  jobject result = env->CallObjectMethod(method, mid, thiz, params);

  return env.Close(env.Wrap(result));  
}
/*
v8::Handle<v8::Value> CJavaContext::NamedGetter(
  v8::Local<v8::String> prop, const v8::AccessorInfo& info)
{
  CJavaContext& ctxt = Unwrap(info.Holder());

  jclass cls = ctxt.m_pEnv->GetObjectClass(ctxt.m_obj);

  
}
v8::Handle<v8::Value> CJavaContext::NamedSetter(
  v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{

}
v8::Handle<v8::Integer> CJavaContext::NamedQuery(
  v8::Local<v8::String> prop, const v8::AccessorInfo& info)
{

}
v8::Handle<v8::Boolean> CJavaContext::NamedDeleter(
  v8::Local<v8::String> prop, const v8::AccessorInfo& info)
{

}
v8::Handle<v8::Array> CJavaContext::NamedEnumerator(const v8::AccessorInfo& info)
{

}
*/
} // namespace jni