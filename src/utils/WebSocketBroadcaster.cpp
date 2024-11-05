#include "WebSocketBroadcaster.hpp"
#include <iostream>

void WebSocketBroadcaster::addClient(WebSocketStream& client) {
    if (client.is_open()) {
        std::lock_guard<std::mutex> lock(mtx);
        clients.push_back(&client);
    }
}

void WebSocketBroadcaster::broadcast(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto it = clients.begin(); it != clients.end();) {
        WebSocketStream* client = *it;
        if (client && client->is_open()) {
            try {
                client->write(boost::asio::buffer(message));
                ++it;
            } catch (const boost::system::system_error& e) {
                std::cerr << "Broadcast error: " << e.what() << std::endl;
                it = clients.erase(it);  // Remove client if there's an error
            }
        } else {
            it = clients.erase(it);  // Remove closed or invalid clients
        }
    }
}
