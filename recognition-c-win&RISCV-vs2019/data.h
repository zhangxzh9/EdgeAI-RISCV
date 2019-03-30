#pragma once
#ifndef __DATA__H
#define __DATA__H


#define In 784
#define Out 10
#define Neuron 48

extern unsigned char test_data[1][784];
extern float In_Neuron[In][Neuron];
extern float Neuron_Out[Neuron][Out];
extern float bias0[Neuron];
extern float bias1[Out];
#endif // !__DATA__H
