// See LICENSE for license details.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"




double Output[Out] = { 0.0 };
double exp(double x)
{
	x = 1.0 + x / 256;
	for (int i = 0; i < 8; i++) {
		x *= x;
	}
	return x;
}

//对每一行进行softmax
void softmax(double* x, int row, int column)
{
	unsigned char k, j;
	for (j = 0; j < row; ++j)
	{
		double max = 0.0;
		double sum = 0.0;
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

void result(double * var1)
{
	int i, j;
	double sum=0.0;
	double o[Neuron] = { 0.0 } ;
	double* data = var1;
	/*preprocessing the data*/
	for (i = 0; i < Neuron; ++i) {
		sum = 0;
		for (j=0;j< In;j++) {
			sum += In_Neuron[j][i] * (*data);
			var1++;
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
}

int main(int argc, char **argv)
{
	int i = 0;
	double var[784],sum=0.0;
	for (i = 0; i < 784; i++)
	{
		var[i] = test_data[0][0] / 255.0;
	}
	result(var);
	for (i = 0; i < Out; i++)
	{
		sum += Output[i];
		printf("%d:%lf\n",i,Output[i]);
	}
	printf("sum:%lf\n", sum);
	return 0;

}
