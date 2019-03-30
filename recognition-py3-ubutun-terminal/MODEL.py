#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import os
import shutil

# TensorFlow and tf.keras
import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data
from tensorflow import keras

# Helper libraries
import numpy as np
import matplotlib.pyplot as plt

def MNISTtotxt(train_images,train_labels,test_images,test_labels):
    output_path = os.path.abspath('.')
    #设置图片保存路径
    path = '/MNIST_dataout/train_data'
    try:
        shutil.rmtree(output_path + path)
    except OSError:
        print('dir dose not exits') 
    os.mkdir(output_path + path)
    #保存对应位数据
    for num in range(500):
        save_file = output_path+ path + "/No.{}-{}.txt".format(num,int(train_labels[num]))
        np.savetxt(save_file,train_images[num]*255,fmt='%3d',)
    path = '/MNIST_dataout/test_data'
    try:
        shutil.rmtree(output_path + path)
    except OSError:
        print('dir dose not exits') 
    os.mkdir(output_path + path)
    #保存对应位数据
    for num in range(500):
        save_file = output_path+ path + "/No.{}-{}.txt".format(num,int(test_labels[num]))
        np.savetxt(save_file,test_images[num]*255,fmt='%3d',)
        
def train_simple():
    output_path = os.path.abspath('.')
    print('-----step one: download and load up the datasets-----')    
    #fashion_mnist = keras.datasets.mnist
    #(train_images, train_labels), (test_images, test_labels) = fashion_mnist.load_data()
    #mnist_data_folder="/home/workshape/tensorflow/MNIST_data"
    #fashion_mnist=input_data.read_data_sets(mnist_data_folder,one_hot=True)
    fashion_mnist=input_data.read_data_sets("MNIST_data",one_hot=True)
    x_train, y_train = fashion_mnist.train.images,fashion_mnist.train.labels
    x_test, y_test = fashion_mnist.test.images, fashion_mnist.test.labels
    print('-----step one finsh-----')

    print('-----step two: preprocess the data -----')        
    #train_images = train_images / 255.0
    train_images = x_train.reshape(-1,784).astype('float32')
    y_train = [np.argmax(y, axis=None, out=None) for y in y_train]
    train_labels =np.array(y_train).reshape(-1,1)
    test_images = x_test.reshape(-1,784).astype('float32')
    y_test = [np.argmax(y, axis=None, out=None) for y in y_test]
    test_labels =np.array(y_test).reshape(-1,1)
    MNISTtotxt(train_images,test_labels,test_images,test_labels)#保存成TXT格式
    print('-----step two finsh-----')
    
    print('-----step three: build the model -----')
    model = creat_model()
    model.summary()
    print('-----step three finish -----')

    print('-----step four: train the model -----')
    model.fit(train_images, train_labels, epochs=10)
    print('-----step four finish -----')
    print('-----successully setup a model -----')
    
    print('-----step five: save the model -----')
    
    
    try:
        shutil.rmtree('./model_data')
    except OSError:
        print('dir dose not exits') 
    os.mkdir('./model_data')
    model.save('./model_data/simple_model.h5')
    model.save_weights('./model_data/my_checkpoint')
    model.save_weights('./model_data/weights.h5', save_format = 'h5')
    print('-----step five finish -----')
    
    print('-----now do a prediction -----')
    #test_images = test_images / 255.0
    test_loss, test_acc = model.evaluate(test_images, test_labels)
    print('Test accuracy:', test_acc)
    predictions = model.predict(test_images)
    num_rows = 5
    num_cols = 3
    num_images = num_rows*num_cols
    plt.figure(figsize=(2*2*num_cols, 2*num_rows))
    test_images_plot = test_images.reshape(-1,28,28).astype('float32')
    for i in range(num_images):
          plt.subplot(num_rows, 2*num_cols, 2*i+1)
          plot_image(i, predictions, test_labels, test_images_plot)
          plt.subplot(num_rows, 2*num_cols, 2*i+2)
          plot_value_array(i, predictions, test_labels)
    plt.show()
    
def plot_image(i, predictions_array, true_label, img):
    #class_names = ['T-shirt/top', 'Trouser', 'Pullover', 'Dress', 'Coat', 
    #        'Sandal', 'Shirt', 'Sneaker', 'Bag', 'Ankle boot']
    class_names = ['0','1', '2', '3', '4', '5', 
            '6', '7', '8', '9']
    predictions_array, true_label, img = predictions_array[i],np.asscalar(true_label[i]), img[i]
    plt.grid(False)
    plt.xticks([])
    plt.yticks([])
    plt.imshow(img, cmap=plt.cm.binary)
    predicted_label = np.argmax(predictions_array)
    if predicted_label == true_label:
        color = 'blue'
    else:
        color = 'red'
    plt.xlabel("{} {:2.0f}% ({})".format(class_names[predicted_label],
                              100*np.max(predictions_array),
                              class_names[true_label]),
                              color=color)

def plot_value_array(i, predictions_array, true_label):
    predictions_array, true_label = predictions_array[i], np.asscalar(true_label[i])
    plt.grid(False)
    plt.xticks([])
    plt.yticks([])
    thisplot = plt.bar(range(10), predictions_array, color="#777777")
    plt.ylim([0, 1]) 
    predicted_label = np.argmax(predictions_array)
    thisplot[predicted_label].set_color('red')
    thisplot[true_label].set_color('blue')
    
def RUN_weight():
    fashion_mnist=input_data.read_data_sets("MNIST_data",one_hot=True)
    x_test, y_test = fashion_mnist.test.images, fashion_mnist.test.labels
    test_images = x_test.reshape(-1,28, 28).astype('float32')
    y_test = [np.argmax(y, axis=None, out=None) for y in y_test]
    test_labels =np.array(y_test).reshape(-1,1)
    model = creat_model()
    model.load_weights('./model_data/my_checkpoint')
    model.summary()
    loss,acc = model.evaluate(test_images, test_labels)
    print("Restored model, accuracy: {:5.2f}%".format(100*acc))

def RUN_model():
    fashion_mnist=input_data.read_data_sets("MNIST_data",one_hot=True)
    x_test, y_test = fashion_mnist.test.images, fashion_mnist.test.labels
    test_images = x_test.reshape(-1,28, 28).astype('float32')
    y_test = [np.argmax(y, axis=None, out=None) for y in y_test]
    test_labels =np.array(y_test).reshape(-1,1)
    new_model = keras.models.load_model('./model_data/simple_model.h5')
    new_model.summary()
    loss, acc = new_model.evaluate(test_images, test_labels)
    print("Restored model, accuracy: {:5.2f}%".format(100*acc))
    
def creat_model():    
    model = keras.Sequential([
        keras.layers.Flatten(input_shape=(784,)),
        keras.layers.Dense(48, activation=tf.nn.relu),
        keras.layers.Dense(10, activation=tf.nn.softmax)
    ])
    model.compile(optimizer='adam', 
        loss='sparse_categorical_crossentropy',
        metrics=['accuracy'])
    return model
