package(default_visibility = ["//visibility:public"])

load("//tensorflow:tensorflow.bzl", "tf_copts")

cc_binary(
    name = "libosushi.so",
    srcs = glob([
        "jni/**/*.cpp",
        "jni/**/*.h",
    ]) + [],
    copts = tf_copts(),
    linkopts = [
        "-landroid",
        "-ljnigraphics",
        "-llog",
        "-lm",
        "-z defs",
        "-s",
        "-Wl,--icf=all",  # Identical Code Folding
        "-Wl,--exclude-libs,ALL",  # Exclude syms in all libs from auto export
    ],
    linkshared = 1,
    linkstatic = 1,
    tags = [
        "manual",
        "notap",
    ],
    deps = ["//tensorflow/core:android_tensorflow_lib"],
)

