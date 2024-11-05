#pragma once
#include "KeyValueService.hpp"
#include <unordered_map>
#include <mutex>

class InMemoryKeyValueService : public KeyValueService {
    std::unordered_map<std::string, std::string> db;
    std::mutex db_mutex;

public:
    void insert(const KeyValueModel& model) override {
        std::lock_guard<std::mutex> lock(db_mutex);
        db[model.key] = model.value;
    }

    std::optional<std::string> retrieve(const std::string& key) override {
        std::lock_guard<std::mutex> lock(db_mutex);
        auto it = db.find(key);
        return it != db.end() ? std::optional<std::string>{it->second} : std::nullopt;
    }

    void remove(const std::string& key) override {
        std::lock_guard<std::mutex> lock(db_mutex);
        db.erase(key);
    }
};
