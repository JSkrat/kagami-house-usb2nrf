EESchema Schematic File Version 4
LIBS:usb2nrf-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "usb2nrf"
Date "2019-05-20"
Rev ""
Comp ""
Comment1 ""
Comment2 "Mintytail"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L modules:NRF24L01 NRF24L1
U 1 1 5BE82610
P 9150 5650
F 0 "NRF24L1" H 9150 5950 50  0000 C CNN
F 1 "NRF24L01" H 9150 5350 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x04_Pitch2.54mm" H 9100 5600 50  0001 C CNN
F 3 "" H 9100 5600 50  0001 C CNN
	1    9150 5650
	1    0    0    -1  
$EndComp
$Comp
L conn:CONN_01X05 P4
U 1 1 5BE8273A
P 9800 1350
F 0 "P4" H 9800 1650 50  0000 C CNN
F 1 "USBTTL" V 9900 1350 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x05_Pitch2.54mm" H 9800 1350 60  0001 C CNN
F 3 "" H 9800 1350 60  0000 C CNN
	1    9800 1350
	1    0    0    -1  
$EndComp
Text Label 9600 1250 2    60   ~ 0
tx_usb
Text Label 9600 1350 2    60   ~ 0
rx_usb
Text Label 9600 1450 2    60   ~ 0
gnd
Text Label 9600 1550 2    60   ~ 0
5v
Text Label 8800 5500 2    60   ~ 0
3v3
$Comp
L atmel:ATMEGA8-AI IC1
U 1 1 5BE8306D
P 2600 5100
F 0 "IC1" H 1900 6250 50  0000 L BNN
F 1 "ATMEGA8-AI" H 2900 3600 50  0000 L BNN
F 2 "atmega8:atmega8-TQFP32-08" H 2050 3650 50  0001 C CNN
F 3 "" H 2600 5100 60  0000 C CNN
	1    2600 5100
	1    0    0    -1  
$EndComp
Text Label 2550 3800 1    60   ~ 0
3v3
Text Label 2650 3800 1    60   ~ 0
3v3
$Comp
L conn:CONN_01X03 P1
U 1 1 5BE8370C
P 3450 1600
F 0 "P1" H 3450 1800 50  0000 C CNN
F 1 "CONN_01X03" V 3550 1600 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 3450 1600 60  0001 C CNN
F 3 "" H 3450 1600 60  0000 C CNN
	1    3450 1600
	1    0    0    -1  
$EndComp
$Comp
L conn:CONN_01X02 P2
U 1 1 5BE838BB
P 3450 2100
F 0 "P2" H 3450 2250 50  0000 C CNN
F 1 "CONN_01X02" V 3550 2100 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x01_Pitch2.54mm" H 3450 2100 60  0001 C CNN
F 3 "" H 3450 2100 60  0000 C CNN
	1    3450 2100
	1    0    0    -1  
$EndComp
Text Label 3250 2050 2    60   ~ 0
gnd
Text Label 3250 2150 2    60   ~ 0
reset
Text Label 3250 1500 2    60   ~ 0
mosi
Text Label 3250 1600 2    60   ~ 0
miso
Text Label 3250 1700 2    60   ~ 0
sck
Text Label 8800 5700 2    60   ~ 0
mosi
Text Label 9500 5800 0    60   ~ 0
miso
Text Label 9500 5700 0    60   ~ 0
sck
Text Label 9500 5600 0    60   ~ 0
ce
Text Label 8800 5600 2    60   ~ 0
csn
Text Label 8800 5800 2    60   ~ 0
irq
Text Label 1700 4300 2    60   ~ 0
3v3
Text Label 1700 4100 2    60   ~ 0
reset
$Comp
L Device:Crystal Y1
U 1 1 5BE846FD
P 1200 6300
F 0 "Y1" H 1200 6450 50  0000 C CNN
F 1 "16M" H 1200 6150 50  0000 C CNN
F 2 "Crystals:Crystal_HC18-U_Vertical" H 1200 6300 60  0001 C CNN
F 3 "" H 1200 6300 60  0000 C CNN
	1    1200 6300
	1    0    0    -1  
$EndComp
Text Label 1050 6300 2    60   ~ 0
xtal1
Text Label 1350 6300 0    60   ~ 0
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
L Device:C Cavr1
U 1 1 5BE85D7E
P 1450 5500
F 0 "Cavr1" H 1475 5600 50  0000 L CNN
F 1 "100n" H 1475 5400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1488 5350 30  0001 C CNN
F 3 "" H 1450 5500 60  0000 C CNN
	1    1450 5500
	1    0    0    -1  
