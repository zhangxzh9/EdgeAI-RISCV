// See LICENSE for license details.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

float exp(float x)
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
	uint_8t k, j;
	for (j = 0; j < row; ++j)
	{
		float max = 0.0;
		float sum = 0.0;
		for (k = 0; k < column; ++k)
			if (max < x[k + j * column])
				max = x[k + j * column];
		for (k = 0; k < column; ++k)
		{
			x[k + j * column] = exp(x[k + j * column] - max);    // prevent data overflow
			sum += x[k + j * column];
		}
		for (k = 0; k < column; ++k) 
			x[k + j * column] /= sum;
	}
}   //row*column

float *  result(uint_8t * var1)
{
	short int i, j;
	float sum = 0.0;
	int sumb = 0;
	int o[Neuron] = {0};
	float Output[Out] = { 0.0 };
	uint_8t data[In] ;

	for (i = 0; i < In; i++) {
		data[i] = *var1;
		var1++;
	}
	/*preprocessing the data*/
	for (i = 0; i < Neuron; ++i) {
		sum = 0;
		sumb = 0;
		for (j=0;j< In;j++) {
			sum += In_Neuron[j][i] * data[j];
			sumb += data[j] * b[0];
		}
		o[i] = (sum + sumb) ;//反量化一部分
		o[i] = o[i] + bias0[i] * (255 * k[0]);//不反量化先吧，后面一起
		o[i] = (o[i] >= 0) ? o[i] : 0;//relu activaction
	}
	for (i = 0; i < Out; ++i) {
		sum = 0;
		sumb = 0;
		for (j = 0; j < Neuron; j++) {
			sum += Neuron_Out[j][i] * o[j];
			sumb += o[j] * b[1];
		}
		Output[i] = (sum + sumb) / (k[1]* 255 * k[0]);
		Output[i] = Output[i] + bias1[i];
	}
	softmax(Output, 1, Out);
	return Output;
}

int main(int argc, char **argv)
{
	uint_8t i = 0;
	float *res;
	res = result(test_data[0]);
	printf("the picture's label is %d\n", 7);
	for (i = 0; i < Out; i++)
	{
		printf("%d:%lf\n",i,res[i]);
	}
	return 0;
}
