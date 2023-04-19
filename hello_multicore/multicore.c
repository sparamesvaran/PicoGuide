/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/multicore.h"

#define FLAG_VALUE 123

// function to run core 1
void core1_entry() {

    // we receive the flag value back
    uint32_t g = multicore_fifo_pop_blocking();

    if (g != FLAG_VALUE)
    {
        printf("Hmm, that's not right on core 1. Abort!\n");
        return;
    }
    else
    {
        printf("Its all gone well starting on core 1!\n");
    }

    // we send core 0 the flag value back
    multicore_fifo_push_blocking(FLAG_VALUE);

    while (true)
    {   
        // wait to receive a number from core 0
        uint32_t number = multicore_fifo_pop_blocking();
        printf("core 1: pop  %d\n", number);

        // increment number and send it back
        ++number;
        multicore_fifo_push_blocking(number);
        printf("core 1: push %d\n", number);
    }
}

// main runs on core 0
int main() {
    stdio_init_all();

    // do not start the core handshake until the user enters 'enter'
    while (true)
    {
        char c = getchar_timeout_us(0);        
        if(c == 13)
        {
            printf("Hello, multicore!\n");
            break;
        }
    }

    // launch core 1 with the method core1_entry(), i.e. core 1 will execute core1_entry()
    multicore_launch_core1(core1_entry);

    // start the handshake process, send core 1 the flag value
    multicore_fifo_push_blocking(FLAG_VALUE);

    // Wait for core 1 to send us a flag value back
    uint32_t g = multicore_fifo_pop_blocking();

    // check the data that core 1 sent
    if (g != FLAG_VALUE)
    {
        // we did not receive the flag value we expected, exit
        printf("Hmm, that's not right on core 0. Abort!\n");
        return -1;
    }
    else
    {
        // we received the flag value we expected
        printf("It's all gone well starting on core 0!\n");
    }

    // The handshake between the two cores was successful, now start the rest of the task

    // initial number to send to core 1
    uint32_t number = 0;

    while (true)
    {
        printf("\n");
        // send core 1 a number
        printf("core 0: push %d\n", number);
        multicore_fifo_push_blocking(number);
        uint32_t sent_number = number;

        // read the number core 1 sent back, it should be the number we sent +1
        number = multicore_fifo_pop_blocking();
        
        // check that the number we received is the expected one
        if (sent_number+1 == number)
        {
            printf("core 0: pop  %d\n", number);
        }
        else
        {
            printf("core 0: pop incorrect number %d. Abort!", number);
            return -1;
        }
        
        sleep_ms(2000);  
    }
}