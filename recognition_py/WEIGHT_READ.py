#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
import h5py
import numpy as np

f= h5py.File('weights.h5','r')
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