#pragma once
#ifndef __DATA__H
#define __DATA__H


#define In 784
#define Out 10
#define Neuron 48

extern const unsigned char test_data[1][784];
extern const float In_Neuron[In][Neuron];
extern const float Neuron_Out[Neuron][Out];
extern const float bias0[Neuron];
extern const float bias1[Out];
#endif // !__DATA__H
