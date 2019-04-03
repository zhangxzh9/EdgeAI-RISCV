#pragma once
#ifndef __DATA__H
#define __DATA__H


#define In 784
#define Out 10
#define Neuron 48
typedef unsigned char uint_8t;

extern uint_8t test_data[1][784];
extern uint_8t In_Neuron[In][Neuron];
extern uint_8t Neuron_Out[Neuron][Out];
extern float bias0[Neuron];
extern float bias1[Out];
extern short int b[2];
extern float k[2];

#endif // !__DATA__H