$EndComp
Text Label 3600 4100 0    60   ~ 0
irq
Text Label 3600 4300 0    60   ~ 0
ce
Text Label 3600 4200 0    60   ~ 0
csn
Text Label 10100 2750 2    60   ~ 0
lcd_rst
Text Label 10100 2850 2    60   ~ 0
lcd_cs
Text Label 9750 3050 2    60   ~ 0
lcd_sda
Text Label 9750 3150 2    60   ~ 0
lcd_clk
Text Label 3600 5200 0    60   ~ 0
lcd_sda
Text Label 3600 5300 0    60   ~ 0
lcd_clk
Text Label 3600 5100 0    60   ~ 0
lcd_cs
Text Label 3600 5000 0    60   ~ 0
lcd_rst
$Comp
L Device:C C2
U 1 1 5BE89070
P 1050 6750
F 0 "C2" H 1075 6850 50  0000 L CNN
F 1 "22p" H 1075 6650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1088 6600 30  0001 C CNN
F 3 "" H 1050 6750 60  0000 C CNN
	1    1050 6750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5BE890C5
P 1350 6750
F 0 "C3" H 1375 6850 50  0000 L CNN
F 1 "22p" H 1375 6650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1388 6600 30  0001 C CNN
F 3 "" H 1350 6750 60  0000 C CNN
	1    1350 6750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 6600 1050 6300
Wire Wire Line
	1350 6300 1350 6600
$Comp
L usb2nrf-rescue:LED D2
U 1 1 5BE89C52
P 4050 7150
F 0 "D2" H 4050 7250 50  0000 C CNN
F 1 "LED" H 4050 7050 50  0000 C CNN
F 2 "LEDs:LED_0805" H 4050 7150 60  0001 C CNN
F 3 "" H 4050 7150 60  0000 C CNN
	1    4050 7150
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 5BE89D1A
P 4050 6800
F 0 "R2" V 4130 6800 50  0000 C CNN
F 1 "R" V 4050 6800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3980 6800 30  0001 C CNN
F 3 "" H 4050 6800 30  0000 C CNN
	1    4050 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 7350 4050 7350
Wire Wire Line
	3600 6300 4050 6300
Wire Wire Line
	4050 6300 4050 6650
$Comp
L Device:R R1
U 1 1 5BE89EC3
P 3750 6800
F 0 "R1" V 3830 6800 50  0000 C CNN
F 1 "R" V 3750 6800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3680 6800 30  0001 C CNN
F 3 "" H 3750 6800 30  0000 C CNN
	1    3750 6800
	1    0    0    -1  
$EndComp
$Comp
L usb2nrf-rescue:LED D1
U 1 1 5BE89F24
P 3750 7150
F 0 "D1" H 3750 7250 50  0000 C CNN
F 1 "LED" H 3750 7050 50  0000 C CNN
F 2 "LEDs:LED_0805" H 3750 7150 60  0001 C CNN
F 3 "" H 3750 7150 60  0000 C CNN
	1    3750 7150
	0    -1   -1   0   
$EndComp
Connection ~ 4050 7350
Wire Wire Line
	3600 6400 3750 6400
Wire Wire Line
	3750 6400 3750 6650
$Comp
L atmel:AVR-JTAG-10 CON1
U 1 1 5CB35750
P 2150 1850
F 0 "CON1" H 1980 2180 50  0000 C CNN
F 1 "AVR-JTAG-10" H 1810 1520 50  0000 L BNN
F 2 "Pin_Headers:Pin_Header_Straight_2x05_Pitch2.54mm" V 1580 1870 50  0001 C CNN
F 3 "" H 2150 1850 60  0000 C CNN
	1    2150 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 2050 2600 2050
Text Label 1600 1950 2    60   ~ 0
sck
Wire Wire Line
	1600 1650 1950 1650
Text Label 1600 1650 2    60   ~ 0
mosi
Text Label 1600 2050 2    60   ~ 0
miso
Wire Wire Line
	1600 2050 1950 2050
$Comp
L Device:C Cnrf1
U 1 1 5CB37C1D
P 9150 5150
F 0 "Cnrf1" H 9175 5250 50  0000 L CNN
F 1 "100n" H 9175 5050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 9188 5000 30  0001 C CNN
F 3 "" H 9150 5150 60  0000 C CNN
	1    9150 5150
	0    1    1    0   
$EndComp
Text Label 6850 4800 0    60   ~ 0
3v3
$Comp
L conn:CONN_01X12 P5
U 1 1 5CE1954D
P 10300 3200
F 0 "P5" H 10378 3241 50  0000 L CNN
F 1 "LCD_CONN" H 10378 3150 50  0000 L CNN
F 2 "lcd_n1202:SIL-12" H 10300 3200 60  0001 C CNN
F 3 "" H 10300 3200 60  0000 C CNN
	1    10300 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	10100 3250 10000 3250
Wire Wire Line
	10000 3250 10000 3350
Wire Wire Line
	10000 3350 10100 3350
Wire Wire Line
	10100 3550 10000 3550
Wire Wire Line
	10000 3550 10000 3450
Wire Wire Line
	10000 3450 10100 3450
Wire Wire Line
	10100 2950 9850 2950
