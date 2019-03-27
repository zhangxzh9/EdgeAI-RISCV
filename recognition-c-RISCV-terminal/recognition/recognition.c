// See LICENSE for license details.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "platform.h"
#include <string.h>
#include "plic/plic_driver.h"
#include "encoding.h"
#include <unistd.h>
#include "stdatomic.h"

#define In 2
#define Out 1
#define Neuron 45

void reset_demo (void);

// Structures for registering different interrupt handlers
// for different parts of the application.
typedef void (*function_ptr_t) (void);

void no_interrupt_handler (void) {};

function_ptr_t g_ext_interrupt_handlers[PLIC_NUM_INTERRUPTS];


// Instance data for the PLIC.

plic_instance_t g_plic;


/*Entry Point for PLIC Interrupt Handler*/
void handle_m_ext_interrupt(){
  plic_source int_num  = PLIC_claim_interrupt(&g_plic);
  if ((int_num >=1 ) && (int_num < PLIC_NUM_INTERRUPTS)) {
    g_ext_interrupt_handlers[int_num]();
  }
  else {
    exit(1 + (uintptr_t) int_num);
  }
  PLIC_complete_interrupt(&g_plic, int_num);
}


/*Entry Point for Machine Timer Interrupt Handler*/
void handle_m_time_interrupt(){

  clear_csr(mie, MIE_MTIE);

  // Reset the timer for 3s in the future.
  // This also clears the existing timer interrupt.

  volatile uint64_t * mtime       = (uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIME);
  volatile uint64_t * mtimecmp    = (uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIMECMP);
  uint64_t now = *mtime;
  uint64_t then = now + 0.5 * RTC_FREQ;
  *mtimecmp = then;


  GPIO_REG(GPIO_OUTPUT_VAL) ^= (0x1 << RED_LED_GPIO_OFFSET);
  
  // Re-enable the timer interrupt.
  set_csr(mie, MIE_MTIE);

}

static void _putc(char c) {
  while ((int32_t) UART0_REG(UART_REG_TXFIFO) < 0);
  UART0_REG(UART_REG_TXFIFO) = c;
}

int _getc(char * c){
  int32_t val = (int32_t) UART0_REG(UART_REG_RXFIFO);
  if (val > 0) {
    *c =  val & 0xFF;
    return 1;
  }
  return 0;
}

char * read_instructions_msg= " \
\n\
 ";


const char * printf_instructions_msg= " \
\n\
\n\
\n\
\n\
This is printf function printed:  \n\
\n\
             !! Here We Go, HummingBird !! \n\
\n\
     ######    ###    #####   #####          #     #\n\
     #     #    #    #     # #     #         #     #\n\
     #     #    #    #       #               #     #\n\
     ######     #     #####  #        #####  #     #\n\
     #   #      #          # #                #   #\n\
     #    #     #    #     # #     #           # #\n\
     #     #   ###    #####   #####             #\n\
\n\
 ";




void button_1_handler(void) {

  // Green LED On
  GPIO_REG(GPIO_OUTPUT_VAL) |= (1 << GREEN_LED_GPIO_OFFSET);

  // Clear the GPIO Pending interrupt by writing 1.
  GPIO_REG(GPIO_RISE_IP) = (0x1 << BUTTON_1_GPIO_OFFSET);

};


void button_2_handler(void) {

  // Blue LED On
  GPIO_REG(GPIO_OUTPUT_VAL) |= (1 << BLUE_LED_GPIO_OFFSET);

  GPIO_REG(GPIO_RISE_IP) = (0x1 << BUTTON_2_GPIO_OFFSET);

};

