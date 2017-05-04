#ifndef LEVERBOARDHARDWARE_H_INCLUDED
#define LEVERBOARDHARDWARE_H_INCLUDED

// __GPIOs______________________________________________________
#define PORT_LEDS PORTA
#define TRISTATE_LEDS DDRA
#define PIN_LED0 0
#define PIN_LED1 1
#define PIN_LED2 2
#define PIN_LED3 3

#define PORT_PIEZO PORTA
#define TRISTATE_PIEZO DDRA
#define PIN_PIEZO 4

#define PORT_ENABLE_DRIVER PORTA
#define TRISTATE_ENABLE_DRIVER DDRA
#define PIN_ENABLE_DRIVER 5

#define PORT

// __PWM________________________________________________________

// CALCULATIONS:
// PWM Frequenz f_pwm: 25kHz
// Taktfrequenz uC f_uC: 16MHz
// PWM Auflösung RESOLUTION_PWM
// Faktor prescaler F_pre

// f_uC/F_pre = f_pwm*RESOLUTION_PWM
// --> F_pre = f_uC/(f_pwm*RESOLUTION_PWM)
// --> RESOLUTION_PWM = f_uC/(f_pwm*F_pre)

// RESOLUTION_PWM = 256
// --> F_pre = f_uC/(f_pwm*RESOLUTION_PWM) = 2.5

// f_pwm = 40kHz, RESOLUTION_PWM = 256
// --> F_pre = f_uC/(f_pwm*RESOLUTION_PWM) = 1.56

// F_pre = 8
// --> RESOLUTION_PWM = f_uC/(f_pwm*F_pre) = 80

// F_pre = 8, RESOLUTION_PWM=256
// --> f_pwm = f_uC/(F_pre*RESOLUTION_PWM) = 7.8kHz (zu tiefe Frequenz. Bis auf weiteres genommen)

// F_pre = 8, phase correct --> needs two timer cycles! --> f_pwm_pc = 2*f_pwm = 50kHz
// --> RESOLUTION_PWM = f_uC/(2*f_pwm*F_pre) = 40

// F_pre = 8, f_pwm = 10kHz, phase correct --> needs two timer cycles! --> f_pwm_pc = 2*f_pwm = 20kHz
// --> RESOLUTION_PWM = f_uC/(2*f_pwm*F_pre) = 100
#define RESOLUTION_PWM 100
#define FACTOR_PRESCALER 8

// REGISTERS:
// TCNT0 timer/counter 1
#define VALUE_PWM_TIMER TCNT0

// OCR0A output/compare (Vergleichswerte des Timers 1). Bei fast PWM obere Grenze.
#define MAX_VALUE_PWM_TIMER OCR0A

// OCR0B output/compare (Vergleichswerte des Timers 1)
#define COMPARE_VALUE_PWM_TIMER OCR0B

// OCF0A OCF0B output compare flag (gesetzt wenn timer 1 = OCR0A/OCR0B)
// OC0x (OC0A/OC0B) output compare pins


// TCCR0A – Timer/Counter Control Register A p.126
//  |7      |6      |5      |4      |3  |2  |1      |0      |
//  |COM0A1 |COM0A0 |COM0B1 |COM0B0 |-  |-  |WGM01  |WGM00  |
#define CONTROL_REGISTER_A_PWM_TIMER TCCR0A

// COM0x1:0 compare output mode
//  -definieren des states von OC0x bei match
//  -definieren der output source von OC0x

// Compare Output Mode, non PWM Mode: Table 16-2 p.126
// Normal port operation, OC0A disconnected (für soft pwm)
// COM0A1 = 0
// COM0A0 = 0

// Compare Output Mode, Fast PWM Mode: Table 16-3 p.126
// inverting fast pwm (Set OC0A on Compare Match, clear OC0A at BOTTOM)
// COM0A1 = 1
// COM0A0 = 1


// fast PWM: timer zählt immer von 0 bis TOP (entweder 0xFF WGM2:0 = 3, oder OCR0A wenn WGM2:0 = 7)
//  -non inverting: ab match auf BOTTOM
//  -invertinmg: ab match auf TOP <-- benötigt

// Waveform Generation Mode Bit Description: Table 16-8 p.128
// CTC TOP = OCR0A
// WGM02 = 0 (TCCR0B register)
// WGM01 = 1
// WGM00 = 0
#define CTC_SOFT_PWM_A ((0<<WGM00) | (1<<WGM01) | (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0))

// Waveform Generation Mode Bit Description: Table 16-8 p.128
// PWM, Phase Correct TOP = OCR0A
// WGM2 = 1 (TCCR0B register)
// WGM1 = 0
// WGM0 = 1
#define PC_SOFT_PWM_A ((1<<WGM00) | (0<<WGM01) | (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0))

