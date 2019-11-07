/*
 * ui.c
 *
 * Created: 03.11.2019 17:25:32
 *  Author: Mintytail
 */ 
#include "ui.h"
#include "1202_lib.h"
#include "messages.h"
#include <stdbool.h>
#include <avr/interrupt.h>
#include "defines.h"
#include "my-stdlib.h"


// debounce time in ticks
#define DEBOUNCE_TIME 100
typedef uint16_t ui_timestamp;

typedef struct {
	bool state;
	ui_timestamp was_changed;
	bool stable;
} anti_bounce_btn;

typedef struct {
	union {
		const string *str;
		uint16_t uinteger;
		int16_t integer;
	} value;
	ui_varaint type;
} t_subsystem;

void ui_error();
void ui_tick();
void ui_print(ui_subsystems subsystem, tFontStyle style);



static ui_timestamp now = 0;
static anti_bounce_btn buttons[ui_number_of_buttons];
static t_subsystem subsystems[ui_s_number];

void ui_init() {
	// ���������� �����
	ACSR = (1 << ACD);
	// leds
	DDRD |= (1 << poLED_D1) | (1 << poLED_D2);
	portLEDS |= (1 << poLED_D1);
	
	// lcd
	DDRC = (1 << poLCD_CLK) | (1 << poLCD_CS) | (1 << poLCD_DATA) | (1 << poLCD_RST);
	lcdInit();
	
	// timer to update leds, CTC mode, from 0 to OCR0A
	// clk io / 64 (tick every 4us, need 250 ticks for 1ms)
	TCCR0A = (0 << WGM00) | (1 << WGM01);
	TCCR0B = (0 << CS02) | (1 << CS01) | (1 << CS00) | (0 << WGM02);
	OCR0A = 250;
	
	TIMSK0 = (1 << TOIE0);
	
	lcdClear();
	lcdLocate(0, 0);
	lcdPrint(&m_hello, normal);
}

ISR(TIMER0_OVF_vect) {
	ui_tick();
	return;
	uint8_t rpd;
	//nRF24L01_read_register(rfTransiever, RPD, &rpd, 1);
	if (0 == rpd) {
		portLEDS &= ~(1 << poLED_D1);
	} else {
		portLEDS |= (1 << poLED_D1);
	}
}

void ui_tick() {
	// should be called every millisecond
	now++;
	for (int8_t i = 0; i < ui_number_of_buttons; i++) {
		if (! buttons[i].stable) {
			if (DEBOUNCE_TIME < (now - buttons[i].was_changed)) {
				buttons[i].stable = true;
			}
		}
	}
}

void ui_btn_state_changed(enum ui_buttons source, bool state) {
	if (ui_number_of_buttons <= source) return;
	if (state == buttons[source].state) return;
	if (buttons[source].stable) {
		buttons[source].state = state;
		buttons[source].stable = false;
		buttons[source].was_changed = now;
		// generate an event here \0/
	}
};

void ui_redraw() {
	// leds
	
	// no screens for now
	lcdLocate(2, 0);
	ui_print(ui_s_uart_packet_fsm, normal);
}

void ui_error() {
	portLEDS |= (1 <<poLED_D1);
}

void ui_print(ui_subsystems subsystem, tFontStyle style) {
	if (ui_s_number <= subsystem) {
		ui_error();
		return;
	}
	switch (subsystems[subsystem].type) {
		case ui_v_flash_str: {
			lcdPrint(subsystems[subsystem].value.str, style);
			break;
		}
		case ui_v_mem_str: {
			lcdRamPrint(subsystems[subsystem].value.str, style);
			break;
		}
		case ui_v_int16: {
			lcdRamPrint(SStr(subsystems[subsystem].value.integer), style);
		}
	}
}

void ui_subsystem_str(ui_subsystems subsystem, const string *message, bool in_flash) {
	if (ui_s_number <= subsystem) {
		ui_error();
		return;
	}
	subsystems[subsystem].value.str = message;
	if (in_flash) subsystems[subsystem].type = ui_v_flash_str;
	else subsystems[subsystem].type = ui_v_mem_str;
}

void ui_subsystem_int(ui_subsystems subsystem, int16_t value) {
	if (ui_s_number <= subsystem) {
		ui_error();
		return;
	}
	subsystems[subsystem].value.uinteger = value;
	subsystems[subsystem].type = ui_v_int16;
}

void ui_e_main_cycle() {
	portLEDS ^= (1 << poLED_D2);
}
