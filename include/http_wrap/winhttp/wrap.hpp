#pragma once

#include "http_wrap/winhttp/options.hpp"

#include <array>
#include <string_view>
#include <vector>
#include <windows.h>
#include <winhttp.h>

namespace http_wrap::winhttp {
class connection {
public:
    connection(HINTERNET handle) : handle{handle} {}
    connection() : connection{0} {}
    connection(connection&&) = default;
    connection& operator=(connection&&) = default;
    virtual ~connection() {
        WinHttpCloseHandle(handle);
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
    internet(const options::internet& opts)
    : connection{WinHttpOpen(opts.agent,
                             opts.access_type,
                             opts.proxy,
                             opts.proxy_bypass,
                             opts.flags)} {}
    internet() : internet{options::internet()} {}
};
class session : public connection {
public:
    session(HINTERNET internet_handle, std::wstring_view host, unsigned short port)
    : connection{WinHttpConnect(internet_handle,
                                host.data(),
                                port,
                                0)} {}
};
class request : public connection {
    options::request opts;
public:
    request(HINTERNET session_handle, std::wstring_view object, std::wstring_view type, const options::request& opts_in)
    : connection{}, opts{opts_in} {
        handle = WinHttpOpenRequest(session_handle,
                                    type.data(),
                                    object.data(),
                                    opts.version,
                                    opts.referrer,
                                    nullptr, // accept-types passed through headers
                                    opts.flags);
    }
    std::vector<unsigned char> read() {
        std::vector<unsigned char> data_read{};
        if (WinHttpSendRequest(handle,
                               opts.headers_cwstr(),
                               -1,
                               (void*)opts.optional.c_str(),
                               opts.optional.size(),
                               opts.optional.size(),
                               0) && WinHttpReceiveResponse(handle, 0)) {
            std::array<unsigned char, 8192> buffer{};
            DWORD n_bytes_read{0};
            while(WinHttpReadData(handle, buffer.data(), buffer.size(), &n_bytes_read) && n_bytes_read) {
                data_read.insert(data_read.end(),
                                 std::make_move_iterator(buffer.begin()),
                                 std::make_move_iterator(buffer.begin() + n_bytes_read));
            }
        }
        return data_read;
    }
};
}
