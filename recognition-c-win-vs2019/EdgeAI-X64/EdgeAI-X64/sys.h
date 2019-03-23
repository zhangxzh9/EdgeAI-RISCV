//#pragma once
#ifndef __SYS__H
#define __SYS__H


#include "picture-data.h"
#include "bp-model.h"

//#define TRAIN_MODEL

#define Data  820
#define In 2
#define Out 1
#define Neuron 45
#define TrainC 20000
#define A  0.2
#define B  0.4
#define a  0.2
#define b  0.3

extern double d_in[Data][In], d_out[Data][Out];
extern double w[Neuron][In], o[Neuron], v[Out][Neuron];
extern double Maxin[In], Minin[In], Maxout[Out], Minout[Out];
extern double OutputData[Out];
extern double dv[Out][Neuron], dw[Neuron][In];
extern double e;


#endif // !__MODEL__H