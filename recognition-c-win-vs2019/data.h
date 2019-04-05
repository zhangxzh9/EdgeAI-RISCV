#pragma once
#ifndef __DATA__H
#define __DATA__H
//#define RISCV_E203

#ifndef RISCV_E203

typedef unsigned short int uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long int  uint64_t;
typedef char      int8_t;

typedef int       int32_t;
typedef long int  int64_t;
#endif

typedef unsigned char uint8_t;
typedef short int int16_t;

#define In 784
#define Out 10
#define Neuron 48


extern const uint8_t test_data[3][784];
extern const float In_Neuron[In][Neuron];
extern const float Neuron_Out[Neuron][Out];
extern const float bias0[Neuron];
extern const float bias1[Out];
#endif // !__DATA__H