Wire Wire Line
	9850 2950 9850 3550
Wire Wire Line
	9850 3550 10000 3550
Connection ~ 10000 3550
NoConn ~ 10100 3750
NoConn ~ 10100 2650
Text Label 9750 3350 2    50   ~ 0
3v3
Text Label 10100 3650 2    50   ~ 0
lcd_backlight
Wire Wire Line
	9750 3050 10100 3050
Wire Wire Line
	10100 3150 9750 3150
$Comp
L Device:R R11
U 1 1 5CE1DD38
P 9400 3650
F 0 "R11" V 9193 3650 50  0000 C CNN
F 1 "R" V 9284 3650 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 9330 3650 50  0001 C CNN
F 3 "~" H 9400 3650 50  0001 C CNN
	1    9400 3650
	0    1    1    0   
$EndComp
Wire Wire Line
	9550 3650 10100 3650
Wire Wire Line
	9250 3650 9250 3350
Wire Wire Line
	9250 3350 10000 3350
Connection ~ 10000 3350
$Comp
L Device:C Clcd1
U 1 1 5CE1F247
P 9000 3150
F 0 "Clcd1" H 9115 3196 50  0000 L CNN
F 1 "100n" H 9115 3105 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 9038 3000 50  0001 C CNN
F 3 "~" H 9000 3150 50  0001 C CNN
	1    9000 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 5150 8800 5150
Wire Wire Line
	8800 5150 8800 5500
Wire Wire Line
	9300 5150 9500 5150
Wire Wire Line
	9500 5150 9500 5500
Wire Wire Line
	9000 3000 9000 2950
Wire Wire Line
	9000 3350 9000 3300
$Comp
L Device:C Cavr2
U 1 1 5CE2870D
P 1100 5500
F 0 "Cavr2" H 1125 5600 50  0000 L CNN
F 1 "100n" H 1125 5400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1138 5350 30  0001 C CNN
F 3 "" H 1100 5500 60  0000 C CNN
	1    1100 5500
	1    0    0    -1  
$EndComp
Text Label 1150 5350 0    60   ~ 0
3v3
$Comp
L Switch:SW_Push SW1
U 1 1 5CE29910
P 5350 5900
F 0 "SW1" H 5350 6185 50  0000 C CNN
F 1 "SW_Push" H 5350 6094 50  0000 C CNN
F 2 "Buttons_Switches_ThroughHole:SW_PUSH_6mm_h4.3mm" H 5350 6100 50  0001 C CNN
F 3 "~" H 5350 6100 50  0001 C CNN
	1    5350 5900
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 5CE29E43
P 5350 6550
F 0 "SW2" H 5350 6835 50  0000 C CNN
F 1 "SW_Push" H 5350 6744 50  0000 C CNN
F 2 "Buttons_Switches_ThroughHole:SW_PUSH_6mm_h4.3mm" H 5350 6750 50  0001 C CNN
F 3 "~" H 5350 6750 50  0001 C CNN
	1    5350 6550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 5CE2C13C
P 4900 6100
F 0 "R9" H 4970 6146 50  0000 L CNN
F 1 "10k" H 4970 6055 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 4830 6100 50  0001 C CNN
F 3 "~" H 4900 6100 50  0001 C CNN
	1    4900 6100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 5CE2C7D4
P 4900 6800
F 0 "R10" H 4830 6754 50  0000 R CNN
F 1 "10k" H 4830 6845 50  0000 R CNN
F 2 "Resistors_SMD:R_0805" V 4830 6800 50  0001 C CNN
F 3 "~" H 4900 6800 50  0001 C CNN
	1    4900 6800
	-1   0    0    1   
$EndComp
$Comp
L Device:R R7
U 1 1 5CE2EA5E
P 4100 5900
F 0 "R7" V 3893 5900 50  0000 C CNN
F 1 "150" V 3984 5900 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 4030 5900 50  0001 C CNN
F 3 "~" H 4100 5900 50  0001 C CNN
	1    4100 5900
	0    1    1    0   
$EndComp
$Comp
L Device:R R8
U 1 1 5CE2F6EA
P 4400 6000
F 0 "R8" V 4193 6000 50  0000 C CNN
F 1 "150" V 4284 6000 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 4330 6000 50  0001 C CNN
F 3 "~" H 4400 6000 50  0001 C CNN
	1    4400 6000
	0    1    1    0   
$EndComp
Wire Wire Line
	4550 6000 4600 6000
Wire Wire Line
	4250 6000 3600 6000
Wire Wire Line
	3600 5900 3950 5900
Text Label 5750 5900 0    50   ~ 0
3v3
Wire Wire Line
	5650 6550 5550 6550
Wire Wire Line
	2600 1650 2200 1650
Text Label 2600 1650 0    50   ~ 0
5v
NoConn ~ 1950 1750
Wire Wire Line
	2600 1750 2200 1750
