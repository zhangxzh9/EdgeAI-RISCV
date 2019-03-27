#pragma once
#ifndef __DATA__H
#define __DATA__H


#define In 784
#define Out 10
#define Neuron 48

extern const double In_Neuron[In][Neuron];
extern const double Neuron_Out[Neuron][Out];
extern const double bias0[Neuron];
extern const double bias1[Out];
#endif // !__DATA__H
