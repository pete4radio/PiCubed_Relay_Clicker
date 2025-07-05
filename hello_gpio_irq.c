/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define GPIO_WATCH_PIN 2

#define RELAY_GPIO 44   // GPIO pin for the relay
//Check the pin is compatible with the platform
#if RELAY_GPIO >= NUM_BANK0_GPIOS
#error "Recompile specifying the RP2350B platform SAMWISE"
#endif

#define ENAB_BURN_A_GPIO 36 // GPIO pin for enabling the "burn wire"
#define ENAB_BURN_B_GPIO 35 // GPIO pin for enabling the "burn wire"

static char event_str[128];

void gpio_event_string(char *buf, uint32_t events);

void gpio_callback(uint gpio, uint32_t events) {
    // Put the GPIO event(s) that just happened into event_str
    // so we can print it
    gpio_event_string(event_str, events);
    printf("GPIO %d %s\n", gpio, event_str);
}

int main() {
    stdio_init_all();
    sleep_ms(10000);

    printf("Hello GPIO IRQ\n");
    gpio_init(GPIO_WATCH_PIN);
    gpio_set_irq_enabled_with_callback(GPIO_WATCH_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Trigger the relay.  Set the GPIO to output mode
    gpio_init(RELAY_GPIO);
    gpio_set_dir(RELAY_GPIO, GPIO_OUT);
    
    while (1){
        // set the GPIO 44 Low, wait 5 seconds, and set it high
        gpio_put(RELAY_GPIO, 0);
        sleep_ms(5000);
        gpio_put(RELAY_GPIO, 1);
        // Turn on enable burn wire A for 1 second, turn it off
        gpio_init(ENAB_BURN_A_GPIO);
        gpio_set_dir(ENAB_BURN_A_GPIO, GPIO_OUT);
        gpio_put(ENAB_BURN_A_GPIO, 1);
        printf(" Burn Wire A is on ...");
        sleep_ms(1000);
        gpio_put(ENAB_BURN_A_GPIO, 0);
        // Turn on enable burn wire B for 1 second, turn it off
        gpio_init(ENAB_BURN_B_GPIO);
        gpio_set_dir(ENAB_BURN_B_GPIO, GPIO_OUT);
        gpio_put(ENAB_BURN_B_GPIO, 1);
        printf(" Burn Wire B is on\n");
        sleep_ms(1000);
        gpio_put(ENAB_BURN_B_GPIO, 0);
        printf(".");
    }
}


static const char *gpio_irq_str[] = {
        "LEVEL_LOW",  // 0x1
        "LEVEL_HIGH", // 0x2
        "EDGE_FALL",  // 0x4
        "EDGE_RISE"   // 0x8
};

void gpio_event_string(char *buf, uint32_t events) {
    for (uint i = 0; i < 4; i++) {
        uint mask = (1 << i);
        if (events & mask) {
            // Copy this event string into the user string
            const char *event_str = gpio_irq_str[i];
            while (*event_str != '\0') {
                *buf++ = *event_str++;
            }
            events &= ~mask;

            // If more events add ", "
            if (events) {
                *buf++ = ',';
                *buf++ = ' ';
            }
        }
    }
    *buf++ = '\0';
}
