#pragma once
#include <unordered_map>
#include <string>
using codable_properties_t = std::unordered_map<std::string, std::string>;
class Codable {
    public:
    Codable(){};
    virtual ~Codable(){};
    virtual void decode(const codable_properties_t&& props) = 0;
    virtual const codable_properties_t encode() const = 0;
};