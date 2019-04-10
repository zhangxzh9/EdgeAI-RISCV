#usr/bin/python3
# -*- coding: utf-8 -*-
import sys
from serial import Serial
from time import sleep
import numpy as np
import struct
import matplotlib.pyplot as plt
from PIL import Image
import os
import shutil

def parese_idx3(idx3_file):
    """
    idx3文件解析方法
    :param idx3_file: idx3文件路径
    :return: 数据集
    """
    # 读取二进制数据
    bin_data = open(idx3_file, 'rb').read()
    # 解析文件头信息 magic、imgs、height、width
    # '>IIII'是说使用大端法读取4个unsinged int32
    offset = 0
    fmt_header = '>iiii'
    magic, imgs, height, width = struct.unpack_from(fmt_header, bin_data, offset)
    print ('magic:%d, imgs: %d, heightXwidth: %dX%d' % (magic, imgs, height, width))
    # 解析数据集
    image_size = height * width
    offset += struct.calcsize(fmt_header)
    fmt_image = '>' + str(image_size) + 'B'
    images = np.empty((imgs, height, width))
    for i in range(imgs):
        if (i + 1) % 10000 == 0:
            print ('已解析 %d' % (i + 1) + '张');
        images[i] = np.array(struct.unpack_from(fmt_image, bin_data, offset)).reshape((height, width))
        offset += struct.calcsize(fmt_image)
    return images

def parese_idx1(idx1_file):
    """
    idx1文件解析方法
    :param idx1_file: idx1文件路径
    :return: 数据集
    """
    # 读取二进制数据
    bin_data = open(idx1_file, 'rb').read()

    # 解析文件头信息 magic、imgs
    offset = 0
    fmt_header = '>ii'
    magic, imgs = struct.unpack_from(fmt_header, bin_data, offset)
    print ('magic:%d, imgs: %d' % (magic, imgs))

    # 解析数据集
    offset += struct.calcsize(fmt_header)
    fmt_image = '>B'
    labels = np.empty(imgs)
    for i in range(imgs):
        if (i + 1) % 10000 == 0:
            print ('已解析 %d' % (i + 1) + '张')
        labels[i] = struct.unpack_from(fmt_image, bin_data, offset)[0]
        offset += struct.calcsize(fmt_image)
    return labels



 
 
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

if __name__ == '__main__':

    output_path = os.path.abspath('.')+'/MNIST_data_output'
    print(output_path)
    picture_path = '/picture'
    imgs = parese_idx3(output_path+"/t10k-images.idx3-ubyte")
    labs = parese_idx1(output_path+"/t10k-labels.idx1-ubyte")

    try:
        os.mkdir(output_path+picture_path)
        for i in range(100):
            #print(imgs[i])
            #print(labs[i])
            plt.imshow(imgs[i])
            #plt.show()
            img = imgs[i]
            num = labs[i]
            print(img)
            print(num)
            #创建新28*28图片对象
            
            image = Image.new('L', (28, 28))
            for x in range(28):
                for y in range(28):
                                #print img[x][y]
                    image.putpixel((y, x), int(img[x][y])) #按像素写入
            #保存图片(序号-label.png)
            save_file = output_path+ picture_path +'/' + 'No.'+str(i) + '.png'
            image.save(save_file) 
            save_file = output_path+ picture_path + "/No.{}-{}.txt".format(i,int(num))
            np.savetxt(save_file,img,fmt='%3d',)
    except OSError:
        print('dir exits') 
    '''

    '''
    try:
        ser = Serial(sys.argv[1], 115200, timeout=0.5)
    except:
        print('could not open'+sys.argv[1]+'please check u connection')
        exit();

    if sys.argv[2]== '-testall':
        #read the 
        while True:
            data = recv_line(ser)
            print(bytes.decode(data))
            if data == b'\rfinish\r\n':
                break
            elif data == b'':
                print('please reset the E203')
            else:
                pass

        for pit_num in range (imgs.length()):
            print("the picture labels is" + num[pit_num])
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
            
            print("------------------------begin download-----------------------")
            for i in range(imgs[pit_num].length()):
                num = imgs[pit_num][i]
                ser.write(bytes([num]))
                data = recv_line(ser)
            print("-----------------finish download NO.{} picture-----------------".format(pit_num))
            while True:
                data = recv_line(ser)
                print(bytes.decode(data))
                if data ==b'\rfinish\r\n':
                    break

