#include "CrudController.hpp"
#include <ctime>

std::string current_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    return std::ctime(&time);
}

void CrudController::handleRequest(const boost::beast::http::request<boost::beast::http::string_body>& req, boost::beast::http::response<boost::beast::http::string_body>& res) {
    boost::json::value json_val = JsonUtil::parse(req.body());

    const auto& json_obj = json_val.as_object();

    std::string key = json_obj.at("key").as_string().c_str();
    std::string value = JsonUtil::serialize(json_obj.at("value"));

    if (req.method() == boost::beast::http::verb::post) {
        service->insert({key, value});
        broadcaster.broadcast("Insert: " + key + ", " + value + ", " + current_timestamp());
        res.result(boost::beast::http::status::ok);
        res.body() = "Inserted";
    } else if (req.method() == boost::beast::http::verb::get) {
        auto result = service->retrieve(key);
        if (result) {
            broadcaster.broadcast("Retrieve: " + key + ", " + *result + ", " + current_timestamp());
            res.result(boost::beast::http::status::ok);
            res.body() = *result;
        } else {
            res.result(boost::beast::http::status::not_found);
            res.body() = "Key not found";
        }
    } else if (req.method() == boost::beast::http::verb::delete_) {
        service->remove(key);
        broadcaster.broadcast("Delete: " + key + ", " + current_timestamp());
        res.result(boost::beast::http::status::ok);
        res.body() = "Deleted";
    }
    res.prepare_payload();
}