void register_plic_irqs (){

   /**************************************************************************
   * Set up the PLIC
   *
   *************************************************************************/
  PLIC_init(&g_plic,
	    PLIC_CTRL_ADDR,
	    PLIC_NUM_INTERRUPTS,
	    PLIC_NUM_PRIORITIES);


  for (int ii = 0; ii < PLIC_NUM_INTERRUPTS; ii ++){
    g_ext_interrupt_handlers[ii] = no_interrupt_handler;
  }

  g_ext_interrupt_handlers[PLIC_INT_DEVICE_BUTTON_1] = button_1_handler;
  g_ext_interrupt_handlers[PLIC_INT_DEVICE_BUTTON_2] = button_2_handler;


  // Have to enable the interrupt both at the GPIO level,
  // and at the PLIC level.
  PLIC_enable_interrupt (&g_plic, PLIC_INT_DEVICE_BUTTON_1);
  PLIC_enable_interrupt (&g_plic, PLIC_INT_DEVICE_BUTTON_2);

  // Priority must be set > 0 to trigger the interrupt.
  PLIC_set_priority(&g_plic, PLIC_INT_DEVICE_BUTTON_1, 1);
  PLIC_set_priority(&g_plic, PLIC_INT_DEVICE_BUTTON_2, 1);

 } 


void setup_mtime() {

	// Set the machine timer to go off in 3 seconds.
	// The
	volatile uint64_t* mtime = (uint64_t*)(CLINT_CTRL_ADDR + CLINT_MTIME);
	volatile uint64_t * mtimecmp = (uint64_t*)(CLINT_CTRL_ADDR + CLINT_MTIMECMP);
	uint64_t now = *mtime;
	uint64_t then = now + 10 * RTC_FREQ;
	*mtimecmp = then;
}


double Minin[In]   = { 0.000000 ,0.000000 };
double Maxin[In]   = { 9.990000 ,9.980000 };
double Minout[Out] = { 1.160000 };
double Maxout[Out] = {19.780000 };
double w[Neuron][In] = { 
	-5.941391,-7.042417,-8.256303,-8.914795,-6.458501,-6.202561,
	-6.161421,-7.112312,-7.285661,-8.391404,-7.113584,-9.817297,
	-6.409601,-5.582646,-5.466988,-6.360028,-7.978790,-8.876974,
	1.021428,0.228133,-7.212666,-8.298154,-0.239933,-0.879425,
	-6.798776,-8.574813,-8.438633,-9.748820,-6.388837,-6.884289,
	-7.014530,-8.254206,-5.981233,-8.232124,-4.379607,-3.348173,
	-6.964674,-7.590547,-4.903317,-5.684819,-6.761769,-8.012713,
	-6.864295,-8.902241,-5.640155,-6.905969,-5.819553,-5.857288,
	-4.960448,-6.475596,-7.968107,-9.410235,-7.082113,-7.071225,
	-8.144074,-10.335302,-6.025334,-7.716133,-6.140890,-7.866162,
	-6.436218,-8.205764,-5.404179,-4.923604,-7.031002,-8.517496,
	-9.433551,-11.231112,-6.308033,-7.140151,-7.298664,-7.847023,
	-5.955736,-7.723252,-7.277890,-9.994728,-7.688505,-9.481877,
	-5.946489,-5.606852,-6.784355,-6.708318,-7.293060,-9.167451,
	-5.478129,-7.188360,-6.806597,-8.335728,-6.931984,-9.364435, 
};
double v[Out][Neuron] = {
	-0.735509,0.108887,-0.059266,0.293081,0.527383,
	-0.334262,-0.022926,0.039794,-0.147576,2.027628,
	0.367462,-2.395317,-0.616199,0.221925,0.666476,
	-0.582127,0.431272,0.304061,-0.374525,0.069475,
	-0.496031,0.196500,-0.511539,-0.466028,-0.196031,
	0.173783,0.564532,-0.326716,0.212770,0.368843,
	-0.433193,-0.163408,0.172430,-0.246579,0.499205,
	-0.399994,0.512568,-0.485735,0.119432,0.158398,
	0.473727,0.062837,0.069770,-0.246529,0.112892, 
};
double o[Neuron] = {0.0};

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

