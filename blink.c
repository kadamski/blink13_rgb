#include <avr/io.h>
#include <util/delay.h>

#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#define DURATION_MIN 15

#define LED0 (1<<PB0)
#define LED1 (1<<PB3)
#define LED2 (1<<PB4)

#define BUTT (1<<PB2)

#define COUNTER_START (5*DURATION_MIN)
#define DEBOUNCE_TIME 100

char state = 1;
char counter = COUNTER_START;

ISR(WDT_vect)
{
}

ISR(PCINT0_vect)
{
    uint8_t read = PINB;
    PCMSK &= ~(1<<2);
    if ((read & BUTT) == 0) {
        state = 0;
    }
    _delay_ms(DEBOUNCE_TIME);
    PCMSK |= (1<<2);
}

void power_down(void)
{
        int old = PORTB;

        WDTCR &= ~(1<<WDTIE);
        PORTB &= ~(LED0 | LED1 | LED2);

        state = 2;

        do {
            sleep_mode();
        } while(state == 2);

        state = 1;
        counter = COUNTER_START;
        WDTCR |= (1<<WDTIE);

        PORTB = old;
}

void sleep(void)
{
    wdt_reset();
    sleep_mode();
    if (state == 0) {
        power_down();
    }
}

inline void setup_ports(void)
{
    DDRB |= LED0 | LED1 | LED2;
    DDRB &= ~BUTT;

    PORTB ^= LED0; /* LED on */
    PORTB |= BUTT; /* pull-up */

    PCMSK = (1<<2);
}

int main(void)
{
    MCUSR = 0;

    setup_ports();

    WDTCR |= (1<<WDP2) | (1<<WDP1);  /* 128K */
    WDTCR |= (1<<WDTIE);

    GIMSK |= (1<<PCIE);

    sei();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    for(;;) {
        if (counter-- == 0) {
            power_down();
        }

        sleep();
        PORTB ^= LED0 | LED1;
        sleep();
        PORTB ^= LED1 | LED2;
        sleep();
        PORTB ^= LED2;

        sleep();
        PORTB ^= LED0;
        sleep();
        PORTB ^= LED0;
        sleep();
        PORTB ^= LED1;
        sleep();
        PORTB ^= LED1;
        sleep();
        PORTB ^= LED2;
        sleep();
        PORTB ^= LED2;
        sleep();
        PORTB ^= LED0;
    }

    return 0;
}