Text Label 1600 1850 2    50   ~ 0
reset
Wire Wire Line
	1600 1850 1950 1850
Wire Wire Line
	2200 1850 2600 1850
Wire Wire Line
	2600 1850 2600 1750
Wire Wire Line
	1600 1950 1950 1950
Wire Wire Line
	2200 1950 2600 1950
Wire Wire Line
	2600 1950 2600 1850
Connection ~ 2600 1850
Wire Wire Line
	2600 2050 2600 1950
Connection ~ 2600 1950
$Comp
L Device:LED D5
U 1 1 5CE3EBF7
P 6850 4950
F 0 "D5" V 6889 4833 50  0000 R CNN
F 1 "LED" V 6798 4833 50  0000 R CNN
F 2 "LEDs:LED_0805" H 6850 4950 50  0001 C CNN
F 3 "~" H 6850 4950 50  0001 C CNN
	1    6850 4950
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R4
U 1 1 5CE40F52
P 6850 5350
F 0 "R4" H 6920 5396 50  0000 L CNN
F 1 "R" H 6920 5305 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 6780 5350 50  0001 C CNN
F 3 "~" H 6850 5350 50  0001 C CNN
	1    6850 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 5650 6850 5500
Wire Wire Line
	6850 5200 6850 5100
Text Label 7850 5650 3    60   ~ 0
tx_usb
Text Label 7850 4800 2    60   ~ 0
3v3
$Comp
L Device:LED D6
U 1 1 5CE45E01
P 7850 5100
F 0 "D6" V 7889 4983 50  0000 R CNN
F 1 "LED" V 7798 4983 50  0000 R CNN
F 2 "LEDs:LED_0805" H 7850 5100 50  0001 C CNN
F 3 "~" H 7850 5100 50  0001 C CNN
	1    7850 5100
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R5
U 1 1 5CE45E0B
P 7850 5500
F 0 "R5" H 7780 5454 50  0000 R CNN
F 1 "R" H 7780 5545 50  0000 R CNN
F 2 "Resistors_SMD:R_0805" V 7780 5500 50  0001 C CNN
F 3 "~" H 7850 5500 50  0001 C CNN
	1    7850 5500
	-1   0    0    1   
$EndComp
Wire Wire Line
	7850 4800 7850 4950
Wire Wire Line
	7850 5250 7850 5350
Text Label 7450 5650 3    60   ~ 0
rx_usb
Text Label 7450 4800 2    60   ~ 0
3v3
$Comp
L Device:LED D7
U 1 1 5CE481EC
P 7450 5100
F 0 "D7" V 7489 4983 50  0000 R CNN
F 1 "LED" V 7398 4983 50  0000 R CNN
F 2 "LEDs:LED_0805" H 7450 5100 50  0001 C CNN
F 3 "~" H 7450 5100 50  0001 C CNN
	1    7450 5100
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R6
U 1 1 5CE481F6
P 7450 5500
F 0 "R6" H 7380 5454 50  0000 R CNN
F 1 "R" H 7380 5545 50  0000 R CNN
F 2 "Resistors_SMD:R_0805" V 7380 5500 50  0001 C CNN
F 3 "~" H 7450 5500 50  0001 C CNN
	1    7450 5500
	-1   0    0    1   
$EndComp
Wire Wire Line
	7450 4800 7450 4950
Wire Wire Line
	7450 5250 7450 5350
$Comp
L conn:CONN_01X07 Pnrf1
U 1 1 5CE57BA3
P 10450 5350
F 0 "Pnrf1" H 10528 5391 50  0000 L CNN
F 1 "nrf_test" H 10528 5300 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x07_Pitch2.54mm" H 10450 5350 60  0001 C CNN
F 3 "" H 10450 5350 60  0000 C CNN
	1    10450 5350
	1    0    0    -1  
$EndComp
Text Label 10250 5050 2    50   ~ 0
gnd
Text Label 10250 5150 2    50   ~ 0
ce
Text Label 10250 5250 2    50   ~ 0
csn
Text Label 10250 5350 2    50   ~ 0
sck
Text Label 10250 5450 2    50   ~ 0
mosi
Text Label 10250 5550 2    50   ~ 0
miso
Text Label 10250 5650 2    50   ~ 0
irq
$Comp
L conn:CONN_01X03 Puart1
U 1 1 5CE5A141
P 10450 1350
F 0 "Puart1" H 10528 1391 50  0000 L CNN
F 1 "uart_test" H 10528 1300 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 10450 1350 60  0001 C CNN
F 3 "" H 10450 1350 60  0000 C CNN
	1    10450 1350
	1    0    0    -1  
$EndComp
Text Label 10250 1250 2    50   ~ 0
gnd
Text Label 10250 1350 2    50   ~ 0
tx_usb
Text Label 10250 1450 2    50   ~ 0
rx_usb
$Comp
L conn:CONN_01X03 Psrv1
U 1 1 5CE5B6CF
P 5500 7200
F 0 "Psrv1" H 5578 7241 50  0000 L CNN
F 1 "CONN_srv" H 5578 7150 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 5500 7200 60  0001 C CNN
F 3 "" H 5500 7200 60  0000 C CNN
	1    5500 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 6100 4500 6100
