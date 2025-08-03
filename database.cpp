#include "database.h"

void Database::set(const std::string& key, const std::string& value) {
    store[key] = value;
}

std::string Database::get(const std::string& key) const {
    auto it = store.find(key);
    return it != store.end() ? it->second : "(nil)\n";
}

bool Database::del(const std::string& key) {
    return store.erase(key) > 0;
}
