#pragma once
#include "models/KeyValueModel.hpp"
#include <optional>
#include <string>

class KeyValueService {
public:
    virtual void insert(const KeyValueModel& model) = 0;
    virtual std::optional<std::string> retrieve(const std::string& key) = 0;
    virtual void remove(const std::string& key) = 0;
    virtual ~KeyValueService() = default;
};
