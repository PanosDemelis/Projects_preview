#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/sleep.h>

#define MrLCDsCrib      PORTC
#define DataDir_Crib    DDRC
#define MrLCDsControl   PORTB
#define DataDir_Control DDRB
#define Enable          2
#define ReadWrite       1
#define RS              0

void Initialisation(void);
void Check_if_busy(void);
void Peek_A_Boo(void);
void Send_A_Command(unsigned char command);
void Send_A_Character(unsigned char character);
void Send_A_String(char *characters);
void settings(void);
void welcome(void);
void reverse(void);

uint16_t EEMEM d;
uint16_t EEMEM e;
uint16_t EEMEM n;
uint16_t a=0,b=0,c=0,f=1,j=0,k=0,i=0,g=0,h=0,l=0,m=0,o=0,y=0,z=1,t,manual=1;
char rev[4];



int main(void)
{


    DDRD          |= 0b00110011;                             // Assigning portD pin directions
    PORTD         &= ~(1<<0) & ~(1<<1) & ~(1<<4) & ~(1<<5);  // Turning output pins 'Low'

    MCUCSR        |=  1<<JTD;                                // Disabling Jtag chain
    MCUCSR        |=  1<<JTD;                                // Disabling Jtag chain
    GICR          |=  1<<INT0;                               // Enabling external interrupt INT0

    sei();                                                   // Setting Global interrupt pin I in SREG


    char cnt[2],std[2];

     h=eeprom_read_word(&d);                                  //loading Data from eeprom to Register variables
     l=eeprom_read_word(&e);
     m=eeprom_read_word(&n);

     Initialisation();                                        // Initializing Display
     welcome();                                               // Welcoming User
     cli();
     WDTCR |= 1<<WDE;
     WDTCR |= 1<<WDP2 | 1<<WDP1 | 1<<WDP0;
     sei();
     _delay_ms(100);
    wdt_reset();

    while(1)
    {


                if(bit_is_set(PIND,7))                            // Day Loop sensing Daylight form photoresistor
                   {

                         Send_A_Command(0x01);
                         _delay_ms(2);
                         Send_A_String("      MEPA      ");      // Declaring Day Subroutine
                         _delay_ms(1500);
                         wdt_reset();
                         Send_A_Command(0x01);
                         _delay_ms(2);

                         k=0;
                         while(f<=(h))                             //  Step while loop counting
                              {

                                  for(j=0; j<=(l-1); j++)
                                     {
                                         PORTD |= 1<<0;                            //turning LED1 on "blinking process"


                                           if(j<(l-m))                             // Hold Counting
                                             {
                                              Send_A_Command(0x01);                // Clearing Display
                                             _delay_ms(2);
                                              Send_A_String("BHMA");               // Send Strings and cursor position commands
                                              Send_A_Command(0b10000101);
                                             _delay_us(50);
                                              itoa((f),cnt,10);                  // Using itoa function to convert variables int o strings
                                              Send_A_String(cnt);                  // in order to display them
                                              Send_A_Command(0b10001000);
                                             _delay_us(50);
                                             Send_A_String("ANAMONH");
                                              itoa((j+1),std,10);
                                              Send_A_Command(0b11000101);
                                             _delay_us(50);
                                              Send_A_String(std);
                                              for(t=1;t<=480;t++)
                                                 {
                                                   _delay_ms(1);
                                                 }
                                                 PORTD &= ~(1<<0);                 // turning LED1 off "blinking process
                                              for(t=1;t<=499;t++)
                                                 {
                                                   _delay_ms(1);
                                                 }
                                          wdt_reset();
                                        Send_A_Command(0x01);
                                       _delay_ms(2);
                                             }



                                    else if(j >=(l-m) && j<=(l-1))
                                           {
                                               PORTD |= 1<<1;                    //Turning Motor on
                                               PORTD |= 1<<0;

                                                   Send_A_Command(0x01);
                                                  _delay_ms(2);
                                                   Send_A_String("BHMA");
                                                   Send_A_Command(0b10000101);
                                                  _delay_us(50);
                                                   itoa((f),cnt,10);
                                                   Send_A_String(cnt);
                                                   Send_A_Command(0b10000111);
                                                  _delay_us(50);
                                                   Send_A_String("KINHSH");

                                               itoa((j+1),std,10);
                                               Send_A_Command(0b11000101);
                                               Send_A_String(std);
                                               wdt_reset();
                                              for(t=1;t<=480;t++)
                                                 {
                                                   _delay_ms(1);
                                                 }
                                               PORTD &= ~(1<<0);
                                              for(t=1;t<=499;t++)
                                                 {
                                                   _delay_ms(1);
                                                 }

                                                     if(j == (l-1))
                                                      {
                                                        PORTD &= ~(1<<1);     // Final count turning Motor off
                                                      }

                                            }

                                       }
                                    wdt_reset();
                                   f++;
                                 }

                               }
                          else
                            {
                              reverse();
                            }

    }

}