// Waveform Generation Mode Bit Description: Table 16-8 p.128
// fast PWM TOP = OCR0A
// WGM2 = 1 (TCCR0B register)
// WGM1 = 1
// WGM0 = 1
#define FAST_SOFT_PWM_A ((1<<WGM00) | (1<<WGM01) | (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0))

// TCCR0B – Timer/Counter Control Register B p.129
//  |7     |6       |5  |4  |3      |2      |1      |0      |
//  |FOC0A |FOC0B   |-  |-  |WGM02  |CS02   |CS01   |CS00   |
#define CONTROL_REGISTER_B_PWM_TIMER TCCR0B

// Clock Select Bit Description Table 16-9 p.130
// clk/8
// CS02 = 0
// CS01 = 1
// CS00 = 0

// Force Output Compare A/B
// In PWM Mode zu kompatibilitätszwecken mit 0 beschrieben
// FOC0A = 0
// FOC0B = 0
#define FAST_SOFT_PWM_PRESC_8_B ((1<<WGM02) | (0<<CS00) | (1<<CS01) | (0<<CS02) | (0<<FOC0A) | (0<<FOC0B))
#define FAST_SOFT_PWM_PRESC_1024_B ((1<<WGM02) | (1<<CS00) | (0<<CS01) | (1<<CS02) | (0<<FOC0A) | (0<<FOC0B))
#define CTC_SOFT_PWM_PRESC_1024_B ((0<<WGM02) | (1<<CS00) | (0<<CS01) | (1<<CS02) | (0<<FOC0A) | (0<<FOC0B))
#define PC_SOFT_PWM_PRESC_8_B ((1<<WGM02) | (0<<CS00) | (1<<CS01) | (0<<CS02) | (0<<FOC0A) | (0<<FOC0B))

// TIMSK0 – Timer/Counter Interrupt Mask Register p.131
//  |7  |6  |5  |4  |3  |2      |1      |0      |
//  |-  |-  |-  |-  |-  |OCIE0B |OCIE0A |TOIE0  |
// OCIE0B = 1: Timer/Counter Compare Match B interrupt is enabled
// OCIE0A = 1: Timer/Counter Compare Match A interrupt is enabled
// TOIE0 = 1: Timer/Counter0 Overflow Interrupt Enable
#define ENABLE_INTERRUPTS_PWM_TIMER TIMSK0
#define COMPARE_A_B_INTERRUPT ((1<<OCIE0B) | (1<<OCIE0A) | (0<<TOIE0))
#define COMPARE_B_INTERRUPT ((1<<OCIE0B) | (0<<OCIE0A) | (0<<TOIE0))

// TIFR0 – Timer/Counter 0 Interrupt Flag Register p.131
//  |7  |6  |5  |4  |3  |2      |1      |0      |
//  |-  |-  |-  |-  |-  |OCF0B  |OCF0A  |TOV0   |
// OCF0B: Timer/Counter 0 Output Compare B Match Flag. Gesetzt falls OCR0B=TCNT0
// OCF0A: Timer/Counter 0 Output Compare A Match Flag. Gesetzt falls OCR0A=TCNT0
// TOV0: Timer/Counter0 Overflow Flag
#define INTERRUPT_FLAGS_PWM_TIMER TIFR0
#define SET_HIGH_FLAG OCF0B
#define SET_LOW_FLAG OCF0A

// __USART________________________________________________________

// REGISTERS:

// UDRn – USART I/O Data Register n p.218
// Wenn gelesen: Empfangene Daten von Receive Data Buffer Register (RXB)
// Wenn beschrieben: Daten auf Data Buffer Register (TXB) schreiben
//      -> nur beschreibbar wenn UDREn Flag in UCSRAnA Register gesetzt (sonst ignoriert)
#define UART_SEND_DATA UDR0

// UCSRnA – USART Control and Status Register A p.219
//  |7      |6      |5      |4      |3      |2      |1      |0      |
//  |RXCn   |TXCn   |UDREn  |FEn    |DORn   |UPEn   |U2Xn   |MPCMn  |
#define UART_REGISTER_A UCSR0A

// RXCn: USART Receive Complete
// -> gesetzt wenn ungelesene Daten in receive register

// TXCn: USART Transmit Complete
// -> gesetzt wenn alle Daten gesendet & keine neuen daten in UDRn verfügbar

// UDREn: USART Data Register Empty
// -> gesetzt wenn UDRn leer ist
#define UART_DATA_REG_EMPTY_BIT UDRE0

// FEn: Frame Error
// -> gesetzt wenn nächstes zeichen in receive buffer ein frame error aufweist

// DORn: Data OverRun
// -> gesett wenn empfangbuffer voll ist (zwei Zeichen)

