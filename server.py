#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import socket

HOST = '0.0.0.0'  # Standard loopback interface address (localhost)
PORT = 53534        # Port to listen on (non-privileged ports are > 1023)

f = open('schema.json')
text = f.read()
f.close()

print(text)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    while True:
        conn, addr = s.accept()
        with conn:
            print('Connected by', addr)

            #while True:
                #buffer = conn.recv(1024)
                #if not buffer:
                    #break
                #data += buffer
            #print(data)
            conn.send(text.encode('utf8'))
