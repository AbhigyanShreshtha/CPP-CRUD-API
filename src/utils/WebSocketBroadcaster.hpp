#pragma once
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <vector>
#include <mutex>
#include <memory>

class WebSocketBroadcaster {
    using WebSocketStream = boost::beast::websocket::stream<boost::asio::ip::tcp::socket>;

    std::vector<std::shared_ptr<WebSocketStream>> clients;
    std::mutex mtx;

public:
    void addClient(std::shared_ptr<WebSocketStream> client);
    void broadcast(const std::string& message);
};
