#include "tensorflow/proj/osushi-native/tensorflow.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>

#include <jni.h>

using namespace tensorflow;

JNIEXPORT jstring JNICALL TENSORFLOW_METHOD(test)(JNIEnv* env, jobject instance)
{
    LOG(INFO) << "Loaded Native OSUSHI.";
    return (*env)->NewStringUTF(env, "UNKO");
}



