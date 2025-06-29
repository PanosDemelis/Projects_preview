
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <stdio.h>
#include "SolarTracker.h"


uint16_t a=0,b=0,c=0,f=1,j=0,k=0,i=0,g=0,h=0,l=0,m,o=0,x=0,
y=0,z=1,t=0,sch =0,manual=1, count=0, dir=0;

char cnt[4], sd[4],rev[4],step[2],hold[4],move[2],test0[2],test1[2];;

/////////////////////////////Forward Movenments Function///////////////////////////////////

void forward(void) {

    

    if(bit_is_set(PIND,7))                            // Day Loop sensing Daylight form photoresistor
                   {

                         LCD_Control(0x01);
                         _delay_ms(2);
                         LCD_Print("      DAY      ");      // Declaring Day Subroutine
                         _delay_ms(1500);
                         wdt_reset();
                         LCD_Control(0x01);
                         _delay_ms(2);

                         k=0;
                         while(f<=(h))                             //  Step while loop counting
                              {

                                  
                                  
                                         PORTD ^= 1<<0;                            //turning LED1 on "blinking process"


                                           if(j<(l-m))                             // Hold Counting
                                             {
                                              LCD_Control(0x01);                // Clearing Display
                                             _delay_ms(2);
                                              LCD_Print("STEP");               // Send Strings and cursor position commands
                                              LCD_Control(0b10000101);
                                             _delay_us(50);
                                              itoa((f),cnt,10);                  // Using itoa funtction to convert variables int o strings
                                              LCD_Print(cnt);                  // in order to dislay them
                                              LCD_Control(0b10001000);
                                             _delay_us(50);
                                             LCD_Print("HOLD");
                                              itoa((j+1),sd,10);
                                              LCD_Control(0b11000101);
                                             _delay_us(50);
                                              LCD_Print(sd);

                                          wdt_reset();
                                        LCD_Control(0x01);
                                       _delay_ms(2);
                                             }



                                    else if(j >=(l-m) && j<=(l-1))
                                           {
                                               PORTD |= 1<<1;                    //Turning Motor on
                                               PORTD |= 1<<0;

                                                   LCD_Control(0x01);
                                                  _delay_ms(2);
                                                   LCD_Print("STEP");
                                                   LCD_Control(0b10000101);
                                                  _delay_us(50);
                                                   itoa((f),cnt,10);
                                                   LCD_Print(cnt);
                                                   LCD_Control(0b10000111);
                                                  _delay_us(50);
                                                   LCD_Print("MOVE");

                                               itoa((j+1),sd,10);
                                               LCD_Control(0b11000101);
                                               LCD_Print(sd);TCCR1B &= ~(1<<WGM13 | 1<<WGM12| 1<<CS10);  //Turning off timer

                                                     if(j == (l-1))
                                                      {
                                                        PORTD &= ~(1<<1);     // Final count turning Motor off
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

/////////////////////////////Reverse Motor Movement Function///////////////////////////////

void reverse(void) {

     LCD_Control(0x01);
     _delay_ms(2);
     LCD_Print("     NIGHT      ");
     _delay_ms(800);
     wdt_reset();

     LCD_Control(0x01);
     _delay_ms(2);
     LCD_Print("   RETURN   ");
     LCD_Control(0b11000110);
     _delay_us(50);
     itoa((k+1),rev,10);
     LCD_Print(rev);


                        if(k==((h*m)-1))
                          {

                            wdt_reset();
                            TCCR1B &= ~(1<<WGM13 | 1<<WGM12| 1<<CS10);  //Turning off timer
                                                           
                          }
                    
                     wdt_reset();
                    

                  LCD_Control(0x01);
                 _delay_ms(2);
                  LCD_Print("     NIGHT      ");
                   wdt_reset();

                    if(bit_is_set(PIND,7))
                       {

                           f=1;
                           PORTD &= ~(1<<4);
                           wdt_reset();
                           cli();
                           WDTCSR |= 1<<WDE;
                           WDTCSR |= 1<<WDP2 | 1<<WDP1 | 1<<WDP0;
                           TCCR1B |= 1<<WGM13 | 1<<WGM12| 1<<CS10;
                           
                           sei();
                           wdt_reset();
                       }
}

/////////////////////////////INT External Interrupt Routine used for settings/////////////

ISR(INT0_vect) {
  
   wdt_reset();
   settings();

}

//////////////////////////////Timer Interruot Routine//////////////////////////////////////

ISR(TIMER1_OVF_vect)  {

   if (count<=30){
     count++;
     
     if(count==30){

      if(bit_is_set(PIND,7)) {
        if(j<=(l-1)){
          j++;
        }
        else{
          j=0;
        }
      }
      
  else{
       if(k<=((h*m)-1)){
          PORTD |= 1<<5;
          PORTD ^= 1<<4;
          k++;
          if(k==(h*m)-1){
          PORTD &= ~1<<5;
          PORTD &= ~1<<4;
             k=0;
          }
        }
 
  }

   if(g==4){
     
    if(sch==0){

       if (x<=c){
          PORTD |= 1<<1;
          LCD_Control(0b11000111);
          _delay_us(50);
          itoa(x,test1,10);
          LCD_Print(test1);
          wdt_reset();
          x++;
       }
      else if(x==c) {

           PORTD &= ~(1<<1); 
           z++;          
      }
    }
   else if(sch==1){

       if(y<=((a*c)-1)){

               PORTD |= 1<<5;
               PORTD |= 1<<1;
               LCD_Control(0b11000111);
               _delay_us(50);
               itoa((y+1),test1,10);
               LCD_Print(test1);
               wdt_reset();
              y++;
            }
      else if(y==((a*c)-1))
            PORTD &= ~(1<<5);
            PORTD &= ~(1<<1);
         }
  }
    if(g==5){
     if(dir==0){
      manual++;
     }
  else if(dir==1){
     manual--;
   }
  }
  count=0;
} 

}  

}


//////////////////////////////////Settings Function/////////////////////////////////////////

void settings(void) {

     PORTD = 0b00000000;
     PORTA = 0b00000111;
     cli();
     //TCCR1B &= ~(1<<WGM13 | 1<<WGM12| 1<<CS10);  //Turning off timer

     wdt_reset(); 
     

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
                         LCD_Control(0x01);
                       _delay_ms(2);  
                         LCD_Print("  SETTINGS:  ");
                         LCD_Control(0b11000010);
                         _delay_us(50);
                         LCD_Print("WITH BUTTON3");
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
                             LCD_Control(0x01);
                             _delay_ms(2);
                             LCD_Print("    STEPS:  ");
                             _delay_ms(40);
                             itoa(a,step,10);
                             LCD_Control(0b10001011);
                             _delay_us(50);
                             LCD_Print(step);
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
                      LCD_Control(0x01);
                             _delay_ms(2);
                      LCD_Print("HOLD TIME:");
                      _delay_ms(40);
                             itoa(b,hold,10);
                             LCD_Control(0b11000111);
                             _delay_us(50);
                             LCD_Print(hold);

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

                                    if(o==1)
                                      {
                                          b = 30;
                                          eeprom_write_word(&e,b);
                                      }

                                else if(o==2)
                                     {
                                         b = 60;
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
                                         b = 1800;
                                         eeprom_write_word(&e,b);
                                     }
                                else if(o==6)
                                     {
                                         b=3600;
                                         eeprom_write_word(&e,b);
                                     }
                                else if(o==7 || o==0)
                                     {
                                         b=0;
                                     }

                         }

                  else if(g==3)
                         {
                             c = eeprom_read_word(&n);
                         }
                                else if(g==3)
                         {
                             c = eeprom_read_word(&n);
                             LCD_Control(0x01);
                            _delay_ms(2);
                             LCD_Print("MOVEMENT TIME:");
                            _delay_ms(40);
                             itoa(c,move,10);
                             LCD_Control(0b11000111);
                            _delay_us(50);
                             LCD_Print(move);
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

                             LCD_Control(0x01);
                            _delay_ms(2);
                            LCD_Print("STEP TEST:");
                            LCD_Control(0b10001110);

                                _delay_ms(450);
                        
                               wdt_reset();

                               TCCR1B |= 1<<WGM13 | 1<<WGM12| 1<<CS10;   // Turnung timer on
                               

                                 if(bit_is_clear(PINA,1))
                                    {
                                       sch=0;
                                       sei();
                                        if((z)<=a)
                                          {
                                            itoa(z,test0,10);
                                            LCD_Print(test0);
                                            wdt_reset();
                                            
                                          }
                                        else if(z==a){
                                          cli();
                                        }
                                       
                                    }
                              else if(bit_is_clear(PINA ,0))
                                   {
                                       sch=1;
                                       LCD_Control(0x01);
                                      _delay_ms(2);
                                       LCD_Print("RETURN TEST");
                                       LCD_Control(0b10001110);


                                       while(y<=((a*c)-1))
                                          {   
                                              LCD_Control(0b11000111);
                                              _delay_us(50);
                                               itoa((y+1),test1,10);
                                              LCD_Print(test1);
                                              wdt_reset();
                                          }

                                      
                                   }

                        }

                 else if(g==5)
                        {
                             LCD_Control(0x01);
                             _delay_ms(2);
                             LCD_Print("MANUAL");
                             cli();
                             wdt_reset();
                                if(bit_is_clear(PINA,1))
                                  {
                                      sei();
                                      dir=0;
                                      PORTD |= 1<<1;
                                      LCD_Control(0b11000110);
                                      _delay_us(50);
                                      itoa(manual,move,10);
                                      LCD_Print(move);
                                      wdt_reset();
                                      

                                  }

                             else if(bit_is_clear(PINA,0))
                                  {
                                      sei();
                                      dir=1;
                                      PORTD |= 1<<5;
                                      LCD_Control(0b11000110);
                                      _delay_us(50);
                                      itoa(manual,move,10);
                                      LCD_Print(move);
                                       wdt_reset();
                                      
                                  }
                        }

                 else if(g==6)
                        {
                          LCD_Control(0x01);
                             _delay_ms(2);
                            LCD_Print("START STEP");
                            wdt_reset();
                            itoa(f,step,10);
                             LCD_Control(0b11000111);
                            _delay_us(50);
                            LCD_Print(step);

                                             
                                _delay_ms(200);
                               
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
                           LCD_Control(0x01);
                             _delay_ms(2);
                            LCD_Print("FINISH");
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


void Initialisation(void) {
    Data_Read_Write = 0xFF;
    DataDir_Control = 0xFF;
    _delay_ms(50);
   LCD_ASCII = 0b00111100;       // <---------
    Command_Read_Write |= 1<<Enable;    //          |
    _delay_ms(1);                  //          |
    Command_Read_Write &= ~ 1<<Enable;  //          |--Function Set
    _delay_ms(1);                  //          |
    Command_Read_Write |= 1<<Enable;    //          |
    _delay_ms(1);                  //          |
    Command_Read_Write &= ~ 1<<Enable;  // <---------
    LCD_Control(0b00001100);    // Set Display ON
    _delay_ms(1);
    LCD_Control(0b00000001);    // Clear Display
    _delay_ms(2);
    LCD_Control(0b00000110);    // Entry Mode set
    _delay_ms(50);
}



void LCD_Control(unsigned char command)
{
   Data_Read_Write = 0b00000000;
   Command_Read_Write |= 1<< ReadWrite;
   Command_Read_Write &= ~ 1<< RS;

    while(LCD_ASCII >= 0x80)
      {
              Command_Read_Write |= 1<<Enable;
              _delay_us(120);
              Command_Read_Write &= ~ 1<<Enable;
      }

   Data_Read_Write = 0xFF;
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <stdio.h>
#include "SolarTracker.h"


uint16_t a=0,b=0,c=0,f=1,j=0,k=0,i=0,g=0,h=0,l=0,m,o=0,x=0,
y=0,z=1,t=0,sch =0,manual=1, count=0, dir=0;

char cnt[4], sd[4],rev[4],step[2],hold[4],move[2],test0[2],test1[2];;

/////////////////////////////Forward Movenments Function///////////////////////////////////

void forward(void) {

    

    if(bit_is_set(PIND,7))                            // Day Loop sensing Daylight form photoresistor
                   {

                         LCD_Control(0x01);
                         _delay_ms(2);
                         LCD_Print("      DAY      ");      // Declaring Day Subroutine
                         _delay_ms(1500);
                         wdt_reset();
                         LCD_Control(0x01);
                         _delay_ms(2);

                         k=0;
                         while(f<=(h))                             //  Step while loop counting
                              {

                                  
                                  
                                         PORTD ^= 1<<0;                            //turning LED1 on "blinking process"


                                           if(j<(l-m))                             // Hold Counting
                                             {
                                              LCD_Control(0x01);                // Clearing Display
                                             _delay_ms(2);
                                              LCD_Print("STEP");               // Send Strings and cursor position commands
                                              LCD_Control(0b10000101);
                                             _delay_us(50);
                                              itoa((f),cnt,10);                  // Using itoa funtction to convert variables int o strings
                                              LCD_Print(cnt);                  // in order to dislay them
                                              LCD_Control(0b10001000);
                                             _delay_us(50);
                                             LCD_Print("HOLD");
                                              itoa((j+1),sd,10);
                                              LCD_Control(0b11000101);
                                             _delay_us(50);
                                              LCD_Print(sd);

                                          wdt_reset();
                                        LCD_Control(0x01);
                                       _delay_ms(2);
                                             }



                                    else if(j >=(l-m) && j<=(l-1))
                                           {
                                               PORTD |= 1<<1;                    //Turning Motor on
                                               PORTD |= 1<<0;

                                                   LCD_Control(0x01);
                                                  _delay_ms(2);
                                                   LCD_Print("STEP");
                                                   LCD_Control(0b10000101);
                                                  _delay_us(50);
                                                   itoa((f),cnt,10);
                                                   LCD_Print(cnt);
                                                   LCD_Control(0b10000111);
                                                  _delay_us(50);
                                                   LCD_Print("MOVE");

                                               itoa((j+1),sd,10);
                                               LCD_Control(0b11000101);
                                               LCD_Print(sd);TCCR1B &= ~(1<<WGM13 | 1<<WGM12| 1<<CS10);  //Turning off timer

                                                     if(j == (l-1))
                                                      {
                                                        PORTD &= ~(1<<1);     // Final count turning Motor off
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

/////////////////////////////Reverse Motor Movement Function///////////////////////////////

void reverse(void) {

     LCD_Control(0x01);
     _delay_ms(2);
     LCD_Print("     NIGHT      ");
     _delay_ms(800);
     wdt_reset();

     LCD_Control(0x01);
     _delay_ms(2);
     LCD_Print("   RETURN   ");
     LCD_Control(0b11000110);
     _delay_us(50);
     itoa((k+1),rev,10);
     LCD_Print(rev);


                        if(k==((h*m)-1))
                          {

                            wdt_reset();
                            TCCR1B &= ~(1<<WGM13 | 1<<WGM12| 1<<CS10);  //Turning off timer
                                                           
                          }
                    
                     wdt_reset();
                    

                  LCD_Control(0x01);
                 _delay_ms(2);
                  LCD_Print("     NIGHT      ");
                   wdt_reset();

                    if(bit_is_set(PIND,7))
                       {

                           f=1;
                           PORTD &= ~(1<<4);
                           wdt_reset();
                           cli();
                           WDTCSR |= 1<<WDE;
                           WDTCSR |= 1<<WDP2 | 1<<WDP1 | 1<<WDP0;
                           TCCR1B |= 1<<WGM13 | 1<<WGM12| 1<<CS10;
                           
                           sei();
                           wdt_reset();
                       }
}

/////////////////////////////INT External Interrupt Routine used for settings/////////////

ISR(INT0_vect) {
  
   wdt_reset();
   settings();

}

//////////////////////////////Timer Interruot Routine//////////////////////////////////////

ISR(TIMER1_OVF_vect)  {

   if (count<=30){
     count++;
     
     if(count==30){

      if(bit_is_set(PIND,7)) {
        if(j<=(l-1)){
          j++;
        }
        else{
          j=0;
        }
      }
      
  else{
       if(k<=((h*m)-1)){
          PORTD |= 1<<5;
          PORTD ^= 1<<4;
          k++;
          if(k==(h*m)-1){
          PORTD &= ~1<<5;
          PORTD &= ~1<<4;
             k=0;
          }
        }
 
  }

   if(g==4){
     
    if(sch==0){

       if (x<=c){
          PORTD |= 1<<1;
          LCD_Control(0b11000111);
          _delay_us(50);
          itoa(x,test1,10);
          LCD_Print(test1);
          wdt_reset();
          x++;
       }
      else if(x==c) {

           PORTD &= ~(1<<1); 
           z++;          
      }
    }
   else if(sch==1){

       if(y<=((a*c)-1)){

               PORTD |= 1<<5;
               PORTD |= 1<<1;
               LCD_Control(0b11000111);
               _delay_us(50);
               itoa((y+1),test1,10);
               LCD_Print(test1);
               wdt_reset();
              y++;
            }
      else if(y==((a*c)-1))
            PORTD &= ~(1<<5);
            PORTD &= ~(1<<1);
         }
  }
    if(g==5){
     if(dir==0){
      manual++;
     }
  else if(dir==1){
     manual--;
   }
  }
  count=0;
} 

}  

}


//////////////////////////////////Settings Function/////////////////////////////////////////

void settings(void) {

     PORTD = 0b00000000;
     PORTA = 0b00000111;
     cli();
     //TCCR1B &= ~(1<<WGM13 | 1<<WGM12| 1<<CS10);  //Turning off timer

     wdt_reset(); 
     

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
                         LCD_Control(0x01);
                       _delay_ms(2);  
                         LCD_Print("  SETTINGS:  ");
                         LCD_Control(0b11000010);
                         _delay_us(50);
                         LCD_Print("WITH BUTTON3");
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
                             LCD_Control(0x01);
                             _delay_ms(2);
                             LCD_Print("    STEPS:  ");
                             _delay_ms(40);
                             itoa(a,step,10);
                             LCD_Control(0b10001011);
                             _delay_us(50);
                             LCD_Print(step);
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
                      LCD_Control(0x01);
                             _delay_ms(2);
                      LCD_Print("HOLD TIME:");
                      _delay_ms(40);
                             itoa(b,hold,10);
                             LCD_Control(0b11000111);
                             _delay_us(50);
                             LCD_Print(hold);

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

                                    if(o==1)
                                      {
                                          b = 30;
                                          eeprom_write_word(&e,b);
                                      }

                                else if(o==2)
                                     {
                                         b = 60;
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
                                         b = 1800;
                                         eeprom_write_word(&e,b);
                                     }
                                else if(o==6)
                                     {
                                         b=3600;
                                         eeprom_write_word(&e,b);
                                     }
                                else if(o==7 || o==0)
                                     {
                                         b=0;
                                     }

                         }

                  else if(g==3)
                         {
                             c = eeprom_read_word(&n);
                         }
                                else if(g==3)
                         {
                             c = eeprom_read_word(&n);
                             LCD_Control(0x01);
                            _delay_ms(2);
                             LCD_Print("MOVEMENT TIME:");
                            _delay_ms(40);
                             itoa(c,move,10);
                             LCD_Control(0b11000111);
                            _delay_us(50);
                             LCD_Print(move);
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

                             LCD_Control(0x01);
                            _delay_ms(2);
                            LCD_Print("STEP TEST:");
                            LCD_Control(0b10001110);

                                _delay_ms(450);
                        
                               wdt_reset();

                               TCCR1B |= 1<<WGM13 | 1<<WGM12| 1<<CS10;   // Turnung timer on
                               

                                 if(bit_is_clear(PINA,1))
                                    {
                                       sch=0;
                                       sei();
                                        if((z)<=a)
                                          {
                                            itoa(z,test0,10);
                                            LCD_Print(test0);
                                            wdt_reset();
                                            
                                          }
                                        else if(z==a){
                                          cli();
                                        }
                                       
                                    }
                              else if(bit_is_clear(PINA ,0))
                                   {
                                       sch=1;
                                       LCD_Control(0x01);
                                      _delay_ms(2);
                                       LCD_Print("RETURN TEST");
                                       LCD_Control(0b10001110);


                                       while(y<=((a*c)-1))
                                          {   
                                              LCD_Control(0b11000111);
                                              _delay_us(50);
                                               itoa((y+1),test1,10);
                                              LCD_Print(test1);
                                              wdt_reset();
                                          }

                                      
                                   }

                        }

                 else if(g==5)
                        {
                             LCD_Control(0x01);
                             _delay_ms(2);
                             LCD_Print("MANUAL");
                             cli();
                             wdt_reset();
                                if(bit_is_clear(PINA,1))
                                  {
                                      sei();
                                      dir=0;
                                      PORTD |= 1<<1;
                                      LCD_Control(0b11000110);
                                      _delay_us(50);
                                      itoa(manual,move,10);
                                      LCD_Print(move);
                                      wdt_reset();
                                      

                                  }

                             else if(bit_is_clear(PINA,0))
                                  {
                                      sei();
                                      dir=1;
                                      PORTD |= 1<<5;
                                      LCD_Control(0b11000110);
                                      _delay_us(50);
                                      itoa(manual,move,10);
                                      LCD_Print(move);
                                       wdt_reset();
                                      
                                  }
                        }

                 else if(g==6)
                        {
                          LCD_Control(0x01);
                             _delay_ms(2);
                            LCD_Print("START STEP");
                            wdt_reset();
                            itoa(f,step,10);
                             LCD_Control(0b11000111);
                            _delay_us(50);
                            LCD_Print(step);

                                             
                                _delay_ms(200);
                               
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


/////////////////////////////Forward Movenments Function///////////////////////////////////

void forward(void) {

    

    if(bit_is_set(PIND,7))                            // Day Loop sensing Daylight form photoresistor
                   {

                         LCD_Control(0x01);
                         _delay_ms(2);
                         LCD_Print("      DAY      ");      // Declaring Day Subroutine
                         _delay_ms(1500);
                         wdt_reset();
                         LCD_Control(0x01);
                         _delay_ms(2);

                         k=0;
                         while(f<=(h))                             //  Step while loop counting
                              {

                                  
                                  
                                         PORTD ^= 1<<0;                            //turning LED1 on "blinking process"


                                           if(j<(l-m))                             // Hold Counting
                                             {
                          

                                          wdt_reset();
                                        LCD_Control(0x01);
                                       _delay_ms(2);
                                             }



                                    else if(j >=(l-m) && j<=(l-1))
                                           {
                                               PORTD |= 1<<1;                    //Turning Motor on
                                               PORTD |= 1<<0;

                                     
                                               LCD_Print(sd);TCCR1B &= ~(1<<WGM13 | 1<<WGM12| 1<<CS10);  //Turning off timer

                                                     if(j == (l-1))
                                                      {
                                                        PORTD &= ~(1<<1);     // Final count turning Motor off
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

/////////////////////////////Reverse Motor Movement Function///////////////////////////////

void reverse(void) {




                        if(k==((h*m)-1))
                          {

                            wdt_reset();
                            TCCR1B &= ~(1<<WGM13 | 1<<WGM12| 1<<CS10);  //Turning off timer
                                                           
                          }
                    
                     wdt_reset();
                    

                  LCD_Control(0x01);
                 _delay_ms(2);
                  LCD_Print("     NIGHT      ");
                   wdt_reset();

                    if(bit_is_set(PIND,7))
                       {

                           f=1;
                           PORTD &= ~(1<<4);
                           wdt_reset();
                           cli();
                           WDTCSR |= 1<<WDE;
                           WDTCSR |= 1<<WDP2 | 1<<WDP1 | 1<<WDP0;
                           TCCR1B |= 1<<WGM13 | 1<<WGM12| 1<<CS10;
                           
                           sei();
                           wdt_reset();
                       }
}

/////////////////////////////INT External Interrupt Routine used for settings/////////////

ISR(INT0_vect) {
  
   wdt_reset();
   settings();

}

//////////////////////////////Timer Interruot Routine//////////////////////////////////////

ISR(TIMER1_OVF_vect)  {

   if (count<=30){
     count++;
     
     if(count==30){

      if(bit_is_set(PIND,7)) {
        if(j<=(l-1)){
          j++;
        }
        else{
          j=0;
        }
      }
      
  else{
       if(k<=((h*m)-1)){
          PORTD |= 1<<5;
          PORTD ^= 1<<4;
          k++;
          if(k==(h*m)-1){
          PORTD &= ~1<<5;
          PORTD &= ~1<<4;
             k=0;
          }
        }
 
  }

   if(g==4){
     
    if(sch==0){

       if (x<=c){
          PORTD |= 1<<1;
     
      else if(x==c) {

           PORTD &= ~(1<<1); 
           z++;          
      }
    }
   else if(sch==1){

       if(y<=((a*c)-1)){

               PORTD |= 1<<5;
               PORTD |= 1<<1;
   
              y++;
            }
      else if(y==((a*c)-1))
            PORTD &= ~(1<<5);
            PORTD &= ~(1<<1);
         }
  }
    if(g==5){
     if(dir==0){
      manual++;
     }
  else if(dir==1){
     manual--;
   }
  }
  count=0;
} 

}  

}


//////////////////////////////////Settings Function/////////////////////////////////////////

void settings(void) {

     PORTD = 0b00000000;
     PORTA = 0b00000111;
     cli();
     //TCCR1B &= ~(1<<WGM13 | 1<<WGM12| 1<<CS10);  //Turning off timer

     wdt_reset(); 
     

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
                         LCD_Control(0x01);
                       _delay_ms(2);  
                         LCD_Print("  SETTINGS:  ");
                         LCD_Control(0b11000010);
                         _delay_us(50);
                         LCD_Print("WITH BUTTON3");
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
                             LCD_Control(0x01);
                             _delay_ms(2);
                             LCD_Print("    STEPS:  ");
                             _delay_ms(40);
                             itoa(a,step,10);
                             LCD_Control(0b10001011);
                             _delay_us(50);
                             LCD_Print(step);
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
                      LCD_Control(0x01);
                             _delay_ms(2);
                      LCD_Print("HOLD TIME:");
                      _delay_ms(40);
                             itoa(b,hold,10);
                             LCD_Control(0b11000111);
                             _delay_us(50);
                             LCD_Print(hold);

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

                                    if(o==1)
                                      {
                                          b = 30;
                                          eeprom_write_word(&e,b);
                                      }

                                else if(o==2)
                                     {
                                         b = 60;
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
                                         b = 1800;
                                         eeprom_write_word(&e,b);
                                     }
                                else if(o==6)
                                     {
                                         b=3600;
                                         eeprom_write_word(&e,b);
                                     }
                                else if(o==7 || o==0)
                                     {
                                         b=0;
                                     }

                         }

                  else if(g==3)
                         {
                             c = eeprom_read_word(&n);
                         }
                                else if(g==3)
                         {
                             c = eeprom_read_word(&n);
                             LCD_Control(0x01);
                            _delay_ms(2);
                             LCD_Print("MOVEMENT TIME:");
                            _delay_ms(40);
                             itoa(c,move,10);
                             LCD_Control(0b11000111);
                            _delay_us(50);
                             LCD_Print(move);
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

                             LCD_Control(0x01);
                            _delay_ms(2);
                            LCD_Print("STEP TEST:");
                            LCD_Control(0b10001110);

                                _delay_ms(450);
                        
                               wdt_reset();

                               TCCR1B |= 1<<WGM13 | 1<<WGM12| 1<<CS10;   // Turnung timer on
                               

                                 if(bit_is_clear(PINA,1))
                                    {
                                       sch=0;
                                       sei();
                                        if((z)<=a)
                                          {
                                            itoa(z,test0,10);
                                            LCD_Print(test0);
                                            wdt_reset();
                                            
                                          }
                                        else if(z==a){
                                          cli();
                                        }
                                       
                                    }
                              else if(bit_is_clear(PINA ,0))
                                   {
                                       sch=1;
                                       LCD_Control(0x01);
                                      _delay_ms(2);
                                       LCD_Print("RETURN TEST");
                                       LCD_Control(0b10001110);


                                       while(y<=((a*c)-1))
                                          {   
                                              LCD_Control(0b11000111);
                                              _delay_us(50);
                                               itoa((y+1),test1,10);
                                              LCD_Print(test1);
                                              wdt_reset();
                                          }

                                      
                                   }

                        }

                 else if(g==5)
                        {
                             LCD_Control(0x01);
                             _delay_ms(2);
                             LCD_Print("MANUAL");
                             cli();
                             wdt_reset();
                                if(bit_is_clear(PINA,1))
                                  {
                                      sei();
                                      dir=0;
                                     
                                      

                                  }

                             else if(bit_is_clear(PINA,0))
                                  {
                                      sei();
                                      dir=1;
                       
                                  }
                        }

                 else if(g==6)
                        {
                          LCD_Control(0x01);
                             _delay_ms(2);
                            LCD_Print("START STEP");
                            wdt_reset();
                            itoa(f,step,10);
 
                        }


                 else if(g==7)
                        {
 
                            wdt_reset();
                            h=eeprom_read_word(&d);
                            l=eeprom_read_word(&e);
                            m=eeprom_read_word(&n);
                             g=0;

                        }
  }


/////////////////////////////Display Functions/////////////////////////////////////////////


void Initialisation(void) {
    Data_Read_Write = 0xFF;
    DataDir_Control = 0xFF;
    _delay_ms(50);
   LCD_ASCII = 0b00111100;       // <---------
    Command_Read_Write |= 1<<Enable;    //          |
    _delay_ms(1);                  //          |
    Command_Read_Write &= ~ 1<<Enable;  //          |--Function Set
    _delay_ms(1);                  //          |
    Command_Read_Write |= 1<<Enable;    //          |
    _delay_ms(1);                  //          |
    Command_Read_Write &= ~ 1<<Enable;  // <---------
    LCD_Control(0b00001100);    // Set Display ON
    _delay_ms(1);
    LCD_Control(0b00000001);    // Clear Display
    _delay_ms(2);
    LCD_Control(0b00000110);    // Entry Mode set
    _delay_ms(50);
}



void LCD_Control(unsigned char command)
{
   Data_Read_Write = 0b00000000;
   Command_Read_Write |= 1<< ReadWrite;
   Command_Read_Write &= ~ 1<< RS;



}

void Transmit_ASCII(unsigned char ascii)
{
   Data_Read_Write = 0b00000000;
   Command_Read_Write |= 1<< ReadWrite;
   Command_Read_Write &= ~ 1<< RS;

}

void LCD_Print(char *ascii_qeue)
{
    while(*ascii_qeue > 0)
    {
       Transmit_ASCII(*ascii_qeue++);
    }
}

///////////////////////////////////////Welcome Funtion////////////////////////////////////////

void welcome(void)
{
    LCD_Print("Welcome!");
    _delay_ms(500);
    LCD_Control(0b11000101);
    _delay_us(50);
    LCD_Print(":-)");
    _delay_ms(2000);
    LCD_Control(0x01);
    _delay_ms(2);
    LCD_Print("     I'm your");
    LCD_Control(0b11000010);
    _delay_us(50);
    LCD_Print("Solarbot");
    _delay_ms(2000);
    LCD_Control(0x01);
    _delay_ms(2);
    LCD_Print("  Starting photon harvest!");
    LCD_Control(0b11000110);
    _delay_us(50);
    LCD_Print(";-)");
    _delay_ms(2000);
}




