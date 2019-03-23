// EdgeAI-X64.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"



double d_in[Data][In], d_out[Data][Out];
double w[Neuron][In], o[Neuron], v[Out][Neuron];
double Maxin[In], Minin[In], Maxout[Out], Minout[Out];
double OutputData[Out];
double dv[Out][Neuron], dw[Neuron][In];
double e;





int  main()
{
	int i=0;
	double var1, var2;
#ifdef TRAIN_MODEL
	writeTest();							//随机修改训练输入输出测试数据
	readData();								//读取训练输入输出数据
	initBPNework();
	trainNetwork();
	writeNeuron();
#else
	initBPNework();
#endif
	while ((i++) < 20) {
		var1 = (rand() % 1000 / 100.0);
		var2 = (rand() % 1000 / 100.0);
		printf("%lf+%lf forecast is %lf \n", var1, var2, result(var1, var2));
	}
		
	system("pause");
	
	return 0;
}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
