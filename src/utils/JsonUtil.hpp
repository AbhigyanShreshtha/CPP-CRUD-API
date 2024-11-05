#pragma once
#include <boost/json.hpp>
#include <string>

class JsonUtil {
public:
    static boost::json::value parse(const std::string& str) {
        return boost::json::parse(str);
    }

    static std::string serialize(const boost::json::value& json) {
        return boost::json::serialize(json);
    }
};
