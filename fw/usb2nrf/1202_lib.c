#include "1202_lib.h"
#include <stdint.h>
#include <avr/pgmspace.h>
#include "defines.h"
#include <util/delay.h>
#include "messages.h"
//#include "main.h"

//#define LCD_HUGE_FONT
//#define LCD_CYRILLIC_FONT
#define charDataWidth ((uint8_t) 5)
#define charWidth ((uint8_t) 6)
#define bigCharDataWidth ((uint8_t) 20)
#define bigCharWidth ((uint8_t) 12)

#ifdef USE_LCD

uint8_t lcdMutex = 0;
uint8_t lcd_x, lcd_y;

const uint8_t font_1[] PROGMEM = {
    0b00001110, 0b01011111, 0b00001110, 0b00000000, 0b00000000, // !
    0b00000110, 0b00001110, 0b00000000, 0b00001110, 0b00000110, // "
    0b00100100, 0b01111110, 0b00100100, 0b01111110, 0b00100100, // #
    0b00100100, 0b00101010, 0b01111111, 0b00101010, 0b00010010, // $
    0b01000100, 0b00100100, 0b00010000, 0b01001000, 0b01000100, // %
    0b00110100, 0b01001010, 0b01011010, 0b00100100, 0b01010000, // &
    0b00001000, 0b00000110, 0b00000000, 0b00000000, 0b00000000, // '
    0b00000000, 0b00000000, 0b00000000, 0b00111110, 0b01000001, // (
    0b01000001, 0b00111110, 0b00000000, 0b00000000, 0b00000000, // )
    0b00001010, 0b00000100, 0b00011111, 0b00000100, 0b00001010, // *
    0b00010000, 0b00010000, 0b01111100, 0b00010000, 0b00010000, // +
    0b10000000, 0b01000000, 0b00000000, 0b00000000, 0b00000000, // ,
    0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, // -
    0b00000000, 0b01000000, 0b01000000, 0b00000000, 0b00000000, // .
    0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, // /
    0b00111110, 0b01010001, 0b01001001, 0b01000101, 0b00111110, // 0
    0b00000000, 0b01000010, 0b01111111, 0b01000000, 0b00000000, // 1
    0b01000010, 0b01100001, 0b01010001, 0b01001001, 0b01000110, // 2
    0b00100010, 0b01001001, 0b01001001, 0b01001001, 0b00110110, // 3
    0b00011000, 0b00010100, 0b01010010, 0b01111111, 0b01010000, // 4
    0b00100111, 0b01000101, 0b01000101, 0b01000101, 0b00111001, // 5
    0b00111110, 0b01001001, 0b01001001, 0b01001001, 0b00110010, // 6
    0b00000011, 0b00000001, 0b01100001, 0b00011001, 0b00000111, // 7
    0b00110110, 0b01001001, 0b01001001, 0b01001001, 0b00110110, // 8
    0b00100110, 0b01001001, 0b01001001, 0b01001001, 0b00111110, // 9
    0b01000100, 0b01000100, 0b00000000, 0b00000000, 0b00000000, // :
    0b10000100, 0b01000100, 0b00000000, 0b00000000, 0b00000000, // ;
    0b00000000, 0b00001000, 0b00010100, 0b00100010, 0b01000001, // <
    0b00100100, 0b00100100, 0b00100100, 0b00100100, 0b00100100, // =
    0b01000001, 0b00100010, 0b00010100, 0b00001000, 0b00000000, // >
    0b00000010, 0b00000001, 0b01010001, 0b00001001, 0b00000110, // ?
    0b00111110, 0b01000001, 0b01011001, 0b01011001, 0b00011110, // @
    0b01111110, 0b00001001, 0b00001001, 0b00001001, 0b01111110, // A
    0b01111111, 0b01001001, 0b01001001, 0b01001001, 0b00110110, // B
    0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b00100010, // C
    0b01111111, 0b01000001, 0b01000001, 0b01000001, 0b00111110, // D
    0b01111111, 0b01001001, 0b01001001, 0b01001001, 0b01000001, // E
    0b01111111, 0b00001001, 0b00001001, 0b00001001, 0b00000001, // F
    0b00111110, 0b01000001, 0b01010001, 0b01010001, 0b00110010, // G
    0b01111111, 0b00001000, 0b00001000, 0b00001000, 0b01111111, // H
    0b00000000, 0b01000001, 0b01111111, 0b01000001, 0b00000000, // I
    0b00100001, 0b01000001, 0b01000001, 0b01000001, 0b00111111, // J
    0b01111111, 0b00010000, 0b00101000, 0b01000100, 0b01000011, // K
    0b01111111, 0b01000000, 0b01000000, 0b01000000, 0b01100000, // L
    0b01111111, 0b00000110, 0b00011000, 0b00000110, 0b01111111, // M
    0b01111111, 0b00000110, 0b00001000, 0b00110000, 0b01111111, // N
    0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b00111110, // O
    0b01111111, 0b00001001, 0b00001001, 0b00001001, 0b00000110, // P
    0b00111110, 0b01000001, 0b01110001, 0b01100001, 0b00111110, // Q
    0b01111111, 0b00001001, 0b00011001, 0b00101001, 0b01000110, // R
    0b00100110, 0b01001001, 0b01001001, 0b01001001, 0b00110010, // S
    0b00000011, 0b00000001, 0b01111111, 0b00000001, 0b00000011, // T
    0b00111111, 0b01000000, 0b01000000, 0b01000000, 0b00111111, // U
    0b00011111, 0b00100000, 0b01000000, 0b00100000, 0b00011111, // V
    0b00111111, 0b01000000, 0b00111000, 0b01000000, 0b00111111, // W
    0b01100011, 0b00010100, 0b00001000, 0b00010100, 0b01100011, // X
    0b00100111, 0b01001000, 0b01001000, 0b01001000, 0b00111111, // Y
    0b01100011, 0b01010001, 0b01001001, 0b01000101, 0b01100011, // Z
    0b00000000, 0b00000000, 0b00000000, 0b01111111, 0b01000001, // [
    0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, // /
    0b01000001, 0b01111111, 0b00000000, 0b00000000, 0b00000000, // ]
    0b00110000, 0b01100000, 0b00110000, 0b00011000, 0b00001100, // ^ (check)
    0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, // _
    0b00000001, 0b00111100, 0b01000010, 0b01000010, 0b00100100, // ` (°C)
    0b00100000, 0b01010100, 0b01010100, 0b00010100, 0b01111000, // a
    0b01111111, 0b00101000, 0b01000100, 0b01000100, 0b00111000, // b
    0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b00101000, // c
    0b00111000, 0b01000100, 0b01000100, 0b00101000, 0b01111111, // d
    0b00111000, 0b01010100, 0b01010100, 0b01010100, 0b00001000, // e
    0b00010000, 0b01111100, 0b00010010, 0b00010010, 0b00000010, // f
    0b01001100, 0b01010010, 0b01010010, 0b01010010, 0b00111100, // g
    0b01111111, 0b00000100, 0b00000100, 0b00000100, 0b01111000, // h
    0b00000000, 0b01001000, 0b01111010, 0b01000000, 0b00000000, // i
    0b00000000, 0b10001000, 0b01111010, 0b00000000, 0b00000000, // j
    0b01111111, 0b00010000, 0b00101000, 0b01000100, 0b00000000, // k
    0b00000000, 0b01000001, 0b01111111, 0b01000000, 0b00000000, // l
    0b01111100, 0b00000100, 0b01111000, 0b00000100, 0b01111000, // m
    0b01111100, 0b00001000, 0b00000100, 0b00000100, 0b01111000, // n
    0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b00111000, // o
    0b11111110, 0b00100100, 0b00100010, 0b00100010, 0b00011100, // p
    0b00011100, 0b00100010, 0b00100010, 0b00100100, 0b11111110, // q
    0b01111100, 0b00001000, 0b00000100, 0b00000100, 0b00001000, // r
    0b01001000, 0b01010100, 0b01010100, 0b01010100, 0b00100100, // s
    0b00000000, 0b00000100, 0b01111110, 0b01000100, 0b00000000, // t
    0b00111100, 0b01000000, 0b01000000, 0b01000000, 0b01111100, // u
    0b00011100, 0b00100000, 0b01000000, 0b00100000, 0b00011100, // v
    0b00111100, 0b01000000, 0b00110000, 0b01000000, 0b00111100, // w
    0b01000100, 0b00101000, 0b00010000, 0b00101000, 0b01000100, // x
    0b00001100, 0b01010000, 0b01010000, 0b01010000, 0b00111100, // y
    0b01100100, 0b01010100, 0b01010100, 0b01010100, 0b01001100  // z
};

