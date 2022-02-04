#pragma once

#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <wininet.h>

namespace http_wrap::wininet {
namespace options {
struct internet {
    std::string  agent{"Internet"};
    unsigned int access_type{INTERNET_OPEN_TYPE_PRECONFIG};
    std::string  proxy{};
    std::string  proxy_bypass{};
    unsigned int flags{0};
};
struct session {unsigned short port{0};
    std::string    user{};
    std::string    pass{};
    unsigned int  service{INTERNET_SERVICE_HTTP};
    unsigned int  flags{0};
    unsigned int  context{0};
};
struct request {
    unsigned int            flags{0};
    std::string              version{"HTTP/1.1"};
    std::string              referrer{};
    std::vector<std::string> accept_types{};
    unsigned int            context{0};
    std::vector<std::string> headers{};
    std::string              optional{};

    // compiled
    std::vector<const char*> accept_types_compiled{};
    std::string              headers_compiled{};
    const char** accept_types_cstr_ptr() {
        if (accept_types.empty()) {
            return nullptr;
        }
        accept_types_compiled.clear();
        for (const auto& a : accept_types) {
            accept_types_compiled.push_back(a.c_str());
        }
        accept_types_compiled.push_back(nullptr);
        return (const char**)accept_types_compiled.data();
    }
    const char* headers_cstr() {
        if (headers.empty()) {
            return nullptr;
        }
        std::stringstream ss{};
        for (const auto& h : headers) {
            ss << h << "\r\n";
        }
        headers_compiled = ss.str();
        return headers_compiled.c_str();
    }
};
std::pair<session, request> make_options(protocol_t protocol) {
    constexpr static auto default_request_flags {INTERNET_FLAG_RELOAD | INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_NO_UI};
    session s{.port = (protocol == protocol_t::https) ? (unsigned short)INTERNET_DEFAULT_HTTPS_PORT : (unsigned short)INTERNET_DEFAULT_HTTP_PORT};
    request r{.flags = (protocol == protocol_t::https) ? (default_request_flags | INTERNET_FLAG_SECURE) : default_request_flags};
    return std::make_pair(std::move(s), std::move(r));
}
}
}
