#include <grpcpp/grpcpp.h>
#include "sketchboard.grpc.pb.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void SendUpdate(shared_ptr<Sketchboard::Stub> stub, int x, int y, const string& color) {
    DrawingUpdate update;
    update.set_x(x);
    update.set_y(y);
    update.set_color(color);

    Ack ack;
    grpc::ClientContext context;

    grpc::Status status = stub->UpdateDrawing(&context, update, &ack);
    if (status.ok()) {
        cout << "Update sent successfully: (" << x << ", " << y << ") Color: " << color << endl;
    } else {
        cerr << "Failed to send update: " << status.error_message() << endl;
    }
}

void FetchBoardState(shared_ptr<Sketchboard::Stub> stub) {
    Empty request;
    BoardState board_state;
    grpc::ClientContext context;

    grpc::Status status = stub->GetBoardState(&context, request, &board_state);
    if (status.ok()) {
        for (const auto& update : board_state.updates()) {
            cout << "Drawing at (" << update.x() << ", " << update.y() << ") Color: " << update.color() << endl;
        }
    } else {
        cerr << "Failed to fetch board state: " << status.error_message() << endl;
    }
}

int main() {
    auto stub = Sketchboard::NewStub(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));

    thread sender([&]() {
        for (int i = 0; i < 10; ++i) {
            SendUpdate(stub, i, i, "red");
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    });

    thread fetcher([&]() {
        while (true) {
            FetchBoardState(stub);
            this_thread::sleep_for(chrono::seconds(2));
        }
    });

    sender.join();
    fetcher.detach();

    return 0;
}
