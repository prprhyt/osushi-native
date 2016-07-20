#include "./tensorflow.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>

#include <jni.h>

#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/logging.h"

JNIEXPORT jstring JNICALL TENSORFLOW_METHOD(test)(JNIEnv* env, jobject instance)
{
    LOG(INFO) << "Loaded Native OSUSHI.";
    return env->NewStringUTF("UNKO");
}



