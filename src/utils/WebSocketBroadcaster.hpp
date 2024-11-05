#pragma once
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <vector>
#include <mutex>

class WebSocketBroadcaster {
    using WebSocketStream = boost::beast::websocket::stream<boost::asio::ip::tcp::socket>;

    std::vector<WebSocketStream*> clients;
    std::mutex mtx;

public:
    void addClient(WebSocketStream& client);
    void broadcast(const std::string& message);
};
