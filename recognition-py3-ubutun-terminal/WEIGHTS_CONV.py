#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
import h5py
import numpy as np

def weights_h5totxt():
    h5_name = './model_data/weights.h5'
    f= h5py.File(h5_name,'r')
    f_keys = f.keys()
    weight = {}
    w = {}
    data ={}

    n1 = 0
    n2 = 0
    n3 = 0
    for f_keys_list in list(f_keys):
        weight[n1] = f[f_keys_list]
        weight_keys = weight[n1].keys()
        for weight_keys_list in list(weight_keys):
            w[n2] = weight[n1][weight_keys_list]
            w_keys = w[n2].keys()
            for w_keys_list in list(w_keys):
                data[n3] = w[n2][w_keys_list]
                n3 = n3+1
            n2 = n2+1
        n1 = n1+1
    
    for n in range(n3): 
        if n%2 == 0:
            np.savetxt("./model_data/bias{}.txt".format(n//2), data[n],fmt='%f')
        else:
            data_array = np.zeros(shape = data[n].shape)
            np.savetxt("./model_data/kernel{}.txt".format(n//2), data[n],fmt='%f')
            data[n].read_direct(data_array)
            k,b,quant_data_array = ListQuant(data_array)
            np.savetxt("./model_data/Quart_kernel{}.txt".format(n//2), quant_data_array,fmt='%3d')
            Quant_para =[k]
            np.savetxt("./model_data/Quant_para_k{}.txt".format(n//2), Quant_para,fmt='%f')
            Quant_para =[b]
            np.savetxt("./model_data/Quant_para_b{}.txt".format(n//2), Quant_para,fmt='%d')

# 根据参数和原始浮点数量化为定点数
def Quant(Vx, Q, RQM):
    Vx = Q * Vx- RQM
    return Vx.round()



def ListQuant(data_array):
    # 数组范围估计
    data_min = data_array.min()
    data_max = data_array.max()

    # 量化参数估计
    k = ((1 << 8) - 1) * 1.0 / (data_max - data_min)
    b = (int)(round(k*data_min))

    # 产生量化后的数组
    quant_data_array = Quant(data_array,k,b)
    return (k, b, quant_data_array)

# 根据量化参数还原回浮点
def QuantRevert(VxQuant, Q, RQM):
    return (VxQuant + RQM) / Q
    

def ListQuantRevert(quant_data_list, Q, RQM):
    quant_revert_data_list = []
    for quant_data in quant_data_list:
        # 量化数据还原为原始浮点数据
        revert_quant_data = QuantRevert(quant_data, Q, RQM)
        quant_revert_data_list.append(revert_quant_data)
    quant_revert_data_list = np.array(quant_revert_data_list)
    return quant_revert_data_list
