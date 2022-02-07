#pragma once

#include <string>
#include <vector>

namespace http_wrap {
class abstract_simple {
public:
    std::string url;
    std::vector<std::string> request_headers;
    std::string request_data;
    abstract_simple(std::string_view url) : url{url}, request_headers{}, request_data{} {}
    virtual ~abstract_simple() = default;
    virtual std::string get() const = 0;
    virtual std::string post() const = 0;
};
}
