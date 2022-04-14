#pragma once

#include "http_wrap/winhttp/strcvt.hpp"

#include <cstring>
#include <winhttp.h>

namespace http_wrap::winhttp {
struct url_components {
    url_components(std::wstring_view str_view) : url{str_view}, components{} {
        components.dwStructSize      = sizeof(components);
        components.dwHostNameLength  = -1u;
        components.dwUrlPathLength   = -1u;
        components.dwExtraInfoLength = -1u;
        if (!WinHttpCrackUrl(url.c_str(), url.size(), 0, &components) && (ERROR_WINHTTP_UNRECOGNIZED_SCHEME == GetLastError())) {
            url = L"http://" + url;
            WinHttpCrackUrl(url.c_str(), url.size(), 0, &components);
        }
    }
    url_components(std::string_view str_view) : url_components{strcvt::to_wstr(str_view)} {}
    int protocol() const {
        return components.nScheme;
    }
    unsigned short port() const {
        return components.nPort;
    }
    std::wstring_view host_v() const {
        if (components.lpszHostName) {
            return {components.lpszHostName, components.dwHostNameLength};
        } else {
            return {url.substr(0, 0)};
        }
    }
    std::wstring_view object_v() const {
        if (components.lpszUrlPath) {
            return {components.lpszUrlPath, components.dwUrlPathLength};
        } else {
            return {url.substr(0, 0)};
        }
    }
    std::wstring_view parameters_v() const {
        if (components.lpszExtraInfo) {
            return {components.lpszExtraInfo + 1, components.dwExtraInfoLength};
        } else {
            return {url.substr(0, 0)};
        }
    }
    std::wstring host() const {
        return std::wstring(host_v());
    }
    std::wstring object() const {
        return std::wstring(object_v());
    }
    std::wstring parameters() const {
        return std::wstring(parameters_v());
    }
    std::wstring object_and_parameters() const {
        std::wstring params {parameters()};
        if (params.empty()) {
            return object();
        } else {
            return object() + L'?' + params;
        }
    }
    std::wstring url;
    URL_COMPONENTS components;
};
}
