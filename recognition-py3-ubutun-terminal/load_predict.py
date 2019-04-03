#!/usr/bin/env python

import tensorflow as tf
import numpy as np

from tensorflow.python.platform import gfile

# OpenCV
import cv2
import sys
class_names = ['0','1', '2', '3', '4', '5',
               '6', '7', '8', '9']

# Read image
img = cv2.imread(sys.argv[1], cv2.IMREAD_GRAYSCALE)
print('img.shape = ', img.shape)
img = img.astype('float32')
img /= 255.0
img = img.reshape(1, 784)

# Initialize a tensorflow session
with tf.Session() as sess:
    # Load the protobuf graph
    with gfile.FastGFile("model_data/simple_model.pb",'rb') as f:
        graph_def = tf.GraphDef()
        graph_def.ParseFromString(f.read())
        # Add the graph to the session
        tf.import_graph_def(graph_def, name='')

    # Get graph
    graph = tf.get_default_graph()

    # Get tensor from graph
    pred = graph.get_tensor_by_name("dense_1/Softmax:0")

    # Run the session, evaluating our "c" operation from the graph
    res = sess.run(pred, feed_dict={'flatten_input:0': img})

    # Print test accuracy
    pred_index = np.argmax(res[0])

    # Print test accuracy
    print('Predict:', pred_index, ' Label:', class_names[pred_index])