// UPEn: USART Parity Error
// -> gesetzt wenn zeichen in eingansbuffer ein parity fehler aufweist

// U2Xn: Double the USART Transmission Speed
// -> wenn gesetzt, wird divisor von baudrate von 16 auf 8 reduziert --> verdoppelt speed
// -> hat einfluss auf asynchrone übertragung (sonst auf 0 setzen!).

// MPCMn: Multi-processor Communication Mode
// -> Auf 0 setzen
#define UART_REGISTER_A_VALUE ((0<<U2X0) | (0<<MPCM0))

// UCSRnB – USART Control and Status Register n B
//  |7      |6      |5      |4      |3      |2      |1      |0      |
//  |RXCIEn |TXCIEn |UDRIEn |RXENn  |TXENn  |UCSZn2 |RXB8n  |TXB8n  |
#define UART_REGISTER_B UCSR0B

// RXCIEn: RX Complete Interrupt Enable n
// -> interrupt wenn RXCn gesetzt

// TXCIEn: TX Complete Interrupt Enable n
// -> interrupt wenn TXCn gesetzt

// UDRIEn: USART Data Register Empty Interrupt Enable n

// RXENn: Receiver Enable n
// -> wenn gesetzt wird USART Empfänger aktiviert

// TXENn: Transmitter Enable n
// -> wenn gesetzt wird USART Sender aktiviert

// UCSZn2: Character Size n
// -> mit UCSZn1:0 aus UCSRnC. Legt anzahl von datenbits in einem Frame fest

// RXB8n: Receive Data Bit 8 n
// TXB8n: Transmit Data Bit 8 n
// -> neuntes Datenbyt wenn mit 9 Bit gearbeitet wird
#define UART_REGISTER_B_VALUE ((0<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (0<<RXEN0) | (1<<TXEN0) | (1<<UCSZ02))

// UCSRnC – USART Control and Status Register n C p.221
//  |7      |6      |5      |4      |3      |2      |1      |0      |
//  |UMSELn1|UMSELn0|UPMn1  |UPMn0  |USBSn  |UCSZn1 |UCSZn0 |UCPOLn |
#define UART_REGISTER_C UCSR0C

// UMSELn1:0 USART Mode Select
// -> Modus des USART:
// 0 0 async <-- keine gemeinsahme taktleitung!
// 0 1 sync
// 1 1 master SPI

// UPMn1:0: Parity Mode
// -> Festlegen des parity modus:
// 0 0 disabled <-- da nur zu debug-zwecken
// 1 0 enabled even
// 1 1 enabled odd

// USBSn: Stop Bit Select
// -> anzahl stop bits die sender einfügt
// 0 1-bit <-- normal
// 1 2-bit

// UCSZn1:0: Character Size
// -> anzahl datenbits (mit UCSZn2 aus UCSRnB)
// 0 1 0 7-bit
// 0 1 1 8-bit <-- default picocom
// 1 1 1 9-bit

// UCPOLn: Clock Polarity
// -> nur für sync mode. Beziehung zwischen daten ausgangs änderung, daten input sample und sync clock
#define UART_REGISTER_C_VALUE ((0<<UMSEL01) | (0<<UMSEL00) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00))

// UBRRnL and UBRRnH – USART Baud Rate Registers
//  |7      |6      |5      |4      |3      |2      |1      |0      |
//  |-      |-      |-      |-      |UBRR11 |UBRR10 |UBRR9  |UBRR8  | --> UBRRHn
//  |UBRR7  |UBRR6  |UBRR5  |UBRR4  |UBRR3  |UBRR2  |UBRR1  |UBRR0  | --> UBRRLn
#define UART_BAUD_RATE_HIGH_BYTE UBRR0H
#define UART_BAUD_RATE_LOW_BYTE UBRR0L

// Bit 15:12 -> reserviert, müssen mit 0 beschrieben werden

// Bit 11:0 – UBRR11:0: USART Baud Rate Register
// -> 12bit register für Baud rate
// fehler muss unter 0.5% liegen
//      |U2Xn = 0       |U2Xn = 1       |
// Baud |UBRR   |Error  |UBRR   |Error  |
// 9600 |103    |0.2%   |207    |0.2%   |
// Table 22-12 p.226

// 103 --> 0b0110 0111
#define UART_BAUD_RATE_HIGH_BYTE_VALUE 0
#define UART_BAUD_RATE_LOW_BYTE_VALUE 0b01100111

// ttyACM3
// Terminal starten:
// sudo picocom --baud 9600  --parity n --databits 8 --flow n -l /dev/ttyACM3
// (Quelle: https://developer.ridgerun.com/wiki/index.php/Setting_up_Picocom_-_Ubuntu)


#endif // LEVERBOARDHARDWARE_H_INCLUDED