/////////////////////////////Reverse Motor Movement Function///////////////////////////////

void reverse(void)
{
     Send_A_Command(0x01);
     _delay_ms(2);
     Send_A_String("     NYXTA      ");
     _delay_ms(800);
     wdt_reset();

                    while(k<=((h*m)-1))
                    {
                     PORTD |= 1<<5;
                     PORTD |= 1<<4;
                     wdt_reset();
                     Send_A_Command(0x01);
                     _delay_ms(2);
                     Send_A_String("   EPISTROFH");
                    for(t=1;t<=499;t++)
                       {
                         _delay_ms(1);
                       }
                     PORTD &= ~(1<<4);
                     Send_A_Command(0b11000110);
                    _delay_us(50);
                    itoa((k+1),rev,10);
                    Send_A_String(rev);
                    for(t=1;t<=498;t++)
                       {
                        _delay_ms(1);
                       }
                        if(k==((h*m)-1))
                          {
                            PORTD &= ~(1<<5);
                            PORTD |= 1<<4;
                            wdt_reset();

                          }
                     k++;
                     wdt_reset();
                    }

                  Send_A_Command(0x01);
                 _delay_ms(2);
                  Send_A_String("     NYXTA      ");
                   wdt_reset();

                    if(bit_is_set(PIND,7))
                       {

                           f=1;
                           PORTD &= ~(1<<4);
                           wdt_reset();
                           cli();
                           WDTCR |= 1<<WDE;
                           WDTCR |= 1<<WDP2 | 1<<WDP1 | 1<<WDP0;
                           sei();
                           wdt_reset();
                       }
}

/////////////////////////////INT2 External Interrupt Routine used for settings/////////////

ISR(INT0_vect)
{
   wdt_reset();
   settings();

}


//////////////////////////////////Settings Function/////////////////////////////////////////

