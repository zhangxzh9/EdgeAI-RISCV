#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#define Data  820
#define In 2
#define Out 1
#define Neuron 45
#define TrainC 20000
#define A  0.2
#define B  0.4
#define a  0.2
#define b  0.3

double d_in[Data][In],d_out[Data][Out];
double w[Neuron][In],o[Neuron],v[Out][Neuron];
double Maxin[In],Minin[In],Maxout[Out],Minout[Out];
double OutputData[Out];
double dv[Out][Neuron],dw[Neuron][In];
double e;


void writeTest(){
	FILE *fp1,*fp2;
	errno_t err;
	double r1,r2;
	int i;
	srand((unsigned)time(NULL)); 
	if((err=fopen_s(&fp1,"D:\\in.txt","w"))!=0){
		printf("can not open the in file\n");
		exit(0);
	}
	if((err =fopen_s(&fp2,"D:\\out.txt","w"))!=0){
		printf("can not open the out file\n");
		exit(0);
	}


	for(i=0;i<Data;i++){
		r1=rand()%1000/100.0;
		r2=rand()%1000/100.0;
		fprintf(fp1,"%lf  %lf\n",r1,r2);
		fprintf(fp2,"%lf \n",r1+r2);
	}
	fclose(fp1);
	fclose(fp2);
}

void readData(){

	FILE *fp1,*fp2;
	errno_t err;
	int i,j;
	if((err=fopen_s(&fp1,"D:\\in.txt","r"))!=0){
		printf("can not open the in file\n");
		exit(0);
	}
	for(i=0;i<Data;i++)
		for(j=0; j<In; j++)
			fscanf_s(fp1,"%lf",&d_in[i][j]);
	fclose(fp1);

	if((err=fopen_s(&fp2,"D:\\out.txt","r"))!=0){
		printf("can not open the out file\n");
		exit(0);
	}
	for(i=0;i<Data;i++)
		for(j=0; j<Out; j++)
			fscanf_s(fp1,"%lf",&d_out[i][j]);
	fclose(fp2);
}

void initBPNework(){

	int i,j;

	for(i=0; i<In; i++){
		Minin[i]=Maxin[i]=d_in[0][i];
		for(j=0; j<Data; j++)
		{
			Maxin[i]=Maxin[i]>d_in[j][i]?Maxin[i]:d_in[j][i];
			Minin[i]=Minin[i]<d_in[j][i]?Minin[i]:d_in[j][i];
		}
	}
	for(i=0; i<Out; i++){
		Minout[i]=Maxout[i]=d_out[0][i];
		for(j=0; j<Data; j++)
		{
			Maxout[i]=Maxout[i]>d_out[j][i]?Maxout[i]:d_out[j][i];
			Minout[i]=Minout[i]<d_out[j][i]?Minout[i]:d_out[j][i];
		}
	}

	for (i = 0; i < In; i++)
		for(j = 0; j < Data; j++)
			d_in[j][i]=(d_in[j][i]-Minin[i]+1)/(Maxin[i]-Minin[i]+1);

	for (i = 0; i < Out; i++)
		for(j = 0; j < Data; j++)
			d_out[j][i]=(d_out[j][i]-Minout[i]+1)/(Maxout[i]-Minout[i]+1);

	for (i = 0; i < Neuron; ++i)	
		for (j = 0; j < In; ++j){	
			w[i][j]=rand()*2.0/RAND_MAX-1;
			dw[i][j]=0;
		}

		for (i = 0; i < Neuron; ++i)	
			for (j = 0; j < Out; ++j){
				v[j][i]=rand()*2.0/RAND_MAX-1;
				dv[j][i]=0;
			}
}

void computO(int var){

	int i,j;
	double sum,y;
	for (i = 0; i < Neuron; ++i){
		sum=0;
		for (j = 0; j < In; ++j)
			sum+=w[i][j]*d_in[var][j];
		o[i]=1/(1+exp(-1*sum));
	}

	for (i = 0; i < Out; ++i){
		sum=0;
		for (j = 0; j < Neuron; ++j)
			sum+=v[i][j]*o[j];

		OutputData[i]=sum;
	}	
}

void backUpdate(int var)
{
	int i,j;
	double t;
	for (i = 0; i < Neuron; ++i)
	{
		t=0;
		for (j = 0; j < Out; ++j){
			t+=(OutputData[j]-d_out[var][j])*v[j][i];

			dv[j][i]=A*dv[j][i]+B*(OutputData[j]-d_out[var][j])*o[i];
			v[j][i]-=dv[j][i];
		}

		for (j = 0; j < In; ++j){
			dw[i][j]=a*dw[i][j]+b*t*o[i]*(1-o[i])*d_in[var][j];
			w[i][j]-=dw[i][j];
		}
	}
}

double result(double var1,double var2)
{
	int i,j;
	double sum,y;

	var1=(var1-Minin[0]+1)/(Maxin[0]-Minin[0]+1);
	var2=(var2-Minin[1]+1)/(Maxin[1]-Minin[1]+1);

	for (i = 0; i < Neuron; ++i){
		sum=0;
		sum=w[i][0]*var1+w[i][1]*var2;
		o[i]=1/(1+exp(-1*sum));
	}
	sum=0;
	for (j = 0; j < Neuron; ++j)
		sum+=v[0][j]*o[j];

	return sum*(Maxout[0]-Minout[0]+1)+Minout[0]-1;
}

void writeNeuron()
{
	FILE *fp1;
	errno_t err;
	int i,j;
	if((err=fopen_s(&fp1,"D:\\neuron.txt","w")) !=0)
	{
		printf("can not open the neuron file\n");
		exit(0);
	}
	for (i = 0; i < Neuron; ++i)	
		for (j = 0; j < In; ++j){
			fprintf(fp1,"%lf ",w[i][j]);
		}
	fprintf(fp1,"\n\n\n\n");

	for (i = 0; i < Neuron; ++i)	
		for (j = 0; j < Out; ++j){
			fprintf(fp1,"%lf ",v[j][i]);
		}

	fclose(fp1);
}

void  trainNetwork(){

	int i,c=0,j;
	do{
		e=0;
		for (i = 0; i < Data; ++i){
			computO(i);
			for (j = 0; j < Out; ++j)
				e+=fabs((OutputData[j]-d_out[i][j])/d_out[i][j]);
			backUpdate(i);
		}
		printf("%d  %lf\n",c,e/Data);
		c++;
	}while(c<TrainC && e/Data>0.01);
}



int  main(int argc, char const *argv[])
{
	writeTest();
	readData();
	initBPNework();
	trainNetwork();
	printf("%lf \n",result(6,8));
	printf("%lf \n",result(2.1,7));
	printf("%lf \n",result(4.3,8));
	writeNeuron();
	system("pause");
	return 0;
}