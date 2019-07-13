// See LICENSE for license details.

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include <string.h>
#include "plic/plic_driver.h"
#include "encoding.h"
#include <unistd.h>
#include "stdatomic.h"

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

uint64_t readMtime(){
    volatile uint64_t * mtime       = (uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIME);
    return *mtime;
}

uint8_t _getdata(uint8_t * data){
  int32_t val = (int32_t) UART0_REG(UART_REG_RXFIFO);
  if (!((val& 0x80000000)>>31)) {
    *data =  val & 0xFF;
    return 1;
  }
  return 0;
}

int main(int argc, char **argv)
{
    uint16_t j = 0;
    float var[In] = {0.0};
    int8_t num=-1;
    uint8_t i = 0;
    uint8_t uratdata_q;
    uint64_t begintime = 0,endtime = 0;
    uint64_t usetime = 0,thistime = 0;
    
    
/*     for (i = 0; i < 3; i++) {
        for (j = 0; j < 784; j++)
        {
            var[j] = test_data[i][j] / 255.0;
        }
        num = result(var);
        for (j = 0; j < Out; j++)
        {
            printf("%d:%f\r\n", j, Output[j]);
        }
        printf("predict NO.%d num is: %d \r\n", i, num);
    } */
    printf("finish\r\n");
    
    while (1) {
        //handshade
        while(1){
            while(_getdata(&uratdata_q) == 0);
            if(uratdata_q == 'b'){
                printf ("ok1\r\n");
                while (_getdata(&uratdata_q) == 0);
                if(uratdata_q == 'e'){
                    printf ("ok2\r\n");
                    break;
                }
            }
        }
        // download picture
        for (j = 0;j < 784;j++) {
            while (!_getdata(&uratdata_q));
            test_data[0][j] = uratdata_q;
            //printf("\r\n");
        }
        
        begintime = readMtime();
        for (j = 0; j < 784; j++){
            var[j] = test_data[0][j] / 255.0;
        }
        num = result(var);
        endtime = readMtime();
        
        
        for (i = 0; i < Out; i++){
            printf("%d:%f\r\n", i, Output[i]);
        }
        printf("pridicted number is:\r\n", j, num);
        printf("%d\r\n",num);
        
        thistime = endtime - begintime;
        //printf("this time:%lu\r\n", thistime);
        usetime += thistime;
        printf("use time:%lu\r\n",usetime);
        
        printf("finish\r\n");

    }
    
    return 0;
}
