#!/usr/bin/python
# -*- coding: utf-8 -*-

from __future__ import absolute_import, division, print_function
import sys
import MODEL
import WEIGHTS_CONV as conv

def Fuc():
	print ('hello')

if __name__ == '__main__':
	if len(sys.argv) != 3 and len(sys.argv) != 2 :
		print ('Usage: python -option(-train/-run_weight/-run_model/-conv)')
		exit(1)
	else:
		Fuc()
	if sys.argv[1] == '-train':
		print ('----TRAIN MODEL----')
		MODEL.train_simple()
	elif sys.argv[1] == '-run_weight':
		print ('----RUN MODEL BY LOADING WEIGHTS----')
		MODEL.RUN_weight()
	elif sys.argv[1] == '-conv':
		if sys.argv[2] == '':
			print('exam: python -conv weights.h5')
		else:
			print ('----CONVERT THE WEIGHTS TO TXT FORMAT----')
			h5_name = sys.argv[2]
			conv.weights_h5totxt(h5_name)
	else:
		print ('----RUN MODEL BY LOADING THE HOLD MODEL----')
		MODEL.RUN_model()
