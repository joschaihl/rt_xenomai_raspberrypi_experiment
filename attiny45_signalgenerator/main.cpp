// ***********************************************************
// Project:              ATtiny 45 Signal-Generator
// Author:               Joscha Ihl
// Module description:   Generiert LOW, HIGH und verschiedene
//                       Rechtspannungen in unterschiedlichen
//                       hohen Frequenzen wenn Taster gedrückt
//                       wird oder Signal anliegt
// ***********************************************************

#include <avr\io.h>
#include <avr\interrupt.h>

#define LOW  0
#define HIGH 1

#define TAKT 4000000   // 4 MHz Taktfrequenz
#define TIMER2_MAX_VALUE     65536UL
// 4.000.000 Hz   40000
// ---------    = ----- Hz = 20000 Hz = 20 kHz
//       200          2

#define TIMER_DELAY_IN_TICKS 36
#define TIMER_OVERFLOW_10KHZ TIMER2_MAX_VALUE-(200-TIMER_DELAY_IN_TICKS)
#define TIMER_OVERFLOW_1KHZ  TIMER2_MAX_VALUE-(2000-TIMER_DELAY_IN_TICKS)
#define TIMER_OVERFLOW_100HZ TIMER2_MAX_VALUE-(2000-TIMER_DELAY_IN_TICKS)
#define TIMER_OVERFLOW_10HZ  TIMER2_MAX_VALUE-(20000-TIMER_DELAY_IN_TICKS)
#define TIMER_OVERFLOW_1HZ   TIMER2_MAX_VALUE-(200000-TIMER_DELAY_IN_TICKS)

// 10kHz
#define TIMER_OVERFLOW TIMER_OVERFLOW_10KHZ

// 1kHz
//#define TIMER_OVERFLOW 65536UL-(2000-36)

// Benutzte Ports und Data Direction Register
#define SIGGEN_PORT                 PORTB
#define SIGGEN_DDR                  DDRB

// Ausgabe Ports
//
// Die Programmstatus Ausgabe soll informieren, welche Art von Signal gerade läuft
// Über diese Ausgabe man auch herausfinden ob der Signalgenerator nicht angeschlossen ist
// bzw. nicht funktioniert
// Permanent LOW: Signal-Typ 0 bis 1 läuft gerade
// Permanent HIGH: Signal-Typ 2 bis n (Schwingung) läuft gerade
#define P_SIG_OUT            PB3
#define D_SIG_OUT            DDB3
#define P_PRG_INFO_OUT       PB0         // Leitung mit dem Signal das generiert wird
#define D_PRG_INFO_OUT       DDB0

// Eingabe Ports
#define P_PRG_IN1            PB2         // Taster (mit Software Entprellung)
#define P_PRG_IN2            PB1         // Datenleitung

// Globale Variablen

// Programm-Modus
enum SignalType {permanentLow=0, permanentHigh=1, Hz1, Hz10, Hz100, kHz1, kHz10} signal;
#define NEXTSIGNAL() signal++

// Interrupt Service Routine für Signal an PRG_IN1 (INT0)
//
SIGNAL(SIG_INTERRUPT0) {
   NEXTSIGNAL();
   asm("nop");
   asm("nop");
}

// Interrupt Service Routine für Signal an PRG_IN2 (PCINT1)
//

// Interrupt Service Routine für Timer1
//
SIGNAL(SIG_OVERFLOW1) {
//   TCNT1 = TIMER_OVERFLOW;
	SIGGEN_PORT ^= (1 << P_SIG_OUT);
}


// ** Entprellung **
// Motivation:
// -----------
// Wenn der Taster gedrückt wird wechselt der Signalzustand von LOW auf HIGH
// Dabei "schwingt" sich der Signalzustand langsam auf HIGH ein, da das
// Bauteil mechanisch "prellt", so dass für einen Zeitraum von einigen ms
// mehrfach von LOW auf HIGH gewechselt wird.
// Dies gilt ebenso wenn der Taster losgelassen wird, er schwingt sich dann
// langsam von HIGH auf LOW ein.
// Wie das Problem gelöst wird:
// Problemlösung:
// --------------
// Sobald der Zustand von LOW auf HIGH und von HIGH auf LOW
// wechselt wird für 100ms eine Sperrvariable auf TRUE gesetzt.
// Solange diese Sperrvariable gesetzt ist, wird ein Signal
// am Port ignoriert.



// ***********************************************************
// Hintergrundroutine
//
int main(void)
{
   // Eingabe-Pins festlegen
   SIGGEN_DDR = (0 << P_PRG_IN1) | (0 << P_PRG_IN2);

   // Ausgabe-Ports festlegen
   SIGGEN_DDR |= (1 << D_SIG_OUT);

   // Timer1 aktivieren
   // Systemtakt / 1
   // TCCR1B = 0x01;

	// Verzögerungszeit:
   // Soll kontinuierlich aufgerufen werden
   //  TCNT1 = TIMER_OVERFLOW;
   // TIMSK |= (1 << TOIE1);

   // Erst beim Wechsel von HIGH auf LOW auf dem INT0 Pin
   // soll in die ISR gesprungen werden
   // The faling edge of INT0 generates an interrupt request.
   MCUCR |= (1 << ISC01) | (0 << ISC01);
   // INT0 aktivieren
   GIMSK |= (1 << INT0);
   // Interrupts zulassen
   sei();
   while(1) {
      // Wenn an Timer
      asm("nop"); // Nichts tun und Strom sparen
   }

}

