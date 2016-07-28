#include "./tensorflow.h"

#include <memory>
#include <mutex>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>

#include <jni.h>

#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/logging.h"

namespace android {
    template<typename... T>
    void print(android_LogPriority level, char const* tag, T... args){
        __android_log_print(level, tag, args...);
    }
    class log {
    private:
        std::string _tag;
    public:
        log(std::string const& _tag)
            : _tag{ _tag }
        {}
        template<typename... T>
        void d(T... args){
            print(ANDROID_LOG_DEBUG, _tag.c_str(), )
        }
        template<typename... T>
        void i(T... args){
            print(ANDROID_LOG_INFO, _tag.c_str(), )
        }
    }
}

namespace osushi {
    class toro {
    public:
        android::log log;
    public:
        toro()
            :log{ "OSUSHI" } {
                initialize();
        }
        bool initialize(){
            return true;
        }
    public:
    };

    jbyteArray get_jptr(void* ptr){
        jbyteArray pointer = env->NewByteArray(sizeof(void*));
        env->SetByteArrayRegion(pointer, 0, sizeof(void*), (jbyte*)ptr);
        return pointer;
    }
    void* revert_jptr(jbyteArray ptr){
        void* buffer;
        env->GetByteArrayRegion(ptr, 0, sizeof(buffer), (jbyte*)&buffer);
        return buffer;
    }
}

JNIEXPORT jbyteArray JNICALL OSUSHI_NATIVE_METHOD(create)(JNIENV* env, jobject instance){
    auto toro = new osushi::toro;
    toro.log.i("OSUSHI was successfully created.");
    return osushi::get_jptr(toro);
}

JNIEXPORT jboolean JNICALL OSUSHI_NATIVE_METHOD(method)(JNIENV* env, jobject instance, jbyteArray ptr){
    auto toro = (osushi::toro*)osushi::revert_ptr(ptr);
    toro->log.i("Call method");
    return JNI_TRUE;
}