Wire Wire Line
	4500 6100 4500 7300
Wire Wire Line
	4500 7300 5300 7300
Text Label 5300 7100 2    50   ~ 0
gnd
Text Label 5300 7200 2    50   ~ 0
5v
$Comp
L Device:C Csrv3
U 1 1 5CE60A6B
P 6150 7200
F 0 "Csrv3" H 6175 7300 50  0000 L CNN
F 1 "100n" H 6175 7100 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6188 7050 30  0001 C CNN
F 3 "" H 6150 7200 60  0000 C CNN
	1    6150 7200
	1    0    0    -1  
$EndComp
Text Label 6150 7050 0    60   ~ 0
5v
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5CE69476
P 9500 1800
F 0 "#FLG0101" H 9500 1875 50  0001 C CNN
F 1 "PWR_FLAG" H 9500 1973 50  0000 C CNN
F 2 "" H 9500 1800 50  0001 C CNN
F 3 "~" H 9500 1800 50  0001 C CNN
	1    9500 1800
	-1   0    0    1   
$EndComp
Connection ~ 8800 5150
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5CE69CE2
P 9000 1450
F 0 "#FLG0102" H 9000 1525 50  0001 C CNN
F 1 "PWR_FLAG" V 9000 1577 50  0000 L CNN
F 2 "" H 9000 1450 50  0001 C CNN
F 3 "~" H 9000 1450 50  0001 C CNN
	1    9000 1450
	0    -1   -1   0   
$EndComp
Connection ~ 9500 5150
$Comp
L Device:C C1
U 1 1 5CEC4A15
P 1150 4350
F 0 "C1" H 1265 4396 50  0000 L CNN
F 1 "100n" H 1265 4305 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 1188 4200 50  0001 C CNN
F 3 "~" H 1150 4350 50  0001 C CNN
	1    1150 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 4100 1150 4100
Wire Wire Line
	1150 4100 1150 4200
Wire Wire Line
	1700 4500 1150 4500
$Comp
L Switch:SW_Push SWrst1
U 1 1 5CEC8F16
P 550 4300
F 0 "SWrst1" V 504 4448 50  0000 L CNN
F 1 "SW_Push" V 595 4448 50  0000 L CNN
F 2 "Buttons_Switches_ThroughHole:SW_PUSH_6mm_h4.3mm" H 550 4500 50  0001 C CNN
F 3 "~" H 550 4500 50  0001 C CNN
	1    550  4300
	0    1    1    0   
$EndComp
$Comp
L Device:CP Cnrf2
U 1 1 5CF4AF4A
P 9150 4850
F 0 "Cnrf2" V 9405 4850 50  0000 C CNN
F 1 "10u" V 9314 4850 50  0000 C CNN
F 2 "Capacitors_SMD:C_1210" H 9188 4700 50  0001 C CNN
F 3 "~" H 9150 4850 50  0001 C CNN
	1    9150 4850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9000 4850 8800 4850
Wire Wire Line
	8800 4850 8800 5150
Wire Wire Line
	9300 4850 9500 4850
Wire Wire Line
	9500 4850 9500 5150
$Comp
L power:GNDPWR #PWR0101
U 1 1 5CF6AFB8
P 6850 5650
F 0 "#PWR0101" H 6850 5450 50  0001 C CNN
F 1 "GNDPWR" H 6854 5496 50  0000 C CNN
F 2 "" H 6850 5600 50  0001 C CNN
F 3 "" H 6850 5600 50  0001 C CNN
	1    6850 5650
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0102
U 1 1 5CF6C06D
P 9500 4850
F 0 "#PWR0102" H 9500 4650 50  0001 C CNN
F 1 "GNDPWR" V 9505 4742 50  0000 R CNN
F 2 "" H 9500 4800 50  0001 C CNN
F 3 "" H 9500 4800 50  0001 C CNN
	1    9500 4850
	0    -1   -1   0   
$EndComp
Connection ~ 9500 4850
$Comp
L power:GNDPWR #PWR0103
U 1 1 5CF6D4C7
P 8300 2950
F 0 "#PWR0103" H 8300 2750 50  0001 C CNN
F 1 "GNDPWR" H 8304 2796 50  0000 C CNN
F 2 "" H 8300 2900 50  0001 C CNN
F 3 "" H 8300 2900 50  0001 C CNN
	1    8300 2950
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0104
U 1 1 5CF6E020
P 2600 2050
F 0 "#PWR0104" H 2600 1850 50  0001 C CNN
F 1 "GNDPWR" H 2604 1896 50  0000 C CNN
F 2 "" H 2600 2000 50  0001 C CNN
F 3 "" H 2600 2000 50  0001 C CNN
	1    2600 2050
	1    0    0    -1  
