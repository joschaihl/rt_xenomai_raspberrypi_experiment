; ******************************************************
; BASIC .ASM template file for AVR
; ******************************************************

.include "Tn45def.inc"

; Define here the variables
;
.def  temp  =r16
.def  temp2 =r17
.def  maxwait =r18
.def  delay_value_lsb=r19
.def  delay_value_msb=r20

.equ P_SIG_OUT            =PB4
.equ D_SIG_OUT            =DDB4

; Eingabe Ports
.equ D_PRG_IN1			  =DDB2
.equ P_PRG_IN1            =PB2         ; Taster (mit Software Entprellung)

.macro wait_3_us
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
.endm


; Define here Reset and interrupt vectors, if any
;
reset:
   rjmp start
   reti      ; Addr $01
   reti      ; Addr $02
   reti      ; Addr $03
   reti      ; Addr $04
   reti      ; Addr $05
   reti      ; Addr $06        Use 'rjmp myVector'
   reti      ; Addr $07        to define a interrupt vector
   reti      ; Addr $08
   reti      ; Addr $09
   reti      ; Addr $0A
   reti      ; Addr $0B        This is just an example
   reti      ; Addr $0C        Not all MCUs have the same
   reti      ; Addr $0D        number of interrupt vectors
   reti      ; Addr $0E
   reti      ; Addr $0F
   reti      ; Addr $10

signal_low:
	cbi PORTB, P_SIG_OUT
	signal_low_m1:
	sbic PINB, P_PRG_IN1   ; Skip next rjmp loop if P_PRG_IN1 is low (low-active)
	rjmp signal_low_m1
	ret
	
signal_high:
	sbi PORTB, P_SIG_OUT
	signal_high_m1:
	sbic PINB, P_PRG_IN1   ; Skip loop if Button is pressed (low-active)
	rjmp signal_high_m1
	ret
	
; Parameter: 16 bit value
;            delay_value_lsb lower byte
; 	          delay_value_msb higher byte
delay_2us:
    subi   delay_value_lsb, low(1)   ; 1 cycle
    sbci   delay_value_msb, high(1)  ; +1 cycle

    SBIS PINB,P_PRG_IN1 ;Skip if bit 2 on PINB is set: 2 cycles
	 ret
	 nop
	 nop
    brne delay_2us ; 2 cycles
	ret

signal_with_10Hz:
	sbi PORTB, P_SIG_OUT               ; 2 cycles
	ldi delay_value_lsb, low(25000)
	ldi delay_value_msb, high(25000)
	rcall delay_2us
	cbi PORTB, P_SIG_OUT               ; 2 cycles
	ldi delay_value_lsb, low(25000)
	ldi delay_value_msb, high(25000)
	rcall delay_2us
	SBIS PINB,P_PRG_IN1 ;Skip if bit 2 on PINB is set  1/2 cycle
	ret
	rjmp signal_with_10Hz
	ret
	
signal_with_100Hz:
	sbi PORTB, P_SIG_OUT               ; 2 cycles
	ldi delay_value_lsb, low(2500)
	ldi delay_value_msb, high(2500)
	rcall delay_2us
	cbi PORTB, P_SIG_OUT               ; 2 cycles
	ldi delay_value_lsb, low(2500)
	ldi delay_value_msb, high(2500)
	rcall delay_2us
	SBIS PINB,P_PRG_IN1 ;Skip if bit 2 on PINB is set  1/2 cycle
	ret
	rjmp signal_with_100Hz
	ret

signal_with_1kHz:
	sbi PORTB, P_SIG_OUT               ; 2 cycles
	ldi delay_value_lsb, low(250)
	ldi delay_value_msb, high(250)
	rcall delay_2us
	cbi PORTB, P_SIG_OUT               ; 2 cycles
	ldi delay_value_lsb, low(250)
	ldi delay_value_msb, high(250)
	rcall delay_2us
	SBIS PINB,P_PRG_IN1 ;Skip if bit 2 on PINB is set  1/2 cycle
	ret
	rjmp signal_with_1kHz
	ret

signal_with_10kHz:
	sbi PORTB, P_SIG_OUT               ; 2 cycles
	ldi delay_value_lsb, low(25)
	ldi delay_value_msb, high(25)
	rcall delay_2us
	cbi PORTB, P_SIG_OUT               ; 2 cycles
	ldi delay_value_lsb, low(25)
	ldi delay_value_msb, high(25)
	rcall delay_2us
	SBIS PINB,P_PRG_IN1 ;Skip if bit 2 on PINB is set  1/2 cycle
	ret
	rjmp signal_with_10kHz
	ret
	