#ifdef LCD_CYRILLIC_FONT
const uint8_t font_2[] PROGMEM = {
    0b01111111, 0b01001001, 0b01001001, 0b01001001, 0b00110001, // Б
    0b01111111, 0b00000001, 0b00000001, 0b00000001, 0b00000001, // Г
    0b01111100, 0b01010101, 0b01010100, 0b01010101, 0b01000100, // Ё
    0b01101011, 0b00011100, 0b01101011, 0b00011100, 0b01101011, // Ж
    0b00100010, 0b01000001, 0b01001001, 0b01001001, 0b00110110, // З
    0b01111111, 0b00110000, 0b00001000, 0b00000110, 0b01111111, // И
    0b01111100, 0b00100001, 0b00010001, 0b00001001, 0b01111100, // Й
    0b01000000, 0b01111111, 0b00000001, 0b00000001, 0b01111111, // Л
    0b01111111, 0b00000001, 0b00000001, 0b00000001, 0b01111111, // П
    0b01000111, 0b01001000, 0b01001000, 0b01001000, 0b00111111, // У
    0b00111110, 0b00100010, 0b01111111, 0b00100010, 0b00111110, // Ф
    0b00001111, 0b00001000, 0b00001000, 0b00001000, 0b01111111, // Ч
    0b01111111, 0b01000000, 0b01111110, 0b01000000, 0b01111111, // Ш
    0b00000001, 0b01111111, 0b00101000, 0b01000100, 0b00111000, // Ъ
    0b01111111, 0b01000100, 0b00111000, 0b00000000, 0b01111111, // Ы
    0b00100010, 0b01001001, 0b01001001, 0b01001001, 0b00111110, // Э
    0b01111111, 0b00001000, 0b00111111, 0b01000001, 0b01111110, // Ю
    0b01000110, 0b00101001, 0b00011001, 0b00001001, 0b01111111, // Я
    0b00111100, 0b01001010, 0b01001010, 0b01001001, 0b00110001, // б
    0b01111110, 0b01001010, 0b01001010, 0b01001010, 0b00110100, // в
    0b01111110, 0b00000010, 0b00000010, 0b00000010, 0b00000010, // г
    0b00111000, 0b01010101, 0b01010100, 0b01010101, 0b00001000, // ё
    0b01100110, 0b00011000, 0b01111110, 0b00011000, 0b01100110, // ж
    0b00100100, 0b01000010, 0b01001010, 0b01001010, 0b00110100, // з
    0b00111110, 0b01000000, 0b01000000, 0b00100000, 0b01111110, // и
    0b00111100, 0b01000001, 0b01000001, 0b00100001, 0b01111100, // й
    0b01111110, 0b00001000, 0b00001000, 0b00010100, 0b01100010, // к
    0b01000000, 0b00111110, 0b00000010, 0b00000010, 0b01111110, // л
    0b01111110, 0b00001100, 0b00110000, 0b00001100, 0b01111110, // м
    0b01111110, 0b00001000, 0b00001000, 0b00001000, 0b01111110, // н
    0b01111110, 0b00000010, 0b00000010, 0b00000010, 0b01111110, // п
    0b00000010, 0b00000010, 0b01111110, 0b00000010, 0b00000010, // т
    0b00011110, 0b00010000, 0b00010000, 0b00010000, 0b01111110, // ч
    0b01111110, 0b01000000, 0b01111100, 0b01000000, 0b01111110, // ш
    0b00000010, 0b01111110, 0b01001000, 0b01001000, 0b00110000, // ъ
    0b01111110, 0b01001000, 0b00110000, 0b00000000, 0b01111110, // ы
    0b01111110, 0b01001000, 0b01001000, 0b01001000, 0b00110000, // ь
    0b00100100, 0b01000010, 0b01001010, 0b01001010, 0b00111100, // э
    0b01111110, 0b00001000, 0b01111100, 0b01000010, 0b00111110, // ю
    0b01001100, 0b01010010, 0b00110010, 0b00010010, 0b01111110  // я
};
const uint8_t font_3[] PROGMEM = {
    0b01100000, 0b00111111, 0b00100001, 0b00111111, 0b01100000, // Д
    0b00111111, 0b00100000, 0b00100000, 0b00111111, 0b01100000, // Ц
    0b00111111, 0b00100000, 0b00111110, 0b00100000, 0b01111111, // Щ
    0b01100000, 0b00111110, 0b00100010, 0b00111110, 0b01100000, // д
    0b00011100, 0b00100010, 0b01111111, 0b00100010, 0b00011100, // ф
    0b00111110, 0b00100000, 0b00100000, 0b00111110, 0b01100000, // ц
    0b00111110, 0b00100000, 0b00111100, 0b00100000, 0b01111110  // щ
};
#endif

