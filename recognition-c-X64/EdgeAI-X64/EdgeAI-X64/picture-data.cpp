#include "picture-data.h"
#include "pch.h"

void writeTest() {
	FILE* fp1, * fp2;
	errno_t err;
	double r1, r2;
	int i;
	srand((unsigned)time(NULL));
	if ((err = fopen_s(&fp1, "D:\\in.txt", "w")) != 0) {
		printf("can not open the in file\n");
		exit(0);
	}
	if ((err = fopen_s(&fp2, "D:\\out.txt", "w")) != 0) {
		printf("can not open the out file\n");
		exit(0);
	}


	for (i = 0; i < Data; i++) {
		r1 = rand() % 1000 / 100.0;
		r2 = rand() % 1000 / 100.0;
		fprintf(fp1, "%lf  %lf\n", r1, r2);
		fprintf(fp2, "%lf \n", r1 + r2);
	}
	fclose(fp1);
	fclose(fp2);
}

void readData() {

	FILE* fp1, * fp2;
	errno_t err;
	int i, j;
	if ((err = fopen_s(&fp1, "D:\\in.txt", "r")) != 0) {
		printf("can not open the in file\n");
		exit(0);
	}
	for (i = 0; i < Data; i++)
		for (j = 0; j < In; j++)
			fscanf_s(fp1, "%lf", &d_in[i][j]);
	fclose(fp1);

	if ((err = fopen_s(&fp2, "D:\\out.txt", "r")) != 0) {
		printf("can not open the out file\n");
		exit(0);
	}
	for (i = 0; i < Data; i++)
		for (j = 0; j < Out; j++)
			fscanf_s(fp1, "%lf", &d_out[i][j]);
	fclose(fp2);
}

void writeNeuron()
{
	FILE* fp1;
	errno_t err;
	int i, j;
	if ((err = fopen_s(&fp1, "D:\\neuron_in.txt", "w")) != 0)
	{
		printf("can not open the neuron file\n");
		exit(0);
	}
	for (i = 0; i < Neuron; ++i)
		for (j = 0; j < In; ++j) {
			fprintf(fp1, "%lf ", w[i][j]);
		}
	fclose(fp1);
	if ((err = fopen_s(&fp1, "D:\\neuron_out.txt", "w")) != 0)
	{
		printf("can not open the neuron file\n");
		exit(0);
	}
	for (i = 0; i < Neuron; ++i)
		for (j = 0; j < Out; ++j) {
			fprintf(fp1, "%lf ", v[j][i]);
		}
	fclose(fp1);
	if ((err = fopen_s(&fp1, "D:\\max_min.txt", "w")) != 0)
	{
		printf("can not open the max_min file\n");
		exit(0);
	}
	for (i = 0; i < In; ++i){
			fprintf(fp1, "%lf\n", Minin[i]);
			fprintf(fp1, "%lf\n", Maxin[i]);
			fprintf(fp1, "%lf\n", Minout[i]);
			fprintf(fp1, "%lf\n", Maxout[i]);
		}
	fclose(fp1);
}