int main(int argc, char **argv)
{
  // Set up the GPIOs such that the LED GPIO
  // can be used as both Inputs and Outputs.
  int i = 0;
  double var1, var2;
  while ((i++) < 20) {
	  var1 = (rand() % 1000 / 100.0);
	  var2 = (rand() % 1000 / 100.0);
	  printf("%lf+%lf forecast is %lf \n", var1, var2, result(var1, var2));
  }
  
  GPIO_REG(GPIO_OUTPUT_EN)  &= ~((0x1 << BUTTON_1_GPIO_OFFSET) | (0x1 << BUTTON_2_GPIO_OFFSET));
  GPIO_REG(GPIO_PULLUP_EN)  &= ~((0x1 << BUTTON_1_GPIO_OFFSET) | (0x1 << BUTTON_2_GPIO_OFFSET));
  GPIO_REG(GPIO_INPUT_EN)   |=  ((0x1 << BUTTON_1_GPIO_OFFSET) | (0x1 << BUTTON_2_GPIO_OFFSET));

  GPIO_REG(GPIO_RISE_IE) |= (1 << BUTTON_1_GPIO_OFFSET);
  GPIO_REG(GPIO_RISE_IE) |= (1 << BUTTON_2_GPIO_OFFSET);


  GPIO_REG(GPIO_INPUT_EN)    &= ~((0x1<< RED_LED_GPIO_OFFSET) | (0x1<< GREEN_LED_GPIO_OFFSET) | (0x1 << BLUE_LED_GPIO_OFFSET)) ;
  GPIO_REG(GPIO_OUTPUT_EN)   |=  ((0x1<< RED_LED_GPIO_OFFSET)| (0x1<< GREEN_LED_GPIO_OFFSET) | (0x1 << BLUE_LED_GPIO_OFFSET)) ;

  GPIO_REG(GPIO_OUTPUT_VAL)  |=   (0x1 << RED_LED_GPIO_OFFSET) ;
  GPIO_REG(GPIO_OUTPUT_VAL)  &=  ~((0x1<< BLUE_LED_GPIO_OFFSET) | (0x1<< GREEN_LED_GPIO_OFFSET)) ;
 

  // Print the message
  printf ("%s",printf_instructions_msg);

  printf ("%s","\nPlease enter any letter from keyboard to continue!\n");

  char c;
  // Check for user input
  while(1){
    if (_getc(&c) != 0){
       printf ("%s","I got an input, it is\n\r");
       break;
    }
  }
  _putc(c);
  printf ("\n\r");
  printf ("%s","\nThank you for supporting RISC-V, you will see the blink soon on the board!\n");

  // Disable the machine & timer interrupts until setup is done.
  clear_csr(mie, MIE_MEIE);
  clear_csr(mie, MIE_MTIE);

  register_plic_irqs();

  setup_mtime();

  // Enable the Machine-External bit in MIE
  set_csr(mie, MIE_MEIE);
  // Enable the Machine-Timer bit in MIE
  set_csr(mie, MIE_MTIE);
  // Enable interrupts in general.
  set_csr(mstatus, MSTATUS_MIE);


  /**************************************************************************
   * Demonstrate fast GPIO bit-banging.
   * One can bang it faster than this if you know
   * the entire OUTPUT_VAL that you want to write, but 
   * Atomics give a quick way to control a single bit.
   *************************************************************************/
    
  // For Bit-banging with Atomics demo.
  
  uint32_t bitbang_mask = 0;
  bitbang_mask = (1 << 13);

  GPIO_REG(GPIO_OUTPUT_EN) |= bitbang_mask;

  // For Bit-banging with Atomics demo.
  
  while (1){
    GPIO_REG(GPIO_OUTPUT_VAL) ^= bitbang_mask;
    //atomic_fetch_xor_explicit(&GPIO_REG(GPIO_OUTPUT_VAL), bitbang_mask, memory_order_relaxed);
  }

  return 0;

}