void settings(void)
{
     PORTD = 0b00000000;
     PORTA = 0b00000111;


     wdt_reset();

     uint16_t x;
     char step[2],hold[4],move[2],test0[2],test1[2];

     if(bit_is_clear(PINA,2))
                   {
                       g++;

                   }
   else
       {
         g=g;
       }

                   if(g==0)
                      {
                         Send_A_Command(0x01);
                        _delay_ms(2);
                         Send_A_String("  RYTHMISEIS:");
                         Send_A_Command(0b11000010);
                         _delay_us(50);
                         Send_A_String("ME TO Button3");
                         wdt_reset();
                        for(t=1;t<=800;t++)
                           {
                            _delay_ms(1);
                           }
                        wdt_reset();

                       }

                  else if(g==1)
                         {

                             a = eeprom_read_word(&d);
                             Send_A_Command(0x01);
                             _delay_ms(2);
                             Send_A_String("    BHMATA:");
                             _delay_ms(40);
                             itoa(a,step,10);
                             Send_A_Command(0b10001011);
                             _delay_us(50);
                             Send_A_String(step);
                             wdt_reset();
                             for(t=1;t<=300;t++)
                                {
                                  _delay_ms(1);
                                }

                                wdt_reset();

                                      if(bit_is_clear(PINA,1))
                                         {
                                             a++;
                                             eeprom_write_word(&d,a);
                                         }
                                    else if(bit_is_clear(PINA,0))
                                       {
                                         a--;
                                         eeprom_write_word(&d,a);
                                       }



                         }



                  else if(g==2)
                        {
                          b = eeprom_read_word(&e);
                      Send_A_Command(0x01);
                             _delay_ms(2);
                      Send_A_String("XRONOS ANAMONHS:");
                      _delay_ms(40);
                             itoa(b,hold,10);
                             Send_A_Command(0b11000111);
                             _delay_us(50);
                             Send_A_String(hold);

                      for(t=1;t<=300;t++)
                           {
                            _delay_ms(1);
                           }

                            wdt_reset();

                                      if(bit_is_clear(PINA,1))
                                         {
                                             o++;

                                         }
                                    else if(bit_is_clear(PINA,0))
                                       {
                                            o--;

                                       }

                                    if(o==0)
                                      {
                                          b = 30;
                                          eeprom_write_word(&e,b);
                                      }

                                else if(o==1)
                                     {
                                         b = 60;
                                         eeprom_write_word(&e,b);
                                     }
                                else if(o==2)
                                     {
                                         b = 600;
                                         eeprom_write_word(&e,b);
                                     }
                                else if(o==3)
                                     {
                                         b = 900;
                                         eeprom_write_word(&e,b);
                                     }
                                else if(o==4)
                                     {
                                         b = 1200;
                                         eeprom_write_word(&e,b);
                                     }
                                else if(o==5)
                                     {
                                           b=1800;
                                         eeprom_write_word(&e,b);
                                     }
                                else if(o==6)
                                     {
                                           b=3600;
                                           eeprom_write_word(&e,b);
                                     }


                         }

                  else if(g==3)
                         {
                             c = eeprom_read_word(&n);
                             Send_A_Command(0x01);
                            _delay_ms(2);
                             Send_A_String("XRONOS KINHSHS:");
                            _delay_ms(40);
                             itoa(c,move,10);
                             Send_A_Command(0b11000111);
                            _delay_us(50);
                             Send_A_String(move);
                            for(t=1;t<=300;t++)
                               {
                                _delay_ms(1);
                               }

                               wdt_reset();

                                      if(bit_is_clear(PINA,1))
                                         {
                                             c++;
                                             eeprom_write_word(&n,c);
                                         }
                                    else if(bit_is_clear(PINA,0))
                                       {
                                         c--;
                                         eeprom_write_word(&n,c);
                                       }

                         }

                 else if(g==4)
                        {
                             PORTA ^= PINA1;

                             Send_A_Command(0x01);
                            _delay_ms(2);
                            Send_A_String("DOKIMH  BHMA:");
                            Send_A_Command(0b10001110);
                            for(t=1;t<=450;t++)
                               {
                                _delay_ms(1);
                               }

                               wdt_reset();

                                 if(bit_is_clear(PINA,1))
                                    {

                                        if((z)<=a)
                                          {

                                            itoa(z,test0,10);
                                            Send_A_String(test0);

                                             for(x=1; x<=(c); x++)
                                                {
                                                  PORTD |= 1<<1;
                                                  Send_A_Command(0b11000111);
                                                  _delay_us(50);
                                                  itoa(x,test1,10);
                                                  Send_A_String(test1);
                                                  wdt_reset();
                                                for(t=1;t<=980;t++)
                                                   {
                                                    _delay_ms(1);
                                                   }

                                                }
                                          }
                                      PORTD &= ~(1<<1);
                                       z++;
                                    }
                              else if(bit_is_clear(PINA ,0))
                                   {
                                       Send_A_Command(0x01);
                                      _delay_ms(2);
                                       Send_A_String("DOKIMH EPISTROFH");
                                       Send_A_Command(0b10001110);
                                      for(t=1;t<=499;t++)
                                         {
                                          _delay_ms(1);
                                         }

                                       while(y<=((a*c)-1))
                                          {

                                              PORTD |= 1<<5;
                                              Send_A_Command(0b11000111);
                                              _delay_us(50);
                                               itoa((y+1),test1,10);
                                              Send_A_String(test1);
                                              wdt_reset();
                                              for(t=1;t<=480;t++)
                                                 {
                                                  _delay_ms(1);
                                                 }

                                                wdt_reset();
                                              y++;
                                          }

                                      PORTD &= ~(1<<5);
                                   }

                        }

                 else if(g==5)
                        {
                             Send_A_Command(0x01);
                             _delay_ms(2);
                             Send_A_String("  XEIROKINHTO");
                             for(t=1; t<=598; t++)
                                {
                                  _delay_ms(1);
                                }
                             wdt_reset();
                                if(bit_is_clear(PINA,1))
                                  {
                                      PORTD |= 1<<1;
                                      Send_A_Command(0b11000110);
                                      _delay_us(50);
                                      itoa(manual,move,10);
                                      Send_A_String(move);
                                      for(t=1; t<=400; t++)
                                         {
                                             _delay_ms(1);
                                         }
                                      wdt_reset();
                                      manual++;

                                  }

                             else if(bit_is_clear(PINA,0))
                                  {
                                      PORTD |= 1<<5;
                                      Send_A_Command(0b11000110);
                                      _delay_us(50);
                                      itoa(manual,move,10);
                                      Send_A_String(move);
                                      for(t=1; t<=400; t++)
                                         {
                                             _delay_ms(1);
                                         }
                                       wdt_reset();
                                      manual--;
                                  }
                        }

                 else if(g==6)
                        {
                          Send_A_Command(0x01);
                             _delay_ms(2);
                            Send_A_String(" BHMA EKKINHSHS");
                            wdt_reset();
                            itoa(f,step,10);
                             Send_A_Command(0b11000111);
                            _delay_us(50);
                            Send_A_String(step);

                            for(t=1;t<=400;t++)
                               {
                                _delay_ms(1);
                               }
                            wdt_reset();

                              if(bit_is_clear(PINA,1))
                                {
                                     f++;
                                     wdt_reset();
                                }

                          else if(bit_is_clear(PINA,0))
                                {
                                    f--;
                                    wdt_reset();
                                }


                        }


                 else if(g==7)
                        {
                           Send_A_Command(0x01);
                             _delay_ms(2);
                            Send_A_String("     TELOS");
                            wdt_reset();
                            for(t=1;t<=1000;t++)
                               {
                                _delay_ms(1);
                               }
                            wdt_reset();
                            h=eeprom_read_word(&d);
                            l=eeprom_read_word(&e);
                            m=eeprom_read_word(&n);
                             g=0;

                        }



}

