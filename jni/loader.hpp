#ifndef OSUSHI_LOADER_HPP
#define OSUSHI_LOADER_HPP

#include <jni.h>

#include <string>
#include <vector>

#include "tensorflow/core/platform/types.h"

namespace google {
    namespace protobuf {
        class MessageLite;
    }  // google
} // protobuf

namespace osushi {
    bool load_model(char const* file_name, google::protobuf::MessageLite* graph);
}

#endif//OSUSHI_LOADER_HPP
