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
void softmax(float* x, uint8_t row, uint8_t column)
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
	uint8_t num_max = 0, i = 0;
	float num = 0.0;
	for (i = 0; i < Out; i++) {
		if (num <= *x) {
			num = *x;
			num_max = i;
		}
		x++;
	}
	return num_max;
}
float Output[Out] = { 0.0 };
uint8_t  result(float * var1)
{
	uint8_t i;
	uint16_t j;
	float sum=0.0;
	float data[In] = {0.0} , o[Neuron] = { 0.0 };
	for (j = 0; j < In; j++) {
		data[j] = *var1;
		var1++;
	}
	/*preprocessing the data*/
	for (i = 0; i < Neuron; ++i) {
		sum = 0;
		for (j=0;j< In;j++) {
			sum += In_Neuron[j][i] * data[j];
		}
		o[i] = sum + bias0[i];
		o[i] = (o[i] >= 0) ? o[i] : 0;//relu activaction
	}
	sum = 0;
	for (i = 0; i < Out; ++i) {
		sum = 0;
		for (j = 0; j < Neuron; j++) {
			sum += Neuron_Out[j][i] * o[j];
		}	
		Output[i] = sum + bias1[i];
	}
	softmax(Output, 1, Out);
	return max_probability(Output);
}

int main(int argc, char **argv)
{
	uint16_t i = 0;
	float var[In] = {0.0};
	uint8_t res,j;


		for (j = 0; j < 3; j++) {
			printf("NO.%d picture label is %d\n" , j , test_labels[j]);
			for (i = 0; i < 784; i++)
			{
				var[i] = test_data[j][i] / 255.0;
			}
			res = result(var);
			for (i = 0; i < Out; i++)
			{
				printf("%d:%lf\n", i, Output[i]);
			}
			printf("prection is %d\n\n", res);
		}
	
	return 0;
}
