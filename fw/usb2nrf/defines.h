/*
 * defines.h
 *
 * Created: 02.01.2019 20:21:14
 *  Author: J-sama
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_

#define CEILING(x,y) (((x) + (y) - 1) / (y))
#define STR2(s) #s
#define STR(s) STR2(s)

/*** USART ***/
#define BAUDRATE 1000000


/*** LCD ***/
#define portLCD PORTC
#define poLCD_CS PORTC3
#define poLCD_DATA PORTC4
#define poLCD_CLK PORTC5
#define poLCD_RST PORTC2

#define LCD_WIDTH   96
#define LCD_HEIGHT  68
#define LCD_PAGES   9
// высота букавы Ч 8 пикселей
#define FONT_HEIGHT 8
#define FONT_WIDTH  6
#define LCD_ROWS    CEILING(LCD_HEIGHT, FONT_HEIGHT)
#define LCD_COLUMNS CEILING(LCD_WIDTH, FONT_WIDTH)


/*** LEDs ***/
#define portLEDS PORTD
#define poLED_RED PORTD5
#define poLED_YELLOW PORTD6
#define poLED_GREEN PORTD7

/*** transiever ***/
#define MAC_SIZE 5
#define PAYLOAD_SIZE 32

#define portTransiever PORTB
#define poTransiever_IRQ PORTB0

#endif /* DEFINES_H_ */