#pragma once

#include <codecvt>
#include <locale>
#include <string>
#include <string_view>

namespace http_wrap::winhttp::strcvt {
std::wstring to_wstr(std::string_view str) {
    return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(str.data());
}
std::string to_str(std::wstring_view str) {
    return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(str.data());
}
}
