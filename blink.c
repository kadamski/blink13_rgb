#include <avr/io.h>
#include <util/delay.h>

#include <avr/sleep.h>
#include <avr/interrupt.h>

#define LED0 (1<<PB0)
#define LED1 (1<<PB3)
#define LED2 (1<<PB4)

char state = 1;

ISR(WDT_vect) {
}

ISR(PCINT0_vect)
{
    PCMSK &= ~(1<<2);
    state = 0;
}

void sleep(void)
{
    sleep_mode();
    if (state == 0) {
        int old = PORTB;

        WDTCR &= ~(1<<WDTIE);
        PORTB &= ~(LED0 | LED1 | LED2);

        _delay_ms(500);
        PCMSK |= (1<<2);

        sleep_mode();

        PORTB = old;

        state = 1;
        _delay_ms(500);
        PCMSK |= (1<<2);

        WDTCR |= (1<<WDTIE);
    }
    // if (PINB & (1<<PB2) == 0) {
    //     WDTCR &= ~(1<<WDTIE);
    //     PORTB = 0;
    //     for (;;) {}
    // }
}

int main(void) {
    MCUSR = 0;
    DDRB |= LED0 | LED1 | LED2;

    DDRB &= ~(1<<PB2);

    PORTB ^= LED0; /* LED on */

    WDTCR |= (1<<WDP2) | (1<<WDP1);  /* 128K */
    WDTCR |= (1<<WDTIE);

    PORTB |= (1<<PB2); /* pull-up */
    GIMSK |= (1<<PCIE);
    PCMSK = (1<<2);

    sei();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    for(;;) {
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
