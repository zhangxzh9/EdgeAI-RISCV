#!/usr/bin/python
# -*- coding: utf-8 -*-

from __future__ import absolute_import, division, print_function
import sys
import MODEL

def Fuc():
	print ('hello')


    
if __name__ == '__main__':
	if len(sys.argv) != 2:
		print ('Usage: python -option(-train/-run_weight/-run_model)')
		exit(1)
	else:
		Fuc()
	if sys.argv[1] == '-train':
		print ('----TRAIN MODEL----')
		MODEL.train_simple()
	elif sys.argv[1] == '-run_weight':
		print ('----RUN MODEL----')
		MODEL.RUN_weight()
	else:
		print ('----RUN MODEL----')
		MODEL.RUN_model()
