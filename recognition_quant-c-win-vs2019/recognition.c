// See LICENSE for license details.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

float my_exp(float x)
{
	x = 1.0 + x / 256.0;
	for (int i = 0; i < 8; i++) {
		x *= x;
	}
	return x;
}

//对每一行进行softmax
void softmax(float* x, unsigned char row, unsigned char column)
{
	uint8_t k, j;
	for (j = 0; j < row; ++j)
	{
		float max = 0.0;
		float sum = 0.0;
		for (k = 0; k < column; ++k)
			if (max < x[k + j * column])
				max = x[k + j * column];
		for (k = 0; k < column; ++k)
		{
			x[k + j * column] = my_exp(x[k + j * column] - max);    // prevent data overflow
			sum += x[k + j * column];
		}
		for (k = 0; k < column; ++k) 
			x[k + j * column] /= sum;
	}
}   //row*column

uint8_t max_probability(float* x) {
	uint8_t num_max=0,i = 0;
	float num = 0.0;
	for (i = 0; i < Out; i++) {
		if (num <= * x) {
			num = *x;
			num_max = i;
		}
		x++;
	}
	return num_max;
}
float Output[Out] = { 0.0 };
uint8_t  result(uint8_t * var1)
{
	uint8_t  i=0;
	uint8_t data[In] = {0};
	uint16_t j=0;
	int32_t o[Neuron] = { 0 };//max:2^8 * 2^8 * 784
	int64_t sum = 0;        //max:2^8 * 2^8 * 784 * 2^8 * 48
	int64_t sumb = 0;       //max:2^8 * 2^8 * 784 * 2^8 * 48
	

	for (j = 0; j < In; j++) {
		data[j] = *var1;
		var1++;
	}
	/*preprocessing the data*/
	for (i = 0; i < Neuron; ++i) {
		sum = 0;
		sumb = 0;
		for (j=0;j< In;j++) {
			sum += In_Neuron[j][i] * data[j];//
			sumb += data[j] * b[0];
		}
		o[i] = (sum - sumb) ;
		o[i] = o[i] + bias0[i];
		o[i] = (o[i] >= 0) ? o[i] : 0;//relu activaction
	}
	for (i = 0; i < Out; ++i) {
		sum = 0;
		sumb = 0;
		for (j = 0; j < Neuron; j++) {
			sum += Neuron_Out[j][i] * o[j];
			sumb += o[j] * b[1];
		}
		Output[i] = (sum - sumb) / (k[1]* 255 * k[0]);
		Output[i] = Output[i] + bias1[i];
	}
	softmax(Output, 1, Out);
	return max_probability(Output);
}

int main(int argc, char **argv)
{
	uint8_t i = 0,j = 0;
	int8_t num = -1;


		for (j = 0; j < 3; j++) {
			printf("NO.%d picture label is %d\n", j, test_labels[j]);
			num = result(test_data[j]);
					for (i = 0; i < Out; i++)
					{
						printf("%d:%f\n", i, Output[i]);
					}
			printf("quantized model predict NO.%d num is: %d\n\n", j, num);
		}
	
	return 0;
}
