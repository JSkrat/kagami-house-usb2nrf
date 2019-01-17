EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:analog
LIBS:kt315
LIBS:ltc4054-4
LIBS:modules
LIBS:new relays
LIBS:usb2nrf-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L NRF24L01 D4
U 1 1 5BE82610
P 6750 2950
F 0 "D4" H 6750 3250 50  0000 C CNN
F 1 "NRF24L01" H 6750 2650 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x04_Pitch2.54mm" H 6700 2900 50  0001 C CNN
F 3 "" H 6700 2900 50  0001 C CNN
	1    6750 2950
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X05 P4
U 1 1 5BE8273A
P 6800 2200
F 0 "P4" H 6800 2500 50  0000 C CNN
F 1 "USBTTL" V 6900 2200 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x05_Pitch2.54mm" H 6800 2200 60  0001 C CNN
F 3 "" H 6800 2200 60  0000 C CNN
	1    6800 2200
	1    0    0    -1  
$EndComp
Text Label 6600 2000 2    60   ~ 0
3v3
Text Label 6600 2100 2    60   ~ 0
tx_usb
Text Label 6600 2200 2    60   ~ 0
rx_usb
Text Label 6600 2300 2    60   ~ 0
gnd
Text Label 6600 2400 2    60   ~ 0
5v
Text Label 6400 2800 2    60   ~ 0
3v3
Text Label 7100 2800 0    60   ~ 0
gnd
$Comp
L ATMEGA8-AI IC1
U 1 1 5BE8306D
P 2600 5100
F 0 "IC1" H 1900 6250 50  0000 L BNN
F 1 "ATMEGA8-AI" H 2900 3600 50  0000 L BNN
F 2 "atmega8:atmega8-TQFP32-08" H 2050 3650 50  0001 C CNN
F 3 "" H 2600 5100 60  0000 C CNN
	1    2600 5100
	1    0    0    -1  
$EndComp
Text Label 2550 6700 3    60   ~ 0
gnd
Text Label 2650 6700 3    60   ~ 0
gnd
Text Label 1700 4500 2    60   ~ 0
gnd
Text Label 2550 3800 1    60   ~ 0
3v3
Text Label 2650 3800 1    60   ~ 0
3v3
$Comp
L CONN_01X03 P1
U 1 1 5BE8370C
P 5250 1200
F 0 "P1" H 5250 1400 50  0000 C CNN
F 1 "CONN_01X03" V 5350 1200 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 5250 1200 60  0001 C CNN
F 3 "" H 5250 1200 60  0000 C CNN
	1    5250 1200
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P2
U 1 1 5BE838BB
P 5250 1700
F 0 "P2" H 5250 1850 50  0000 C CNN
F 1 "CONN_01X02" V 5350 1700 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x01_Pitch2.54mm" H 5250 1700 60  0001 C CNN
F 3 "" H 5250 1700 60  0000 C CNN
	1    5250 1700
	1    0    0    -1  
$EndComp
Text Label 5050 1750 2    60   ~ 0
gnd
Text Label 5050 1650 2    60   ~ 0
reset
Text Label 5050 1100 2    60   ~ 0
mosi
Text Label 5050 1200 2    60   ~ 0
miso
Text Label 5050 1300 2    60   ~ 0
sck
Text Label 6400 3000 2    60   ~ 0
mosi
Text Label 7100 3100 0    60   ~ 0
miso
Text Label 7100 3000 0    60   ~ 0
sck
Text Label 7100 2900 0    60   ~ 0
ce
Text Label 6400 2900 2    60   ~ 0
csn
Text Label 6400 3100 2    60   ~ 0
irq
Text Label 1700 4300 2    60   ~ 0
3v3
Text Label 1700 4100 2    60   ~ 0
reset
$Comp
L Crystal Y1
U 1 1 5BE846FD
P 5200 2200
F 0 "Y1" H 5200 2350 50  0000 C CNN
F 1 "Crystal" H 5200 2050 50  0000 C CNN
F 2 "Crystals:Crystal_HC18-U_Vertical" H 5200 2200 60  0001 C CNN
F 3 "" H 5200 2200 60  0000 C CNN
	1    5200 2200
	1    0    0    -1  
$EndComp
Text Label 5050 2200 2    60   ~ 0
xtal1
Text Label 5350 2200 0    60   ~ 0
xtal2
Text Label 1700 4800 2    60   ~ 0
xtal1
Text Label 1700 5000 2    60   ~ 0
xtal2
Text Label 3600 4400 0    60   ~ 0
mosi
Text Label 3600 4500 0    60   ~ 0
miso
Text Label 3600 4600 0    60   ~ 0
sck
Text Label 3600 5700 0    60   ~ 0
tx_usb
Text Label 3600 5800 0    60   ~ 0
rx_usb
NoConn ~ 3600 5400
NoConn ~ 3600 5500
NoConn ~ 1700 4400
$Comp
L C C1
U 1 1 5BE85D7E
P 1550 5500
F 0 "C1" H 1575 5600 50  0000 L CNN
F 1 "100n" H 1575 5400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 1588 5350 30  0001 C CNN
F 3 "" H 1550 5500 60  0000 C CNN
	1    1550 5500
	1    0    0    -1  
