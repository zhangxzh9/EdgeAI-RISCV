#coding=UTF-8
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
if __name__ == '__main__':
    output_path = '/home/zhang/workshape/MNIST_data'
    output_path = os.path.abspath('.')
    print(output_path)
    path = '/data'
    try:
        shutil.rmtree(output_path+path)
    except OSError:
        print('dir dose not exits') 
    imgs = parese_idx3("t10k-images.idx3-ubyte")
    labs = parese_idx1("t10k-labels.idx1-ubyte")
    os.mkdir(output_path+path)
    for i in range(100):
        print(imgs[i])
        print(labs[i])
        plt.imshow(imgs[i])
        #plt.show()
        img = imgs[i]
        num = labs[i]
        #print img
        #print num
        #创建新28*28图片对象
        image = Image.new('L', (28, 28))
        for x in range(28):
            for y in range(28):
                            #print img[x][y]
                image.putpixel((y, x), int(img[x][y])) #按像素写入
        #保存图片(序号-label.png)
        save_file = output_path+ path +'/' + 'No.'+str(i) + '.png'
        image.save(save_file) 
        #保存对应位数据
        np.savetxt("./data/No.{}-{}.txt".format(i,int(num)),img,fmt='%3d',)