$EndComp
Connection ~ 2600 2050
$Comp
L power:GNDPWR #PWR0105
U 1 1 5CF6F02A
P 2650 6800
F 0 "#PWR0105" H 2650 6600 50  0001 C CNN
F 1 "GNDPWR" H 2654 6646 50  0000 C CNN
F 2 "" H 2650 6750 50  0001 C CNN
F 3 "" H 2650 6750 50  0001 C CNN
	1    2650 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 6800 2650 6700
Wire Wire Line
	2550 6700 2650 6700
Connection ~ 2650 6700
$Comp
L power:GNDPWR #PWR0106
U 1 1 5CF73AB5
P 4050 7350
F 0 "#PWR0106" H 4050 7150 50  0001 C CNN
F 1 "GNDPWR" H 4054 7196 50  0000 C CNN
F 2 "" H 4050 7300 50  0001 C CNN
F 3 "" H 4050 7300 50  0001 C CNN
	1    4050 7350
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0107
U 1 1 5CF74AC3
P 1250 6900
F 0 "#PWR0107" H 1250 6700 50  0001 C CNN
F 1 "GNDPWR" H 1254 6746 50  0000 C CNN
F 2 "" H 1250 6850 50  0001 C CNN
F 3 "" H 1250 6850 50  0001 C CNN
	1    1250 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 5650 1300 5650
$Comp
L power:GNDPWR #PWR0108
U 1 1 5CF79986
P 1300 5650
F 0 "#PWR0108" H 1300 5450 50  0001 C CNN
F 1 "GNDPWR" H 1304 5496 50  0000 C CNN
F 2 "" H 1300 5600 50  0001 C CNN
F 3 "" H 1300 5600 50  0001 C CNN
	1    1300 5650
	1    0    0    -1  
$EndComp
Connection ~ 1300 5650
Wire Wire Line
	1300 5650 1450 5650
Wire Wire Line
	1100 5350 1450 5350
$Comp
L power:GNDPWR #PWR0109
U 1 1 5CF7C75E
P 1150 4500
F 0 "#PWR0109" H 1150 4300 50  0001 C CNN
F 1 "GNDPWR" H 1154 4346 50  0000 C CNN
F 2 "" H 1150 4450 50  0001 C CNN
F 3 "" H 1150 4450 50  0001 C CNN
	1    1150 4500
	1    0    0    -1  
$EndComp
Connection ~ 1150 4500
$Comp
L power:GNDPWR #PWR0110
U 1 1 5CF7CB1A
P 9250 1450
F 0 "#PWR0110" H 9250 1250 50  0001 C CNN
F 1 "GNDPWR" H 9254 1296 50  0000 C CNN
F 2 "" H 9250 1400 50  0001 C CNN
F 3 "" H 9250 1400 50  0001 C CNN
	1    9250 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 1450 9600 1450
$Comp
L Device:CP Csrv1
U 1 1 5CF807DF
P 6500 7200
F 0 "Csrv1" H 6618 7246 50  0000 L CNN
F 1 "10u" H 6618 7155 50  0000 L CNN
F 2 "Capacitors_SMD:C_1210" H 6538 7050 50  0001 C CNN
F 3 "~" H 6500 7200 50  0001 C CNN
	1    6500 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 7050 6500 7050
Wire Wire Line
	6150 7350 6400 7350
$Comp
L power:GNDPWR #PWR03
U 1 1 5CF860E2
P 6400 7350
F 0 "#PWR03" H 6400 7150 50  0001 C CNN
F 1 "GNDPWR" H 6404 7196 50  0000 C CNN
F 2 "" H 6400 7300 50  0001 C CNN
F 3 "" H 6400 7300 50  0001 C CNN
	1    6400 7350
	1    0    0    -1  
$EndComp
Connection ~ 6400 7350
Wire Wire Line
	6400 7350 6500 7350
$Comp
L Device:CP Clcd2
U 1 1 5CF8A020
P 8650 3150
F 0 "Clcd2" H 8532 3104 50  0000 R CNN
F 1 "10u" H 8532 3195 50  0000 R CNN
F 2 "Capacitors_SMD:C_1210" H 8688 3000 50  0001 C CNN
F 3 "~" H 8650 3150 50  0001 C CNN
	1    8650 3150
	-1   0    0    1   
$EndComp
Wire Wire Line
	8300 2950 8650 2950
Connection ~ 9000 2950
Wire Wire Line
	8650 3000 8650 2950
Connection ~ 8650 2950
Wire Wire Line
	8650 2950 9000 2950
Wire Wire Line
	8650 3300 8650 3350
Wire Wire Line
	8650 3350 9000 3350
