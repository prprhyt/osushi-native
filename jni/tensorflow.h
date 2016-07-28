#ifndef OSUSHI_NATIVE_TENSORFLOW_H
#define OSUSHI_NATIVE_TENSORFLOW_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif// __cplusplus

#define OSUSHI_NATIVE_METHOD(METHOD_NAME) \
  Java_jagsc_dlfa_osushi_NativeOsushi_##METHOD_NAME  // NOLINT


JNIEXPORT jbyteArray JNICALL OSUSHI_NATIVE_METHOD(create)(JNIEnv* env, jobject instance);
JNIEXPORT jboolean JNICALL OSUSHI_NATIVE_METHOD(method)(JNIEnv* env, jobject instance, jbyteArray ptr);

#ifdef __cplusplus
}
#endif// __cplusplus

#endif// OSUSHI_NATIVE_TENSORFLOW_H
