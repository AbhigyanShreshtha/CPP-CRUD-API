#include "WebSocketBroadcaster.hpp"

void WebSocketBroadcaster::addClient(std::shared_ptr<WebSocketStream> client) {
    std::lock_guard<std::mutex> lock(mtx);
    clients.push_back(client);
}

void WebSocketBroadcaster::broadcast(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& client : clients) {
        if (client->is_open()) {
            client->write(boost::asio::buffer(message));
        }
    }
}
