# Sketchboard gRPC Service

This project implements a simple gRPC service for a collaborative sketchboard.

## Prerequisites

- [gRPC](https://grpc.io/docs/languages/cpp/quickstart/)
- [Protocol Buffers](https://developers.google.com/protocol-buffers)
- C++ compiler

## Setting Up the Environment

1. Install gRPC and Protocol Buffers:

    ```sh
    git clone -b v1.42.0 https://github.com/grpc/grpc
    cd grpc
    git submodule update --init
    mkdir -p cmake/build
    cd cmake/build
    cmake ../.. -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR
    make -j4
    make install
    ```

2. Set environment variables:

    ```sh
    export PATH=$MY_INSTALL_DIR/bin:$PATH
    export LD_LIBRARY_PATH=$MY_INSTALL_DIR/lib:$LD_LIBRARY_PATH
    ```

## Building the Project

1. Generate the gRPC and Protocol Buffers code:

    ```sh
    protoc -I=. --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` sketchboard.proto
    protoc -I=. --cpp_out=. sketchboard.proto
    ```

2. Compile the server and client:

    ```sh
    g++ -std=c++17 -o server server.cpp sketchboard.pb.cc sketchboard.grpc.pb.cc -lgrpc++ -lprotobuf -lpthread
    g++ -std=c++17 -o client client.cpp sketchboard.pb.cc sketchboard.grpc.pb.cc -lgrpc++ -lprotobuf -lpthread
    ```

## Running the Project

1. Start the server:

    ```sh
    ./server
    ```

2. In another terminal, start the client:

    ```sh
    ./client
    ```

The client will send drawing updates to the server and fetch the board state in real-time.