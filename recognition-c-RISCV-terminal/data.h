#pragma once
#ifndef __DATA__H
#define __DATA__H
typedef unsigned char      uint_8t;
typedef unsigned short int uint_16t;
typedef unsigned int       uint_32t;
typedef char      int_8t;
typedef short int int_16t;
typedef int       int_32t;
typedef long int  int_64t;
#define In 784
#define Out 10
#define Neuron 48

extern float Output[Out];
extern uint_8t test_data[3][784];
extern uint_8t In_Neuron[In][Neuron];
extern uint_8t Neuron_Out[Neuron][Out];
extern int_16t bias0[Neuron];
extern float bias1[Out];
extern uint_8t b[2];
extern float k[2];

#endif // !__DATA__H
