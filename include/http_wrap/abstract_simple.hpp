#pragma once

#include "http_wrap/url.hpp"

#include <string>
#include <vector>

namespace http_wrap {
class abstract_simple {
public:
    url_t url;
    std::vector<std::string> headers;
    std::string request_data;
    abstract_simple(url_t url) : url{url}, headers{}, request_data{} {}
    virtual ~abstract_simple() = default;
    virtual std::string get() const = 0;
    virtual std::string post() const = 0;
};
}
