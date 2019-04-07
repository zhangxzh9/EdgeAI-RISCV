#usr/bin/python3
# -*- coding: utf-8 -*-
import serial
from time import sleep
 
ser = serial.Serial('/dev/ttyUSB2', 115200, timeout=0.5)
 
 
def recv(serial):
        data = ''
        i = 0
        while True:
            data = serial.read(1000)
            if data == b'':
                i = i + 1
                if(i<20):
                    pass
                else:
                    print('time out!')
                    break
            else:
                break
            sleep(0.5)
        return data
 
while True:
        data = recv(ser)
        print(data.decode("utf-8"))
        while True:        
            ser.write(b'b')
            sleep(0.5)
            data = ser.readline()
            print(data)
            if data == b'\rok1\r\n':
                ser.write(b'e')
                sleep(0.5)
                data = ser.readline()
                print(data)
                if data == b'\rok2\r\n':
                    break
                else:
                    print('fail to handshake')
            else:
                print('fail to handshake')

            
        ser.write(123)
        data = recv(ser)
        print(data.decode("utf-8"))
        break

