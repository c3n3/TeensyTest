//
// TeensyTest
//
// 
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author 		William Lies
// 				William Lies
//
// Date			10/7/17 10:49 PM
// Version		<#version#>
//
// Copyright	© William Lies, 2017
// Licence		<#licence#>
//
// See         ReadMe.txt for references
//

#include "SPI.h"
#include <TLC_lib.h>
#include "MUX_lib.h"

//keyboard library
//#include <PS2Keyboard.h>
// Core library for code-sense - IDE-based
#if defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(ESP8266) // ESP8266 specific
#include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.8 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif // end IDE


void set_led( uint8_t x, uint8_t y, uint8_t z, uint16_t r, uint16_t g, uint16_t b);
void drawBall( int x, int y, int z, uint16_t color);
void bounceBall(int iterations);


volatile uint8_t     gs_buf[NUM_BYTES];  //Buffer written to TLCs over SPI (12 bit color values)
volatile uint16_t    px_buf[NUM_LEDS];   //Pixel buffer storing each LED color as a 16 bit value ( RRRRRGGGGGGBBBBB )

int animation = 0;
uint8_t x;
uint8_t y;
uint8_t z;
uint16_t blue = 0x000F;
int count = 0;
void setup()
{
    
    delay(200);
    mux_init();
    init_TLC();
    memset((uint8_t *)gs_buf, 0xFF, NUM_BYTES);
    delay(100);
    write_dot_correction( (uint8_t *)gs_buf );
    memset((uint8_t *)gs_buf, 0x00, NUM_BYTES);
    memset((uint16_t *)px_buf, 0x0000, NUM_LEDS * 2);
    mux_begin();
    delay(200);
    
    pinMode(23, OUTPUT);
    digitalWrite( 23, HIGH);


}

//Coordinates and RGB values and buffer. RGB range 0-255
void set_led( uint8_t x, uint8_t y, uint8_t z, uint16_t r, uint16_t g, uint16_t b){
    px_buf[ z * NUM_LEDS_LYR + NUM_LEDS_DIM * x + y] = ((r & 0x00F8) << 8 ) |
    ((g & 0x00FC) << 3 ) |
    ((b & 0x00F8) >> 3 );
}
uint16_t pk_color(uint16_t r, uint16_t g, uint16_t b) {
    return ((r & 0x00F8) << 8 ) |
    ((g & 0x00FC) << 3 ) |
    ((b & 0x00F8) >> 3 );
}

//Set led with packed color value
void set_led_pk( uint8_t x, uint8_t y, uint8_t z, uint16_t c){
    px_buf[ z * NUM_LEDS_LYR + NUM_LEDS_DIM * x + y] = c;
}
 //use color = to 1 to make this function shift with the already displayed color
 void shiftRow(int direction, int firstCord, int secondCord, uint16_t color, boolean collective) {
 

 
     if (direction == 0) {
     set_led_pk(firstCord, 11, secondCord, 0);
     for (int i = 11; i >= 1; i--){
         if (px_buf[ secondCord * NUM_LEDS_LYR + NUM_LEDS_DIM * firstCord + i] != 0) {
             set_led_pk(firstCord, i + 1, secondCord, 0x000F);
             set_led_pk(firstCord, i, secondCord, 0);
         }
     }
     }
     if (direction == 1){
     set_led_pk(firstCord, secondCord, 11, 0);
     for (int i = 11; i >= 1; i--){
         y = i;
         if (px_buf[ i * NUM_LEDS_LYR + NUM_LEDS_DIM * firstCord + secondCord] != 0) {
             set_led_pk(firstCord, secondCord, i + 1, 0x000F);
             set_led_pk(firstCord, secondCord, i, 0);
         }
     }
 
     }
 }
 



// Add loop code
void loop()
{
    
    switch(5){
    case 0:
        for(int h=0; h<12; h++){
            for(int i=0; i<200; i++){
                for(int j=0; j<12; j++){
                    for(int k=0; k<12; k++){
                        set_led( j,h,k,i,i,i);
                    }
                }
                
            }
            memset((uint16_t *)px_buf, 0x0000, NUM_LEDS * 2);
        }
        break;

    case 1:
        for(int r=0; r<12; r++){
            for(int g=0; g<12; g++){
                for(int b=0; b<12; b++){
                    set_led(r,g,b,r*20, g*20, b*12);
                }
            }
        }
        delay(5000);
        break;
        
    case 2:
        //three color swipes
        for(int h=0; h<12; h++){
                for(int j=0; j<12; j++){
                    for(int k=0; k<12; k++){
                        set_led( j,h,k,255,0,0);
                    }
                }
                delay(100);
            memset((uint16_t *)px_buf, 0x0000, NUM_LEDS * 2);
        }
        for(int h=0; h<12; h++){
            for(int j=0; j<12; j++){
                for(int k=0; k<12; k++){
                    set_led( j,k,h,0,0,255);
                }
            }
            delay(100);
            memset((uint16_t *)px_buf, 0x0000, NUM_LEDS * 2);
        }
        for(int h=0; h<12; h++){
            for(int j=0; j<12; j++){
                for(int k=0; k<12; k++){
                    set_led( h,j,k,0,255,0);
                }
            }
            delay(100);
            memset((uint16_t *)px_buf, 0x0000, NUM_LEDS * 2);
        }
        break;
        
    case 3: //ball?
            drawBall( 5,5,5, 0x001F );
            delay(1000);
        break;
    
    case 4:
            bounceBall(300);
            break;
            
    
    
case 5:
            count += 1;
            delay(500);
            if (count == 0){
            set_led_pk(5, 5, 5, 0x0F00);
            }
            delay(500);
            shiftRow(0, 5, 5, blue, true);
                break;
    
    animation = (animation+1) % 5;
    }
    
}
/*
void pk_color(uint16_t r, uint16_t g, uint16_t b){
    return ((r & 0x00F8) << 8 ) |
    ((g & 0x00FC) << 3 ) |
    ((b & 0x00F8) >> 3 );
}
*/


void drawBall( int x, int y, int z, uint16_t color){
    for(int i=-1; i<=1; i++){
        for(int j=-1; j<=1; j++){
            for(int k=-1; k<=1; k++){
                if( i*j*k == 0 )    //make sure one is zero so the corners don't get drawn
                set_led_pk(x+i,y+j,z+k, color);
            }
        }
    }
}

void bounceBall(int iterations){
    int x_vec = 1;
    int y_vec = 1;
    int z_vec = 1;
    int x = 6;
    int y = 7;
    int z = 1;
    
    memset((uint16_t *)px_buf, 0x0000, NUM_LEDS * 2);
    
    for( int q = 0; q< iterations; q++){
        
        drawBall( x, y, z, 0);
        
        if(x+x_vec >= 11 || x+x_vec <= 0){
            x += (x_vec = -x_vec);
        }
        else{ x += x_vec; }
        
        if(y+y_vec >= 11 || y+y_vec <= 0){
            y += (y_vec = -y_vec);
        }
        else{ y += y_vec; }
        
        if(z+z_vec >= 11 || z+z_vec <= 0){
            z += (z_vec = -z_vec);
        }
        else{ z += z_vec; }
        
        drawBall(x,y,z, 0xFFFF);
        delay(200);
    }
}
