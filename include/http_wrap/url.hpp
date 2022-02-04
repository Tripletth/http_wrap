#pragma once

#include <string>
#include <string_view>

namespace http_wrap {
    enum class protocol_t { http, https };
struct url_t {
    url_t(std::string_view url) : url{url} {}
    protocol_t protocol() const {
        std::string_view url_view{url};
        if ((url_view.size() > 5) && (url_view.substr(0, 5) == "https")) {
            return protocol_t::https;
        } else {
            return protocol_t::http;
        }
    }
    std::string_view host_and_port_v() const {
        std::string_view url_view{url};
        auto begin {url_view.find("://")};
        if (begin == std::string_view::npos) {
            begin = 0;
        } else {
            begin += 3;
        }
        return url_view.substr(begin, end_pos(url_view.find("/", begin)) - begin);
    }
    std::string_view host_v() const {
        std::string_view url_view{url};
        std::string_view url_host_and_port{host_and_port_v()};
        return url_host_and_port.substr(0, end_pos(url_host_and_port.find(":")));
    }
    std::string_view port_v() const {
        std::string_view url_view{url};
        std::string_view url_host_and_port{host_and_port_v()};
        auto begin(url_host_and_port.find(":"));
        if (begin == std::string_view::npos) {
            return url_view.substr(0, 0);
        } else {
            return url_host_and_port.substr(begin + 1, url_host_and_port.size() - begin);
        }
    }
    std::string_view object_v() const {
        std::string_view url_view{url};
        std::string_view url_host{host_and_port_v()};
        auto begin {url_view.find(url_host) + url_host.size()};
        return url_view.substr(begin, end_pos(url_view.find("?", begin) - begin));
    }
    std::string_view parameters_v() const {
        std::string_view url_view{url};
        std::string_view url_object{object_v()};
        if (!url_object.empty()) {
            auto begin {url_view.find(url_object)};
            if (begin != std::string_view::npos) {
                begin += url_object.size();
                if (begin != url_view.size()) {
                    return url_view.substr(begin + 1);
                }
            }
        }
        return url_view.substr(0, 0);
    }

    std::string host() const { return std::string(host_v()); }
    std::string port() const { return std::string(port_v()); }
    std::string host_and_port() const { return std::string(host_and_port_v()); }
    std::string object() const { return std::string(object_v()); }
    std::string parameters() const { return std::string(parameters_v()); }

    std::string url;
private:
    std::size_t end_pos(std::size_t n) const {
        return std::min(url.size(), n);
    }
};
}
