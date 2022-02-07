#pragma once

#include "http_wrap/abstract_simple.hpp"
#include "http_wrap/curl/wrap.hpp"

namespace http_wrap::curl {
class simple : public abstract_simple {
public:
    simple(std::string_view url) : abstract_simple{url} {}
    std::string get() const override {
        curl::request::options options {
            .url = url,
            .headers = headers
        };
        curl::request request{options};
        if (request.perform()) {
            return std::string(request.to_str_view());
        } else {
            return request.error();
        }
    }
    std::string post() const override {
        curl::request::options request_options {
            .url = url,
            .headers = headers
        };
        curl::post::options post_options {
            .post_data = request_data
        };
        curl::post request{request_options, post_options};
        if (request.perform()) {
            return std::string(request.to_str_view());
        } else {
            return request.error();
        }
    }
};
}
