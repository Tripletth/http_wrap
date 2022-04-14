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
        winhttp::url_components url_components{url};
        winhttp::options::request request_options{url_components.port()};
        request_options.headers = request_headers;

        winhttp::session session{*internet, url_components.host(), url_components.port()};
        std::wstring object = (request_data.empty() ? url_components.object_and_parameters() : (url_components.object() + L'?' + strcvt::to_wstr(request_data)));
        winhttp::request request{session, object, L"GET", request_options};

        auto buffer{request.read()};
        return {buffer.begin(), buffer.end()};
    }
    std::string post() const override {
        winhttp::url_components url_components{url};
        winhttp::options::request request_options{url_components.port()};
        request_options.headers = request_headers;
        request_options.optional = request_data.empty() ? strcvt::to_str(url_components.parameters()) : request_data;

        winhttp::session session{*internet, url_components.host(), url_components.port()};
        winhttp::request request{session, url_components.object(), L"POST", request_options};

        auto buffer{request.read()};
        return {buffer.begin(), buffer.end()};
    }
};
}