#ifdef LCD_HUGE_FONT
const uint8_t big_digits_lcd12x16[] PROGMEM = {
    0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF,  0x3F, 0x3F, 0x30, 0x30, 0x30,  0x30, 0x30, 0x30, 0x3F, 0x3F,  // Code for char 0
    0x00, 0x00, 0x03, 0x03, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x3F,  0x3F, 0x00, 0x00, 0x00, 0x00,  // Code for char 1
    0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF,  0x3F, 0x3F, 0x30, 0x30, 0x30,  0x30, 0x30, 0x30, 0x30, 0x30,  // Code for char 2
    0x03, 0x03, 0x03, 0x03, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF,  0x30, 0x30, 0x30, 0x30, 0x30,  0x30, 0x30, 0x30, 0x3F, 0x3F,  // Code for char 3
    0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x3F, 0x3F,  // Code for char 4
    0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3,  0x30, 0x30, 0x30, 0x30, 0x30,  0x30, 0x30, 0x30, 0x3F, 0x3F,  // Code for char 5
    0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3,  0x3F, 0x3F, 0x30, 0x30, 0x30,  0x30, 0x30, 0x30, 0x3F, 0x3F,  // Code for char 6
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x3F, 0x3F,  // Code for char 7
    0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF,  0x3F, 0x3F, 0x30, 0x30, 0x30,  0x30, 0x30, 0x30, 0x3F, 0x3F,  // Code for char 8
    0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x3F, 0x3F,  // Code for char 9
};
#endif

