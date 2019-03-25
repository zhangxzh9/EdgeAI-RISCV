// See LICENSE for license details.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "data.h"

const double Minin[In]   = { 0.000000 ,0.000000 };
const double Maxin[In]   = { 9.990000 ,9.980000 };
const double Minout[Out] = { 1.160000 };
const double Maxout[Out] = {19.780000 };

double o[Neuron] = {0.0};
double Output[Out] = {0.0};
unsigned int result(double *var1)
{
	int i, j;
	unsigned int max_num;
	double sum, *y;
	/*preprocessing the data*/
	for (i = 0; i < Neuron; ++i) {
		sum = 0;
		for (j=0;j< In;j++) {
			sum += In_Neuron[j][i] * (*var1);
			var1++;
		}
		o[i] = sum + bias0[i];
		o[i] =	1 / (1 + exp(-1 * o[i]));
	}
	sum = 0;
	for (i = 0; i < Out; ++i) {
		sum = 0;
		for (j = 0; j < Neuron; j++) {
			sum += Neuron_Out[j][i] * o[j];
		}	
		Output[i] = sum + bias1[i];
	}
	sum = Output[0];
	max_num = 0;
	for (i = 1; i < Out; ++i) {
		if (sum < Output[i]){
			sum = Output[i];
			max_num = i;
		}
	}
	return max_num;
}

int main(int argc, char **argv)
{
  int i = 0;
  double var1[784];

	printf("%lf forecast is %d \n", var1[0], result(var1));
  return 0;

}
