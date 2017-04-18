/*AD messung
 * Work2.c
 *
 * Created: 17.04.17 21:18:49
 * Author : Samuel
 */ 

#include <avr/io.h>

void ADC_Init(void)
{
  // die Versorgungsspannung AVcc als Referenz wählen:
  ADMUX = (1<<REFS0);    
  // Bit ADFR ("free running") in ADCSRA steht beim Einschalten
  // schon auf 0, also single conversion
  ADCSRA = (1<<ADPS1) | (1<<ADPS0);     // Frequenzvorteiler
  ADCSRA |= (1<<ADEN);                  // ADC aktivieren

  /* nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, man liest
     also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen" */

  ADCSRA |= (1<<ADSC);                  // eine ADC-Wandlung 
  while (ADCSRA & (1<<ADSC) ) {         // auf Abschluss der Konvertierung warten
  }
  /* ADCW muss einmal gelesen werden, sonst wird Ergebnis der nächsten
     Wandlung nicht übernommen. */
  (void) ADCW;
}

/* ADC Einzelmessung */
uint16_t ADC_Read( uint8_t channel )
{
  // Kanal waehlen, ohne andere Bits zu beeinflußen
  ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
  ADCSRA |= (1<<ADSC);            // eine Wandlung "single conversion"
  while (ADCSRA & (1<<ADSC) ) {   // auf Abschluss der Konvertierung warten
  }
  return ADCW;                    // ADC auslesen und zurückgeben
}



int main( void )
{
  uint16_t adcval;
  ADC_Init();
  //Ganz A als Output konfigurieren
  DDRA =0b11111111;
 adcval = 0;
	

  while( 1 ) {
	
	adcval = ADC_Read(0);  // Kanal 0
    PORTA = adcval;		// mach was mit adcval
  }
}


