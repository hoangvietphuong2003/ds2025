#include <grpcpp/grpcpp.h>
#include "sketchboard.grpc.pb.h"
#include <iostream>
#include <mutex>
#include <vector>

using namespace std;

class SketchboardServiceImpl final : public Sketchboard::Service {
    mutex mutex_;
    BoardState board_state_;

public:
    grpc::Status UpdateDrawing(grpc::ServerContext* context, const DrawingUpdate* request, Ack* response) override {
        lock_guard<mutex> lock(mutex_);
        *board_state_.add_updates() = *request;
        response->set_success(true);
        return grpc::Status::OK;
    }

    grpc::Status GetBoardState(grpc::ServerContext* context, const Empty* request, BoardState* response) override {
        lock_guard<mutex> lock(mutex_);
        *response = board_state_;
        return grpc::Status::OK;
    }
};

void RunServer() {
    string server_address("0.0.0.0:50051");
    SketchboardServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    unique_ptr<grpc::Server> server(builder.BuildAndStart());
    cout << "Server listening on " << server_address << endl;
    server->Wait();
}

int main() {
    RunServer();
    return 0;
}
