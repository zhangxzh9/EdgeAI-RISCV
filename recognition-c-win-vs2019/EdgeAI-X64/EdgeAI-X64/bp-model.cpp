#include "bp-model.h"
#include "pch.h"


void initBPNework() {
	
	int i, j;
#ifndef TRAIN_MODEL
	FILE* fp1;
	errno_t err;
	if ((err = fopen_s(&fp1, "D:\\max_min.txt", "r")) != 0)
	{
		printf("can not open the max_min file\n");
		exit(0);
	}
	for (i = 0; i < In; ++i) {
		fscanf_s(fp1, "%lf", &Minin[i]);
		fscanf_s(fp1, "%lf", &Maxin[i]);
		fscanf_s(fp1, "%lf", &Minout[i]);
		fscanf_s(fp1, "%lf", &Maxout[i]);
	}
	fclose(fp1);
	if ((err = fopen_s(&fp1, "D:\\neuron_in.txt", "r")) != 0)
	{
		printf("can not open the neuron_in file\n");
		exit(0);
	}
	for (i = 0; i < Neuron; ++i)
		for (j = 0; j < In; ++j) {
			fscanf_s(fp1, "%lf", &w[i][j]);
		}
	fclose(fp1);
	if ((err = fopen_s(&fp1, "D:\\neuron_out.txt", "r")) != 0)
	{
		printf("can not open the neuron_out file\n");
		exit(0);
	}
	for (i = 0; i < Neuron; ++i)
		for (j = 0; j < Out; ++j) {
			fscanf_s(fp1, "%lf ", &v[j][i]);
		}

	fclose(fp1);

#else
	for (i = 0; i < In; i++) {
		Minin[i] = Maxin[i] = d_in[0][i];
		for (j = 0; j < Data; j++)
		{
			Maxin[i] = Maxin[i] > d_in[j][i] ? Maxin[i] : d_in[j][i];
			Minin[i] = Minin[i] < d_in[j][i] ? Minin[i] : d_in[j][i];
		}
	}
	for (i = 0; i < Out; i++) {
		Minout[i] = Maxout[i] = d_out[0][i];
		for (j = 0; j < Data; j++)
		{
			Maxout[i] = Maxout[i] > d_out[j][i] ? Maxout[i] : d_out[j][i];
			Minout[i] = Minout[i] < d_out[j][i] ? Minout[i] : d_out[j][i];
		}
	}

	for (i = 0; i < In; i++)
		for (j = 0; j < Data; j++)
			d_in[j][i] = (d_in[j][i] - Minin[i] + 1) / (Maxin[i] - Minin[i] + 1);

	for (i = 0; i < Out; i++)
		for (j = 0; j < Data; j++)
			d_out[j][i] = (d_out[j][i] - Minout[i] + 1) / (Maxout[i] - Minout[i] + 1);

	for (i = 0; i < Neuron; ++i)
		for (j = 0; j < In; ++j) {
			w[i][j] = rand() * 2.0 / RAND_MAX - 1;
			dw[i][j] = 0;
		}
	for (i = 0; i < Neuron; ++i)
		for (j = 0; j < Out; ++j) {
			v[j][i] = rand() * 2.0 / RAND_MAX - 1;
			dv[j][i] = 0;
		}
#endif // !TRAIN_MODEL

	

	

}

void computO(int var) {

	int i, j;
	double sum, y;
	for (i = 0; i < Neuron; ++i) {
		sum = 0;
		for (j = 0; j < In; ++j)
			sum += w[i][j] * d_in[var][j];
		o[i] = 1 / (1 + exp(-1 * sum));
	}

	for (i = 0; i < Out; ++i) {
		sum = 0;
		for (j = 0; j < Neuron; ++j)
			sum += v[i][j] * o[j];

		OutputData[i] = sum;
	}
}

void backUpdate(int var)
{
	int i, j;
	double t;
	for (i = 0; i < Neuron; ++i)
	{
		t = 0;
		for (j = 0; j < Out; ++j) {
			t += (OutputData[j] - d_out[var][j]) * v[j][i];

			dv[j][i] = A * dv[j][i] + B * (OutputData[j] - d_out[var][j]) * o[i];
			v[j][i] -= dv[j][i];
		}

		for (j = 0; j < In; ++j) {
			dw[i][j] = a * dw[i][j] + b * t * o[i] * (1 - o[i]) * d_in[var][j];
			w[i][j] -= dw[i][j];
		}
	}
}



void  trainNetwork() {

	int i, c = 0, j;
	do {
		e = 0;
		for (i = 0; i < Data; ++i) {
			computO(i);
			for (j = 0; j < Out; ++j)
				e += fabs((OutputData[j] - d_out[i][j]) / d_out[i][j]);
			backUpdate(i);
		}
		printf("%d  %lf\n", c, e / Data);
		c++;
	} while (c<TrainC && e / Data>0.01);
}

double result(double var1, double var2)
{
	int i, j;
	double sum, y;

	var1 = (var1 - Minin[0] + 1) / (Maxin[0] - Minin[0] + 1);
	var2 = (var2 - Minin[1] + 1) / (Maxin[1] - Minin[1] + 1);

	for (i = 0; i < Neuron; ++i) {
		sum = 0;
		sum = w[i][0] * var1 + w[i][1] * var2;
		o[i] = 1 / (1 + exp(-1 * sum));
	}
	sum = 0;
	for (j = 0; j < Neuron; ++j)
		sum += v[0][j] * o[j];

	return sum * (Maxout[0] - Minout[0] + 1) + Minout[0] - 1;
}