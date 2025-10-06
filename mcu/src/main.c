// Christian Wu
// chrwu@g.hmc.edu
// 09/30/25

#include "../lib/main.h"

int direction;
int deltaT;
float speed;

// Function used by printf to send characters to the laptop - Taken from E155 Website
int _write(int file, char *ptr, int len) {
  int i = 0;
  for (i = 0; i < len; i++) {
    ITM_SendChar((*ptr++));
  }
  return len;
}

int main(void) {
    // Enable A Signal Pin
    gpioEnable(GPIO_PORT_A);
    pinMode(ASIGNAL_PIN, GPIO_OUTPUT);
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD9, 0b01); // Set PA9 as pull-up

    // Enable B Signal Pin
    gpioEnable(GPIO_PORT_A);
    pinMode(BSIGNAL_PIN, GPIO_INPUT);
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD6, 0b01); // Set PA6 as pull-up

    // Initialize timer
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    initDelayTIM(DELAY_TIM);

    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM6EN;
    initCounterTIM(COUNTER_TIM);

    // TODO
    // 1. Enable SYSCFG clock domain in RCC
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    // 2. Configure EXTICR for the input button interrupt
    SYSCFG->EXTICR[2] |= _VAL2FLD(SYSCFG_EXTICR3_EXTI9, 0b000);
    SYSCFG->EXTICR[1] |= _VAL2FLD(SYSCFG_EXTICR2_EXTI6, 0b000);
    // Enable interrupts globally
    __enable_irq();

    // TODO: Configure interrupt for falling edge of GPIO pin for button
    // 1. Configure mask bit
    EXTI->IMR1 |= (1 <<gpioPinOffset(ASIGNAL_PIN));
    EXTI->IMR1 |= (1 <<gpioPinOffset(BSIGNAL_PIN));
    // 2. Enable rising edge trigger
    EXTI->RTSR1 |= (1 << gpioPinOffset(ASIGNAL_PIN));
    EXTI->RTSR1 |= (1 << gpioPinOffset(BSIGNAL_PIN));
    // 3. Enable falling edge trigger
    EXTI->FTSR1 |= (1 << gpioPinOffset(ASIGNAL_PIN));
    EXTI->FTSR1 |= (1 << gpioPinOffset(BSIGNAL_PIN));
    // 4. Turn on EXTI interrupt in NVIC_ISER
    NVIC->ISER[0] |= (1 << EXTI9_5_IRQn);

    while(1){ 
        if (direction) {
            printf("Direction: Clockwise \n");
        } 
        else {
            printf("Direction: Counter Clockwise \n");
        }
        speed = 1000000.0 / (408.0*4.0*deltaT);
        printf("Speed: %f revolutions/second \n", speed);
        delay_millis(DELAY_TIM, 200);
    }

}

// TODO: What is the right name for the IRQHandler?
void EXTI9_5_IRQHandler(void){
    int a_reading = digitalRead(ASIGNAL_PIN);
    int b_reading = digitalRead(BSIGNAL_PIN);
    // Check that the button was what triggered our interrupt
    if (EXTI->PR1 & (1 << 9)){
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << 9);

        if((a_reading && b_reading) || (!a_reading && !b_reading)) {
            deltaT = COUNTER_TIM->CNT;
            direction = 0;
        }

        COUNTER_TIM->CNT = 0;
    }

    // Check that the button was what triggered our interrupt
    if (EXTI->PR1 & (1 << 6)){
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << 6);

        if((a_reading && b_reading) || (!a_reading && !b_reading)) {
            deltaT = COUNTER_TIM->CNT;
            direction = 1;
        }

        COUNTER_TIM->CNT = 0;
    }

}