#usr/bin/python3
# -*- coding: utf-8 -*-
from serial import Serial
from time import sleep
 
ser = Serial('/dev/ttyUSB1', 115200, timeout=0.5)
 
 
def recv_all(serial):
    data = ''
    i = 0
    while True:
        data = serial.read_all()
        if data == b'':
            i = i + 1
            if(i<10):
                pass
            else:
                print('time out!')
                break
        else:
            break
        sleep(0.5)
    return data
 
def recv_line(serial):
    data = ''
    i = 0
    while True:
        data = serial.readline()
        if data == b'':
            i = i + 1
            if(i<10):
                pass
            else:
                print('time out!')
                break
        else:
            break
        sleep(0.1)
    return data
while True:
    data = recv_line(ser)
    print(bytes.decode(data))
    if data ==b'\rfinish\r\n':
        break
while True:
    while True:
        ser.write(b'b')
        data = recv_line(ser)
        #print('read data is: ' + str(data))
        if data == b'ok1\r\n' or data == b'\rok1\r\n':
            ser.write(b'e')
            data = recv_line(ser)
            #print('read data is: ' + str(data))
            if data == b'\rok2\r\n':
                break
            else:
                print('fail to handshake')
        else:
            print('fail to handshake')
    print("the picture labels is")
    print("----begin download----")
    for i in range(784):
        num = i % 256
        ser.write(bytes([num]))
        data = recv_line(ser)
    print("-----------------finish download-----------------")
    while True:
        data = recv_line(ser)
        print(bytes.decode(data))
        if data ==b'\rfinish\r\n':
            break

