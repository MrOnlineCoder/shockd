# shockd

shockd is a simple webserver written in C. You can use it to serve static files.

Features
--------

* Accepts HTTP/1.1 and HTTP/1.0 requests with GET and POST methods.

* Serves HTML, CSS, JS, images, executables or any other files

* Logs all requests and errors to file

* Multi-threading (every new connection is processed in new thread)

* HTTP ETag cache support

* It is fast enough for home usage :D

Building
--------
shockd supports Windows and Unix based systems with sockets and pthread support.

1.Get source code by downloading archive or by **git clone**

2.Build for your platform using make:

For Windows:
```
make win32
```

For Linux: 
```
make linux
```

Config
-----

You can configure server by editing options in **shockd.conf** file.

Check default config, it has explanation in comments.

TODO
----

* Add CGI support

* Add keep-alive support

* ~~Implement config parser and use it~~

* POST body parser

* ~~Add cache support~~

* Simple module system?

* ~~Logging to files~~

How server handles HTTP request
-------------------------------

When incoming connection is received on TCP socket, server tries to create new thread that will process the request and make a response. (function **processRequest in main.c**)

It creates empty **shock_request_t** struct object that will represent the request itself. Then calls parser to fill the request (function **shock_parse_request in request.c**). Then, server tries to send file requested by browser chunk-by-chunk over TCP and then closes the connection
(function **shock_serve_file in response.c**)

Contributing
------------
If you found a bug, security caveat or want to request a feature - **create an issue**

If you want to improve code, close existing issues - **create a pull request**

Code must be working, cross-platform and clean.