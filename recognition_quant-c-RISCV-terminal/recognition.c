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
#include "model.h"

int _getc(char * c){
  int32_t val = (int32_t) UART0_REG(UART_REG_RXFIFO);
  if (val > 0) {
    *c =  val & 0xFF;
    return 1;
  }
  return 0;
}

static void _putc(char c) {
  while ((int32_t) UART0_REG(UART_REG_TXFIFO) < 0);
  UART0_REG(UART_REG_TXFIFO) = c;
}

static void _putdat(uint8_t data) {
  while ((int32_t) UART0_REG(UART_REG_TXFIFO) < 0);
  UART0_REG(UART_REG_TXFIFO) = data;
}
uint8_t _getdata(uint8_t * data){
  int32_t val = (int32_t) UART0_REG(UART_REG_RXFIFO);
  if (!((val& 0x80000000)>>31)) {
    *data =  val & 0xFF;
    return 1;
  }
  return 0;
}
/* uint8_t _getdata(uint8_t * data){
  int32_t val = (int32_t) UART0_REG(UART_REG_RXFIFO);
  if ((val& 0x8000)>>32 != 1) {
    *data =  val & 0x00FF;
    return 1;
  }
  return 0;
} */
uint64_t readMtime(){
    volatile uint64_t * mtime       = (uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIME);
    return *mtime;
}


int main(int argc, char **argv)
{
	uint8_t i = 0;
	uint16_t j = 0;
	int8_t num = -1;
	uint8_t uratdata_q;
    uint64_t begintime = 0,endtime = 0;
    uint64_t usetime = 0,thistime = 0;
    
    for (j = 0; j < 3; j++) {
		num = result(test_data[j]);
		for (i = 0; i < Out; i++){
			printf("%d:%f \r\n", i, Output[i]);
		}
		printf("predict NO.%d num is: %d \r\n", j, num);
	}
    printf("finish\r\n");
	while(1){
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
		for (j = 0;j < 784;j++) {
			while (!_getdata(&uratdata_q));
			test_data[0][j] = uratdata_q;
            //printf("\r\n");
		}

        begintime = readMtime();
		num = result(test_data[0]);
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
