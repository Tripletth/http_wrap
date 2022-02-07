#pragma once

#include "http_wrap/winhttp/strcvt.hpp"

#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <winhttp.h>

namespace http_wrap::winhttp {
namespace options {
    struct wstring_or_null {
        wstring_or_null() : string{} {}
        wstring_or_null(std::wstring_view str_in) : string{str_in} {}
        wstring_or_null(std::string_view str_in) : string{strcvt::to_wstr(str_in)} {}
        wstring_or_null& operator=(std::wstring_view other) {
            string = other;
            return *this;
        }
        wstring_or_null& operator=(std::string_view other) {
            string = strcvt::to_wstr(other);
            return *this;
        }
        operator const std::wstring::value_type*() const { return !string.empty() ? string.c_str() : nullptr; }
        std::wstring string;
    };
struct internet {
    wstring_or_null agent{};
// mingw bug?
#ifndef WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY
#define WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY 4
#endif
    unsigned int    access_type{WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY};
    wstring_or_null proxy{};
    wstring_or_null proxy_bypass{};
    unsigned int    flags{0};
};
struct request {
    request(unsigned short port = 0)
    : flags{port == INTERNET_DEFAULT_HTTPS_PORT ? (unsigned int)WINHTTP_FLAG_REFRESH | WINHTTP_FLAG_SECURE : (unsigned int)WINHTTP_FLAG_REFRESH}
    {}

    unsigned int              flags{WINHTTP_FLAG_REFRESH | WINHTTP_FLAG_SECURE};
    wstring_or_null           version{};
    wstring_or_null           referrer{};
    unsigned int              context{0};
    std::vector<std::string>  headers{};
    std::string               optional{}; // not wstring

    // compiled
    wstring_or_null headers_compiled{};
    const wchar_t* headers_cwstr() {
        if (headers.empty()) {
            return nullptr;
        }
        std::wstringstream ss{};
        for (const auto& h : headers) {
            ss << strcvt::to_wstr(h) << L"\r\n";
        }
        headers_compiled = ss.str();
        return headers_compiled;
    }
};
}
}
