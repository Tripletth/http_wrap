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
#error no http_wrap::simple implementation defined. define HTTP_WRAP_USE_WINHTTP or HTTP_WRAP_USE_CURL.
#endif // HTTP_WRAP_USE_WINHTTP