$EndComp
Text Label 1550 5350 0    60   ~ 0
3v3
Text Label 1550 5650 0    60   ~ 0
gnd
Text Label 3600 4100 0    60   ~ 0
irq
Text Label 3600 4300 0    60   ~ 0
ce
Text Label 3600 4200 0    60   ~ 0
csn
$Comp
L CONN_01X06 P3
U 1 1 5BE870D7
P 6800 1250
F 0 "P3" H 6800 1600 50  0000 C CNN
F 1 "CONN_01X06" V 6900 1250 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm" H 6800 1250 60  0001 C CNN
F 3 "" H 6800 1250 60  0000 C CNN
	1    6800 1250
	1    0    0    -1  
$EndComp
Text Label 6600 1000 2    60   ~ 0
gnd
Text Label 6600 1100 2    60   ~ 0
3v3
Text Label 6600 1200 2    60   ~ 0
lcd_rst
Text Label 6600 1300 2    60   ~ 0
lcd_cs
Text Label 6600 1400 2    60   ~ 0
lcd_sda
Text Label 6600 1500 2    60   ~ 0
lcd_clk
Text Label 3600 5200 0    60   ~ 0
lcd_sda
Text Label 3600 5300 0    60   ~ 0
lcd_clk
Text Label 3600 5100 0    60   ~ 0
lcd_cs
Text Label 3600 5000 0    60   ~ 0
lcd_rst
NoConn ~ 3600 5900
NoConn ~ 3600 4800
NoConn ~ 3600 4900
NoConn ~ 3600 6000
NoConn ~ 3600 6100
$Comp
L C C2
U 1 1 5BE89070
P 5050 2650
F 0 "C2" H 5075 2750 50  0000 L CNN
F 1 "C" H 5075 2550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5088 2500 30  0001 C CNN
F 3 "" H 5050 2650 60  0000 C CNN
	1    5050 2650
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5BE890C5
P 5350 2650
F 0 "C3" H 5375 2750 50  0000 L CNN
F 1 "C" H 5375 2550 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5388 2500 30  0001 C CNN
F 3 "" H 5350 2650 60  0000 C CNN
	1    5350 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 2500 5050 2200
Wire Wire Line
	5350 2200 5350 2500
Text Label 5150 2800 0    60   ~ 0
gnd
Wire Wire Line
	5050 2800 5350 2800
$Comp
L LED D2
U 1 1 5BE89C52
P 4050 7150
F 0 "D2" H 4050 7250 50  0000 C CNN
F 1 "LED" H 4050 7050 50  0000 C CNN
F 2 "LEDs:LED_0805_HandSoldering" H 4050 7150 60  0001 C CNN
F 3 "" H 4050 7150 60  0000 C CNN
	1    4050 7150
	0    -1   -1   0   
$EndComp
$Comp
L LED D3
U 1 1 5BE89CDD
P 4350 7150
F 0 "D3" H 4350 7250 50  0000 C CNN
F 1 "LED" H 4350 7050 50  0000 C CNN
F 2 "LEDs:LED_0805_HandSoldering" H 4350 7150 60  0001 C CNN
F 3 "" H 4350 7150 60  0000 C CNN
	1    4350 7150
	0    -1   -1   0   
$EndComp
$Comp
L R R2
U 1 1 5BE89D1A
P 4050 6800
F 0 "R2" V 4130 6800 50  0000 C CNN
F 1 "R" V 4050 6800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3980 6800 30  0001 C CNN
F 3 "" H 4050 6800 30  0000 C CNN
	1    4050 6800
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 5BE89D5B
P 4350 6800
F 0 "R3" V 4430 6800 50  0000 C CNN
F 1 "R" V 4350 6800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4280 6800 30  0001 C CNN
F 3 "" H 4350 6800 30  0000 C CNN
	1    4350 6800
	1    0    0    -1  
$EndComp
Text Label 4150 7350 0    60   ~ 0
gnd
Wire Wire Line
	3750 7350 4350 7350
Wire Wire Line
	3600 6200 4350 6200
Wire Wire Line
	4350 6200 4350 6650
Wire Wire Line
	3600 6300 4050 6300
Wire Wire Line
	4050 6300 4050 6650
$Comp
L R R1
U 1 1 5BE89EC3
P 3750 6800
F 0 "R1" V 3830 6800 50  0000 C CNN
F 1 "R" V 3750 6800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3680 6800 30  0001 C CNN
F 3 "" H 3750 6800 30  0000 C CNN
	1    3750 6800
	1    0    0    -1  
$EndComp
$Comp
L LED D1
U 1 1 5BE89F24
P 3750 7150
F 0 "D1" H 3750 7250 50  0000 C CNN
F 1 "LED" H 3750 7050 50  0000 C CNN
F 2 "LEDs:LED_0805_HandSoldering" H 3750 7150 60  0001 C CNN
F 3 "" H 3750 7150 60  0000 C CNN
	1    3750 7150
	0    -1   -1   0   
$EndComp
Connection ~ 4050 7350
Wire Wire Line
	3600 6400 3750 6400
Wire Wire Line
	3750 6400 3750 6650
$EndSCHEMATC
