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
#define BAUDRATE 200000


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
#define poLED_D1 PORTD7
#define poLED_D2 PORTD6

/*** transiever ***/
#define MAC_SIZE 5
#define PAYLOAD_SIZE 32

#define portTransiever PORTB
#define poTransiever_IRQ PORTB0

/** RF packet **/
/*** encryption ***/
#define RF_ENC_TOTAL 1

#define RF_VERSION 0
#define RF_TRANSACTION_ID 1
// request-related
#define RF_UNIT_ID 2
#define RF_NEXT_CHANNEL 3
#define RF_FUNCTION_ID 4
#define RF_DATA 5
// response-related
#define RF_RESP_CODE 2
// data is the last field, so its starting is minimal length of the packet, used in "RF parser.c"
#define RF_RESP_DATA 3
// data packet available length
#define RF_RESP_DATA_LENGTH (PAYLOAD_SIZE - RF_RESP_DATA - RF_ENC_TOTAL)

/*** common response codes ***/
#define C_OK 0
#define C_NOT_IMPLEMENTED 0x7F
#define C_BAD_REQUEST_DATA 0xE0
/*** custom (per-function) error codes ***/
// set address
#define C_AD_BAD_LENGTH 1
// read/write channel
#define C_CH_BAD_CHANNELS 1
#define C_CH_RESPONSE_TOO_BIG 2
#define C_CH_BAD_PERMISSIONS 3
#define C_CH_VALIDATION_FAILED 4

#endif /* DEFINES_H_ */