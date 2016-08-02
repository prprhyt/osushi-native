#include "./loader.hpp"

#include <stdlib.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/message_lite.h"

namespace {
    class IfstreamInputStream : public ::google::protobuf::io::CopyingInputStream {
    public:
        explicit IfstreamInputStream(const std::string& file_name)
            : ifs_(file_name.c_str(), std::ios::in | std::ios::binary) {}
        ~IfstreamInputStream() { ifs_.close(); }

        int Read(void* buffer, int size) {
            if (!ifs_) {
                return -1;
            }
            ifs_.read(static_cast<char*>(buffer), size);
            return ifs_.gcount();
        }

    private:
        std::ifstream ifs_;
    };
}

namespace osushi{
    bool load_model(char const* file_name, google::protobuf::MessageLite* graph){
        ::google::protobuf::io::CopyingInputStreamAdaptor stream(
                new IfstreamInputStream(file_name));
        stream.SetOwnsCopyingStream(true);
        // TODO(jiayq): the following coded stream is for debugging purposes to allow
        // one to parse arbitrarily large messages for MessageLite. One most likely
        // doesn't want to put protobufs larger than 64MB on Android, so we should
        // eventually remove this and quit loud when a large protobuf is passed in.
        ::google::protobuf::io::CodedInputStream coded_stream(&stream);
        // Total bytes hard limit / warning limit are set to 1GB and 512MB
        // respectively.
        coded_stream.SetTotalBytesLimit(1024LL << 20, 512LL << 20);
        return graph->ParseFromCodedStream(&coded_stream);
    }
}