signal_with_40kHz:
	sbi PORTB, P_SIG_OUT               ; 2 cycles
	ldi delay_value_lsb, low(5)
	ldi delay_value_msb, high(5)
	rcall delay_2us
	cbi PORTB, P_SIG_OUT               ; 2 cycles
	ldi delay_value_lsb, low(5)
	ldi delay_value_msb, high(5)
	rcall delay_2us
	SBIS PINB,P_PRG_IN1 ;Skip if bit 2 on PINB is set  1/2 cycle
	ret
	rjmp signal_with_40kHz
	ret
	
signal_with_100kHz:
	sbi PORTB, P_SIG_OUT               ; 2 cycles
	ldi delay_value_lsb, low(1)
	ldi delay_value_msb, high(1)
	rcall delay_2us
	cbi PORTB, P_SIG_OUT               ; 2 cycles
	ldi delay_value_lsb, low(1)
	ldi delay_value_msb, high(1)
	rcall delay_2us
	SBIS PINB,P_PRG_IN1 ;Skip if bit 2 on PINB is set  1/2 cycle
	ret
	rjmp signal_with_100kHz
	ret
	
signal_with_500kHz:
   ; 1 cycle on 4 MHz = 1/4.000.000 = 250 ns
   ; 8 cycles with 250 ns = 250ns * 8 = 2 us
   ; Frequency = 1 / 2 us = 500.000 Hz = 500 kHz = 1/2 MHz
	sbi PORTB, P_SIG_OUT               ; 2 cycles
	;nop                        ; 1 cycle
	SBIS PINB,P_PRG_IN1 ;Skip if bit 2 on PINB is set  1/2 cycle
	ret
	cbi PORTB, P_SIG_OUT               ; 2 cycles
	rjmp signal_with_500kHz    ; 2 cycles
	ret
	
	
; dauert mindestens 3,8 ms
; Solange temp < 255:
; 	if Zustand = LOW:
; 		Addiere 1
;  warte 15 us
debounce_low:
	ldi temp, 0x00
	ldi temp2, 255
	ldi maxwait, 0
	
	debounce_low_m1:	cp temp, temp2
	; Branch to end of algorithm if Equal (temp = 255)
	breq debounce_low_m2
	
	cp maxwait, temp2
	breq debounce_low_m2
	
	inc maxwait ; increment because this must be terminate
	SBIS PINB, P_PRG_IN1  ; increment temp if Pin is low
		inc temp
	wait_3_us
	wait_3_us
   	wait_3_us
	wait_3_us

	rjmp debounce_low_m1
	debounce_low_m2: ret
	
debounce_high:
	ldi temp, 0x00
	ldi temp2, 255
	ldi maxwait, 0
	
	debounce_high_m1:	cp temp, temp2
	; Branch to end of algorithm if Equal (temp = 255)
	breq debounce_high_m2
	
	cp maxwait, temp2
	breq debounce_high_m2
	
	inc maxwait ; increment because this must be terminate
	SBIC PINB, P_PRG_IN1  ; increment temp if Pin is high
		inc temp
	wait_3_us
	wait_3_us
    wait_3_us
	wait_3_us

	rjmp debounce_high_m1
	debounce_high_m2: ret
	
pushbutton_release:
	rcall debounce_low
	
	pushbutton_release_m1:
	; Wait for Push-Button level is high back
	sbis PINB, P_PRG_IN1
	rjmp pushbutton_release_m1
	
	rcall debounce_high
	ret
	
; Program starts here after Reset
;
start:
   ldi temp, (1 << D_SIG_OUT)
   out DDRB, temp

	; Set PB0 as Input and activate internal pull-up
	; resistor (DDRB = 0 AND PORTB = 1)
	ldi temp, (1 << P_PRG_IN1)
	out PORTB, temp

main_loop:
	rcall signal_low
	rcall pushbutton_release
	rcall signal_high
	rcall pushbutton_release
	rcall signal_with_10Hz
	rcall pushbutton_release
	rcall signal_with_100Hz
	rcall pushbutton_release
	rcall signal_with_1kHz
	rcall pushbutton_release
	rcall signal_with_10kHz
	rcall pushbutton_release
	rcall signal_with_40kHz
	rcall pushbutton_release
	rcall signal_with_100kHz
	rcall pushbutton_release
	rcall signal_with_500kHz
	rcall pushbutton_release
   rjmp main_loop
