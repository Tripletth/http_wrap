#pragma once

#include "http_wrap/winhttp/options.hpp"
#include "http_wrap/winhttp/url_components.hpp"
#include "http_wrap/winhttp/wrap.hpp"
#include "http_wrap/abstract_simple.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <tuple>

namespace http_wrap::winhttp {
class simple : public abstract_simple {
    inline static std::unique_ptr<winhttp::internet> internet{};
public:
    simple(std::string_view url) : http_wrap::abstract_simple{url} {
        if (!internet) {
            internet = std::make_unique<winhttp::internet>();
        }
    }
    std::string get() const override {
        return perform(L"GET");
    }
    std::string post() const override {
        return perform(L"POST");
    }
    std::string perform(std::wstring_view type) const {
        winhttp::url_components url_components{url};
        winhttp::options::request request_options{url_components.port()};
        request_options.headers = request_headers;
        request_options.optional = request_data.empty() ? strcvt::to_str(url_components.parameters()) : request_data;

        winhttp::session session{*internet, url_components.host(), url_components.port()};
        winhttp::request request{session, url_components.object(), type, request_options};
        auto buffer{request.read()};
        return {buffer.begin(), buffer.end()};
    }
};
}