void lcdInit() {
    // magic sequence of commands performs initialization of LC D
    if (lcdMutex) return;
    lcdMutex = 1;
	portLCD &= ~(1 << poLCD_RST);
	// maximum reset reaction is 5us, so let wait a little more
	_delay_us(6);
	portLCD |= (1 << poLCD_RST);
	_delay_us(6);
    LCD_write_byte(0xE2, 0); // reset
    LCD_write_byte(0xA4, 0); // power saver off
    LCD_write_byte(0x2F, 0); // power control set
    LCD_write_byte(0xAF, 0); // LCD display on
    LCD_write_byte(0x20+0, 0); // V0R (voltage range set, 8 step, 3.00V, default is 4, 8.12V)
    LCD_write_byte(0xE1, 0); // VOP
    LCD_write_byte(0x00, 0); // (contrast, signed char)
    LCD_write_byte(0x80+0, 0); // Electronic volume (user contrast, 32 step)
    LCD_write_byte(0x40, 0); // display start address is 0
    // display rotate :)
    LCD_write_byte(0xA0, 0); // segment driver direction select (column inversion, A0/A1)
    LCD_write_byte(0xC0, 0); // common driver direction select (line inversion, C0/C8)
    lcd_x = 0; lcd_y = 0;
    lcdMutex = 0;
}

