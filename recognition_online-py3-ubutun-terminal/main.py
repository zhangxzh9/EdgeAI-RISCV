#!/usr/bin/python
# -*- coding: utf-8 -*-

from __future__ import absolute_import, division, print_function
import sys
import MODEL_STORE
import MODEL_TRAIN


def Fuc():
	print ('hello')


    
if __name__ == '__main__':
	if len(sys.argv) != 3:
		print ('Usage: python input_name output_name')
		exit(1)
	else:
		Fuc()
	if sys.argv[2] == 'train':
		print ('----TRAIN MODEL----')
		MODEL_TRAIN.train()
	elif sys.argv[2] == 'store':
		print ('----STORE MODEL----')
		MODEL_STORE.store()
	else:
		print ('close')
