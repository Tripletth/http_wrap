#pragma once

#include "http_wrap/wininet/options.hpp"
#include "http_wrap/wininet/wrap.hpp"
#include "http_wrap/abstract_simple.hpp"
#include "http_wrap/url.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <tuple>

namespace http_wrap::wininet {
class simple : public abstract_simple {
    inline static std::unique_ptr<wininet::internet> internet{};
public:
    simple(url_t url) : http_wrap::abstract_simple{url} {
        if (!internet) {
            internet = std::make_unique<wininet::internet>();
        }
    }
    simple(std::string_view url) : simple{url_t(url)} {}
    std::string get() const override {
        return perform("GET");
    }
    std::string post() const override {
        return perform("POST");
    }
    std::string perform(std::string_view type) const {
        auto [session_options, request_options] = make_options();
        wininet::session session{*internet, url.host(), session_options};
        wininet::request request{session, url.object(), type, request_options};
        auto buffer{request.read()};
        return {buffer.begin(), buffer.end()};
    }
private:
    std::pair<wininet::options::session, wininet::options::request> make_options() const {
        auto [session_options, request_options] = wininet::options::make_options(url.protocol());
        if (!url.port_v().empty()) {
            session_options.port = std::stoi(url.port());
        }
        if (request_data.empty()) {
            request_options.optional = url.parameters();
        } else {
            request_options.optional = request_data;
        }
        request_options.headers = headers;
        return std::make_pair(std::move(session_options), std::move(request_options));
    }
};
}
