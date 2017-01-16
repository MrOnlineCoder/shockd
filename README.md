# shockd

shockd is a simple webserver written in C. You can use it to serve static files.

Features
--------

* Accepts HTTP/1.1 and HTTP/1.0 requests with GET and POST methods.

* Serves HTML, CSS, JS, images, executables or any other files

* Multi-threading (every new connection is processed in new thread)

* It is fast enough for home usage :D

Config
-----

There is no config currently (but will be added soon), just run the executable. Note, that htdocs folder must be present in executable directory.

TODO
----

* Add keep-alive support

* Implement config parser and use it

* POST body parser

* Add cache support

* Simple module system?

* Logging to files

How server handles HTTP request
-------------------------------

When incoming connection is received on TCP socket, server tries to create new thread that will process the request and make a response. Otherwise, request is processed on main thread (function **processRequest in main.c**)

It creates empty **shock_request_t** struct object that will represent the request itself. Then calls parser to fill the request (function **shock_parse_request in request.c**). Then, server tries to send file requested by browser chunk-by-chunk over TCP and then closes the connection
(function **shock_serve_file in response.c**)