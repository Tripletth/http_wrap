# http_wrap
Use this reduntant wrapper for sending http requests.

- header-only
- compiles with c++17
- windows and linux compatible
- uses either libcurl or wininet
- adds nothing of value

## manual
Add the "include" directory to your project's include directories.

Include http_wrap/simple.hpp to use the http_wrap::simple interface. Specify its implementation:
- compile with -DHTTP_WRAP_USE_WININET to use wininet on windows
- compile with -DHTTP_WRAP_USE_CURL to use libcurl on either windows or linux

Use classes in http_wrap/wininet/wrap.hpp to acquire wininet handles.

Use classes in http_wrap/curl/wrap.hpp to acquire CURL pointers.

## using http_wrap::simple
Create a request:
`http_wrap::simple request{"https://host.domain/path/to/object"};`

Add headers:
`request.headers.emplace_back("Accept: text/*");`

Send the request:
`std::string response{request.get()};` or `request.post()`

Add post data to your request:

`http_wrap::simple request{"https://host.domain/path/to/object?key1=value1"};`

or: `request.request_data = "key1=value1";`
