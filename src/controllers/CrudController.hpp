#pragma once
#include "services/KeyValueService.hpp"
#include "utils/JsonUtil.hpp"
#include "utils/WebSocketBroadcaster.hpp"
#include <boost/beast/http.hpp>
#include <memory>

class CrudController {
    std::shared_ptr<KeyValueService> service;
    WebSocketBroadcaster& broadcaster;

public:
    CrudController(std::shared_ptr<KeyValueService> svc, WebSocketBroadcaster& ws)
        : service(svc), broadcaster(ws) {}

    void handleRequest(const boost::beast::http::request<boost::beast::http::string_body>& req,
                       boost::beast::http::response<boost::beast::http::string_body>& res);
};
