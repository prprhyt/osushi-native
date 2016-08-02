# OSUSHI PROJECT: native code programs

## How to Build

### Step 1) Setup Tensorflow

Prepare the development environment for Tensorflow.

See: https://github.com/tensorflow/tensorflow/

It is easy if use Tensorflow-Android(https://github.com/ornew/tensorflow-android).

### Step 2) Clone this repository into Tensorflow repository.

```sh
$ cd <Tensorflow Dir>/tensorflow
$ mkdir proj
$ git clone https://github.com/ornew/osushi-native.git
```

### Step 3) Install JNI++ library

JNI++: https://github.com/ornew/jnipp

```sh
$ cd osushi-native
$ git clone https://github.com/ornew/jnipp.git
```

### Step 4) Build by Bazel.

You must use following options.

```sh
$ cd <Tensorflow Dir>
$ bazel build //tensorflow/proj/osushi-native:libosushi.so --crosstool_top=//external:android/crosstool --cpu=armeabi-v7a --host_crosstool_top=@bazel_tools//tools/cpp:toolchain
```

Output binary file is located in `<Tensorflow Dir>/bazel-bin/tensorflow/proj/osushi-native/libosushi.so`.

