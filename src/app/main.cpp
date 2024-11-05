#include "controllers/CrudController.hpp"
#include "services/InMemoryKeyValueService.hpp"
#include "utils/WebSocketBroadcaster.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <thread>
#include <iostream>

using namespace boost::asio;
using tcp = ip::tcp;

const int HTTP_PORT = 8080;
const int WEBSOCKET_PORT = 8081;

class HttpSession : public std::enable_shared_from_this<HttpSession> {
    tcp::socket socket_;
    boost::beast::flat_buffer buffer_;
    boost::beast::http::request<boost::beast::http::string_body> req_;
    CrudController& controller_;

public:
    HttpSession(tcp::socket socket, CrudController& controller)
        : socket_(std::move(socket)), controller_(controller) {}

    void run() { doRead(); }

private:
    void doRead() {
        auto self = shared_from_this();
        boost::beast::http::async_read(socket_, buffer_, req_,
            [self](boost::beast::error_code ec, std::size_t) {
                if (!ec) self->handleRequest();
            });
    }

    void handleRequest() {
        boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::ok, req_.version()};
        res.set(boost::beast::http::field::server, "Beast");
        res.keep_alive(req_.keep_alive());

        // Delegate request handling to the CrudController
        controller_.handleRequest(req_, res);

        auto self = shared_from_this();
        boost::beast::http::async_write(socket_, res,
            [self](boost::beast::error_code ec, std::size_t) {
                self->socket_.shutdown(tcp::socket::shutdown_send, ec);
            });
    }
};

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
    boost::beast::websocket::stream<tcp::socket> ws_;
    WebSocketBroadcaster& broadcaster_;

public:
    WebSocketSession(tcp::socket socket, WebSocketBroadcaster& broadcaster)
        : ws_(std::move(socket)), broadcaster_(broadcaster) {
        broadcaster_.addClient(std::make_shared<boost::beast::websocket::stream<tcp::socket>>(std::move(ws_)));
    }

    void run() {
        ws_.async_accept([self = shared_from_this()](boost::beast::error_code ec) {
            if (!ec) self->doRead();
        });
    }

private:
    void doRead() {
        auto self = shared_from_this();
        boost::beast::flat_buffer buffer;
        ws_.async_read(buffer,
            [self, buffer = std::move(buffer)](boost::beast::error_code ec, std::size_t) mutable {
                if (!ec) self->doRead();
            });
    }
};

void startHttpServer(io_context& ioc, CrudController& controller) {
    auto endpoint = tcp::endpoint(tcp::v4(), HTTP_PORT);
    auto acceptor = std::make_shared<tcp::acceptor>(ioc, endpoint);

    acceptor->async_accept(
        [&ioc, &controller, acceptor](boost::beast::error_code ec, tcp::socket socket) {
            if (!ec) std::make_shared<HttpSession>(std::move(socket), controller)->run();
            startHttpServer(ioc, controller);
        });
}

void startWebSocketServer(io_context& ioc, WebSocketBroadcaster& broadcaster) {
    auto endpoint = tcp::endpoint(tcp::v4(), WEBSOCKET_PORT);
    auto acceptor = std::make_shared<tcp::acceptor>(ioc, endpoint);

    acceptor->async_accept(
        [&ioc, &broadcaster, acceptor](boost::beast::error_code ec, tcp::socket socket) {
            if (!ec) std::make_shared<WebSocketSession>(std::move(socket), broadcaster)->run();
            startWebSocketServer(ioc, broadcaster);
        });
}

int main() {
    try {
        io_context ioc{1};

        auto service = std::make_shared<InMemoryKeyValueService>();
        WebSocketBroadcaster broadcaster;
        CrudController controller(service, broadcaster);

        startHttpServer(ioc, controller);
        startWebSocketServer(ioc, broadcaster);

        std::cout << "Server running on HTTP port " << HTTP_PORT << " and WebSocket port " << WEBSOCKET_PORT << "\n";

        ioc.run();
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}