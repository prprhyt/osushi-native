#include "./tensorflow.h"

#include <memory>
#include <mutex>
#include <thread>

#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>

#include <jni.h>

#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/logging.h"

#include "jnipp.hpp"

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
            print(ANDROID_LOG_DEBUG, _tag.c_str(), args...);
        }
        template<typename... T>
        void i(T... args){
            print(ANDROID_LOG_INFO, _tag.c_str(), args...);
        }
    };
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

    jbyteArray get_jptr(JNIEnv* env, void* ptr){
        jbyteArray pointer = env->NewByteArray(sizeof(void*));
        env->SetByteArrayRegion(pointer, 0, sizeof(void*), (jbyte*)ptr);
        return pointer;
    }
    template<typename T>
    std::unique_ptr<T> revert_jptr(JNIEnv* env, jbyteArray ptr){
        auto len = env->GetArrayLength(ptr);
        auto buffer = std::unique_ptr<T>{reinterpret_cast<T*>(new char[(size_t)len])};
        env->GetByteArrayRegion(ptr, 0, len, reinterpret_cast<jbyte*>(buffer.get()));
        return buffer;
    }
}

std::string get_this_thread_id_string(){
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

JNIEXPORT jbyteArray JNICALL OSUSHI_NATIVE_METHOD(create)(JNIEnv* env, jobject instance){
    auto toro = new osushi::toro();
    toro->log.i("OSUSHI was successfully created.");
    toro->log.d(("tid: " + get_this_thread_id_string()).c_str());
    return osushi::get_jptr(env, toro);
}

JNIEXPORT jboolean JNICALL OSUSHI_NATIVE_METHOD(method)(JNIEnv* env, jobject instance, jbyteArray ptr){
    auto toro = osushi::revert_jptr<osushi::toro>(env, ptr);
    toro->log.i("Call method");
    toro->log.d(("tid: " + get_this_thread_id_string()).c_str());
    return JNI_TRUE;
}



