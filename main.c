#include <avr/io.h>
#include <math.h>
#include <inttypes.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define F_CPU 10000000UL
#define pwm1 OCR1A
#define pwm2 OCR1B
#define period 49

int x[51]= {10,43,74,105,136,167,198,228,258,289,319,348,377,406,434,461,489,516,542,568,593,617,641,664,687,709,730,750,770,789,807,824,851,856,871,
885,898,910,921,931,940,948,956,962,967,972,975,978,979,979,979};
int z[51]={3,6,9,12,19,22,25,28,31,34,37,40,43,45,48,51,54,56,59,61,64,66,68,70,73,75,77,79,81,83,84,86,88,89,90,92,93,
94,95,96,97,98,98,99,99,99,99,99,99,99};

uint16_t EEMEM h;
uint16_t a,d,e,f,g,i;
int c;

int main(void)
{

    DDRB   |= 1<<1 | 1<<2;                                                   /** Setting pwm outputs in portB*/
    DDRD   |= 0b00010000;
    PORTD  |= 1<<7 | 1<<6 | 1<<4;
    TCCR1A |= 1<<COM1A1 | 1<<COM1A0 | 1<<COM1B1 | 1<<COM1B0 | 1<<WGM11;      /** Setting inverting pwm for both outpus OC1A and OC1B*/
    TCCR1B |= 1<<WGM13 | 1<<WGM12| 1<<CS10;                                  /** Setting ICR1 register as the TOP value register and activating Counter1
                                                                              with no prescale*/

    ICR1=999;                                                               /** Setting top count value pwm frequency at 10kHz*/
    c=0;
    pwm1 = 0;                                                               /** setting both compare registers initial value to 0*/
    pwm2 = 0;
    a=0;
    d=0;
    f=0;
    e=0;
    WDTCSR  |= 1<<WDE;                                /** Enabling watchdog timer*/
    WDTCSR  |= 1<<WDP0;
    TIMSK1 |= 1<<TOIE1;                             /** Enabling overflow interrupt flag*/
    sei();                                          /** Enabling global interrupts*/
    wdt_reset();

    while(1)
    {
       if(bit_is_set(PIND,6))
            {
               if(c==-300)
                  {
                   c=-300;
                  }
              else
                 {
                  for(i=0; i<=51; i++)
                     {
                         if(i==51)
                            {
                              c--;
                            }
                         else
                           {
                            c=c;
                           }
                        wdt_reset();
                     }
                }
                i=0;
            }

      else if(bit_is_set(PIND,7))
            {
               if (c==1000)
                 {
                   c=1000;
                 }
            else
                {
                   for(i=0; i<=51; i++)
                     {
                         if(i==51)
                            {
                              c++;
                            }
                         else
                           {
                            c=c;
                           }
                        wdt_reset();
                     }
                 }
                   i=0;

            }
      else
        {
          c=c;
          i=0;
        }
    }

}

ISR(TIMER1_OVF_vect)                               /**interrupt routine when TOV flag is set*/
{

                                    /**clearing pinD5 after int0 interrupt routine is cpmplete*/

                                                   /** a is incremented form 1 to 200 cycles of 10kHZ results to 50Hz overall signal*/
    if(a==0)                                         /** for the first 50 pulses with have the rise of halfwave*/
       {

            pwm2=0;
              if(c+x[d]<=0)
                  {
                    pwm1=0;
                  }
            else
                  {
                    pwm1=c+x[d];
                  }
           d++;

            if(d==50)
               {
                 a=1;
               }
       }

    else if(a==1)                                  /*for pulse 50 to pulse 100 with have the fall of halfwave*/
        {
          if(c+x[d-e]<=0)
             {
               pwm1=0;
             }
          else
              {
                pwm1=c+x[d-e];
              }
            e++;
             if(e==50)
               {
                 a=2;
                 f=0;
                 g=0;

               }
        }

    else if(a==2)             /* until 150th pulse second halfwave rises*/
        {

          pwm1=0;
          if(c+x[f]<=0)
             {
               pwm2=0;
             }
        else
             {
               pwm2=c+x[f];
             }
            f++;
             if(f==50)
               {
                 a=3;

               }
        }

    else if(a==3)            /*after 150th pulse the second halfwave falls*/
        {
          if(c+x[f-g]<=0)
            {
              pwm2=0;
            }
        else
            {
              pwm2=c+x[f-g];
            }

           g++;
             if(g==50)
               {
                 a=0;
                 d=0;
                 e=0;
                 PORTD &= ~1<<2;
               }
        }
  wdt_reset();
}