/////////////////////////////Display Functions/////////////////////////////////////////////

void Check_if_busy()
{
   DataDir_Crib = 0b00000000;
   MrLCDsControl |= 1<< ReadWrite;
   MrLCDsControl &= ~ 1<< RS;

    while(MrLCDsCrib >= 0x80)
      {
          Peek_A_Boo();
      }

   DataDir_Crib = 0xFF;
}

void Initialisation(void)
{
    DataDir_Crib = 0xFF;
    DataDir_Control = 0xFF;
    _delay_ms(50);
    MrLCDsCrib = 0b00111100;       // <---------
    MrLCDsControl |= 1<<Enable;    //          |
    _delay_ms(1);                  //          |
    MrLCDsControl &= ~ 1<<Enable;  //          |--Function Set
    _delay_ms(1);                  //          |
    MrLCDsControl |= 1<<Enable;    //          |
    _delay_ms(1);                  //          |
    MrLCDsControl &= ~ 1<<Enable;  // <---------
    Send_A_Command(0b00001100);    // Set Display ON
    _delay_ms(1);
    Send_A_Command(0b00000001);    // Clear Display
    _delay_ms(2);
    Send_A_Command(0b00000110);    // Entry Mode set
    _delay_ms(50);
}

void Peek_A_Boo()
{
    MrLCDsControl |= 1<<Enable;
    asm volatile("nop");   // assembler command and instruction "nop" that gives a delay to the lcd LightSwitch"Enable"
    asm volatile("nop");
    MrLCDsControl &= ~ 1<<Enable;

}

void Send_A_Command(unsigned char command)
{
   Check_if_busy();
   MrLCDsCrib = command;
   MrLCDsControl &= ~ (1<<ReadWrite | 1<<RS);
   Peek_A_Boo();
   MrLCDsCrib = 0;
}

void Send_A_Character(unsigned char character)
{
   Check_if_busy();
   MrLCDsCrib = character;
   MrLCDsControl &= ~ (1<<ReadWrite);
   MrLCDsControl |= 1<<RS;
   Peek_A_Boo();
   MrLCDsCrib = 0;
}

void Send_A_String(char *characters)
{
    while(*characters > 0)
    {
       Send_A_Character(*characters++);
    }
}

///////////////////////////////////////Welcome Funtion////////////////////////////////////////

void welcome(void)
{
    Send_A_String("GEIA SOU VASILH!");
    _delay_ms(500);
    Send_A_Command(0b11000101);
    _delay_us(50);
    Send_A_String(":-)");
    _delay_ms(2000);
    Send_A_Command(0x01);
    _delay_ms(2);
    Send_A_String("     EIMAI TO");
    Send_A_Command(0b11000010);
    _delay_us(50);
    Send_A_String("SOLARBOT SOU");
    _delay_ms(2000);
    Send_A_Command(0x01);
    _delay_ms(2);
    Send_A_String("  KSEKINAW ETSI?");
    Send_A_Command(0b11000110);
    _delay_us(50);
    Send_A_String(";-)");
    _delay_ms(2000);
}

