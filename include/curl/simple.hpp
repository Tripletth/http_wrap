#pragma once

#include "abstract_simple.hpp"
#include "wrap.hpp"

namespace http_wrap::curl {
class simple : public abstract_simple {\
public:
    simple(url_t url) : http_wrap::abstract_simple{url} {}
    std::string get() const override {
        curl::request::options options {
            .url = url.url,
            .headers = headers
        };
        curl::request request{options};
        return std::string(request.to_str_view());
    }
    std::string post() const override {
        curl::request::options request_options {
            .url = url.url,
            .headers = headers
        };
        curl::post::options post_options {
            .post_data = request_data
        };
        curl::post request{request_options, post_options};
        return std::string(request.to_str_view());
    }
};
}
