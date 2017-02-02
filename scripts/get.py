# Sends GET to local server
# Author: schdub

#!/usr/bin/python
import socket
import sys
def GET(host, path, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(0.30)
    s.connect((host, port))
    s.send("GET %s HTTP/1.0\r\n" % (path))
    total_data = []
    while True:
        data = s.recv(8192)
        if (len(data)>0):
            total_data.append(data)
        else:
            break
    print ''.join(total_data)
    s.shutdown(1)
    s.close()
GET(sys.argv[1], sys.argv[2], 3000)