Connection ~ 9000 3350
$Comp
L Device:CP Cavr3
U 1 1 5CF9C94E
P 650 5500
F 0 "Cavr3" H 768 5546 50  0000 L CNN
F 1 "10u" H 768 5455 50  0000 L CNN
F 2 "Capacitors_SMD:C_1210" H 688 5350 50  0001 C CNN
F 3 "~" H 650 5500 50  0001 C CNN
	1    650  5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	650  5350 1100 5350
Connection ~ 1100 5350
Wire Wire Line
	650  5650 1100 5650
Connection ~ 1100 5650
Wire Wire Line
	4600 6550 4900 6550
Wire Wire Line
	4900 6550 4900 6650
Connection ~ 4900 6550
Wire Wire Line
	4900 6550 5150 6550
$Comp
L power:GNDPWR #PWR02
U 1 1 5CFB0C07
P 4900 6950
F 0 "#PWR02" H 4900 6750 50  0001 C CNN
F 1 "GNDPWR" H 4904 6796 50  0000 C CNN
F 2 "" H 4900 6900 50  0001 C CNN
F 3 "" H 4900 6900 50  0001 C CNN
	1    4900 6950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 5900 4900 5900
$Comp
L power:GNDPWR #PWR01
U 1 1 5CFB213B
P 4900 6250
F 0 "#PWR01" H 4900 6050 50  0001 C CNN
F 1 "GNDPWR" H 4904 6096 50  0000 C CNN
F 2 "" H 4900 6200 50  0001 C CNN
F 3 "" H 4900 6200 50  0001 C CNN
	1    4900 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 5900 5650 5900
Wire Wire Line
	4600 6000 4600 6550
Wire Wire Line
	4900 5950 4900 5900
Connection ~ 4900 5900
Wire Wire Line
	4900 5900 5150 5900
Wire Wire Line
	5650 5900 5650 6550
Connection ~ 5650 5900
Wire Wire Line
	5650 5900 5750 5900
Connection ~ 1150 4100
$Comp
L Device:R R12
U 1 1 5CFCA97B
P 1150 3850
F 0 "R12" H 1220 3896 50  0000 L CNN
F 1 "47k" H 1220 3805 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 1080 3850 50  0001 C CNN
F 3 "~" H 1150 3850 50  0001 C CNN
	1    1150 3850
	1    0    0    -1  
$EndComp
Text Label 1150 3700 0    50   ~ 0
3v3
Wire Wire Line
	1150 4000 1150 4100
Connection ~ 1250 6900
Wire Wire Line
	1250 6900 1350 6900
Wire Wire Line
	1050 6900 1250 6900
Wire Wire Line
	9000 3350 9250 3350
Connection ~ 9250 3350
Wire Wire Line
	9000 2950 9850 2950
Connection ~ 9850 2950
Wire Wire Line
	9000 1450 9250 1450
Connection ~ 9250 1450
$Comp
L Regulator_Linear:AMS1117-3.3 U1
U 1 1 5CFE2C40
P 6500 1300
F 0 "U1" H 6500 1542 50  0000 C CNN
F 1 "AMS1117-3.3" H 6500 1451 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-223-3_TabPin2" H 6500 1500 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 6600 1050 50  0001 C CNN
	1    6500 1300
	1    0    0    -1  
$EndComp
Text Label 6200 1300 2    50   ~ 0
5v
$Comp
L power:GNDPWR #PWR04
U 1 1 5CFE5F09
P 6500 1800
F 0 "#PWR04" H 6500 1600 50  0001 C CNN
F 1 "GNDPWR" H 6504 1646 50  0000 C CNN
F 2 "" H 6500 1750 50  0001 C CNN
F 3 "" H 6500 1750 50  0001 C CNN
	1    6500 1800
	1    0    0    -1  
$EndComp
Text Label 6800 1300 0    50   ~ 0
3v3
$Comp
L Device:CP C4
U 1 1 5D08955F
P 6000 1500
F 0 "C4" H 6118 1546 50  0000 L CNN
F 1 "10u" H 6118 1455 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_D5.0mm_P2.00mm" H 6038 1350 50  0001 C CNN
F 3 "~" H 6000 1500 50  0001 C CNN
	1    6000 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C5
U 1 1 5D08E43F
P 7000 1500
F 0 "C5" H 7118 1546 50  0000 L CNN
F 1 "10u" H 7118 1455 50  0000 L CNN
F 2 "Capacitors_SMD:C_1210" H 7038 1350 50  0001 C CNN
F 3 "~" H 7000 1500 50  0001 C CNN
	1    7000 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 1300 7000 1300
Wire Wire Line
	7000 1300 7000 1350
Wire Wire Line
	6200 1300 6000 1300
Wire Wire Line
	6000 1300 6000 1350
Wire Wire Line
	6000 1650 6000 1700
Wire Wire Line
	7000 1700 7000 1650
Wire Wire Line
	6500 1600 6500 1700
Wire Wire Line
	550  4100 1150 4100
Wire Wire Line
	550  4500 1150 4500
Wire Wire Line
	9500 1800 9500 1550