void lcdLocate(uint8_t y, uint8_t x) { // 471 clocks (59us)
    if (lcdMutex) return;
    lcdMutex = 1;
    y &= 0x0F;
    x = (x & 0x0F) * FONT_WIDTH;
    lcd_x = x; lcd_y = y;
    LCD_write_byte(0xB0 | y, 0); // set page address (row)
    LCD_write_byte(x & 0x0F, 0); // set column address lower nibble
    LCD_write_byte(0x10 | ((x >> 4) & 0x07), 0); // set column address higher nibble
    lcdMutex = 0;
}

void lcdGotoXY(uint8_t x, uint8_t y) { // 464 clocks (58us)
    if (lcdMutex) return;
    lcdMutex = 1;
    y &= 0x0F;
    lcd_x = x & 0x7F; lcd_y = y;
    LCD_write_byte(0xB0 | y, 0); // set page address (row)
    LCD_write_byte(x & 0x0F, 0); // set column address lower nibble
    LCD_write_byte(0x10 | ((x >> 4) & 0x07), 0); // set column address higher nibble
    lcdMutex = 0;
}

void lcdPrintChar(uint8_t symbol, tFontStyle style) { // 947 clocks (118us)+
    const uint8_t *start;
    if (! (style & huge)) {
        start = &font_1[0];
#ifdef LCD_CYRILLIC_FONT
        if (symbol >= 0xE0) {
            start = (&font_3[0]) + ((uint16_t) symbol - 0xE0) * charDataWidth;
        } else if (symbol >= 0xA0) {
            start = (&font_2[0]) + ((uint16_t) symbol - 0xA0) * charDataWidth;
        } else if (symbol >= 0x20) {
#else
		if (symbol < 0xA0 && symbol >= 0x20) {
#endif
            start = (&font_1[0]) + ((uint16_t) symbol - 0x21) * charDataWidth;
        } else return;
    } else {
#ifdef LCD_HUGE_FONT
        if ((symbol < '0' || symbol > '9') && symbol != ' ') return;
        start = &big_digits_lcd12x16[0] + (symbol - '0') * bigCharDataWidth;
#else
		return;
#endif
    }

    int8_t offset = 0-1;
    for (int8_t i = 0; i < ((style & huge)? bigCharWidth*2: charWidth); i++) {
        uint8_t col;
        if ((style & huge) && i == bigCharWidth) {
            lcdGotoXY(lcd_x, lcd_y+1);
        }
        if ((style & huge) && (i == bigCharDataWidth/2 || i == bigCharDataWidth/2+1)) { // interletter spacing at first row
            col = 0;
        } else if (i < ((style & huge)? bigCharDataWidth+2: charDataWidth) && symbol != ' ') { // hardcoded space
            offset++;
            col = pgm_read_byte(&start[offset]);
        } else { // interletter spacing
            col = 0;
        }
        if (style & invisible) col = 0;
        if (style & inversed) col = ~col;
        LCD_write_byte(col, 1);
    }
    if (style & huge)
        lcdGotoXY(lcd_x+bigCharWidth, lcd_y-1);
    else
        lcd_x += charWidth;
}

#define META_PRINT(get_byte_func, message, style) \
    /*unsigned char symbol;*/ \
    int8_t count = 0; \
	int8_t length = get_byte_func; \
    do { \
	    message++; \
	    lcdPrintChar(get_byte_func, style); \
    } while (0 < length-- && LCD_COLUMNS > count++); \
    return count;


uint8_t lcdPrint(const string *message, tFontStyle style) { // 13 + (13 + lcdPrintChar) * length
	META_PRINT(pgm_read_byte(message), message, style);
}

uint8_t lcdRamPrint(const string *message, tFontStyle style) {
	META_PRINT((message->length), message, style);
}

void lcdClear() {
    lcdLocate(0, 0);
    for (uint16_t i = 0; i < ((uint16_t)LCD_WIDTH * LCD_PAGES); i++) {
        LCD_write_byte(0, 1);
    }
}
#endif
