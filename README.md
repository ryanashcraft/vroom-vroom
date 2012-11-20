# vroom vroom

## Description

An HTTP server that interprets server-side JavaScript with the V8 JavaScript engine.

## Prerequisites

The V8 JavaScript interpreter library is required for interpreting the server-side JavaScript files. V8 can be installed with any package manager or from [source](https://developers.google.com/v8/build). If installing from source, you can simply run `make native` to compile for your current system architecture. See [Google's guide for building V8](http://code.google.com/p/v8/wiki/BuildingWithGYP) for more information on building V8.

This project depends on several C++11 features. The compiler you use must support C++11 threads, lambda expressions, and unique pointers.

## Compiling

Start by duplicating `Makefile.sample` to `Makefile`. There are default values for all of the variables, but some values may need to be changed to fit your preferences and platform requirements.

If the V8 library is installed outside of normal library search paths, you will need to edit the Makefile to define the locations of the V8 static libraries by setting the `V8HOME` and `ARCH` variables.

## Running

There is a target for compiling and running. Simply run `make run`. Or simply run the linked `vroomvroom` executable after compiling with `make`.

By default, the server opens on port 8000. This can be defined at runtime by setting a command-line argument (e.g. `vroomvroom 8001`).

## Handling HTTP Requests

HTTP requests are resolved by accessing files from the public directory. If the file does not exist, a 404 HTTP status code will be returned to the client.

## Server-Side JavaScript

HTTP requests for files with an `.ssjs` extension will be automatically interpreted by V8, and the generated string from running the file will be returned in the HTTP response.

### The `require(path)` JavaScript Function

In a server-side JavaScript file, a call to the require function will look up the requested file and interpret it with V8. The return value from this requested file will be returned as a JavaScript object, not necessarily a string, to the caller.

Relative and absolute paths are supported from server-side JavaScript files. These paths are resolved relative to the caller.

### Additional JavaScript Support

HTTP POST and GET data is initalized before interpreting `.ssjs` files. This can be access in the global `__POST__` and `__GET__` variables, which is an object that acts as a key-value map of the data.

The parent directory of the interpreted file is stored as a string in the global variable `__CURRENT_DIRECTORY__`.

Custom headers for the HTTP response can be defined in the server-side JavaScript with the global `__HEADERS__` array.

## Known Issues and Limitations

### HTTP

- No support for PUT or DELETE requests
- Does not accept absolute URLs
- Does not require or use the `Host` header from HTTP 1.1 requests
- No support for chunked transfers
- No support for persistent connections
- No support for handling requests with multipart content, thus no support for file uploads
- No support for various mime types
- No support for handling requests with `If-Modified-Since` and `If-Unmodified-Since` headers
- Untested handling of HTTP 1.0 requests
- Untested handling of HTTP requests with `Expects: 100 Continue` header
- Opens and interprets files for HEAD requests

### Server-Side JavaScript

- No API for retrieving system time
- No API for storing persistent data or interacting with a database
- No API for performing sending HTTP requests or email messages
- File path argument for `require(path)` can request a file outside of the public directory
- Error handling and logging is lacking with various scenarios (i.e. invalid file path argument in `require(path)`)

### Customization

- No support for custom configurations

### Testing

- There are no proper tests
