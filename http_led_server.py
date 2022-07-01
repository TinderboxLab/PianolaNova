#!/usr/bin/env python3

import board
import neopixel
import os
import threading
from http.server import BaseHTTPRequestHandler, HTTPServer

host_name = '127.0.0.1'  # IP Address of Raspberry Pi
host_port = 8000
pixels = neopixel.NeoPixel(board.D18, 96)

class MyServer(BaseHTTPRequestHandler):

    def do_POST(self):

        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length).decode("utf-8")
        post_data = post_data.split(",")
        ledNum = int(post_data[1]) - 32

        if post_data[0] == '144':
            pixels[ledNum] = (0, 255, 0)
            timer = threading.Timer(0.2, self.noteOff,[ledNum])
            timer.start()
        else:
            pixels[ledNum] = (0,0,0)

        print("LED is {}".format(ledNum))

    def noteOff(self, num):
        pixels[num] = (0,0,0)
# # # # # Main # # # # #

if __name__ == '__main__':
    http_server = HTTPServer((host_name, host_port), MyServer)
    print("Server Starts - %s:%s" % (host_name, host_port))

    try:
        http_server.serve_forever()
    except KeyboardInterrupt:
        http_server.server_close()