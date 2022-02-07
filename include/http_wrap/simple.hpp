#pragma once

#if defined(HTTP_WRAP_USE_WINHTTP)
#include "http_wrap/winhttp/simple.hpp"
namespace http_wrap {
using simple = winhttp::simple;
}
#elif defined(HTTP_WRAP_USE_CURL)
#include "http_wrap/curl/simple.hpp"
namespace http_wrap {
using simple = curl::simple;
}
#else
#include "abstract_simple.hpp"
#include <string>
namespace http_wrap {
class simple : public abstract_simple {
public:
    simple(std::string_view url) : abstract_simple{url} {}
    std::string get() const override { return "no http_wrap implementation defined"; }
    std::string post() const override { return "no http_wrap implementation defined"; }
};
}
#endif // HTTP_WRAP_USE_WINHTTP
