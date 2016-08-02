

#include <memory>
#include <mutex>
#include <thread>

#include <android/log.h>

#include <jni.h>

#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/util/stat_summarizer.h"

//#include "jnipp.hpp"

#include "./tensorflow.h"
#include "./loader.hpp"

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
        std::unique_ptr<android::log> log;
        std::unique_ptr<tensorflow::Session> session;
        std::unique_ptr<tensorflow::StatSummarizer> status;
    public:
        toro()
            :log{ new android::log{ "OSUSHI" } } {
                initialize();
        }
        ~toro(){
            destroy();
        }
        void destroy(){
            log.reset(nullptr);
            session.reset(nullptr);
            status.reset(nullptr);
        }
        bool initialize(){
            return true;
        }
    public:
    };

    // NO T destruct
    template<typename T>
    struct storage_only_deleter {
        void operator()(T* ptr){
            delete[] reinterpret_cast<char*>(reinterpret_cast<void*>(ptr));
        }
    };
    template<typename T>
    using wrapper = std::unique_ptr<T, storage_only_deleter<T>>;

    jbyteArray get_jptr(JNIEnv* env, void* ptr){
        jbyteArray pointer = env->NewByteArray(sizeof(void*));
        env->SetByteArrayRegion(pointer, 0, sizeof(void*), (jbyte*)ptr);
        return pointer;
    }
    template<typename T>
    wrapper<T> revert_jptr(JNIEnv* env, jbyteArray ptr){
        auto len = env->GetArrayLength(ptr);
        auto buffer = wrapper<T>{reinterpret_cast<T*>(new char[(size_t)len])};
        env->GetByteArrayRegion(ptr, 0, len, reinterpret_cast<jbyte*>(buffer.get()));
        return buffer;
    }
}

std::string get_this_thread_id_string(){
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

JNIEXPORT jbyteArray JNICALL OSUSHI_NATIVE_METHOD(create)(JNIEnv* env, jobject instance, jstring j_model_name){
    const char* const model_name = env->GetStringUTFChars(j_model_name, NULL);

    //static char const* model_name = "model-KO0csvBjXVu4VQjcQvm";

    auto toro = new osushi::toro();
    toro->log->i("new: %p", toro);
    toro->log->d("tid: %s", get_this_thread_id_string().c_str());

    tensorflow::SessionOptions options;
    tensorflow::ConfigProto& config = options.config;
    toro->log->i("Got config, %d devices.", config.device_count_size());

    toro->session.reset(tensorflow::NewSession(options));
    toro->log->i("Session created.");

    tensorflow::GraphDef graph;
    toro->log->i("Graph created.");

    toro->log->i("Reading file to proto: %s", model_name);
    auto ok = osushi::load_model(model_name, &graph);
    if (!ok) {
        env->FatalError("Could not open model file.");
    }

    toro->status.reset(new tensorflow::StatSummarizer(graph));

    toro->log->i("Creating session.");
    tensorflow::Status s = toro->session->Create(graph);
    if (!s.ok()) {
        env->FatalError("Could not create TensorFlow Graph");
    }

    // Clear the proto to save memory space.
    graph.Clear();
    toro->log->i("TensorFlow graph loaded from: %s", model_name);

    return osushi::get_jptr(env, toro);
}

JNIEXPORT jboolean JNICALL OSUSHI_NATIVE_METHOD(destroy)(JNIEnv* env, jobject instance, jbyteArray ptr){
    auto toro = osushi::revert_jptr<osushi::toro>(env, ptr);
    toro->log->i("delete: %p", toro.get());
    toro->destroy();
    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL OSUSHI_NATIVE_METHOD(method)(JNIEnv* env, jobject instance, jbyteArray ptr){
    auto toro = osushi::revert_jptr<osushi::toro>(env, ptr);
    toro->log->i("Call method");
    return JNI_TRUE;
}



