#pragma once

#include "http_wrap/wininet/options.hpp"

#include <array>
#include <string_view>
#include <vector>

#include <windows.h>
#include <wininet.h>

namespace http_wrap::wininet {
namespace {
const char* c_str_or_null(std::string_view str) {
    return str.empty() ? nullptr : str.data();
}
}
class connection {
public:
    connection(HINTERNET handle) : handle{handle} {}
    connection(connection&&) = default;
    connection& operator=(connection&&) = default;
    virtual ~connection() {
        InternetCloseHandle(handle);
    }
    operator HINTERNET() {
        return handle;
    }
    HINTERNET handle;
private:
    connection(const connection&) = delete;
    connection& operator=(const connection&) = delete;
};
class internet : public connection {
public:
    internet(const options::internet& opts = options::internet())
    : connection{InternetOpen(c_str_or_null(opts.agent),
                              opts.access_type,
                              c_str_or_null(opts.proxy),
                              c_str_or_null(opts.proxy_bypass),
                              opts.flags)} {}
};
class session : public connection {
public:
    session(HINTERNET internet_handle, std::string_view host, const options::session& opts)
    : connection{InternetConnect(internet_handle,
                                 host.data(),
                                 opts.port,
                                 c_str_or_null(opts.user),
                                 c_str_or_null(opts.pass),
                                 opts.service,
                                 opts.flags,
                                 opts.context)} {}
};
class request : public connection {
    options::request opts;
public:
    request(HINTERNET session_handle, std::string_view object, std::string_view type, const options::request& opts_in)
    : connection{0}, opts{opts_in} {
        handle = HttpOpenRequest(session_handle,
                                 type.data(),
                                 object.data(),
                                 c_str_or_null(opts.version),
                                 c_str_or_null(opts.referrer),
                                 opts.accept_types_cstr_ptr(),
                                 opts.flags,
                                 opts.context);
    }
    std::vector<unsigned char> read() {
        HttpSendRequest(handle,
                        opts.headers_cstr(),
                        -1,
                        (void*)(c_str_or_null(opts.optional)),
                        opts.optional.size());
        std::vector<unsigned char> data_read{};
        std::array<unsigned char, 8192> buffer{};
        DWORD n_bytes_read{0};
        while(InternetReadFile(handle, buffer.data(), buffer.size(), &n_bytes_read) && n_bytes_read) {
            data_read.insert(data_read.end(),
                             std::make_move_iterator(buffer.begin()),
                             std::make_move_iterator(buffer.begin() + n_bytes_read));
        }
        return data_read;
    }
};
}
