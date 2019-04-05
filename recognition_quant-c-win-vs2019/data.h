#pragma once
#ifndef __DATA__H
#define __DATA__H

#define RISCV_VS

#ifdef RISCV_VS

typedef unsigned short int uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long int  uint64_t;
typedef char      int8_t;

typedef int       int32_t;
typedef long int  int64_t;
#endif

typedef unsigned char      uint8_t;
typedef short int int16_t;

#define In 784
#define Out 10
#define Neuron 48

extern uint8_t test_data[3][784];
extern uint8_t In_Neuron[In][Neuron];
extern uint8_t Neuron_Out[Neuron][Out];
extern int16_t bias0[Neuron];
extern float bias1[Out];
extern uint8_t b[2];
extern float k[2];

#endif // !__DATA__H
