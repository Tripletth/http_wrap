#pragma once

#include "curl/curl.h"
#include <functional>
#include <string>
#include <vector>

namespace http_wrap::curl {
class request {
    struct global_fixture {
        global_fixture() {
            curl_global_init(CURL_GLOBAL_ALL);
        }
        ~global_fixture() {
            curl_global_cleanup();
        }
    };
    inline static global_fixture fixture{};
public:
    struct options {
        std::string url{""};
        std::vector<std::string> headers{};
    };
    request(const options& opts_in)
    : curl{curl_easy_init()}, result{CURLE_OK}, headers_ptr{nullptr}, opts{opts_in}, read_data{}  {
        curl_easy_setopt(curl, CURLOPT_URL, opts.url.c_str());
        for (const auto& header : opts.headers) {
            curl_slist_append(headers_ptr, header.data());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers_ptr);
    }
    ~request() {
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers_ptr);
    }
    bool perform() {
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &http_wrap::curl::request::read);
        result = curl_easy_perform(curl);
        return result == CURLE_OK;
    }
    static std::size_t read(void* data_ptr, std::size_t chunk_size, std::size_t data_size, request* this_ptr) {
        this_ptr->read_data.insert(this_ptr->read_data.end(),
                                   (unsigned char*)data_ptr,
                                   ((unsigned char*)data_ptr) + (chunk_size * data_size));
        return chunk_size*data_size;
    }
    std::string error () {
        return curl_easy_strerror(result);
    }
    std::string_view to_str_view() {
        return {(char*)read_data.data(), read_data.size()};
    }
    CURL* curl;
    CURLcode result;
    curl_slist* headers_ptr;
    options opts;
    std::vector<unsigned char> read_data;
};

class post : public request {
public:
    struct options {
        std::string post_data{};
    };
    post(const request::options& request_opts, const options& opts_in)
    : request{request_opts}, opts{opts_in} {
        curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, opts.post_data.c_str());
    }
    options opts{};
};
}