Wire Wire Line
	9500 1550 9600 1550
NoConn ~ 9600 1150
Wire Wire Line
	7000 1700 6500 1700
Connection ~ 6500 1700
Wire Wire Line
	6500 1700 6500 1800
Wire Wire Line
	6500 1700 6000 1700
$Comp
L conn:CONN_01X01 P3
U 1 1 5D130A72
P 4600 1050
F 0 "P3" H 4678 1091 50  0000 L CNN
F 1 "CONN_01X01" H 4678 1000 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 4600 1050 60  0001 C CNN
F 3 "" H 4600 1050 60  0000 C CNN
	1    4600 1050
	1    0    0    -1  
$EndComp
$Comp
L conn:CONN_01X01 P6
U 1 1 5D131D0A
P 4600 1350
F 0 "P6" H 4678 1391 50  0000 L CNN
F 1 "CONN_01X01" H 4678 1300 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 4600 1350 60  0001 C CNN
F 3 "" H 4600 1350 60  0000 C CNN
	1    4600 1350
	1    0    0    -1  
$EndComp
NoConn ~ 4400 1050
NoConn ~ 4400 1350
$Comp
L Device:Rotary_Encoder_Switch SW3
U 1 1 5D6AA756
P 5100 4600
F 0 "SW3" H 5100 4967 50  0000 C CNN
F 1 "Rotary_Encoder_Switch" H 5100 4876 50  0000 C CNN
F 2 "misc:encoder" H 4950 4760 50  0001 C CNN
F 3 "~" H 5100 4860 50  0001 C CNN
	1    5100 4600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R14
U 1 1 5D6ABA80
P 4700 4950
F 0 "R14" H 4770 4996 50  0000 L CNN
F 1 "10k" H 4770 4905 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 4630 4950 50  0001 C CNN
F 3 "~" H 4700 4950 50  0001 C CNN
	1    4700 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5D6AC3C6
P 4450 4950
F 0 "R3" H 4520 4996 50  0000 L CNN
F 1 "10k" H 4520 4905 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 4380 4950 50  0001 C CNN
F 3 "~" H 4450 4950 50  0001 C CNN
	1    4450 4950
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0111
U 1 1 5D6B9B05
P 4700 5350
F 0 "#PWR0111" H 4700 5150 50  0001 C CNN
F 1 "GNDPWR" H 4704 5196 50  0000 C CNN
F 2 "" H 4700 5300 50  0001 C CNN
F 3 "" H 4700 5300 50  0001 C CNN
	1    4700 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R15
U 1 1 5D6BA723
P 5550 5100
F 0 "R15" H 5620 5146 50  0000 L CNN
F 1 "10k" H 5620 5055 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 5480 5100 50  0001 C CNN
F 3 "~" H 5550 5100 50  0001 C CNN
	1    5550 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 5250 4700 5250
Wire Wire Line
	4700 5250 4700 5350
Wire Wire Line
	4700 5250 4700 5100
Connection ~ 4700 5250
Wire Wire Line
	4450 5100 4450 5250
Wire Wire Line
	4450 5250 4700 5250
Wire Wire Line
	4700 4800 4700 4700
Wire Wire Line
	4700 4700 4800 4700
Wire Wire Line
	4450 4800 4450 4500
Wire Wire Line
	4450 4500 4800 4500
Wire Wire Line
	5400 4700 5550 4700
Wire Wire Line
	5550 4700 5550 4950
Text Label 4600 3950 2    50   ~ 0
3v3
$Comp
L Device:R R16
U 1 1 5D6E6C7F
P 5600 4100
F 0 "R16" H 5670 4146 50  0000 L CNN
F 1 "511" H 5670 4055 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 5530 4100 50  0001 C CNN
F 3 "~" H 5600 4100 50  0001 C CNN
	1    5600 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 3950 5600 3950
Wire Wire Line
	5400 4500 5600 4500
Wire Wire Line
	5600 4500 5600 4250
Wire Wire Line
	4800 4600 4600 4600
Wire Wire Line
	4600 4600 4600 4250
$Comp
L Device:R R13
U 1 1 5D6E5A8D
P 4600 4100
F 0 "R13" H 4670 4146 50  0000 L CNN
F 1 "511" H 4670 4055 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V 4530 4100 50  0001 C CNN
F 3 "~" H 4600 4100 50  0001 C CNN
	1    4600 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 4900 4200 4900
Wire Wire Line
	4200 4900 4200 4700
Wire Wire Line
	4200 4700 4700 4700
Connection ~ 4700 4700
Wire Wire Line
	4450 4500 4050 4500
Wire Wire Line
	4050 4500 4050 4800
Wire Wire Line
	4050 4800 3600 4800
Connection ~ 4450 4500
Text Label 5550 4700 0    50   ~ 0
enc_btn
Text Label 3600 6200 0    50   ~ 0
enc_btn
$EndSCHEMATC
