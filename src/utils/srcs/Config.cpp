//
// Created by Stepan Usatiuk on 01.05.2023.
//

#include "../includes/Config.h"
#include "../../repo/includes/Serialize.h"
#include "../includes/Exception.h"
#include <sstream>

Config &Config::add(const std::string &k, const std::string &v) {
    if (keys.count(k) == 0) throw Exception("Unknown key " + k);
    if (data.count(k) > 0)
        if (data.at(k) != v) throw Exception("Trying to rewrite config!");
        else if (data.at(k) == v)
            return *this;

    switch (keys.at(k).type) {
        case KeyType::STRING:
            break;
        case KeyType::INT:
            try {
                std::stoi(v);
            } catch (...) {
                throw Exception("Can't convert " + k + " to integer!");
            }
            break;
        case KeyType::LIST:
            break;
    }

    data.emplace(k, v);
    return *this;
}

int Config::getInt(const std::string &k) const {
    return std::stoi(getStr(k));
}

std::vector<std::string> Config::getList(const std::string &k) const {
    std::vector<std::string> out;
    std::string next;
    std::stringstream inss(getStr(k));
    while (std::getline(inss, next, ',')) {
        if (next != "")
            out.emplace_back(next);
    }
    return out;
}

std::string Config::getStr(const std::string &k) const {
    if (data.count(k) > 0) return data.at(k);
    else if (keys.at(k).defaultval.has_value())
        return keys.at(k).defaultval.value();
    throw Exception("Option " + k + " not specified and no default value exists!");
}

bool Config::exists(const std::string &k) const {
    return (data.count(k) > 0) || (keys.at(k).defaultval.has_value());
}

Config::Config() = default;

Config::Config(std::vector<char, std::allocator<char>>::const_iterator &in, const std::vector<char, std::allocator<char>>::const_iterator &end) {
    data = Serialize::deserialize<decltype(data)>(in, end);
}

void Config::serialize(std::vector<char> &out) const {
    std::vector<decltype(data)::value_type> temp;
    for (const auto &d: data) {
        if (keys.at(d.first).remember) {
            temp.emplace_back(d);
        }
    }
    Serialize::serialize(temp, out);
}

void Config::merge(const Config &config) {
    for (const auto &d: config.data) {
        add(d.first, d.second);
    }
}
